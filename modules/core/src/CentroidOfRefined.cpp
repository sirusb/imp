/**
 *  \file CentroidOfRefined.cpp
 *  \brief CentroidOf a the refined particles with a sphere.
 *
 *  Copyright 2007-2014 IMP Inventors. All rights reserved.
 */

#include "IMP/core/CentroidOfRefined.h"

#include "IMP/core/XYZR.h"
#include "IMP/core/FixedRefiner.h"
#include "IMP/core/SingletonConstraint.h"
#include "IMP/core/DerivativesToRefined.h"

IMPCORE_BEGIN_NAMESPACE

CentroidOfRefined::CentroidOfRefined(Refiner *r, FloatKey weight, FloatKeys ks)
    : refiner_(r), ks_(ks), w_(weight) {}

// compute centroid from refined particles
void CentroidOfRefined::apply_index(kernel::Model *m,
                                    kernel::ParticleIndex pi) const {
  kernel::ParticlesIndexes ps = refiner_->get_refined_indexes(m, pi);
  unsigned int n = ps.size();
  double tw = 0;
  if (w_ != FloatKey()) {
    IMP_USAGE_CHECK( m->get_has_attribute(w_, pi),
                     "Centroid particle lacks non-trivial weight key" << w_ );
    for (unsigned int i = 0; i < n; ++i) {
      IMP_USAGE_CHECK( ps[i]->get_model()->get_has_attribute(w_, ps[i]),
                       "CentroidOfRefined - Fine particle #" << i
                       << " lacks non-trivial weight key" << w_);
      tw += m->get_attribute(w_, ps[i]);
    }
    m->set_attribute(w_, pi, tw);
  } else {
    tw = 1;
  }
  for (unsigned int j = 0; j < ks_.size(); ++j) {
    double v = 0;
    for (unsigned int i = 0; i < n; ++i) {
      double w;
      if (w_ != FloatKey()) {
        w = m->get_attribute(w_, ps[i]) / tw;
      } else {
        w = Float(1.0) / n;
      }
      v += m->get_attribute(ks_[j], ps[i]) * w;
    }
    m->set_attribute(ks_[j], pi, v);
  }
}

ModelObjectsTemp CentroidOfRefined::do_get_inputs(
    kernel::Model *m, const kernel::ParticleIndexes &pis) const {
  kernel::ModelObjectsTemp ret = refiner_->get_inputs(m, pis);
  ret += IMP::kernel::get_particles(m, pis);
  for (unsigned int i = 0; i < pis.size(); ++i) {
    ret +=
        IMP::kernel::get_particles(m, refiner_->get_refined_indexes(m, pis[i]));
  }
  return ret;
}
ModelObjectsTemp CentroidOfRefined::do_get_outputs(
    kernel::Model *m, const kernel::ParticleIndexes &pis) const {
  kernel::ModelObjectsTemp ret = IMP::kernel::get_particles(m, pis);
  return ret;
}

IMP_SUMMARIZE_DECORATOR_DEF(Centroid, XYZ, XYZs,
                            (new CentroidOfRefined(ref, FloatKey(), // pre
                                                   XYZ::get_xyz_keys())),
                            (new DerivativesToRefined(ref)) // post
                            );

IMPCORE_END_NAMESPACE
