/**
 *  \file ListSingletonContainer.cpp   \brief A list of ParticlesTemp.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/core/internal/singleton_helpers.h>
#include <IMP/SingletonModifier.h>
#include <IMP/SingletonScore.h>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

void ListLikeSingletonContainer
::do_show(std::ostream &out) const {
  out << "contains " << data_.size() << std::endl;
}



IMPCORE_END_INTERNAL_NAMESPACE
