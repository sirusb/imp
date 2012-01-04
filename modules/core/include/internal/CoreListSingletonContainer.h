/**
 *  \file ListSingletonContainer.h    \brief Store a list of ParticlesTemp
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPCORE_INTERNAL_CORE_LIST_SINGLETON_CONTAINER_H
#define IMPCORE_INTERNAL_CORE_LIST_SINGLETON_CONTAINER_H

#include "../core_config.h"
#include <IMP/SingletonContainer.h>
#include <IMP/internal/container_helpers.h>
#include "singleton_helpers.h"
#include <IMP/ScoreState.h>
#include <IMP/scoped.h>
#include <IMP/generic.h>

IMPCORE_BEGIN_INTERNAL_NAMESPACE


class IMPCOREEXPORT CoreListSingletonContainer:
  public internal::ListLikeSingletonContainer
{
  IMP_ACTIVE_CONTAINER_DECL(CoreListSingletonContainer);
 public:
  CoreListSingletonContainer(Model *m, std::string name);
  CoreListSingletonContainer(Model *m, const char *name);
  void add_particle(Particle* vt) {
    IMP_USAGE_CHECK(IMP::internal::is_valid(vt),
                    "Passed Singleton cannot be nullptr (or None)");
    add_to_list(IMP::internal::get_index(vt));
  }
  void add_particles(const ParticlesTemp &c) {
    if (c.empty()) return;
    ParticleIndexes cp= IMP::internal::get_index(c);
    add_to_list(cp);
  }
  void remove_particles(const ParticlesTemp &c);
  void set_particles(ParticlesTemp c) {
    ParticleIndexes cp= IMP::internal::get_index(c);
    update_list(cp);
  }
  void set_particles(ParticleIndexes cp) {
    update_list(cp);
  }
  void clear_particles() {
    ParticleIndexes t;
    update_list(t);
  }
  bool get_is_up_to_date() const {
    return true;
  }
  IMP_LISTLIKE_SINGLETON_CONTAINER(CoreListSingletonContainer);
};

IMP_OBJECTS(CoreListSingletonContainer, CoreListSingletonContainers);

IMPCORE_END_INTERNAL_NAMESPACE

#endif  /* IMPCORE_INTERNAL_CORE_LIST_SINGLETON_CONTAINER_H */
