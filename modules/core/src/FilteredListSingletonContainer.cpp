/**
 *  \file FilteredListSingletonContainer.cpp
 *  \brief Simple xyzr decorator.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/FilteredListSingletonContainer.h"
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

namespace {

  class Found {
    typedef FilteredListSingletonContainer
    ::SingletonContainerConstIterator It;
    It b_,e_;
  public:
    Found(It b,
          It e):
      b_(b), e_(e){}
    bool operator()(Particle* vt) const {
      for (It c=b_; c != e_; ++c) {
        if ((*c)->get_contains_particle(vt)) return true;
      }
      return false;
    }
  };

}

FilteredListSingletonContainer
::FilteredListSingletonContainer(){
}

FilteredListSingletonContainer::~FilteredListSingletonContainer(){}

void FilteredListSingletonContainer::add_particle(Particle* vt) {
  IMP_CHECK_OBJECT(this);
  // For VC which can't convert the iterators
  const FilteredListSingletonContainer *cthis=
    const_cast<const FilteredListSingletonContainer*>(this);
  if (!Found(cthis->singleton_containers_begin(),
             cthis->singleton_containers_end())(vt)) {
    data_.insert(std::upper_bound(data_.begin(),
                                  data_.end(), vt), vt);
  }
  IMP_IF_CHECK(EXPENSIVE) {
    for (unsigned int i=1; i< data_.size(); ++i) {
      IMP_assert(data_[i-1] < data_[i], "Poorly sorted list at position "
                 << i);
    }
  }
}

bool
FilteredListSingletonContainer
::get_contains_particle(Particle* vt) const {
  return std::binary_search(particles_begin(),
                            particles_end(), vt);
}

void FilteredListSingletonContainer::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "FilteredListSingletonContainer with "
      << get_number_of_particles()
      << " particles." << std::endl;
}

unsigned int
FilteredListSingletonContainer::get_number_of_particles() const {
  return data_.size();
}

Particle*
FilteredListSingletonContainer::get_particle(unsigned int i) const {
  IMP_check(i < get_number_of_particles(),
            "Out of range " << i << " for array of size " << data_.size(),
            IndexException);
  return data_[i];
}



IMP_LIST_IMPL(FilteredListSingletonContainer,
              SingletonContainer,
              singleton_container,
              SingletonContainer*,,,)

IMPCORE_END_NAMESPACE
