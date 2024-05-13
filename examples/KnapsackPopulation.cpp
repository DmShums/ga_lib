#include "KnapsackPopulation.h"

std::vector<size_t> KnapsackPopulation::getRandomSolution(size_t len) {
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

    for (auto i : ind.solution) {
        weight += weights[i];
        fitness += values[i];
    }

    ind.fitness = weight > maxWeight ? -1 : fitness;
}

KnapsackPopulation::KnapsackPopulation(const std::vector<int>& weights,
                                       const std::vector<int>& values,
                                       int maxWeight,
                                       size_t populationSize) : weights(weights), values(values), maxWeight(maxWeight) {
    gen.seed(rd());
    dis = std::uniform_int_distribution<>(0, weights.size() - 1);

    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind{};
        ind.solution = getRandomSolution(values.size());
        evaluate(ind);
        population.emplace_back(ind);
    }
}

Individual KnapsackPopulation::crossover(const Individual &parent1, const Individual &parent2) {
    Individual offspring{};

    // Single-Point Crossover
    size_t solutionLen = parent1.solution.size();
    size_t crossoverPoint = dis(gen) % solutionLen;

    for (size_t i = 0; i < solutionLen; ++i) {
        offspring.solution.emplace_back(i < crossoverPoint ? parent1.solution[i] : parent2.solution[i]);
    }

    evaluate(offspring);
    return offspring;
}