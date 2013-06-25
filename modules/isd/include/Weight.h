/**
 *  \file isd/Weight.h
 *  \brief Add a name to a particle.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPISD_WEIGHT_H
#define IMPISD_WEIGHT_H

#include "isd_config.h"

#include <IMP/core/core_macros.h>
#include <IMP/Particle.h>
#include <IMP/decorator_macros.h>
#include <IMP/Model.h>
#include <IMP/Decorator.h>
#include <IMP/base/exception.h>
#include <sstream>

IMPISD_BEGIN_NAMESPACE

class IMPISDEXPORT Weight: public Decorator
{

  static const int nstates_max = 20;
public:

  IMP_DECORATOR(Weight, Decorator);

  static Weight setup_particle(Particle *p);

//! Get number of states key
  static IntKey get_nstates_key();

//! Get i-th weight key
  static FloatKey get_weight_key(int i);

//! Set all the weights
  void set_weights(algebra::VectorKD w);

//! Add one weight
  void add_weight();

//! Get the i-th weight
  Float get_weight(int i);

//! Get all the weights
  algebra::VectorKD get_weights();

//! Set weights are optimized
  void set_weights_are_optimized(bool tf);

//! Get number of states
  Int get_number_of_states();

  static bool particle_is_instance(Particle *p) {
   return p->has_attribute(get_nstates_key());
  }

};

IMP_VALUES(Weight, Weights);

IMPISD_END_NAMESPACE

#endif  /* IMPISD_WEIGHT_H */