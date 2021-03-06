/**
 *  \file IMP/core/ExcludedVolumeRestraint.h
 *  \brief Prevent spheres from inter-penetrating
 *
 *  Copyright 2007-2014 IMP Inventors. All rights reserved.
 */

#ifndef IMPCORE_EXCLUDED_VOLUME_RESTRAINT_H
#define IMPCORE_EXCLUDED_VOLUME_RESTRAINT_H

#include <IMP/core/core_config.h>

#include "internal/remove_pointers.h"
#include "rigid_bodies.h"
#include <IMP/PairContainer.h>
#include <IMP/SingletonContainer.h>
#include <IMP/kernel/Restraint.h>
#include <IMP/UnaryFunction.h>
#include <IMP/Refiner.h>
#include "RigidClosePairsFinder.h"
#include <IMP/core/SphereDistancePairScore.h>
#include <boost/unordered_map.hpp>

IMPCORE_BEGIN_NAMESPACE

//! Prevent a set of particles and rigid bodies from inter-penetrating
/** Given an arbitrary collection of particles and rigid bodies, this
    restraint prevents the particles from interpenetrating. Such restraints
    are also known as steric clash restraints.

    \note Pairs of particles within a single rigid body are not restrained
    (and are ignored in the returned score).

    \note See IMP::container::ClosePairContainer for more information about
    close pair based scoring functions and \imp.

    \note Changing the set of particles in the SingletonContainer is not
    currently supported after the first evaluate call.
 */
class IMPCOREEXPORT ExcludedVolumeRestraint : public kernel::Restraint {
  base::PointerMember<SingletonContainer> sc_;
  mutable kernel::ParticleIndexPairs cur_list_;
  mutable bool was_bad_;
  mutable bool initialized_;
  ObjectKey key_;
  IMP::base::PointerMember<SoftSpherePairScore> ssps_;
  // moved stuff
  mutable kernel::ParticleIndexes rbs_;
  mutable kernel::ParticleIndexes xyzrs_;
  mutable boost::unordered_map<kernel::ParticleIndex, kernel::ParticleIndexes>
      constituents_;
  double slack_;
  mutable algebra::Sphere3Ds rbs_backup_sphere_;
  mutable algebra::Rotation3Ds rbs_backup_rot_;
  mutable algebra::Sphere3Ds xyzrs_backup_;

  void reset_moved() const;
  void initialize() const;
  bool get_if_moved() const;
  void fill_list() const;
  double fill_list_if_good(double max) const;
  ExcludedVolumeRestraint(SingletonContainerAdaptor sc,
                          SoftSpherePairScore *ssps, ObjectKey ok,
                          double slack = 10);

 public:
  /** The SingletonContainer contains a set of XYZR particles and RigidMembers.

      The slack is how far the particles must move before the list of close
      pairs is computed. It does not matter for correctness, just running
      time. You may want to fiddle with it or use the
      IMP::container::get_slack_estimate() function.

      The spring constant used is k.*/
  ExcludedVolumeRestraint(SingletonContainerAdaptor sc, double k = 1,
                          double slack = 10,
                          std::string name = "ExcludedVolumeRestraint%1%");

  void clear_caches();

#if !defined(IMP_DOXYGEN) && !defined(SWIG)
  double unprotected_evaluate_if_good(DerivativeAccumulator *da,
                                      double max) const;
#endif
 public:
  double unprotected_evaluate(IMP::DerivativeAccumulator *accum) const
      IMP_OVERRIDE;
  IMP::kernel::ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  IMP_OBJECT_METHODS(ExcludedVolumeRestraint);
  ;
  kernel::Restraints do_create_decomposition() const;
  kernel::Restraints do_create_current_decomposition() const;
#ifndef IMP_DOXYGEN
  const kernel::ParticleIndexPairs &get_indexes() const { return cur_list_; }
#endif
  IMP_LIST_ACTION(public, PairFilter, PairFilters, pair_filter, pair_filters,
                  PairPredicate *, PairPredicates, , , );
};

IMPCORE_END_NAMESPACE

#endif /* IMPCORE_EXCLUDED_VOLUME_RESTRAINT_H */
