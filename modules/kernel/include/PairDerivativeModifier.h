/**
 *  \file PairDerivativeModifier.h
 *  \brief A Modifier on ParticlePairsTemp
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_PAIR_DERIVATIVE_MODIFIER_H
#define IMPKERNEL_PAIR_DERIVATIVE_MODIFIER_H

#include "kernel_config.h"
#include "DerivativeAccumulator.h"
#include "base_types.h"
#include "ParticleTuple.h"
#include "internal/container_helpers.h"


IMP_BEGIN_NAMESPACE
// to keep swig happy
class Particle;

//! A base class for modifiers of ParticlePairsTemp
/** The primary function of such a class is to change
    the derivatives of the passed particles.

    Implementors should see and
    IMP_PAIR_DERIVATIVE_MODIFIER() and
    IMP::PairModifier.
 */
class IMPEXPORT PairDerivativeModifier : public base::Object
{
public:
  typedef ParticlePair Argument;
  PairDerivativeModifier(std::string name="PairModifier %1%");

  /** Apply the function to a single value*/
  virtual void apply(const ParticlePair&,
                     DerivativeAccumulator &da) const=0;

 /** Apply the function to a single value*/
  virtual void apply_index(Model *m, const ParticleIndexPair& v,
                           DerivativeAccumulator &da) const {
    apply(internal::get_particle(m, v), da);
  }

  /** Apply the function to a collection of ParticlePairsTemp */
  virtual void apply_indexes(Model *m, const ParticleIndexPairs &o,
                             DerivativeAccumulator &da) const {
    for (unsigned int i=0; i < o.size(); ++i) {
      apply_index(m, o[i], da);
    }
  }

  /** Get the set of particles read when applied to the arguments.*/
  virtual ParticlesTemp
    get_input_particles(Particle* p) const =0;
  /** Get the set of particles modifier when applied to the arguments.*/
  virtual ParticlesTemp
    get_output_particles(Particle *p) const =0;
  /** Get the set of input containers when this modifier is applied to
      the arguments. */
  virtual ContainersTemp
    get_input_containers(Particle *p) const =0;
  /** Get the set of output containers when this modifier is applied to
      the arguments. */
  virtual ContainersTemp
    get_output_containers(Particle *p) const =0;
};


IMP_OBJECTS(PairDerivativeModifier,PairDerivativeModifiers);


//! Create a functor which can be used with build in C++ and python commands
/** For example, you can do
    \code
    std::for_each(particles.begin(), particles.end(),
                  IMP::SingletonFunctor(new IMP::core::Transform(tr)));
    IMP::for_each(particles,
                  IMP::SingletonFunctor(new IMP::core::Transform(tr)));
    \endcode
    in C++ (the second can be used with when \c particles is a temporary
    value) or
    \verbatim
    map(SingletonFunctor(Transform(tr)), particles)
    \endverbatim
    in python.

    \see IMP::PairModifier
 */
class PairDerivativeFunctor {
  Pointer<const PairDerivativeModifier> f_;
  DerivativeAccumulator *da_;
public:
  //! Store the PairModifier and the optional DerivativeAccumulator
  PairDerivativeFunctor(const PairDerivativeModifier *f,
                   DerivativeAccumulator *da): f_(f), da_(da){
    IMP_USAGE_CHECK(da_,
                    "The passed derivative accumulator should not be null.");
  }
  void operator()( ParticlePair p) const {
    f_->apply(p, *da_);
  }
};

IMP_END_NAMESPACE

#endif  /* IMPKERNEL_PAIR_DERIVATIVE_MODIFIER_H */
