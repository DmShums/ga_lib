#include "KnapsackPopulation.h"

KnapsackPopulation::KnapsackPopulation(const std::vector<std::vector<int>>& distMatrix,
                                       size_t populationSize) : distMatrix(distMatrix) {
    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind{};
        ind.solution = getRandomPermutation(distMatrix.size());
        evaluate(ind);
        population.emplace_back(ind);
    }
}