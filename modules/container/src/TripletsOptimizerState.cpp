/**
 *  \file TripletsOptimizerState.cpp
 *  \brief Use a TripletModifier applied to a TripletContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/container/TripletsOptimizerState.h"
#include <utility>

IMPCONTAINER_BEGIN_NAMESPACE

TripletsOptimizerState
::TripletsOptimizerState(TripletContainer *c,
                           TripletModifier *gm,
                           std::string name):
  OptimizerState(name),
  c_(c)
{
  f_=gm;
}


void TripletsOptimizerState::update()
{
  IMP_OBJECT_LOG;
  if (!f_) return;
  IMP_LOG(TERSE, "Begin TripletsOptimizerState::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  IMP_CHECK_OBJECT(c_);
  c_->apply(f_);

  IMP_LOG(TERSE, "End TripletsOptimizerState::update" << std::endl);
}



void TripletsOptimizerState::do_show(std::ostream &) const {
}

IMPCONTAINER_END_NAMESPACE
