/**
 *  \file AlignmentParams.h
 *  \brief Parameters for alignments.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPMULTIFIT_ALIGNMENT_PARAMS_H
#define IMPMULTIFIT_ALIGNMENT_PARAMS_H

#include <vector>
#include <string>
#include "multifit_config.h"
#include <IMP/base/value_macros.h>
#include <iostream>
#include <stdio.h>

IMPMULTIFIT_BEGIN_NAMESPACE

struct DominoParams {
  DominoParams() : max_value_threshold_(10.),
                   max_anchor_penetration_(0.1),
                   heap_size_(10000),cache_size_(50000){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%f %d %f %d %d", &max_value_threshold_,
              &max_num_states_for_subset_,
              &max_anchor_penetration_,&heap_size_,&cache_size_) != 5)
      return false;
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"domino parameters: max_val_thr="<<max_value_threshold_
     <<" max_num_states4subset="<<max_num_states_for_subset_
     <<" max_anchor_penetration="<<max_anchor_penetration_
     <<" heap_size="<<heap_size_
     <<"cache_size="<<cache_size_<<std::endl;
  }
  float max_value_threshold_;
  int max_num_states_for_subset_;
  float max_anchor_penetration_;
  int heap_size_;
  int cache_size_;
};
IMP_VALUES(DominoParams, DominoParamsList);

struct XlinkParams {
  XlinkParams() : upper_bound_(10.),
                  k_(0.05),
                  max_xlink_val_(3.),
                  treat_between_residues_(true)
                  {}
  bool add(const std::string str) {
    int treat_between_residues;
    int num_scanned=sscanf(str.c_str(), "%f %f %f %d",
              &upper_bound_,
              &k_,
              &max_xlink_val_,
              &treat_between_residues);
    if( num_scanned != 4) {
      std::cout<<str<<std::endl;
      std::cout<<"number scanned:"<<num_scanned<<std::endl;
      return false;
    }
    if (treat_between_residues==1) treat_between_residues_=true; else
      treat_between_residues_=false;
    return true;
  }

  void show(std::ostream& s=std::cout) const{
    s<<"xlink parameters: upper_bound:"<<upper_bound_<<" k:"<<k_<<
      "max_xlink_val_="<<max_xlink_val_
     <<" treat_between_residues_="<<treat_between_residues_
     <<std::endl;
  }
  float upper_bound_,k_;
  float max_xlink_val_;
  bool treat_between_residues_;
};
IMP_VALUES(XlinkParams, XlinkParamsList);

struct ConnectivityParams {
  ConnectivityParams() : upper_bound_(10.),
                      k_(0.05),//corresponding to 3.5A
                      max_conn_rest_val_(2.){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%f %f %f",
              &upper_bound_,&k_,&max_conn_rest_val_)
       != 3) {
      std::cerr<<"Connectivity parameters can not be read. Default parameters"
               << " are used"<<std::endl;
      return false;
    }
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"connectivity parameters: upper_bound="<<upper_bound_
     <<" k="<<k_
     <<" max_conn_val="<<max_conn_rest_val_;
  }
  //upper bound for the connectivity restraint
  float upper_bound_;
  //k used for the connectivity restraint
  float k_;
  //maximum value allowed for the protein connectivity restraint
  float max_conn_rest_val_;
};
IMP_VALUES(ConnectivityParams, ConnectivityParamsList);


struct FragmentsParams {
  FragmentsParams() : frag_len_(30),bead_radius_scale_(1.),
                      load_atomic_(false),subunit_rigid_(false){}
  bool add(const std::string str) {
    int atomic,rigid;
    if(sscanf(str.c_str(), "%d %f %d %d",
              &frag_len_,&bead_radius_scale_,
              &atomic,&rigid)
       != 4) {
      std::cerr<<"Fragments can not be read. Default paramrs are used"
               <<std::endl;
      return false;
    }
    if (atomic==1) load_atomic_=true; else load_atomic_=false;
    if (rigid==1) subunit_rigid_=true; else subunit_rigid_=false;
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"fragment parameters: frag_len="<<frag_len_
     <<" bead_rad_scale="<<bead_radius_scale_
     <<" load_atomic="<<load_atomic_
     <<" rigid="<<subunit_rigid_;
  }
  //number of residues for each fragment
  int frag_len_;
  //approximate radius value will be multiplied by this number
  float bead_radius_scale_;
  //true if we should load atomic structures
  bool load_atomic_;
  //true if the subunits are rigid
  bool subunit_rigid_;
};
IMP_VALUES(FragmentsParams, FragmentsParamsList);

struct RogParams {
public:
  RogParams() : max_score_(5),scale_(1.6){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%f %f", &scale_,&max_score_) != 2)
      return false;
    return true;
  }
  float get_max_score()const{return max_score_;}
  float get_scale() const {return scale_;}
  void show(std::ostream& s=std::cout) const{
    s<<"rog params: scale="<<scale_<<" max_score:"<<max_score_<<std::endl;
  }
private:
  //maximum score
  float max_score_,scale_;
};
IMP_VALUES(RogParams, RogParamsList);

struct EVParams {
public:
  EVParams():pair_distance_(3.),
             pair_slack_(1.),
             hlb_mean_(2.),
             hlb_k_(0.59),
             maximum_ev_score_for_pair_(0.3),
             allowed_percentage_of_bad_pairs_(0.05),
             scoring_mode_(1){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%f %f %f %f %f %f %d", &pair_distance_,
              &pair_slack_,&hlb_mean_,&hlb_k_,&maximum_ev_score_for_pair_,
              &allowed_percentage_of_bad_pairs_,
              &scoring_mode_) != 7)
      return false;
    //possible scoring modes are 0-2
    if (scoring_mode_<0)
      return false;
    if (scoring_mode_>2)
      return false;
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"EV params: pair_distance="<<pair_distance_<<
      " pair_slack="<<pair_slack_<<
      " mean="<<hlb_mean_<<" k="<<hlb_k_<<
      " maximum_ev_score_for_pair:"<<maximum_ev_score_for_pair_<<
      " allowed_percentage_of_bad_pairs:"<<allowed_percentage_of_bad_pairs_<<
      " scoing_mode:"<<scoring_mode_<<std::endl;
  }
  float pair_distance_,pair_slack_,hlb_mean_,hlb_k_,maximum_ev_score_for_pair_;
  float allowed_percentage_of_bad_pairs_;
  int scoring_mode_;//0 means EV restraints are OFF
                    //1 means EV between all pairs is calculated
                    //2 means EV only between selected pairs is calculated
};
IMP_VALUES(EVParams, EVParamsList);

struct FiltersParams {
public:
  FiltersParams() : max_num_violated_xlink_(4),
                    max_num_violated_conn_(4),
                    max_num_violated_ev_(3){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%d %d %d", &max_num_violated_conn_,
              &max_num_violated_xlink_,
              &max_num_violated_ev_) != 3)
      return false;
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"filters params: max_num_violated_xlink="<<max_num_violated_xlink_
     <<" max_num_violated_conn:"<<max_num_violated_conn_
     <<"max num_violated_ev:"<<max_num_violated_ev_<<std::endl;
  }
public:
  int max_num_violated_xlink_,max_num_violated_conn_;
  int max_num_violated_ev_;
};
IMP_VALUES(FiltersParams, FiltersParamsList);



struct FittingParams {
public:
  FittingParams() : pca_max_angle_diff_(15.),
    pca_max_size_diff_(10.),
    pca_max_cent_dist_diff_(10.), max_asmb_fit_score_(.5){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%f %f %f %f",
     &pca_max_angle_diff_,
     &pca_max_size_diff_,
     &pca_max_cent_dist_diff_,
    &max_asmb_fit_score_) != 4)
      return false;
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"filters params: pca_max_angle_diff="<<pca_max_angle_diff_
     <<" pca_max_size_diff:"<<pca_max_size_diff_
     <<"pca_max_cent_dist_diff:"<<pca_max_cent_dist_diff_
     <<"max_asmb_fit_score:"<<max_asmb_fit_score_<<std::endl;
  }
public:
  float pca_max_angle_diff_,pca_max_size_diff_;
  float pca_max_cent_dist_diff_,max_asmb_fit_score_;
};
IMP_VALUES(FittingParams, FittingParamsList);


struct ComplementarityParams {
public:
  ComplementarityParams() : max_score_(100000),
    max_penetration_(200),
    interior_layer_thickness_(2),
    boundary_coef_(-3),comp_coef_(1),penetration_coef_(2){}
  bool add(const std::string str) {
    if(sscanf(str.c_str(), "%f %f %f %f %f %f",
     &max_score_,
     &max_penetration_,
     &interior_layer_thickness_,
     &boundary_coef_,
     &comp_coef_,
    &penetration_coef_) != 6)
      return false;
    return true;
  }
  void show(std::ostream& s=std::cout) const{
    s<<"complementarity params: max_score="<<max_score_
     <<" max penetration:"<<max_penetration_
     <<" interior layer thickness:"<<interior_layer_thickness_
     <<" boundary coeffiecent:"<<boundary_coef_
     <<" complementarity coefficient: "<<comp_coef_
     <<" penetration coefficient: "<<penetration_coef_<<std::endl;
  }
public:
  float max_score_,max_penetration_;
  float interior_layer_thickness_;
  float boundary_coef_, comp_coef_, penetration_coef_;
};
IMP_VALUES(ComplementarityParams, ComplementarityParamsList);

class IMPMULTIFITEXPORT AlignmentParams {
 public:
  AlignmentParams(const char* param_filename);
  bool process_parameters();
  const DominoParams& get_domino_params() const { return domino_params_;}
  const FittingParams& get_fitting_params() const { return fitting_params_;}
  const ComplementarityParams& get_complementarity_params() const {
    return complementarity_params_;}
  const XlinkParams& get_xlink_params() const { return xlink_params_;}
  const ConnectivityParams& get_connectivity_params() const {
    return conn_params_;
  }
  const RogParams& get_rog_params() const { return rog_params_;}
  const FragmentsParams& get_fragments_params() const {
    return fragments_params_;
  }
  const FiltersParams& get_filters_params() const { return filters_params_;}
  const EVParams& get_ev_params() const { return ev_params_;}
  void show(std::ostream& s=std::cout) const {
    s<<"alignment parameters"<<std::endl;
    fitting_params_.show(s);s<<std::endl;
    complementarity_params_.show(s);s<<std::endl;
    domino_params_.show(s);s<<std::endl;
    fragments_params_.show(s);s<<std::endl;
    rog_params_.show(s);s<<std::endl;
    conn_params_.show(s);s<<std::endl;
    xlink_params_.show(s);s<<std::endl;
    filters_params_.show(s);s<<std::endl;
    ev_params_.show(s);s<<std::endl;
}
private:
  void get_parameters();
  bool parse_parameters_strings();

private:
  std::string rog_str_;
  std::string domino_str_;
  DominoParams domino_params_;
  std::string fitting_str_;
  FittingParams fitting_params_;
  std::string complementarity_str_;
  ComplementarityParams complementarity_params_;
  std::string xlink_str_;
  XlinkParams xlink_params_;
  std::string conn_str_;
  ConnectivityParams conn_params_;
  std::string fragments_str_;
  FragmentsParams fragments_params_;
  RogParams rog_params_;
  std::string filters_str_;
  FiltersParams filters_params_;
  std::string ev_str_;
  EVParams ev_params_;
};
IMP_VALUES(AlignmentParams, AlignmentParamsList);

IMPMULTIFIT_END_NAMESPACE

#endif  /* IMPMULTIFIT_ALIGNMENT_PARAMS_H */