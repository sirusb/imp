/**
 *  \file IMP/misc/DecayPairContainerOptimizerState.h
 *  \brief A pair container with a decaying list of pairs.
 *
 *  Copyright 2007-2014 IMP Inventors. All rights reserved.
 */

#ifndef IMPMISC_DECAY_PAIR_CONTAINER_OPTIMIZER_STATE_H
#define IMPMISC_DECAY_PAIR_CONTAINER_OPTIMIZER_STATE_H

#include <IMP/misc/misc_config.h>
#include <IMP/OptimizerState.h>
#include <IMP/PairContainer.h>
#include <IMP/PairContainer.h>
#include <IMP/kernel/OptimizerState.h>
#include <IMP/container/DynamicListPairContainer.h>
#include <IMP/kernel/PairPredicate.h>
#include <IMP/kernel/PairContainer.h>
#include <IMP/base/Pointer.h>

IMPMISC_BEGIN_NAMESPACE

//! Maintain a pair container with a decaying list of pairs.
/** The pair container passed to the constructor provides an initial
    list of pairs. Periodically (see the set_period() method), the
    predicate is applied to all the remaining pairs and ones for which
    it evaluates to 0 are removed.
 */
class IMPMISCEXPORT DecayPairContainerOptimizerState
    : public kernel::OptimizerState {
  base::PointerMember<kernel::PairPredicate> pred_;
  base::PointerMember<kernel::PairContainer> input_;
  base::Pointer<container::DynamicListPairContainer> output_;

 public:
  DecayPairContainerOptimizerState(
      PairPredicate *pred, const kernel::ParticlePairsTemp &initial_list,
      std::string name = "DecayPairContainerOptimizerState%1%");
  PairContainer *get_output_container() const { return output_; }
  virtual void do_update(unsigned int call_num) IMP_OVERRIDE;
  IMP_OBJECT_METHODS(DecayPairContainerOptimizerState);
};

IMPMISC_END_NAMESPACE

#endif /* IMPMISC_DECAY_PAIR_CONTAINER_OPTIMIZER_STATE_H */
