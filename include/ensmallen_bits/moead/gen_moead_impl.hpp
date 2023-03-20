#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <vector>


/*

Generation MOEA/D

The MOEA/D algorithm has been shown to work well on many multi-objective optimization problems, 
particularly those with convex Pareto fronts. However, it can suffer from premature convergence, 
where the diversity of the population is reduced as the algorithm progresses, and it can become 
stuck in local optima. The Generation MOEA/D algorithm was developed as an extension to the MOEA/D 
algorithm to address this issue, by using a different approach to maintain diversity in the population.

*/
 
#ifndef ENSMALLEN_GEN_MOEAD_MOEAD_IMP_HPP
#define ENSMALLEN_GEN_MOEAD_MOEAD_IMP_HPP







namespace ens {

/**
 * This class implements the Generation MOEA/D algorithm with Differential Evolution
 * crossover. 
 *
 * 
 */
class gen_moead{


public:
    int population_size;
    int number_generations;
    int number_divisions;
    vector<double> lower_bounds;
    vector<double> upper_bounds;

    gen_moead(int pop_size, int num_generations, int num_divisions, vector<double> lower_bounds, vector<double> upper_bounds):
    population_size(pop_size),
    number_generations(num_generations),
    number_divisions(num_divisions),
    lower_bounds(lower_bounds),
    upper_bounds(upper_bounds)
    {/*  Empty Body */}



// Define the problem function or any other problem can be substitued ex:schafer 
vector<double> problem_function(vector<double> x) {
    vector<double> objectives(2);
    objectives[0] = x[0]*x[0] + x[1]*x[1];
    objectives[1] = (x[0]-1)*(x[0]-1) + x[1]*x[1];
    return objectives;
}

// Define the scalarizing function
vector<double> scalarizing_function(vector<vector<double>> objectives, vector<double> weights) {
    vector<double> scalar_objectives(objectives.size());
    for (int i = 0; i < objectives.size(); i++) {
        double scalar_objective = 0;
        for (int j = 0; j < weights.size(); j++) {
            scalar_objective += weights[j] * objectives[i][j];
        }
        scalar_objectives[i] = scalar_objective;
    }
    return scalar_objectives;
}


// Define the Generation MOEA/D algorithm
vector<vector<double>> genmoead_algorithm() {
    // Initialize the population
    vector<vector<double>> population(population_size, vector<double>(2));
    std::default_random_engine generator;
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < 2; j++) {
            std::uniform_real_distribution<double> distribution(lower_bounds[j], upper_bounds[j]);
            population[i][j] = distribution(generator);
        }
    }
    // Initialize the weight vectors , Other moethods such as  Dirichlet can also be used.
    vector<vector<double>> weights(number_divisions, vector<double>(2));
    for (int i = 0; i < number_divisions; i++) {
        weights[i][0] = (i + 1.0) / number_divisions;
        weights[i][1] = 1 - weights[i][0];
    }
    // Iterate over generations
    for (int gen = 0; gen < number_generations; gen++) {
        // Evaluate the population
        vector<vector<double>> objectives(population_size, vector<double>(2));
        for (int i = 0; i < population_size; i++) {
            objectives[i] = problem_function(population[i]);
        }
        // Iterate over subproblems
        for (int i = 0; i < number_divisions; i++) {
            // Select the parents
            vector<int> parent_indices(2);
            std::uniform_int_distribution<int> distribution(0, population_size - 1);
            parent_indices[0] = distribution(generator);
            parent_indices[1] = distribution(generator);
            vector<double> parent1(2);
            vector<double> parent2(2);
            parent1 = population[parent_indices[0]];
            parent2 = population[parent_indices[1]];
            // Perform crossover and mutation
            vector<double> child(2);
            for (int j = 0; j < 2; j++) {
                if (rand() % 2 == 0) {
                    child[j] = parent1[j];
                } else {
                    child[j] = parent2[j];
                }
                if (rand() % 10 < 1) {
                    std::normal_distribution<double> distribution(0, 0.1);
                    child[j] += distribution(generator);
                }
                child[j] = std::max(lower_bounds[j], child[j]);
            vector<double> child_objectives;
            //Evelautae the child
            child_objectives=problem_function(child);
            //update the population
            for (int j=0; j<population_size; j++){
              vector<double> j_objectives(2);
              vector<double> j_weights(2);
              vector<double> j_scalar_objectives = scalarizing_function({j_objectives, child_objectives}, j_weights);;
              if (j_scalar_objectives[0] < scalarizing_function({j_objectives}, j_weights)[0]) {
                      population[j] = child;
                  }
                }
            }
        }
    }
    return population;
}

}

}

#endif
