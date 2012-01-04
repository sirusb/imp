/**
 *  \file eigen_analysis.h
 *  \brief Principal component analysis of a set of points
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */


#ifndef IMPALGEBRA_EIGEN_ANALYSIS_H
#define IMPALGEBRA_EIGEN_ANALYSIS_H

#include "Vector3D.h"
#include "Transformation3D.h"
#include <IMP/base/log.h>

IMPALGEBRA_BEGIN_NAMESPACE
/** Represent a eigen analysis of some data.
 */
class IMPALGEBRAEXPORT PrincipalComponentAnalysis {
 public:
  PrincipalComponentAnalysis(){
    initialized_=false;}
  PrincipalComponentAnalysis(
    const Vector3D &pc1,const Vector3D &pc2,
    const Vector3D &pc3,Vector3D values,
    Vector3D centroid) : eigen_values_(values), centroid_(centroid){
    eigen_vecs_.push_back(pc1.get_unit_vector());
    eigen_vecs_.push_back(pc2.get_unit_vector());
    eigen_vecs_.push_back(pc3.get_unit_vector());
    initialized_=true;
  }
  Vector3D get_principal_component(unsigned int i) const {
    if (!initialized_){
      IMP_WARN("the PCA was not initialized"<<std::endl);}
    IMP_INTERNAL_CHECK(i<3, "index is not between 0, 1 or 2");
    return eigen_vecs_[i];
  }
  double get_principal_value(unsigned int i) const {
    if (!initialized_){
      IMP_WARN("the PCA was not initialized"<<std::endl);}
    IMP_INTERNAL_CHECK(i<3, "index is not between 0, 1 or 2");
    return eigen_values_[i];
  }
  inline Vector3D get_centroid() const {
    if (!initialized_){
      IMP_WARN("the PCA was not initialized"<<std::endl);}
   return centroid_;}
  void set_centroid(Vector3D cntr) {
   centroid_=cntr;}
  IMP_SHOWABLE(PrincipalComponentAnalysis);
  IMP_COMPARISONS(PrincipalComponentAnalysis);
protected:
  int compare(const PrincipalComponentAnalysis &o) const {
    IMP_USAGE_CHECK(!initialized_ || !o.initialized_,
                    "Cannot compare against anything other than the default"
                    " PrincipalComponentAnalysis");
    if (!initialized_ && !o.initialized_) {
      return 0;
    } else {
      return -1;
    }
  }
  Vector3Ds eigen_vecs_;
  Vector3D eigen_values_;
  Vector3D centroid_;
  bool initialized_;
};

IMP_VALUES(PrincipalComponentAnalysis, PrincipalComponentAnalyses);

//! Perform principle components analysis on a set of vectors
/** \relatesalso PrincipalComponentAnalysis
 */
IMPALGEBRAEXPORT PrincipalComponentAnalysis get_principal_components(
                              const Vector3Ds &ps);
//! Get all possible alignments of the first principle
//! component system to the second one
IMPALGEBRAEXPORT Transformation3Ds get_alignments_from_first_to_second(
                           const PrincipalComponentAnalysis pca1,
                           const PrincipalComponentAnalysis pca2);
IMPALGEBRA_END_NAMESPACE
#endif  /* IMPALGEBRA_EIGEN_ANALYSIS_H */
