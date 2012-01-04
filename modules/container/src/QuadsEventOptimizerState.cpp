/**
 *  \file QuadPredicate.cpp  \brief Define QuadPredicate
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#include <IMP/container/QuadsEventOptimizerState.h>
#include <IMP/Optimizer.h>
IMPCONTAINER_BEGIN_NAMESPACE
QuadsEventOptimizerState::
QuadsEventOptimizerState(QuadPredicate *pred,
                              QuadContainer *container,
                              int value, int min_count, int max_count,
                              std::string name):
    OptimizerState(name), pred_(pred), container_(container),
    v_(value), min_(min_count), max_(max_count) {}


void QuadsEventOptimizerState::do_show(std::ostream &out) const {
  out << "value: " << v_ << std::endl;
  out << "range: [" << min_ << "..." << max_ << ")" << std::endl;
}

void QuadsEventOptimizerState::update() {
  int met=0;
  Model *m=get_optimizer()->get_model();
  IMP_FOREACH_QUAD_INDEX(container_,
                               if (pred_->get_value_index(m, _1)==v_) {
                                 ++met;
                               });
  if (met >= min_ && met < max_) {
    throw IMP::base::EventException("an event occurred");
  }
}
IMPCONTAINER_END_NAMESPACE
