/**
 * @file nsga2.hpp
 * @author Sayan Goswami
 *
 * NSGA-II is a multi-objective optimzation algorithm, widely used in
 * many real-world applications. NSGA-II genererates offsprings using
 * crossover and mutation and then selects the next generation according
 * to nondominated-sorting and crowding distance comparison.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#ifndef ENSMALLEN_NSGA2_NSGA2_HPP
#define ENSMALLEN_NSGA2_NSGA2_HPP

namespace ens {

/**
 * NSGA-II (Non-dominated Sorting Genetic Algorithm - II) is a multi-objective
 * optimzation algorithm. This class implements the NSGA-II algorithm.
 *
 * The algorithm works by generating a candidate population from a fixed
 * starting point. At each stage of optimization, a new population of children
 * is generated. This new population alongwith its predecessor is sorted using
 * non-domiation as the metric. Following this, the population is further
 * seggregated in fronts. A new population is generated from these fronts having
 * size equal to that of the starting population.
 *
 * During evolution, two parents are randomly chosen using binary tournament
 * selection. A pair of children are generated by crossing over these two
 * candidates followed by mutation.
 *
 * The best front (pareto optimal) is returned by the Optimize() method.
 *
 * For more information, see the following:
 *
 * @code
 * @article{10.1109/4235.996017,
 *   author = {Deb, K. and Pratap, A. and Agarwal, S. and Meyarivan, T.},
 *   title = {A Fast and Elitist Multiobjective Genetic Algorithm: NSGA-II},
 *   year = {2002},
 *   issue_date = {April 2002},
 *   publisher = {IEEE Press},
 *   volume = {6},
 *   number = {2},
 *   issn = {1089-778X},
 *   url = {https://doi.org/10.1109/4235.996017},
 *   doi = {10.1109/4235.996017},
 *   journal = {Trans. Evol. Comp},
 *   month = apr,
 *   pages = {182–197},
 *   numpages = {16} }
 * @endcode
 *
 * NSGA-II can optimize arbitrary muli-objective functions.  For more details,
 * see the documentation on function types included with this distribution or
 * on the ensmallen website.
 */
class NSGA2 {
 public:
  /**
   * Constructor for the NSGA2 optimizer
   *
   * The default values provided over here are not necessarily suitable for a
   * given function. Therefore it is highly recommended to adjust the
   * parameters according to the problem.
   *
   * @param populationSize The number of candidates in the population.
   *     This should be atleast 4 in size and a multiple of 4.
   * @param maxGenerations The maximum number of generations allowed for NSGA-II
   * @param crossoverProb The probability that a crossover will occur.
   * @param mutationProb The probability that a mutation will occur.
   * @param mutationStrength The strength of the mutation.
   * @param epsilon The minimum difference reqiured to distinguish between
   *     candidate solutions.
   */
  NSGA2(const size_t populationSize = 100,
        const size_t maxGenerations = 2000,
        const double crossoverProb = 0.6,
        const double mutationProb = 0.3,
        const double mutationStrength = 1e-3,
        const double epsilon = 1e-6);

  /**
   * Optimize a set of objectives. The initial population is generated using the
   * starting point. The output is the best generated front.
   *
   * @tparam MultiobjectiveFunctionType A generic multi-objective function.
   * @tparam MatType Type of matrix to optimize.
   * @tparam CallbackTypes Types of callback functions.
   * @param objectives Vector of objective functions to optimize for.
   * @param iterate Starting point
   * @param callbacks Callback functions.
   * @return std::vector<MatType> The best front obtained after optimzation.
   */
  template<typename MultiobjectiveFunctionType,
           typename MatType,
           typename... CallbackTypes>
  std::vector<MatType> Optimize(MultiobjectiveFunctionType& objectives,
                                MatType& iterate,
                                CallbackTypes&&... callbacks);

  //! Get the population size.
  size_t PopulationSize() const { return populationSize; }
  //! Modify the population size.
  size_t& PopulationSize() { return populationSize; }

  //! Get the maximum number of generations.
  size_t MaxGenerations() const { return maxGenerations; }
  //! Modify the maximum number of generations.
  size_t& MaxGenerations() { return maxGenerations; }

  //! Get the crossover rate.
  double CrossoverRate() const { return crossoverProb; }
  //! Modify the crossover rate.
  double& CrossoverRate() { return crossoverProb; }

  //! Get the mutation probability.
  double MutationProbability() const { return mutationProb; }
  //! Modify the mutation probability.
  double& MutationProbability() { return mutationProb; }

  //! Get the mutation size.
  double MutationSize() const { return mutationStrength; }
  //! Modify the mutation size.
  double& MutationSize() { return mutationStrength; }

  //! Get the tolerance.
  double Epsilon() const { return epsilon; }
  //! Modify the tolerance.
  double& Epsilon() { return epsilon; }


 private:
  /**
   * Evulate objectives for the elite population.
   *
   * @tparam MultiobjectiveFunctionType A generic multi-objective function.
   * @tparam MatType Type of matrix to optimize.
   * @param population The elite population.
   * @param objectives The set of objectives.
   * @param calculatedObjectives Vector to store calculated objectives.
   */
  template<typename MultiobjectiveFunctionType,
           typename MatType>
  void EvaluateObjectives(std::vector<MatType> population,
                          MultiobjectiveFunctionType objectives,
                          std::vector<arma::vec>& calculatedObjectives);

  /**
   * Reproduce candidates from the elite population to generate a new
   * population.
   *
   * @tparam MatType Type of matrix to optimize.
   * @param population The elite population.
   */
  template<typename MatType>
  void BinaryTournamentSelection(std::vector<MatType>& population);

  /**
   * Crossover two parents to create a pair of new children.
   *
   * @tparam MatType Type of matrix to optimize.
   * @param childA A newly generated candidate.
   * @param childB Another newly generated candidate.
   * @param parentA First parent from elite population.
   * @param parentB Second parent from elite population.
   */
  template<typename MatType>
  void Crossover(MatType& childA,
                 MatType& childB,
                 MatType parentA,
                 MatType parentB);

  /**
   * Mutate the coordinates for a candidate.
   * @tparam MatType Type of matrix to optimize.
   * @param child The candidate whose coordinates are being modified.
   */
  template<typename MatType>
  void Mutate(MatType& child);

  /**
   * Sort the candidate population using their domination count and the set of
   * dominated nodes.
   *
   * @param fronts The population is sorted into these pareto fronts. The first
   *     front is the best, the second worse and so on.
   * @param ranks The assigned ranks, used for crowding distance based sorting.
   * @param calculatedObjectives The previously calculated objectives.
   */
  void FastNonDominatedSort(std::vector<std::vector<size_t> >& fronts,
                            std::vector<size_t>& ranks,
                            std::vector<arma::vec> calculatedObjectives);

  /**
   * Operator to check if one candidate pareto-dominates the other.
   *
   * A candidate is said to dominate the other if it is at least as good as the
   * other candidate for all the objectives and there exists at least one
   * objective for which it is strictly better than the other candidate.
   *
   * @param calculatedObjectives The previously calculated objectives.
   * @param candidateP The candidate being compared from the elite population.
   * @param candidateQ The candidate being compared against.
   * @return true candidateP pareto dominates candidateQ
   * @return false candidateQ is not pareto-dominated by candidateP
   */
  bool Dominates(std::vector<arma::vec> calculatedObjectives,
                 size_t candidateP,
                 size_t candidateQ);

  /**
   * Assigns crowding distance metric for sorting.
   *
   * @tparam MultiobjectiveFunctionType A generic multi-objective function.
   * @param front The previoulsy generated pareto fronts.
   * @param objectives The set of objectives.
   * @param crowdingDistance The previously calculated objectives.
   */
  template<typename MultiobjectiveFunctionType>
  void CrowdingDistanceAssignment(std::vector<size_t> front,
                                  MultiobjectiveFunctionType objectives,
                                  std::vector<double>& crowdingDistance);

  /**
   * The operator used in the crowding distance based sorting.
   *
   * If a candidates has a lower rank then it is preferred.
   * Otherwise, if the ranks are equal then the candidate with the larger
   * crowding distance is preferred.
   *
   * @param idxP The index of the first cadidate from the elite population being
   *     sorted.
   * @param idxQ The index of the second cadidate from the elite population
   *     being sorted.
   * @param ranks The previously calculated ranks.
   * @param crowdingDistance The previously calculated objectives.
   * @return true The first candidate is preferred.
   * @return false The second candidate is preferred.
   */
  bool CrowdingOperator(size_t idxP,
                        size_t idxQ,
                        const std::vector<size_t>& ranks,
                        const std::vector<double>& crowdingDistance);

  //! The number of candidates in the population.
  size_t populationSize;

  //! Maximum number of generations before termination criteria is met.
  size_t maxGenerations;

  //! Probability that crossover will occur.
  double crossoverProb;

  //! Probability that mutation will occur.
  double mutationProb;

  //! Strength of the mutation.
  double mutationStrength;

  //! The tolerance for termination.
  double epsilon;
};

} // namespace ens

// Include implementation.
#include "nsga2_impl.hpp"

#endif
