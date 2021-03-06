/**
 *  \file rigid_pair_score.h
 *  \brief utilities for rigid pair scores.
 *
 *  Copyright 2007-2014 IMP Inventors. All rights reserved.
 */

#ifndef IMPCORE_INTERNAL_SINKS_H
#define IMPCORE_INTERNAL_SINKS_H

#include <IMP/core/core_config.h>
#include "IMP/Particle.h"
#include "IMP/PairPredicate.h"
#include "rigid_body_tree.h"
#include <boost/unordered_map.hpp>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

inline bool get_filters_contains(kernel::Model *m,
                                 const PairPredicates &filters,
                                 kernel::ParticleIndexPair pip) {
  for (unsigned int i = 0; i < filters.size(); ++i) {
    if (filters[i]->get_value_index(m, pip)) return true;
  }
  return false;
}

struct ParticleSink {
  kernel::Model *m_;
  kernel::ParticlesTemp &out_;
  ParticleSink(kernel::Model *m, kernel::ParticlesTemp &out)
      : m_(m), out_(out) {}
  typedef kernel::ParticleIndex argument_type;
  bool operator()(kernel::ParticleIndex c) {
    out_.push_back(m_->get_particle(c));
    return true;
  }
  void check_contains(kernel::ParticleIndex c) const {
    if (std::find(out_.begin(), out_.end(), m_->get_particle(c)) ==
        out_.end()) {
      IMP_INTERNAL_CHECK(
          false, "Particle " << c << " not found in list. Coordinates are "
                             << XYZR(m_, c));
    }
  }
};

struct ParticlePairSink {
  kernel::Model *m_;
  const PairPredicates &filters_;
  kernel::ParticlePairsTemp &out_;
  ParticlePairSink(kernel::Model *m, const PairPredicates &filters,
                   kernel::ParticlePairsTemp &out)
      : m_(m), filters_(filters), out_(out) {}
  bool add(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    if (get_filters_contains(m_, filters_, kernel::ParticleIndexPair(a, b)))
      return false;
    out_.push_back(
        kernel::ParticlePair(m_->get_particle(a), m_->get_particle(b)));
    return true;
  }
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    add(a, b);
    return true;
  }
  void check_contains(kernel::ParticleIndex a, kernel::ParticleIndex b) const {
    if (get_filters_contains(m_, filters_, kernel::ParticleIndexPair(a, b)))
      return;
    kernel::ParticlePair pp(m_->get_particle(a), m_->get_particle(b));
    kernel::ParticlePair opp(m_->get_particle(b), m_->get_particle(a));
    if (std::find(out_.begin(), out_.end(), pp) == out_.end() &&
        std::find(out_.begin(), out_.end(), opp) == out_.end()) {
      IMP_INTERNAL_CHECK(false, "Particle pair "
                                    << pp[0]->get_name() << ", "
                                    << pp[1]->get_name()
                                    << " not found in list. Coordinates are "
                                    << XYZR(pp[0]) << " and " << XYZR(pp[1])
                                    << " and distance is "
                                    << get_distance(XYZR(pp[0]), XYZR(pp[1])));
    }
  }
};

struct ParticleIndexPairSink {
  kernel::Model *m_;
  const PairPredicates &filters_;
  kernel::ParticleIndexPairs &out_;
  ParticleIndexPairSink(kernel::Model *m, const PairPredicates &filters,
                        kernel::ParticleIndexPairs &out)
      : m_(m), filters_(filters), out_(out) {}
  bool add(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    if (get_filters_contains(m_, filters_, kernel::ParticleIndexPair(a, b)))
      return false;
    out_.push_back(kernel::ParticleIndexPair(a, b));
    return true;
  }
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    add(a, b);
    return true;
  }
  void check_contains(kernel::ParticleIndex a, kernel::ParticleIndex b) const {
    if (get_filters_contains(m_, filters_, kernel::ParticleIndexPair(a, b)))
      return;
    kernel::ParticleIndexPair pp(a, b);
    kernel::ParticleIndexPair opp(b, a);
    if (std::find(out_.begin(), out_.end(), pp) == out_.end() &&
        std::find(out_.begin(), out_.end(), opp) == out_.end()) {
      IMP_INTERNAL_CHECK(false, "Particle pair " << pp
                                                 << " not found in list.");
    }
  }
};

template <class PS>
struct ParticlePairSinkWithMax : public ParticlePairSink {
  double &score_;
  double max_;
  PS *ssps_;
  DerivativeAccumulator *da_;
  ParticlePairSinkWithMax(kernel::Model *m, const PairPredicates &filters,
                          kernel::ParticlePairsTemp &out, PS *ssps,
                          DerivativeAccumulator *da, double &score, double max)
      : ParticlePairSink(m, filters, out),
        score_(score),
        max_(max),
        ssps_(ssps),
        da_(da) {}
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    if (!ParticlePairSink::add(a, b)) return true;
    double cur = ssps_->PS::evaluate(
        kernel::ParticlePair(m_->get_particle(a), m_->get_particle(b)), da_);
    max_ -= cur;
    score_ += cur;
    if (max_ < 0) {
      return false;
    }
    return true;
  }
};

template <class PS>
struct ParticleIndexPairSinkWithMax : public ParticleIndexPairSink {
  double &score_;
  double max_;
  PS *ssps_;
  DerivativeAccumulator *da_;
  ParticleIndexPairSinkWithMax(kernel::Model *m, const PairPredicates &filters,
                               kernel::ParticleIndexPairs &out, PS *ssps,
                               DerivativeAccumulator *da, double &score,
                               double max)
      : ParticleIndexPairSink(m, filters, out),
        score_(score),
        max_(max),
        ssps_(ssps),
        da_(da) {}
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    if (!ParticleIndexPairSink::add(a, b)) return true;
    double cur = ssps_->PS::evaluate_index(
        ParticleIndexPairSink::m_, kernel::ParticleIndexPair(a, b), da_);
    max_ -= cur;
    score_ += cur;
    if (max_ < 0) {
      return false;
    }
    return true;
  }
};

struct HalfParticlePairSink : public ParticlePairSink {
  kernel::ParticleIndex p_;
  HalfParticlePairSink(kernel::Model *m, const PairPredicates &filters,
                       kernel::ParticlePairsTemp &out, kernel::ParticleIndex p)
      : ParticlePairSink(m, filters, out), p_(p) {}
  bool operator()(kernel::ParticleIndex c) {
    return ParticlePairSink::operator()(p_, c);
  }
  void check_contains(kernel::ParticleIndex c) const {
    ParticlePairSink::check_contains(p_, c);
  }
};

struct HalfParticleIndexPairSink : public ParticleIndexPairSink {
  kernel::ParticleIndex p_;
  HalfParticleIndexPairSink(kernel::Model *m, const PairPredicates &filters,
                            kernel::ParticleIndexPairs &out,
                            kernel::ParticleIndex p)
      : ParticleIndexPairSink(m, filters, out), p_(p) {}
  bool operator()(kernel::ParticleIndex c) {
    return ParticleIndexPairSink::operator()(p_, c);
  }
  void check_contains(kernel::ParticleIndex c) const {
    ParticleIndexPairSink::check_contains(p_, c);
  }
};

struct SwappedHalfParticleIndexPairSink : public ParticleIndexPairSink {
  kernel::ParticleIndex p_;
  SwappedHalfParticleIndexPairSink(kernel::Model *m,
                                   const PairPredicates &filters,
                                   kernel::ParticleIndexPairs &out,
                                   kernel::ParticleIndex p)
      : ParticleIndexPairSink(m, filters, out), p_(p) {}
  bool operator()(kernel::ParticleIndex c) {
    return ParticleIndexPairSink::operator()(c, p_);
  }
  void check_contains(kernel::ParticleIndex c) const {
    ParticleIndexPairSink::check_contains(c, p_);
  }
};

template <class PS>
struct HalfParticlePairSinkWithMax : public ParticlePairSinkWithMax<PS> {
  kernel::ParticleIndex p_;
  HalfParticlePairSinkWithMax(kernel::Model *m, const PairPredicates &filters,
                              kernel::ParticlePairsTemp &out, PS *ssps,
                              DerivativeAccumulator *da, double &score,
                              double max, kernel::ParticleIndex p)
      : ParticlePairSinkWithMax<PS>(m, filters, out, ssps, da, score, max),
        p_(p) {}
  bool operator()(kernel::ParticleIndex c) {
    return ParticlePairSinkWithMax<PS>::operator()(p_, c);
  }
  void check_contains(kernel::ParticleIndex c) const {
    ParticlePairSinkWithMax<PS>::check_contains(p_, c);
  }
};

template <class PS>
struct HalfParticleIndexPairSinkWithMax
    : public ParticleIndexPairSinkWithMax<PS> {
  kernel::ParticleIndex p_;
  HalfParticleIndexPairSinkWithMax(kernel::Model *m,
                                   const PairPredicates &filters,
                                   kernel::ParticleIndexPairs &out, PS *ssps,
                                   DerivativeAccumulator *da, double &score,
                                   double max, kernel::ParticleIndex p)
      : ParticleIndexPairSinkWithMax<PS>(m, filters, out, ssps, da, score, max),
        p_(p) {}
  bool operator()(kernel::ParticleIndex c) {
    return ParticleIndexPairSinkWithMax<PS>::operator()(p_, c);
  }
  void check_contains(kernel::ParticleIndex c) const {
    ParticleIndexPairSinkWithMax<PS>::check_contains(p_, c);
  }
};

struct RigidBodyRigidBodyParticleIndexPairSink : public ParticleIndexPairSink {
  ObjectKey key_;
  double dist_;
  const boost::unordered_map<kernel::ParticleIndex, kernel::ParticleIndexes> &
      map_;
  RigidBodyRigidBodyParticleIndexPairSink(
      kernel::Model *m, const PairPredicates &filters,
      kernel::ParticleIndexPairs &out, ObjectKey key, double dist,
      const boost::unordered_map<kernel::ParticleIndex,
                                 kernel::ParticleIndexes> &map)
      : ParticleIndexPairSink(m, filters, out),
        key_(key),
        dist_(dist),
        map_(map) {}
  RigidBodyHierarchy *get_hierarchy(kernel::ParticleIndex p) const {
    RigidBody rb(m_, p);
    return get_rigid_body_hierarchy(rb, map_.find(p)->second, key_);
  }
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    IMP_LOG_VERBOSE("Processing intersection between " << a << " and " << b
                                                       << std::endl);
    fill_close_pairs(m_, get_hierarchy(a), get_hierarchy(b), dist_,
                     static_cast<ParticleIndexPairSink>(*this));
    return true;
  }
  void check_contains(kernel::ParticleIndex, kernel::ParticleIndex) const {
    // can't look for root pair, too lazy to check for actual pairs
  }
};

struct RigidBodyParticleParticleIndexPairSink : public ParticleIndexPairSink {
  ObjectKey key_;
  double dist_;
  const boost::unordered_map<kernel::ParticleIndex, kernel::ParticleIndexes> &
      map_;
  RigidBodyParticleParticleIndexPairSink(
      kernel::Model *m, const PairPredicates &filters,
      kernel::ParticleIndexPairs &out, ObjectKey key, double dist,
      const boost::unordered_map<kernel::ParticleIndex,
                                 kernel::ParticleIndexes> &map)
      : ParticleIndexPairSink(m, filters, out),
        key_(key),
        dist_(dist),
        map_(map) {}
  RigidBodyHierarchy *get_hierarchy(kernel::ParticleIndex p) const {
    RigidBody rb(m_, p);
    return get_rigid_body_hierarchy(rb, map_.find(p)->second, key_);
  }
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    IMP_LOG_VERBOSE("Processing rb-p intersection between " << a << " and " << b
                                                            << std::endl);
    SwappedHalfParticleIndexPairSink hps(m_, filters_, out_, b);
    fill_close_particles(m_, get_hierarchy(a), b, dist_, hps);
    return true;
  }
  void check_contains(kernel::ParticleIndex, kernel::ParticleIndex) const {
    // can't look for root pair, too lazy to check for actual pairs
  }
};

struct ParticleRigidBodyParticleIndexPairSink : public ParticleIndexPairSink {
  ObjectKey key_;
  double dist_;
  const boost::unordered_map<kernel::ParticleIndex, kernel::ParticleIndexes> &
      map_;
  ParticleRigidBodyParticleIndexPairSink(
      kernel::Model *m, const PairPredicates &filters,
      kernel::ParticleIndexPairs &out, ObjectKey key, double dist,
      const boost::unordered_map<kernel::ParticleIndex,
                                 kernel::ParticleIndexes> &map)
      : ParticleIndexPairSink(m, filters, out),
        key_(key),
        dist_(dist),
        map_(map) {}
  RigidBodyHierarchy *get_hierarchy(kernel::ParticleIndex p) const {
    RigidBody rb(m_, p);
    return get_rigid_body_hierarchy(rb, map_.find(p)->second, key_);
  }
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    IMP_LOG_VERBOSE("Processing p-rb intersection between " << a << " and " << b
                                                            << std::endl);
    HalfParticleIndexPairSink hps(m_, filters_, out_, a);
    fill_close_particles(m_, get_hierarchy(b), a, dist_, hps);
    return true;
  }
  void check_contains(kernel::ParticleIndex, kernel::ParticleIndex) const {
    // can't look for root pair, too lazy to check for actual pairs
  }
};

template <class PS>
struct RigidBodyParticleIndexPairSinkWithMax
    : public ParticleIndexPairSinkWithMax<PS> {
  ObjectKey key_;
  double dist_;
  const boost::unordered_map<kernel::ParticleIndex, kernel::ParticleIndexes> &
      map_;
  RigidBodyParticleIndexPairSinkWithMax(
      kernel::Model *m, const PairPredicates &filters,
      kernel::ParticleIndexPairs &out, PS *ssps, DerivativeAccumulator *da,
      double &score, double max, ObjectKey key, double dist,
      const boost::unordered_map<kernel::ParticleIndex,
                                 kernel::ParticleIndexes> &map)
      : ParticleIndexPairSinkWithMax<PS>(m, filters, out, ssps, da, score, max),
        key_(key),
        dist_(dist),
        map_(map) {}
  RigidBodyHierarchy *get_hierarchy(kernel::ParticleIndex p) const {
    RigidBody rb(ParticleIndexPairSinkWithMax<PS>::m_, p);
    return get_rigid_body_hierarchy(rb, map_.find(p)->second, key_);
  }
  void check_contains(kernel::ParticleIndex, kernel::ParticleIndex) const {
    // can't look for root pair, too lazy to check for actual pairs
  }
};

template <class PS>
struct RigidBodyRigidBodyParticleIndexPairSinkWithMax
    : public RigidBodyParticleIndexPairSinkWithMax<PS> {
  typedef RigidBodyParticleIndexPairSinkWithMax<PS> P;
  RigidBodyRigidBodyParticleIndexPairSinkWithMax(
      kernel::Model *m, const PairPredicates &filters,
      kernel::ParticleIndexPairs &out, PS *ssps, DerivativeAccumulator *da,
      double &score, double max, ObjectKey key, double dist,
      const boost::unordered_map<kernel::ParticleIndex,
                                 kernel::ParticleIndexes> &map)
      : P(m, filters, out, ssps, da, score, max, key, dist, map) {}
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    fill_close_pairs(
        P::m_, RigidBodyParticleIndexPairSinkWithMax<PS>::get_hierarchy(a),
        RigidBodyParticleIndexPairSinkWithMax<PS>::get_hierarchy(b), P::dist_,
        static_cast<ParticleIndexPairSinkWithMax<PS> >(*this));
    return P::max_ > 0;
  }
  void check_contains(kernel::ParticleIndex, kernel::ParticleIndex) const {
    // can't look for root pair, too lazy to check for actual pairs
  }
};

template <class PS>
struct RigidBodyParticleParticleIndexPairSinkWithMax
    : public RigidBodyParticleIndexPairSinkWithMax<PS> {
  typedef RigidBodyParticleIndexPairSinkWithMax<PS> P;
  RigidBodyParticleParticleIndexPairSinkWithMax(
      kernel::Model *m, const PairPredicates &filters,
      kernel::ParticleIndexPairs &out, PS *ssps, DerivativeAccumulator *da,
      double &score, double max, ObjectKey key, double dist,
      const boost::unordered_map<kernel::ParticleIndex,
                                 kernel::ParticleIndexes> &map)
      : P(m, filters, out, ssps, da, score, max, key, dist, map) {}
  bool operator()(kernel::ParticleIndex a, kernel::ParticleIndex b) {
    fill_close_particles(P::m_, P::get_hierarchy(a), b, P::dist_,
                         HalfParticleIndexPairSinkWithMax<PS>(
                             P::m_, P::filters_, P::out_, P::ssps_, P::da_,
                             P::score_, P::max_, b));
    return P::max_ > 0;
  }
  void check_contains(kernel::ParticleIndex, kernel::ParticleIndex) const {
    // can't look for root pair, too lazy to check for actual pairs
  }
};

IMPCORE_END_INTERNAL_NAMESPACE

#endif /* IMPCORE_INTERNAL_SINKS_H */
