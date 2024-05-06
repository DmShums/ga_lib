#include "KnapsackPopulation.h"

std::vector<size_t> getRandomSolution(size_t len) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, 1);

    std::vector<size_t> solution(len);

    for (size_t i = 0; i < len; ++i) {
        solution[i] = dis(gen);
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

KnapsackPopulation::KnapsackPopulation(const std::vector<int>& weights,
                                       const std::vector<int>& values,
                                       int maxWeight,
                                       size_t populationSize) : weights(weights), values(values), maxWeight(maxWeight) {
    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind{};
        ind.solution = getRandomSolution(values.size());
        evaluate(ind);
        population.emplace_back(ind);
    }
}
