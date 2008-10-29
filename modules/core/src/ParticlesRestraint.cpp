/**
 *  \file ParticlesRestraint.cpp
 *  \brief Apply a SingletonScore function to a container of Particle*s .
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/ParticlesRestraint.h"
#include "IMP/core/internal/container_helpers.h"
#include "IMP/core/ListParticleContainer.h"

#include <IMP/SingletonScore.h>
#include <IMP/log.h>


IMPCORE_BEGIN_NAMESPACE

ParticlesRestraint
::ParticlesRestraint(SingletonScore *ss,
                      const Particles &pc):
  ss_(ss),
  pc_(new ListParticleContainer(pc))
{
}

ParticlesRestraint
::ParticlesRestraint(SingletonScore *ss,
                      ParticleContainer *pc): ss_(ss), pc_(pc) {

}


ParticlesRestraint::~ParticlesRestraint()
{
}

Float ParticlesRestraint::evaluate(DerivativeAccumulator *accum)
{

  IMP_CHECK_OBJECT(ss_);
  IMP_CHECK_OBJECT(pc_);

  Float score=0;


  for (ParticleContainer::ParticleIterator
         it= pc_->particles_begin();
       it != pc_->particles_end(); ++it) {
    score += internal::ContainerTraits<Particle>
      ::evaluate(ss_, *it, accum);
  }

  return score;
}


ParticlesList ParticlesRestraint::get_interacting_particles() const
{
  if (!internal::ContainerTraits<Particle>::is_singleton) {
    ParticlesList ret;
    for (ParticleContainer::ParticleIterator it
           = pc_->particles_begin();
         it != pc_->particles_end(); ++it) {
      ret.push_back(internal::ContainerTraits<Particle>::create_set(*it));
    }
    return ret;
  } else {
    return ParticlesList();
  }
}

namespace {
  ListParticleContainer *
  check_methods(ParticleContainer *pc, std::string str) {
    ListParticleContainer *ret
      = dynamic_cast<ListParticleContainer*>(pc);
    if (! ret) {
      std::ostringstream oss;
      oss << "Method ParticlesRestraint::" << str
          << " can only be called if the ParticleContainer "
          << " is a ListParticleContainer.";
      throw InvalidStateException(oss.str().c_str());
    }
    return ret;
  }
}

void ParticlesRestraint::add_particles(const Particles &ps) {
  ListParticleContainer *pc= check_methods(pc_.get(), "add_particles");
  pc->add_particles(ps);
}

void ParticlesRestraint::set_particles(const Particles &ps){
  ListParticleContainer *pc= check_methods(pc_.get(), "add_particles");
  pc->set_particles(ps);
}

void ParticlesRestraint::add_particle(Particle* v){
  ListParticleContainer *pc= check_methods(pc_.get(), "add_particles");
  pc->add_particle(v);
}

void ParticlesRestraint::clear_particles() {
  ListParticleContainer *pc= check_methods(pc_.get(), "add_particles");
  pc->clear_particles();
}


void ParticlesRestraint::show(std::ostream& out) const
{
  out << "ContainerRestraint with score function ";
  ss_->show(out);
  out << " and container ";
  pc_->show(out);
  out << std::endl;
}

IMPCORE_END_NAMESPACE
