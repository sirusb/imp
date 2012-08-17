/**
 *  \file IMP/declare_ScoringFunction.h
 *  \brief Storage of a model, its restraints,
 *                         constraints and particles.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPKERNEL_DECLARE_SCORING_FUNCTION_H
#define IMPKERNEL_DECLARE_SCORING_FUNCTION_H

#include "kernel_config.h"
#include "base_types.h"
#include "dependency_graph.h"
#include "declare_Restraint.h"
#include "ModelObject.h"
#include "model_object_macros.h"
#include <IMP/base/InputAdaptor.h>
#include <IMP/base/tracking.h>
#include <IMP/base/Pointer.h>

#include <limits>


IMP_BEGIN_NAMESPACE
class Model;

/** A ScoringFunction represents a scoring function on the model.
    The Model has a default scoring function
    (Model::get_model_scoring_function()), but it can be useful to use
    others in different contexts during a samping process.

The evaluate process looks like:
1. determine set of ScoreState objects needed by the Restraint objects
being evaluated (this is cached)
1. call ScoreState::before_evaluate() on each of them to update
    configuration
1. call Restraint::unprotected_evaluate() to compute scores
    [and add derivatives in the particles]
1. [call ScoreState::after_evaluate() on each score state to update derivatives]
1. score returned

    \headerfile ScoringFunction.h "IMP/ScoringFunction.h"

*/
class IMPEXPORT ScoringFunction: public ModelObject
 {
  // kept alive in model
  ScoreStatesTemp ss_;
  double last_score_;
  bool last_was_good_;
  inline void ensure_dependencies();
  // hack for null scoring function
  friend class NullScoringFunction;
  ScoringFunction(): ModelObject("NullScoringFunction%1%") {
    last_score_=0.0;
    last_was_good_=true;
  }
  // later make things implement inputs and return restraints
public:
  typedef std::pair<double, bool> ScoreIsGoodPair;
  IMP_PROTECTED_METHOD(virtual ScoreIsGoodPair,
                       do_evaluate_if_good,
                       (bool derivatives,
                        const ScoreStatesTemp &ss),,=0);
  IMP_PROTECTED_METHOD(virtual ScoreIsGoodPair, do_evaluate,
                       (bool derivatives,
                        const ScoreStatesTemp &ss),,=0);
  IMP_PROTECTED_METHOD(virtual ScoreIsGoodPair, do_evaluate_if_below,
                       (bool derivatives,
                        double max,
                        const ScoreStatesTemp &ss),,=0);
  /** Return any extra score states that should be included in the list
      generated by the model for this ScoringFunction in addition to
      the ones implied by the restraints. */
  IMP_PROTECTED_METHOD(virtual ScoreStatesTemp,
                       get_required_score_states,
                       (const DependencyGraph &dg,
                        const DependencyGraphVertexIndex &index),
                       const,=0);

 public:
  ScoringFunction(Model *m, std::string name);
  IMP_MODEL_OBJECT(ScoringFunction);
  inline double evaluate_if_good(bool derivatives);
  inline double evaluate(bool derivatives);
  inline double evaluate_if_below(bool derivatives, double max);
  /** Return true if the last evaluate satisfied all the restraint
      thresholds.*/
  bool get_had_good_score() const {
    return last_was_good_;
  }
  double get_last_score() const {
    return last_score_;
  }
  /** Return a set of restraints equivalent to this scoring function.
   */
  virtual Restraints create_restraints() const=0;
  /** Return the score states needed to evaluate this ScoringFunction.*/
  const ScoreStatesTemp& get_score_states();
};


/** Return a list of ScoringFunction objects where each is as simple
    as possible and evaluating the sum (and anding the good score bits)
    is exactly like evaluating the one ScoringFunction.*/
IMPEXPORT ScoringFunctions create_decomposition(ScoringFunction *sf);

/** This class is to provide a consisted interface for things
    that take ScoringFunctions as arguments.

    \note Passing an empty list of restraints should be supported, but problems
    could arise, so be alert (the problems would not be subtle).
*/
class IMPEXPORT ScoringFunctionAdaptor:
#if !defined(SWIG) && !defined(IMP_DOXYGEN)
  public base::OwnerPointer<ScoringFunction>
#else
  public base::InputAdaptor
#endif
{
  typedef base::OwnerPointer<ScoringFunction> P;
  static ScoringFunction* get(ScoringFunction *sf) {
    return sf;
  }
  static ScoringFunction* get(const RestraintsTemp &sf);
  static ScoringFunction* get(const Restraints &sf);
  static ScoringFunction* get(Model *sf);
  static ScoringFunction* get(Restraint *sf);
 public:
  ScoringFunctionAdaptor(){}
#if !defined(SWIG) && !defined(IMP_DOXYGEN)
  template <class T>
    ScoringFunctionAdaptor(base::internal::PointerBase<T> t):
    P(get(t)){}
#endif
  ScoringFunctionAdaptor(ScoringFunction *sf): P(sf){}
  ScoringFunctionAdaptor(const RestraintsTemp &sf): P(get(sf)){}
  ScoringFunctionAdaptor(const Restraints &sf): P(get(sf)){}
  ScoringFunctionAdaptor(Model *sf): P(get(sf)){}
  ScoringFunctionAdaptor(Restraint *sf): P(get(sf)){}
};

//! Print the hierarchy of restraints
/** The maximum accepted score (Restraint::get_maximum_score())
    and the weight (Restraint::get_weight()) are printed for each restraint.*/
IMPEXPORT void show_restraint_hierarchy(ScoringFunctionAdaptor rs,
                                     std::ostream &out=std::cout);


IMP_END_NAMESPACE

#endif  /* IMPKERNEL_DECLARE_SCORING_FUNCTION_H */
