/**
 *  \file ParticlesRestraint.h
 *  \brief Apply a SingletonScore to each Particle in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPCORE_PARTICLES_RESTRAINT_H
#define IMPCORE_PARTICLES_RESTRAINT_H

#include "core_exports.h"
#include "internal/core_version_info.h"
#include "ParticleContainer.h"

#include <IMP/Restraint.h>
#include <IMP/Pointer.h>
#include <IMP/SingletonScore.h>

#include <iostream>

IMPCORE_BEGIN_NAMESPACE

//! Applies a SingletonScore to each Particle in a list.
/** This restraint stores the used particles in a ParticleContainer.
    The container used can be set so that the list can be shared
    with other containers (or a nonbonded list can be used). By default
    a ListParticleContainer is used and the
    {add_, set_, clear_}particle{s} methods can be used.

    \ingroup restraint
 */
class IMPCOREEXPORT ParticlesRestraint : public Restraint
{
public:
  //! Create the restraint.
  /** \param[in] ss The function to apply to each particle.
      \param[in] ps The list of particles to use in the restraint.
   */
  ParticlesRestraint(SingletonScore *ss,
                      const Particles &ps=Particles());

 //! Create the restraint with a shared container
  /** \param[in] ss The function to apply to each particle.
      \param[in] pc The container containing the stored particles. This
      container is not copied.
   */
  ParticlesRestraint(SingletonScore *ss,
                      ParticleContainer *pc);

  virtual ~ParticlesRestraint();

  IMP_RESTRAINT(internal::core_version_info);

  void add_particles(const Particles &ps);
  void set_particles(const Particles &ps);
  void add_particle(Particle* v);
  void clear_particles();

  //! Get the container used to store Particles
  ParticleContainer* get_particle_container() {
    return pc_;
  }
  //! Set the container used to store Particles
  /** \note The passed container is used, not a copy.
   */
  void set_particle_container(ParticleContainer *pc) {
    pc_=pc;
  }

  virtual ParticlesList get_interacting_particles() const;

private:
  Pointer<SingletonScore> ss_;
  Pointer<ParticleContainer> pc_;
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_PARTICLES_RESTRAINT_H */
