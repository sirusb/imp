/**
 *  \file SingletonContainerSet.cpp
 *  \brief A set of SingletonContainers.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/SingletonContainerSet.h"
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

SingletonContainerSet
::SingletonContainerSet(){
}

SingletonContainerSet::~SingletonContainerSet(){}

bool
SingletonContainerSet
::get_contains_particle(Particle* vt) const {
  for (SingletonContainerConstIterator it= singleton_containers_begin();
       it != singleton_containers_end(); ++it) {
    if ((*it)->get_contains_particle(vt)) return true;
  }
  return false;
}

void SingletonContainerSet::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "SingletonContainerSet with "
      << get_number_of_particles()
      << " particles." << std::endl;
}

unsigned int
SingletonContainerSet::get_number_of_particles() const {
  unsigned int sum=0;
  for (SingletonContainerConstIterator it= singleton_containers_begin();
       it != singleton_containers_end(); ++it) {
    sum+= (*it)->get_number_of_particles();
  }
  return sum;
}

Particle*
SingletonContainerSet::get_particle(unsigned int i) const {
  for (SingletonContainerConstIterator it= singleton_containers_begin();
       it != singleton_containers_end(); ++it) {
    if ( i >= (*it)->get_number_of_particles()) {
      i-= (*it)->get_number_of_particles();
    } else {
      return (*it)->get_particle(i);
    }
  }
  throw IndexException("out of range");
}



IMP_LIST_IMPL(SingletonContainerSet,
              SingletonContainer,
              singleton_container,
              SingletonContainer*,,,)

IMPCORE_END_NAMESPACE
