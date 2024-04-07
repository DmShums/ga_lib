#include <queue>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>

#ifndef GA_LIB_POPULATION_H
#define GA_LIB_POPULATION_H

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
    std::string selectionType;
    std::string crossoverType;
    std::string mutationType;

    std::vector<std::string> selectionList;
    std::vector<std::string> crossoverList;
    std::vector<std::string> mutationList;
public:
    std::vector<Individual> population;

    virtual void evaluate(Individual&) const = 0;
    Individual getBest() const;

    virtual Individual selection() = 0;                     // function to use while selection
    std::vector<std::string> getSelectionTypes() const;     // function to get list of provided selections
    void setSelectionType(const std::string &type);         // function to set selection type

    virtual Individual crossover(const Individual& parent1, const Individual& parent2) = 0;
    std::vector<std::string> getCrossoverTypes() const;
    void setCrossoverType(const std::string &type);

    virtual Individual mutation(const Individual& parent) = 0;
    std::vector<std::string> getMutationTypes() const;
    void setMutationTypes(const std::string &type);
};

#endif //GA_LIB_POPULATION_H
