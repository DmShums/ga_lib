#ifndef GA_LIB_KNAPSACKPOPULATION_H
#define GA_LIB_KNAPSACKPOPULATION_H

#include <vector>
#include "Population.h"

class KnapsackPopulation : public Population {
private:
    std::vector<int> weights;
    std::vector<int> values;
    int maxWeight;

public:
    KnapsackPopulation(const std::vector<int>& weights, const std::vector<int>& values,
                       int maxWeight, size_t populationSize);

    void evaluate(Individual& ind) const override;
    bool isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const override;
};

#endif //GA_LIB_KNAPSACKPOPULATION_H
