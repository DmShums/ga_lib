#include "KnapsackPopulation.h"

std::vector<size_t> getRandomSolution(size_t len) {
    std::vector<size_t> solution(len);

    for (size_t i = 0; i < len; ++i) {
        solution[i] = std::rand() % 2;
    }

    return solution;
}

bool KnapsackPopulation::isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const {
    return ind1.fitness > ind2.fitness;
}

void KnapsackPopulation::evaluate(Individual& ind) const {
    int fitness = 0;
    int weight = 0;

    for (size_t i = 0; i < ind.solution.size(); ++i) {
        if (ind.solution[i] == 1) {
            weight += weights[i];
            fitness += values[i];
        }
    }

    if (weight > maxWeight) {
        ind.fitness = -1;
    } else {
        ind.fitness = fitness;
    }
}

KnapsackPopulation::KnapsackPopulation(const std::vector<int>& values,
                                       const std::vector<int>& weights,
                                       int maxWeight,
                                       size_t populationSize) : values(values), weights(weights), maxWeight(maxWeight) {
    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind{};
        ind.solution = getRandomSolution(values.size());
        evaluate(ind);
        population.emplace_back(ind);
    }
}

// Individual KnapsackPopulation::mutation(const Individual& parent) {
//     Individual offspring(parent);
//     double mutationRate = 0.05;
//     size_t solutionLen = parent.solution.size();

//     for (size_t i = 0; i < solutionLen; ++i) {
//         if ((std::rand() / (double)RAND_MAX) < mutationRate) {
//             offspring.solution[i] = 1 - offspring.solution[i];
//         }
//     }

//     evaluate(offspring);
//     return offspring;
// }