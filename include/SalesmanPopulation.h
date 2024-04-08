#ifndef GA_LIB_SALESMANPOPULATION_H
#define GA_LIB_SALESMANPOPULATION_H

#include <random>
#include "Population.h"

class SalesmanPopulation : public Population {
private:
    std::vector<std::vector<int>> distMatrix;
public:
    SalesmanPopulation() = delete;
    SalesmanPopulation(const std::vector<std::vector<int>>& distMatrix,
                       size_t populationSize);

    void evaluate(Individual& ind) const override;

    /*
     * if you want to override mutation for you implementation, use this.
     *
     * Individual mutation(const Individual& parent) override;
     */
};

#endif //GA_LIB_SALESMANPOPULATION_H
