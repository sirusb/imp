/**
 *  \file MaximumPairScore.cpp  \brief Define PairScore
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#include <IMP/container/MaximumPairScore.h>
#include "IMP/algebra/internal/MinimalSet.h"

IMPCONTAINER_BEGIN_NAMESPACE

namespace {
  unsigned int next_index=0;
}

MaximumPairScore::MaximumPairScore(const PairScoresTemp &scores,
                                           unsigned int n,
                                           std::string name):
  PairScore(IMP::internal::make_object_name(name, next_index++)),
  scores_(scores),
  n_(n)
{
}


namespace {
  typedef algebra::internal::MinimalSet<double,
          PairScore*, std::greater<double> > MS;
  template <class It>
  MS find_minimal_set(It b, It e, const ParticlePair& v, unsigned int n) {
    IMP_LOG(TERSE, "Finding Maximum " << n << " of "
            << std::distance(b,e) << std::endl);
    MS bestn(n);
    for (It it= b; it != e; ++it) {
      double score= (*it)->evaluate(v, NULL);

      if (bestn.can_insert(score)) {
        bestn.insert(score, *it);
      }
    }
    return bestn;
  }
}

double MaximumPairScore::evaluate(const ParticlePair& v,
                                      DerivativeAccumulator *da) const {
  MS bestn= find_minimal_set(scores_.begin(),
                             scores_.end(), v, n_);

  double score=0;
  for (unsigned int i=0; i< bestn.size(); ++i) {
    if (da) {
      bestn[i].second->evaluate(v, da);
    }
    score+= bestn[i].first;
  }
  return score;
}

bool MaximumPairScore::get_is_changed(const ParticlePair& v) const {
  for (unsigned int i=0; i< scores_.size(); ++i) {
    if (scores_[i]->get_is_changed(v)) return true;
  }
  return false;
}


ParticlesTemp MaximumPairScore
::get_input_particles(Particle* p) const {
  ParticlesTemp ret;
  for (unsigned int i=0; i< scores_.size(); ++i) {
    ParticlesTemp c= scores_[i]->get_input_particles(p);
    ret.insert(ret.end(), c.begin(), c.end());
  }
  return ret;
}

ContainersTemp MaximumPairScore
::get_input_containers(Particle* p) const {
  ContainersTemp ret;
  for (unsigned int i=0; i< scores_.size(); ++i) {
    ContainersTemp c= scores_[i]->get_input_containers(p);
    ret.insert(ret.end(), c.begin(), c.end());
  }
  return ret;
}


void MaximumPairScore::do_show(std::ostream &out) const
{
  out << "size scores " << scores_.size() << std::endl;
}

IMPCONTAINER_END_NAMESPACE
