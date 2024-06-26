#include "SalesmanPopulation.h"

#define OVERRIDE false

std::vector<size_t> getRandomPermutation(size_t len) {
    std::vector<size_t> permutation(len);

    for (size_t i = 0; i < len; ++i) {
        permutation[i] = i;
    }

    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    return permutation;
}

bool SalesmanPopulation::isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const {
    return ind1 < ind2;
}

void SalesmanPopulation::evaluate(Individual& ind) const {
    int fitness = 0;
    size_t prev = ind.solution.front();

    for (size_t i = 1; i < ind.solution.size(); ++i) {
        size_t next = ind.solution[i];
        fitness += distMatrix[prev][next];
        prev = next;
    }

    fitness += distMatrix[ind.solution.back()][ind.solution.front()];
    ind.fitness = fitness;
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

#if OVERRIDE
    // if you want to override, use this.
    
    Individual SalesmanPopulation::mutation(const Individual& parent) {
        Individual offspring(parent);

        // your implementation

        return offspring;
    }

#endif