/**
 *  \file PairModifier.h    \brief A Modifier on ParticlePairsTemp
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_PAIR_MODIFIER_H
#define IMPKERNEL_PAIR_MODIFIER_H

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
    the passed particles.

    \see IMP::PairFunctor

    Implementors should see IMP_PAIR_MODIFIER(). Also see
    PairDerivativeModifier.
 */
class IMPEXPORT PairModifier : public base::Object
{
public:
  typedef ParticlePair Argument;
  PairModifier(std::string name="PairModifier %1%");

  /** Apply the function to a single value*/
  virtual void apply(const ParticlePair&) const =0;

 /** Apply the function to a single value*/
  virtual void apply_index(Model *m, const ParticleIndexPair& v) const {
    apply(internal::get_particle(m, v));
  }

  /** Apply the function to a collection of ParticlePairsTemp */
  virtual void apply_indexes(Model *m, const ParticleIndexPairs &o) const {
    for (unsigned int i=0; i < o.size(); ++i) {
      apply_index(m, o[i]);
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

IMP_OUTPUT_OPERATOR(PairModifier);

#ifdef IMP_DOXYGEN
/** Create a modifier from a functor. C++ only. The function should take
    a Pair as an argument.
    This is intended to be used as a temporary object and not stored.
    A reference to the functor is saved.
 */
template <class Functor>
PairModifier *create_particle_pair_modifier(const Functor& f);
#elif !defined(SWIG)
template <class Functor>
class FunctorPairModifier: public PairModifier {
  const Functor &f_;
public:
  FunctorPairModifier(const Functor& f):
    PairModifier("FunctorModifier %1%"),
    f_(f){}
  IMP_PAIR_MODIFIER(FunctorPairModifier);
};

template <class Functor>
void FunctorPairModifier<Functor>::apply(const ParticlePair& v) const {
  f_(v);
}

template <class Functor>
 ParticlesTemp
FunctorPairModifier<Functor>::get_input_particles(Particle* p) const {
  return ParticlesTemp(1,p);
}
template <class Functor>
ParticlesTemp
FunctorPairModifier<Functor>::get_output_particles(Particle *p) const {
  return ParticlesTemp(1,p);
}
template <class Functor>
ContainersTemp
FunctorPairModifier<Functor>::get_input_containers(Particle *) const {
  return ContainersTemp();
}
template <class Functor>
ContainersTemp
FunctorPairModifier<Functor>::get_output_containers(Particle *) const {
  return ContainersTemp();
}

/** Create a modifier from a functor. C++ only.*/
template <class Functor>
inline FunctorPairModifier<Functor> *
create_particle_pair_modifier(const Functor& f) {
  return new FunctorPairModifier<Functor>(f);
}

#endif

IMP_OBJECTS(PairModifier,PairModifiers);

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
class PairFunctor {
  Pointer<const PairModifier> f_;
public:
  //! Store the PairModifier and the optional DerivativeAccumulator
  PairFunctor(const PairModifier *f): f_(f){}
  void operator()( ParticlePair p) const {
    f_->apply(p);
  }
};



IMP_END_NAMESPACE

#endif  /* IMPKERNEL_PAIR_MODIFIER_H */
