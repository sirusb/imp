/**
 *  \file CloseBipartitePairContainer.cpp   \brief A list of ParticlePairs.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. Close rights reserved.
 *
 */

#include "IMP/core/CloseBipartitePairContainer.h"
#include <IMP/core/internal/DifferenceSingletonContainer.h>
#include <IMP/core/BoxSweepClosePairsFinder.h>
#include <IMP/core/GridClosePairsFinder.h>
#include <IMP/core/ListPairContainer.h>
#include <IMP/core/internal/pair_helpers.h>
#include <IMP/core/internal/close_pairs_helpers.h>
#include <IMP/PairModifier.h>
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

IMP_LIST_IMPL(CloseBipartitePairContainer,
              PairFilter,
              pair_filter,
              PairFilter*,
              PairFilters, obj->set_was_owned(true);,,)


CloseBipartitePairContainer
::CloseBipartitePairContainer(SingletonContainer *a,
                                   SingletonContainer *b,
                                   double distance,
                              double slack):
  internal::ListLikePairContainer("CloseBipartitePairContainer") {
  initialize(a,b, distance, slack, a->get_particle(0)->get_model(),
             internal::default_cpf());
}
CloseBipartitePairContainer
::CloseBipartitePairContainer(SingletonContainer *a,
                                   SingletonContainer *b,
                                   Model *m, double distance,
                              double slack):
  internal::ListLikePairContainer("CloseBipartitePairContainer") {
  initialize(a,b, distance, slack, m,
             internal::default_cpf());
}

CloseBipartitePairContainer
::CloseBipartitePairContainer(SingletonContainer *a,
                                   SingletonContainer *b,
                                   double distance,
                                   ClosePairsFinder *cpf,
                              double slack):
  internal::ListLikePairContainer("CloseBipartitePairContainer") {
  initialize(a,b, distance, slack, a->get_particle(0)->get_model(),
             cpf);
}
CloseBipartitePairContainer
::CloseBipartitePairContainer(SingletonContainer *a,
                                   SingletonContainer *b,
                                   Model *m, double distance,
                                   ClosePairsFinder *cpf,
                              double slack):
  internal::ListLikePairContainer("CloseBipartitePairContainer") {
  initialize(a,b, distance, slack, m,
            cpf);
}

void CloseBipartitePairContainer::initialize(SingletonContainer *a,
                                                  SingletonContainer *b,
                                                  double distance,
                                                  double slack, Model *m,
                                                  ClosePairsFinder *cpf) {
  set_model(m);
  slack_=slack;
  distance_=distance;
  a_=a; b_=b;
  cpf_=cpf;
  cpf_->set_distance(distance_+2*slack_);
  first_call_=true;
  moveda_= cpf_->get_moved_singleton_container(a_, m, slack_);
  movedb_= cpf_->get_moved_singleton_container(b_, m, slack_);
}

IMP_ACTIVE_CONTAINER_DEF(CloseBipartitePairContainer)

ParticlesTemp CloseBipartitePairContainer
::get_state_input_particles() const {
  ParticlesTemp reta(cpf_->get_input_particles(a_));
  ParticlesTemp retb(cpf_->get_input_particles(b_));
  if (get_number_of_pair_filters() >0) {
    ParticlePairsTemp all_pairs;
    for (unsigned int i=0; i< reta.size(); ++i) {
      for (unsigned int j=0; j< retb.size(); ++j) {
        all_pairs.push_back(ParticlePair(reta[i], retb[j]));
      }
    }
    for (PairFilterConstIterator it= pair_filters_begin();
         it != pair_filters_end(); ++it) {
      for (unsigned int i=0; i< all_pairs.size(); ++i) {
        ParticlesTemp cur= (*it)->get_input_particles(all_pairs[i]);
        reta.insert(reta.end(), cur.begin(), cur.end());
      }
    }
  }
  reta.insert(reta.end(), retb.begin(), retb.end());
  return reta;
}

void CloseBipartitePairContainer::do_before_evaluate() {
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(a_);
  IMP_CHECK_OBJECT(b_);
  IMP_CHECK_OBJECT(cpf_);
  internal::ListLikePairContainer::do_before_evaluate();
  if (first_call_) {
    ParticlePairsTemp t= cpf_->get_close_pairs(a_, b_);
    internal::filter_close_pairs(this, t);
    moveda_->reset();
    movedb_->reset();
    update_list(t);
    first_call_=false;
  } else {
    // hack until we have the dependency graph
    moveda_->update();
    movedb_->update();
    if (moveda_->get_number_of_particles() != 0
        || movedb_->get_number_of_particles() != 0) {
      if ((moveda_->get_number_of_particles()
           + movedb_->get_number_of_particles())
          < a_->get_number_of_particles()*.1
          + b_->get_number_of_particles()*.1) {
        ParticlePairsTemp ret0= cpf_->get_close_pairs(moveda_, movedb_);
        ParticlePairsTemp ret1= cpf_->get_close_pairs(moveda_, b_);
        ParticlePairsTemp ret2= cpf_->get_close_pairs(a_, movedb_);
        ParticlePairsTemp ret; ret.reserve(ret0.size()
                                           + ret1.size()+ret2.size());
        ret.insert(ret.end(), ret0.begin(), ret0.end());
        ret.insert(ret.end(), ret1.begin(), ret1.end());
        ret.insert(ret.end(), ret2.begin(), ret2.end());
        internal::filter_close_pairs(this, ret);
        add_to_list(ret);
        moveda_->reset_moved();
        movedb_->reset_moved();
      } else {
        ParticlePairsTemp ret= cpf_->get_close_pairs(a_, b_);
        internal::filter_close_pairs(this, ret);
        update_list(ret);
        moveda_->reset();
        movedb_->reset();
      }
    }
  }
}


void CloseBipartitePairContainer::do_after_evaluate() {
  internal::ListLikePairContainer::do_after_evaluate();
}


void CloseBipartitePairContainer::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "CloseBipartitePairContainer on "
      << *a_ << " and " << *b_ << std::endl;
}

ObjectsTemp CloseBipartitePairContainer::get_input_objects() const {
  ObjectsTemp ret(4);
  ret[0]=a_;
  ret[1]=b_;
  ret[2]=moveda_;
  ret[3]=movedb_;
  return ret;
}

IMPCORE_END_NAMESPACE
