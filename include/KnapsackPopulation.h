#ifndef GA_LIB_KNAPSACKPOPULATION_H
#define GA_LIB_KNAPSACKPOPULATION_H

#include <random>
#include "Population.h"

class KnapsackPopulation : public Population {
private:
    std::vector<std::vector<int>> distMatrix;
public:
    KnapsackPopulation() = delete;
    KnapsackPopulation(const std::vector<std::vector<int>>& distMatrix,
                       size_t populationSize);

    void evaluate(Individual& ind) const override;
    bool isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const override;

    /*
     * if you want to override mutation for you implementation, use this.
     *
     * Individual mutation(const Individual& parent) override;
     */
};

#endif //GA_LIB_KNAPSACKPOPULATION_H
