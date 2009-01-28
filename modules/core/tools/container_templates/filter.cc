/**
 *  \file FilteredListGroupnameContainer.cpp
 *  \brief Simple xyzr decorator.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/FilteredListGroupnameContainer.h"
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

namespace {

  class Found {
    typedef FilteredListGroupnameContainer
    ::GroupnameContainerConstIterator It;
    It b_,e_;
  public:
    Found(It b,
          It e):
      b_(b), e_(e){}
    bool operator()(Value vt) const {
      for (It c=b_; c != e_; ++c) {
        if ((*c)->get_contains_classname(vt)) return true;
      }
      return false;
    }
  };

}

FilteredListGroupnameContainer
::FilteredListGroupnameContainer(){
}

FilteredListGroupnameContainer::~FilteredListGroupnameContainer(){}

void FilteredListGroupnameContainer::add_classname(Value vt) {
  IMP_CHECK_OBJECT(this);
  // For VC which can't convert the iterators
  const FilteredListGroupnameContainer *cthis=
    const_cast<const FilteredListGroupnameContainer*>(this);
  if (!Found(cthis->groupname_containers_begin(),
             cthis->groupname_containers_end())(vt)) {
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
FilteredListGroupnameContainer
::get_contains_classname(Value vt) const {
  return std::binary_search(classnames_begin(),
                            classnames_end(), vt);
}

void FilteredListGroupnameContainer::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "FilteredListGroupnameContainer with "
      << get_number_of_classnames()
      << " classnames." << std::endl;
}

unsigned int
FilteredListGroupnameContainer::get_number_of_classnames() const {
  return data_.size();
}

Value
FilteredListGroupnameContainer::get_classname(unsigned int i) const {
  IMP_check(i < get_number_of_classnames(),
            "Out of range " << i << " for array of size " << data_.size(),
            IndexException);
  return data_[i];
}



IMP_LIST_IMPL(FilteredListGroupnameContainer,
              GroupnameContainer,
              groupname_container,
              GroupnameContainer*,,,)

IMPCORE_END_NAMESPACE
