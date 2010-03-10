/**
 *  \file MinimumGroupnameRestraint.h
 *  \brief Score based on the minimum score over a set of Groupnames
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_MINMAX_GROUPNAME_RESTRAINT_H
#define IMPCONTAINER_MINMAX_GROUPNAME_RESTRAINT_H

#include "container_config.h"
#include <IMP/Restraint.h>
#include <IMP/GroupnameScore.h>
#include <IMP/GroupnameContainer.h>

IMPCONTAINER_BEGIN_NAMESPACE

//! Score based on the minmax groupname over a set of Groupnames
/** The score is evaluated for each of the Groupnames in the container
    and the value of the minmax n scores is used. That is,
    if n is 1, the value of the restraint is the value of the minmax
    score over the container.
 */
class IMPCONTAINEREXPORT MinMaxGroupnameRestraint
: public Restraint
{
  IMP::internal::OwnerPointer<GroupnameScore> f_;
  IMP::internal::OwnerPointer<GroupnameContainer> c_;
  unsigned int n_;
public:
  /** n is the number of minmax scores to use.
   */
  MinMaxGroupnameRestraint(GroupnameScore *f,
                                 GroupnameContainer *c,
                                 unsigned int n=1,
                                 std::string name
                                 ="MinMaxGroupnameRestraint %1%");

  IMP_RESTRAINT(MinMaxGroupnameRestraint);

  //! Set the number of lowest scores to use.
  void set_n(unsigned int n) { n_=n;}
};

IMP_OBJECTS(MinMaxGroupnameRestraint,MinMaxGroupnameRestraints);


IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_MINMAX_GROUPNAME_RESTRAINT_H */
