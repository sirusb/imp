/**
 *  \file IMP/kernel/Model.h
 *  \brief Storage of a model, its restraints,
 *                         constraints and particles.
 *
 *  Copyright 2007-2014 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPKERNEL_MODEL_H
#define IMPKERNEL_MODEL_H

#include <IMP/kernel/kernel_config.h>
#include "ModelObject.h"
#include "ScoringFunction.h"
#include "Restraint.h"
#include "RestraintSet.h"
#include "ScoreState.h"
#include "container_macros.h"
#include "base_types.h"
//#include "Particle.h"
#include "Undecorator.h"
#include "internal/AttributeTable.h"
#include "internal/attribute_tables.h"
#include <IMP/base/Object.h>
#include <IMP/base/Pointer.h>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <IMP/base/tuple_macros.h>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include <limits>

IMPKERNEL_BEGIN_NAMESPACE

class ModelObject;
class Undecorator;
class Particle;

#if !defined(SWIG) && !defined(IMP_DOXYGEN)
namespace internal {
enum Stage {
  NOT_EVALUATING,
  BEFORE_EVALUATING,
  EVALUATING,
  AFTER_EVALUATING,
  COMPUTING_DEPENDENCIES
};
}
#endif

class Model;

//! Class for storing model, its restraints, constraints, and particles.
/** The Model maintains a standard \imp container for each of Particle,
    ScoreState and Restraint object types.

    Each Float attribute has an associated range which reflects the
    range of values that it is expected to take on during optimization.
    The optimizer can use these ranges to make the optimization process
    more efficient. By default, the range estimates are simply the
    range of values for that attribute in the various particles, but
    it can be set to another value. For example, an attribute storing
    an angle could have the range set to (0,PI).

    The ranges are not enforced; they are just guidelines. In order to
    enforce ranges, see, for example,
    IMP::example::ExampleSingletonModifier.

    \headerfile Model.h "IMP/Model.h"
 */
class IMPKERNELEXPORT Model : public base::Object
#if !defined(SWIG) && !defined(IMP_DOXYGEN)
                              ,
                              public internal::Masks,
                              // The attribute tables provide fast access to
                              // e.g. particle attributes, etc.
                              public internal::FloatAttributeTable,
                              public internal::StringAttributeTable,
                              public internal::IntAttributeTable,
                              public internal::ObjectAttributeTable,
                              public internal::WeakObjectAttributeTable,
                              public internal::IntsAttributeTable,
                              public internal::ObjectsAttributeTable,
                              public internal::ParticleAttributeTable,
                              public internal::ParticlesAttributeTable
#endif
                              {
  typedef base::Vector<ModelObject *> Edges;
  // must be up top
  // we don't want any liveness checks
  IMP_NAMED_TUPLE_5(NodeInfo, NodeInfos, Edges, inputs, Edges, input_outputs,
                    Edges, outputs, Edges, readers, Edges, writers, );
  typedef boost::unordered_map<const ModelObject *, NodeInfo> DependencyGraph;
  DependencyGraph dependency_graph_;
  boost::unordered_set<const ModelObject *> no_dependencies_;
  boost::unordered_map<const ModelObject *, ScoreStatesTemp>
      required_score_states_;

  // basic representation
  boost::unordered_map<FloatKey, FloatRange> ranges_;

  ParticleIndexes free_particles_;
  base::IndexVector<ParticleIndexTag, base::Pointer<Particle> > particle_index_;
  base::IndexVector<ParticleIndexTag, Undecorators> undecorators_index_;

  base::Vector<base::PointerMember<base::Object> > model_data_;
  ////////////// DEPRECATED
  // for old code that uses the model for the scoring function
  base::PointerMember<RestraintSet> restraints_;

  void do_add_dependencies(const ModelObject *mo);
  void do_clear_required_score_states(kernel::ModelObject *mo);
  void do_check_required_score_states(const ModelObject *mo) const;
  void do_check_update_order(const ScoreState *ss) const;
  void do_check_inputs_and_outputs(const ModelObject *mo) const;
  void do_check_readers_and_writers(const ModelObject *mo) const;
  void do_check_not_in_readers_and_writers(const ModelObject *mo) const;
  void do_clear_dependencies(const ModelObject *mo);
#if !defined(IMP_DOXYGEN) && !defined(SWIG)
  // things the evaluate template functions need, can't be bothered with friends
 public:
#endif
  // check more things on the first call
  bool first_call_;
  // the stage of evaluation
  internal::Stage cur_stage_;

  ModelObjectsTemp get_dependency_graph_inputs(const ModelObject *mo) const;
  ModelObjectsTemp get_dependency_graph_outputs(const ModelObject *mo) const;
  bool do_get_has_dependencies(const ModelObject *mo) const {
    return no_dependencies_.find(mo) == no_dependencies_.end();
  }
  void do_set_has_dependencies(const ModelObject *mo, bool tf);
  void do_set_has_all_dependencies(bool tf);

  void validate_computed_derivatives() const {}
  void set_has_all_dependencies(bool tf);
  bool get_has_all_dependencies() const;
  void check_dependency_invariants() const;
  void check_dependency_invariants(const ModelObject *mo) const;
  ScoreStatesTemp get_ancestor_score_states(const ModelObject *mo) const;
  ScoreStatesTemp get_descendent_score_states(const ModelObject *mo) const;

  void before_evaluate(const ScoreStatesTemp &states);
  void after_evaluate(const ScoreStatesTemp &states, bool calc_derivs);

  internal::Stage get_stage() const { return cur_stage_; }
  ParticleIndex add_particle_internal(Particle *p);
  static void do_remove_score_state(ScoreState *obj);
  void do_add_score_state(ScoreState *obj);
  void do_remove_particle(ParticleIndex pi);
  bool do_get_has_required_score_states(const ModelObject *mo) const;
  void do_set_has_required_score_states(kernel::ModelObject *mo, bool tf);
  const ScoreStatesTemp &do_get_required_score_states(const ModelObject *mo)
      const {
    IMP_USAGE_CHECK(do_get_has_required_score_states(mo),
                    "Doesn't have score states");
    return required_score_states_.find(mo)->second;
  }
  void do_add_model_object(kernel::ModelObject *mo);
  void do_remove_model_object(kernel::ModelObject *mo);

 public:
  /** Construct an empty model */
  Model(std::string name = "Model %1%");

 public:
#if !defined(SWIG) && !defined(IMP_DOXYGEN)
  IMP_MODEL_IMPORT(internal::FloatAttributeTable);
  IMP_MODEL_IMPORT(internal::StringAttributeTable);
  IMP_MODEL_IMPORT(internal::IntAttributeTable);
  IMP_MODEL_IMPORT(internal::ObjectAttributeTable);
  IMP_MODEL_IMPORT(internal::WeakObjectAttributeTable);
  IMP_MODEL_IMPORT(internal::IntsAttributeTable);
  IMP_MODEL_IMPORT(internal::ObjectsAttributeTable);
  IMP_MODEL_IMPORT(internal::ParticleAttributeTable);
  IMP_MODEL_IMPORT(internal::ParticlesAttributeTable);
#endif
  /** Clear all the cache attributes of a given particle.*/
  void clear_particle_caches(ParticleIndex pi);

  //! Add particle to the model
  ParticleIndex add_particle(std::string name);

  //! Get the name of a particle
  std::string get_particle_name(ParticleIndex pi);

  /** Add the passed Undecorator to the particle.*/
  void add_undecorator(ParticleIndex pi, Undecorator *d);

  /** @name States

      ScoreStates can be added to the Model in order to keep them
      alive as long as the model is alive. Being added does affect
      their ability to perform their required action. See ScoreState
      for more information.

      \advancedmethod
  */
  /**@{*/
  IMP_LIST_ACTION(public, ScoreState, ScoreStates, score_state, score_states,
                  ScoreState *, ScoreStates, do_add_score_state(obj), {},
                  do_remove_score_state(obj));
  /**@}*/

 public:
#ifndef SWIG
  using Object::clear_caches;
#endif

  //! Sometimes it is useful to be able to make sure the model is up to date
  /** This method updates all the state but does not necessarily compute the
      score. Use this to make sure that your containers and rigid bodies are
      up to date.
  */
  void update();

#ifdef IMP_DOXYGEN
  /** \name Accessing attributes
      \anchor model_attributes
      All the attribute data associated with each Particle is stored in the
      Model. For each type of attribute, there are the methods detailed below
      (where, eg, TypeKey is FloatKey or StringKey)
      @{
  */
  /** \pre get_has_attribute(attribute_key, particle) is false*/
  void add_attribute(TypeKey attribute_key, ParticleIndex particle, Type value);

  /** \pre get_has_attribute(attribute_key, particle) is true*/
  void remove_attribute(TypeKey attribute_key, ParticleIndex particle);

  bool get_has_attribute(TypeKey attribute_key, ParticleIndex particle) const;

  /** \pre get_has_attribute(attribute_key, particle) is true*/
  void set_attribute(TypeKey attribute_key, ParticleIndex particle, Type value);

  /** \pre get_has_attribute(attribute_key, particle) is true*/
  Type get_attribute(TypeKey attribute_key, ParticleIndex particle);

  /** Cache attributes, unklike normal attributes, can be added during
      evaluation. They are also cleared by the clear_cache_attributes() method.
      Cache attributes should be used when one is adding data to a particle
      to aid scoring (eg cache the rigid body collision acceleration structure).

      When some pertinent aspect of the particle changes, the clear method
      should
      be called (yes, this is a bit vague). Examples where it should be cleared
      include changing the set of members of a core::RigidBody or their
      coordinates, changing the members of an atom::Hierarchy.
  */
  void add_cache_attribute(TypeKey attribute_key, ParticleIndex particle,
                           Type value);

  //! Optimized attributes are the parameters of the model
  /** They will be modified by the samplers and optimizers.
   */
  void set_is_optimized(TypeKey attribute_key, ParticleIndex particle,
                        bool true_or_false);
/** @} */
#endif

#ifdef SWIG
#define IMP_MODEL_ATTRIBUTE_METHODS(Type, Value)                            \
  void add_attribute(Type##Key attribute_key, ParticleIndex particle,       \
                     Value value);                                          \
  void remove_attribute(Type##Key attribute_key, ParticleIndex particle);   \
  bool get_has_attribute(Type##Key attribute_key,                           \
                         ParticleIndex particle) const;                     \
  void set_attribute(Type##Key attribute_key, ParticleIndex particle,       \
                     Value value);                                          \
  Value get_attribute(Type##Key attribute_key, ParticleIndex particle);     \
  void add_cache_attribute(Type##Key attribute_key, ParticleIndex particle, \
                           Value value)

  IMP_MODEL_ATTRIBUTE_METHODS(Float, Float);
  IMP_MODEL_ATTRIBUTE_METHODS(Int, Int);
  IMP_MODEL_ATTRIBUTE_METHODS(Ints, Ints);
  IMP_MODEL_ATTRIBUTE_METHODS(String, String);
  IMP_MODEL_ATTRIBUTE_METHODS(ParticleIndexes, ParticleIndexes);
  IMP_MODEL_ATTRIBUTE_METHODS(ParticleIndex, ParticleIndex);
  IMP_MODEL_ATTRIBUTE_METHODS(Object, Object *);
  IMP_MODEL_ATTRIBUTE_METHODS(WeakObject, Object *);
  void set_is_optimized(FloatKey, ParticleIndex, bool);
#endif

  /** Get the particle from an index. */
  Particle *get_particle(ParticleIndex p) const;

  /** Get the particle from an index. */
  bool get_has_particle(ParticleIndex p) const;

  /** Get all particle indexes */
  ParticleIndexes get_particle_indexes();

  /** Get all the ModelObjects associated with this Model.
   */
  ModelObjectsTemp get_model_objects() const;

  /** Remove a particle from the Model. The particle will then be inactive and
       cannot be used for anything and all data stored in the particle is lost.
   */
  void remove_particle(ParticleIndex pi);

  /** \name Storing data in the model

      One can store data associated with the model. This is used, for example,
      to keep a central ScoreState to normalize rigid body rotational variables.
      @{ */
  /** Store a piece of data in the model referenced by the key. */
  void add_data(kernel::ModelKey mk, base::Object *o);
  /** Get back some data stored in the model. */
  base::Object *get_data(kernel::ModelKey mk) const;
  /** Remove data stored in the model. */
  void remove_data(kernel::ModelKey mk);
  /** Check if the model has a certain piece of data attached. */
  bool get_has_data(kernel::ModelKey mk) const;
  /** @} */

  IMP_OBJECT_METHODS(Model);

 public:
// deprecated
#if !defined(SWIG) && !defined(IMP_DOXYGEN)
  struct NotNull {
    bool operator()(const base::Pointer<Particle> &p) { return p; }
  };
  typedef boost::filter_iterator<
      NotNull, base::Vector<base::Pointer<Particle> >::const_iterator>
      ParticleIterator;
#endif

// all deprecated but too used to add warnings about now
#if !defined(IMP_DOXYGEN)
  ScoringFunction *create_model_scoring_function();
  void add_restraint(Restraint *r);
  void remove_restraint(Restraint *r);
  RestraintsTemp get_restraints() const;
  ScoringFunction *create_scoring_function();
  virtual void do_destroy() IMP_OVERRIDE;
#endif

  /** \deprecated_at{2.1} Use a RestraintSet instead.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  unsigned int get_number_of_restraints() const;
  /** \deprecated_at{2.1} Use a RestraintSet instead.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  Restraint *get_restraint(unsigned int i) const;
  /** \deprecated_at{2.1} Use a ScoringFunction instead.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  double evaluate(bool tf, bool warn = true);
  /** \deprecated_at{2.1} Use the ParticleIndex version.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  void remove_particle(Particle *p);
  /** \deprecated_at{2.1} Use get_particle_indexes(). */
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  unsigned int get_number_of_particles() const;
  /** \deprecated_at{2.1} Use get_particle_indexes(). */
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  ParticlesTemp get_particles() const;
  /** \deprecated_at{2.1} Using a ScoringFunction instead. */
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  RestraintSet *get_root_restraint_set();
  /** \deprecated_at{2.1} Get the maximum directly from the restraint.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  double get_maximum_score(Restraint *r) const;
  /** \deprecated_at{2.1} Set get the maximum directly on the restraint.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  void set_maximum_score(Restraint *r, double s);
  /** \deprecated_at{2.1} You should use a ScoringFunction or a RestraintSet.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  void set_maximum_score(double s);
  /** \deprecated_at{2.1} You should use a ScoringFunction or a RestraintSet.*/
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  double get_maximum_score() const;
#ifndef SWIG
  /** \deprecated_at{2.1} Use get_particle_indexes(). */
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  ParticleIterator particles_begin() const;
  /** \deprecated_at{2.1} Use get_particle_indexes(). */
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  ParticleIterator particles_end() const;
  /** \deprecated_at{2.1} Use a ScoringFunction instead. */
  IMPKERNEL_DEPRECATED_METHOD_DECL(2.1)
  operator Restraint *() const { return restraints_.get(); }
#endif
};

IMPKERNEL_END_NAMESPACE

// This is needed for per cpp compilations, a not even sure why
// (perhaps cause Model returns ParticleIterator here and there?)
// - Feel free to remove if you *really* know what you're doing
#include "IMP/kernel/Particle.h"

#endif /* IMPKERNEL_MODEL_H */
