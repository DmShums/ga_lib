#ifndef GA_LIB_KNAPSACKPOPULATION_H
#define GA_LIB_KNAPSACKPOPULATION_H

#include <vector>
#include "Population.h"

class KnapsackPopulation : public Population {
private:
    std::vector<int> weights;
    std::vector<int> values;
    int maxWeight;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    KnapsackPopulation(const std::vector<int>& weights, const std::vector<int>& values,
                       int maxWeight, size_t populationSize);

    void evaluate(Individual& ind) const override;
    bool isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const override;

    std::vector<size_t> getRandomSolution(size_t len);

    Individual crossover(const Individual& parent1, const Individual& parent2) override;
};

#endif //GA_LIB_KNAPSACKPOPULATION_H
