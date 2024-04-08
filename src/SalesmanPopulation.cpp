#include "SalesmanPopulation.h"


std::vector<size_t> getRandomPermutation(size_t len){
    std::vector<size_t> permutation(len);

    for (size_t i = 0; i < len; ++i) {
        permutation[i] = i;
    }

    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    return permutation;
}

SalesmanPopulation::SalesmanPopulation(const std::vector<std::vector<int>>& distMatrix,
                                       size_t populationSize) : distMatrix(distMatrix) {
    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind{};
        ind.solution = getRandomPermutation(distMatrix.size());
        evaluate(ind);
        population.emplace_back(ind);
    }
}


void SalesmanPopulation::evaluate(Individual& ind) const {
    int fitness = 0;
    size_t prev = ind.solution[0];

    for (size_t i = 1; i < ind.solution.size(); ++i) {
        size_t next = ind.solution[i];
        fitness += distMatrix[prev][next];
        prev = next;
    }

    fitness += distMatrix[ind.solution.back()][ind.solution[0]];
    ind.fitness = fitness;
}

/*
 * if you want to override, use this.
 */
//Individual SalesmanPopulation::mutation(const Individual& parent) {
//    Individual offspring(parent);
//    double mutationRate = 0.05;
//    size_t solutionLen = parent.solution.size();
//
//    for (size_t i = 0; i < solutionLen; ++i) {
//        if ((std::rand() / (double)RAND_MAX) < mutationRate) {
//            int j = std::rand() % solutionLen;
//            std::swap(offspring.solution[i], offspring.solution[j]);
//        }
//    }
//
//    evaluate(offspring);
//    return offspring;
//}
