/**
 *  \file MolecularDynamics.cpp  \brief Simple molecular dynamics optimizer.
 *
 *  Copyright 2007 Sali Lab. All rights reserved.
 *
 */

#include <cmath>

#include "IMP/log.h"
#include "IMP/optimizers/MolecularDynamics.h"

namespace IMP
{

//! Constructor
MolecularDynamics::MolecularDynamics() : time_step_(4.0), temperature_(298.0)
{
}


//! Destructor
MolecularDynamics::~MolecularDynamics()
{
}


//! Get the set of particles to use in this optimization.
/** Populates particles_, and gives each particle a velocity.
    \param[in] model The model to optimize.
    \exception InvalidStateException The model does not contain only
                                     xyz particles.
 */
void MolecularDynamics::setup_particles(Model& model)
{
  xkey_ = FloatKey("x");
  ykey_ = FloatKey("y");
  zkey_ = FloatKey("z");

  int npart = model.number_of_particles();
  int i;
  for (i = 0; i < npart; ++i) {
    Particle *p = model.get_particle(i);
    if (p->has_attribute(xkey_) && p->has_attribute(ykey_)
        && p->has_attribute(zkey_)) {
      particles_.push_back(p);
    }
  }

  OptFloatIndexIterator opt_value_iter;
  unsigned nopt = 0;
  opt_value_iter.reset(model.get_model_data());
  while (opt_value_iter.next()) {
    nopt++;
  }

  if (particles_.size() * 3 != nopt) {
    throw InvalidStateException("Can only do MD on xyz particles");
  }

  vxkey_ = FloatKey("vx");
  vykey_ = FloatKey("vy");
  vzkey_ = FloatKey("vz");
  FloatKey *derivs[3] = { &vxkey_, &vykey_, &vzkey_ };
  for (i = 0; i < npart; ++i) {
    Particle *p = model.get_particle(i);
    for (int dind = 0; dind < 3; ++dind) {
      if (!p->has_attribute(*derivs[dind])) {
        p->add_attribute(*derivs[dind], 0.0, false);
      }
    }
  }
}


//! Perform a single dynamics step.
void MolecularDynamics::step()
{
  // Assuming score is in kcal/mol, its derivatives in kcal/mol/angstrom,
  // and mass is in kg/mol, conversion factor necessary to get accelerations
  // in angstrom/fs/fs
  static const Float unit_conversion = 4.1868e-7;

  // Mass of Carbon-12, in kg/mol
  static const Float c12mass = 0.012011;

  // Multiplier to convert raw derivatives to acceleration
  static const Float deriv_to_acceleration = -unit_conversion / c12mass;

  for (std::vector<Particle *>::iterator iter = particles_.begin();
       iter != particles_.end(); ++iter) {
    Particle *p = *iter;
    Float x = p->get_value(xkey_);
    Float y = p->get_value(ykey_);
    Float z = p->get_value(zkey_);
    Float dvx = p->get_derivative(xkey_);
    Float dvy = p->get_derivative(ykey_);
    Float dvz = p->get_derivative(zkey_);

    // calculate velocity at t+(delta t/2) from that at t-(delta t/2)
    Float vx = p->get_value(vxkey_);
    Float vy = p->get_value(vykey_);
    Float vz = p->get_value(vzkey_);
    vx += dvx * deriv_to_acceleration * time_step_;
    vy += dvy * deriv_to_acceleration * time_step_;
    vz += dvz * deriv_to_acceleration * time_step_;
    p->set_value(vxkey_, vx);
    p->set_value(vykey_, vy);
    p->set_value(vzkey_, vz);

    // get atomic shift
    Float dx = vx * time_step_;
    Float dy = vy * time_step_;
    Float dz = vz * time_step_;

    // calculate position at t+(delta t) from that at t
    x += dx;
    y += dy;
    z += dz;
    p->set_value(xkey_, x);
    p->set_value(ykey_, y);
    p->set_value(zkey_, z);
  }
}


//! Optimize the model.
/** \param[in] max_steps   Maximum number of iterations before aborting.
    \return score of the final state of the model.
 */
Float MolecularDynamics::optimize(unsigned int max_steps)
{
  Model *model = get_model();
  setup_particles(*model);

  // get initial system score
  Float score = model->evaluate(true);

  for (unsigned int i = 0; i < max_steps; ++i) {
    step();
    score = model->evaluate(true);
  }
  return score;
}

} // namespace IMP
