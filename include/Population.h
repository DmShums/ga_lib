#ifndef GA_LIB_POPULATION_H
#define GA_LIB_POPULATION_H

#include <queue>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>
#include <random>
#include <limits>

class Individual {
/*
 * Abstract individual class. Should be inherited by all the other classes
 */
public:
    std::vector<size_t> solution;
    int fitness;

    std::strong_ordering operator <=>(const Individual& other) const {
        return fitness <=> other.fitness;
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
    size_t generationNumber = 0;

    virtual void evaluate(Individual&) const = 0;

    virtual bool isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const {
        return ind1 > ind2;
    }

    virtual Individual getBest() const {
        Individual best = population.front();

        for (const Individual& candidate : population) {
            if (isFirstBetterThanSecond(candidate, best)) {
                best = candidate;
            }
        }

        return best;
    };

//    selections
    selection_t simpleSelection;
    selection_t rankSelection;
    selection_t boltzmannSelection;
    selection_t proportionalSelection;

    enum class selections { simple, rank, boltzmann, proportional };

    selections selectionType = selections::simple;
    void setSelection(selections selectionType);

    virtual Individual selection() {
        switch (selectionType) {
            case selections::rank:
                return rankSelection();
            case selections::boltzmann:
                return boltzmannSelection();
            case selections::proportional:
                return proportionalSelection();
            case selections::simple:
            default:
                return simpleSelection();
        }
    };

//    crossovers
    crossover_t simpleCrossover;
    crossover_t uniformCrossover;

    enum class crossovers { simple, uniform };

    crossovers crossoverType = crossovers::simple;
    void setCrossover(crossovers crossoverType);

    virtual Individual crossover(const Individual &parent1, const Individual &parent2) {
        switch (crossoverType) {
            case crossovers::uniform:
                return uniformCrossover(parent1, parent2);
            case crossovers::simple:
            default:
                return simpleCrossover(parent1, parent2);
        }
    }

//    mutations
    mutation_t simpleMutation;
    mutation_t rotationMutation;
    mutation_t inverseMutation;

    enum class mutations { simple, rotation, inverse };

    mutations mutationType = mutations::simple;

    void setMutation(mutations mutateType);

    virtual Individual mutation(const Individual &parent) {
        switch (mutationType) {
            case mutations::rotation:
                return rotationMutation(parent);
            case mutations::inverse:
                return inverseMutation(parent);
            case mutations::simple:
            default:
                return simpleMutation(parent);
        }
    };
};

#endif //GA_LIB_POPULATION_H
