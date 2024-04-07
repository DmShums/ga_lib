#include <random>

#include "Population.h"

#ifndef GA_LIB_SALESMANPOPULATION_H
#define GA_LIB_SALESMANPOPULATION_H

class SalesmanPopulation : public Population{
private:
    std::vector<std::vector<int>> distMatrix;
public:
    SalesmanPopulation() = delete;
    SalesmanPopulation(const std::vector<std::vector<int>>& distMatrix,
                       size_t populationSize);

    void evaluate(Individual& ind) const override;

    Individual simpleSelection() const;
    Individual selection() override;

    Individual simpleCrossover(const Individual& parent1, const Individual& parent2) const;
    Individual crossover(const Individual& parent1, const Individual& parent2) override;

    Individual simpleMutation(const Individual &parent) const;
    Individual mutation(const Individual& parent) override;
};

#endif //GA_LIB_SALESMANPOPULATION_H
