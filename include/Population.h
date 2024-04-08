#ifndef GA_LIB_POPULATION_H
#define GA_LIB_POPULATION_H

#include <queue>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>

class Individual {
/*
 * Abstract individual class. Should be inherited by all the other classes
 */
public:
    std::vector<size_t> solution;
    int fitness;
    bool operator < (const Individual& other) const {
        return (fitness < other.fitness);
    }
};


class Population {
/*
 * Abstract population class. Should be inherited by all the other classes
 */
protected:
    typedef Individual selection_t();
    typedef Individual crossover_t(const Individual &parent1, const Individual &parent2);
    typedef Individual mutation_t(const Individual &parent);

public:
    std::vector<Individual> population;

    virtual void evaluate(Individual&) const = 0;
    Individual getBest() const;

    selection_t simpleSelection;
    crossover_t simpleCrossover;
    mutation_t simpleMutation;

    enum class selections { simple };
    enum class crossovers { simple };
    enum class mutations { simple };

    selections selectionType = selections::simple;
    crossovers crossoverType = crossovers::simple;
    mutations mutationType = mutations::simple;

    virtual Individual selection() {
        switch (selectionType) {
            case selections::simple:
                return simpleSelection();
        }
    };

    void setSelection(selections selectionType);

    virtual Individual crossover(const Individual &parent1, const Individual &parent2) {
        switch (crossoverType) {
            case crossovers::simple:
                return simpleCrossover(parent1, parent2);
        }
    }
    void setCrossover(crossovers crossoverType);

    virtual Individual mutation(const Individual &parent) {
        switch (mutationType) {
            case mutations::simple:
                return simpleMutation(parent);
        }
    };

    void setMutation(mutations mutateType);
};

#endif //GA_LIB_POPULATION_H
