#include "SalesmanPopulation.h"

std::vector<size_t> getRandomPermutation(size_t len){
    std::vector<size_t> permutation(len);
    for (size_t i = 0; i < len; ++i) {
        permutation[i] = i;
    }
    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    return permutation;
}

void SalesmanPopulation::evaluate(Individual& ind) const{
    ind.fitness = 0;
    size_t prev = ind.solution[0];
    for(size_t i = 1; i < ind.solution.size(); ++i){
        size_t next = ind.solution[i];
        ind.fitness += distMatrix[prev][next];
    }
    ind.fitness += distMatrix[ind.solution.back()][ind.solution[0]];
}

SalesmanPopulation::SalesmanPopulation(const std::vector<std::vector<int>>& distMatrix,
                                       size_t populationSize) : distMatrix(distMatrix){
    for(size_t i = 0; i < populationSize; ++i){
        Individual ind{};
        ind.solution = getRandomPermutation(distMatrix.size());
        evaluate(ind);
        population.push_back(ind);
    }

    selectionList = {"simple"};
}

Individual SalesmanPopulation::selection() {
    if(selectionType == "simple"){
        return simpleSelection();
    }
}

Individual SalesmanPopulation::crossover(const Individual &parent1, const Individual &parent2) {
    if(crossoverType == "simple"){
        return simpleCrossover(parent1, parent2);
    }
}

Individual SalesmanPopulation::mutation(const Individual &parent) {
    if(mutationType == "simple"){
        return simpleMutation(parent);
    }
}

Individual SalesmanPopulation::simpleSelection() const{
    int tournament_size = 5;
    Individual best_individual = population[std::rand() % population.size()];

    for (int i = 1; i < tournament_size; ++i) {
        Individual contender = population[std::rand() % population.size()];

        if (contender.fitness < best_individual.fitness) {
            best_individual = contender;
        }
    }
    return best_individual;
}


Individual SalesmanPopulation::simpleCrossover(const Individual &parent1, const Individual &parent2) const {
    Individual offspring;
    size_t solutionLen = parent1.solution.size();
    int start = std::rand() % solutionLen;
    int end = std::rand() % solutionLen;
    if (start > end) {
        std::swap(start, end);
    }
    std::vector<int> segment(parent1.solution.begin() + start, parent1.solution.begin() + end);

    for (int i = 0, j = 0; i < solutionLen; ++i) {
        if (i >= start && i < end) {
            offspring.solution.push_back(parent1.solution[i]);
        } else {
            while (std::find(segment.begin(), segment.end(), parent2.solution[j]) != segment.end()) {
                ++j;
            }
            offspring.solution.push_back(parent2.solution[j]);
            ++j;
        }
    }
    evaluate(offspring);
    return offspring;
}

Individual SalesmanPopulation::simpleMutation(const Individual& parent) const{
    Individual offspring(parent);
    double mutationRate = 0.05;
    size_t solutionLen = parent.solution.size();
    for (size_t i = 0; i < solutionLen; ++i) {
        if ((std::rand() / (double)RAND_MAX) < mutationRate) {
            int j = std::rand() % solutionLen;
            std::swap(offspring.solution[i], offspring.solution[j]);
        }
    }

    evaluate(offspring);
    return offspring;
}
