#include "population.h"

Individual Population::getBest() const{
    return *min_element(population.begin(), population.end());
}

// Setters
void Population::setSelection(Population::selections selectionType) {
    this->selectionType = selectionType;
}

void Population::setCrossover(Population::crossovers crossoverType) {
    this->crossoverType = crossoverType;
}

void Population::setMutation(Population::mutations mutationType) {
    this->mutationType = mutationType;
}

// default selections
Individual Population::simpleSelection() {
    int tournament_size = 5;

    Individual best_individual = population[std::rand() % population.size()];

    for (int i = 1; i < tournament_size; ++i) {
        Individual contender = population[std::rand() % population.size()];

        if (contender.fitness < best_individual.fitness) {
            best_individual = contender;
        }
    }

    return best_individual;
};


Individual Population::simpleCrossover(const Individual &parent1, const Individual &parent2) {
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

Individual Population::simpleMutation(const Individual& parent) {
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