/**
 *  \file MonteCarlo.h    \brief Simple Monte Carlo optimizer.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCORE_MONTE_CARLO_H
#define IMPCORE_MONTE_CARLO_H

#include "core_config.h"
#include "Mover.h"

#include <IMP/Optimizer.h>
#include <IMP/container_macros.h>

IMPCORE_BEGIN_NAMESPACE


//! A Monte Carlo optimizer.
/** The optimizer uses a set of Mover objects to propose steps. Currently
    each Mover is called at each Monte Carlo iteration. This may change in
    the future. The movers propose some modification, which is then
    accepted or rejected based on the Metropolis criteria. Optionally, a
    number of local optimization steps are taken before the MonteCarlo step
    is accepted or rejected.

    By default, the lowest score state encountered is returned.

    \see Mover
 */
class IMPCOREEXPORT MonteCarlo: public Optimizer
{
public:
  MonteCarlo(Model *m=NULL);

  IMP_OPTIMIZER(MonteCarlo);
 public:
  /** \name Local optimization

      The MonteCarlo optimizer can run a local optimizer following
      each Monte-Carlo move and before it decides whether or not to
      accept the step. Steps taken by the local optimizer do not count
      towards the total number of steps passed to the
      Optimizer::optimize() call. The local optimizer must not have
      OptimizerState objects which change the set of optimized
      particles/attributes. This is not checked.

      @{
  */
  void set_local_optimizer(Optimizer* cg);

  Optimizer *get_local_optimizer() const {
    return cg_.get();
  }

  int get_local_steps() const {
    return num_local_steps_;
  }
  void set_local_steps(unsigned int n) {
    num_local_steps_=n;
  }
  /** @} */

  /** By default, the optimizer returns the lowest score state
      found so far. If, instead, you wish to return the last accepted
      state, set return best to false.
  */
  void set_return_best(bool tf) {
    return_best_=tf;
  }

  /** \name Temperature
      The temperature has to be on the same scale as the differences
      in energy between good and bad states (and so the default is
      likely to not be a good choice).
      @{
  */
  void set_temperature(Float t) {
    IMP_INTERNAL_CHECK(t>0, "Temperature must be positive");
    temp_=t;
  }
  Float get_temperature() const {
    return temp_;
  }
  /** @} */
  /** \name Score threshold

      Optimization will stop if the score falls below the threshold.
      @{
  */
  void set_score_threshold(Float t) {
    stop_energy_=t;
  }
  Float get_score_threshold() const {
    return stop_energy_;
  }
  /** @} */

  //! Set the probability of each move being made
  /** Make this low if the space is rough and there are many particles.
      The movers should make each individual move with this probability.
      That is, a NormalMover with 100 particles will move each particle
      with probability p.
   */
  void set_move_probability(Float p) {
    IMP_USAGE_CHECK(p > 0 && p <= 1, "Not a valid probability");
    probability_=p;
  }
  /** \name Statistics
      @{
   */
  //! Return how many times the optimizer has succeeded in taking a step
  unsigned int get_number_of_forward_steps() const {
    return stat_forward_steps_taken_;
  }
  //! Return how many times the optimizer has stepped to higher energy
  unsigned int get_number_of_upward_steps() const {
    return stat_upward_steps_taken_;
  }
  /** @} */

  /** @name Movers

       The following methods are used to manipulate the list of Movers.
       Each mover is called at each optimization step, giving it a chance
       to change the current configuration.
       @{
  */
  IMP_LIST(public, Mover, mover, Mover*, Movers);
  /** @} */
private:
  Float temp_;
  Float stop_energy_;
  Float probability_;
  IMP::internal::OwnerPointer<Optimizer> cg_;
  unsigned int num_local_steps_;
  unsigned int stat_forward_steps_taken_;
  unsigned int stat_upward_steps_taken_;
  unsigned int stat_num_failures_;
  bool return_best_;
};

IMP_OUTPUT_OPERATOR(MonteCarlo);

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_MONTE_CARLO_H */
