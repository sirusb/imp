/**
 *  \file InContainerSingletonFilter.h    \brief A filter for Singletons.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 Sali Lab. All rights reserved.
 */

#ifndef IMPCONTAINER_IN_CONTAINER_SINGLETON_FILTER_H
#define IMPCONTAINER_IN_CONTAINER_SINGLETON_FILTER_H

#include "container_config.h"
#include <IMP/SingletonFilter.h>
#include <IMP/SingletonContainer.h>
#include <IMP/internal/container_helpers.h>

IMPCONTAINER_BEGIN_NAMESPACE


//! A filter which returns true if a container containers the Singleton
/** Stores a searchable shared collection of Singletons.
    \ingroup restraints

    Implementors should see IMP_SINGLETON_FILTER().
 */
class IMPCONTAINEREXPORT InContainerSingletonFilter : public SingletonFilter
{
  IMP::OwnerPointer<SingletonContainer> c_;
public:
  InContainerSingletonFilter(SingletonContainer *c,
                             std::string name="SingletonFilter %1%");

  IMP_SINGLETON_FILTER(InContainerSingletonFilter);
};

inline bool InContainerSingletonFilter
::get_contains(Particle* p) const {
  return c_->get_contains_particle(p);
}

IMP_OBJECTS(InContainerSingletonFilter, InContainerSingletonFilters);

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_IN_CONTAINER_SINGLETON_FILTER_H */
