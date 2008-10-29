/**
 *  \file ParticleContainer.h    \brief A container for particles.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_PARTICLE_CONTAINER_H
#define IMPCORE_PARTICLE_CONTAINER_H

#include "core_exports.h"
#include "internal/IndexingIterator.h"
#include <IMP/Particle.h>
#include <IMP/base_types.h>
#include <IMP/Pointer.h>

IMPCORE_BEGIN_NAMESPACE

//! A shared container for particles
/** Stores a searchable shared collection of particles.
 */
class IMPCOREEXPORT ParticleContainer : public Object
{
  struct Accessor {
    typedef Accessor This;
    typedef Particle* result_type;
    typedef unsigned int argument_type;
    result_type operator()(argument_type i) const {
      return o_->get_particle(i);
    }
    Accessor(ParticleContainer *pc): o_(pc){}
    Accessor(){}
    IMP_COMPARISONS_1(o_);
  private:
    bool is_default() const { return false;}
    Pointer<ParticleContainer> o_;
  };

public:
  ParticleContainer();

  virtual ~ParticleContainer();

  //!
  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular container.
   */
  virtual bool get_contains_particle(Particle* p) const =0;
  //!
  virtual unsigned int get_number_of_particles() const =0;
  //!
  virtual Particle* get_particle(unsigned int i) const=0;

  virtual void show(std::ostream &out = std::cout) const;

  //!
  typedef internal::IndexingIterator<Accessor> ParticleIterator;
  //!
  ParticleIterator particles_begin() const {
    // Since I can't make the count mutable in Object
    return
      ParticleIterator(Accessor(const_cast<ParticleContainer*>(this)),
                        0);
  }
  //!
  ParticleIterator particles_end() const {
    return
      ParticleIterator(Accessor(const_cast<ParticleContainer*>(this)),
                        get_number_of_particles());
    }
};

IMP_OUTPUT_OPERATOR(ParticleContainer);

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_PARTICLE_CONTAINER_H */
