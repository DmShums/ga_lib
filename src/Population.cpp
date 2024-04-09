#include "Population.h"

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

        if (isFirstBetterThanSecond(contender, best_individual)) {
            best_individual = contender;
        }
    }

    return best_individual;
}

Individual Population::rankSelection() {
    std::vector<Individual> sortedPopulation = population;
    std::sort(sortedPopulation.begin(), sortedPopulation.end());

    size_t populationSize = population.size();
    std::vector<double> probabilities(populationSize);

    for (size_t i = 0; i < populationSize; ++i) {
        // Higher probability to individuals with higher rank
        probabilities[i] = (2.0 * (populationSize - i)) / (populationSize * (populationSize + 1));
    }

    // Selection using roulette wheel selection
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
    int selectedIndex = dist(gen);

    return sortedPopulation[selectedIndex];
}

double calculateTemperature(size_t generation) {
    double initial_temperature = 100.0;
    double cooling_factor = 0.9;

    return initial_temperature * std::pow(cooling_factor, generation);
}

Individual Population::boltzmannSelection() {
    // Sum of Boltzmann factors
    auto temperature = calculateTemperature(generationNumber);
    double sum = 0.0;
    for (const Individual& individual : population) {
        sum += std::exp(individual.fitness / temperature);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, sum);
    double randNum = dis(gen);

    // Find the selected individual based on Boltzmann probabilities
    double partialSum = 0.0;
    for (const Individual& individual : population) {
        partialSum += std::exp(individual.fitness / temperature);

        if (partialSum >= randNum) {
            return individual;
        }
    }

    return population.back();
}

Individual Population::proportionalSelection() {
        int totalFitness = 0;
        for (const Individual& ind : population) {
            totalFitness += ind.fitness;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, totalFitness);

        int randValue = dis(gen);
        int accumulatedFitness = 0;
        for (const Individual& ind : population) {
            accumulatedFitness += ind.fitness;

            if (accumulatedFitness >= randValue) {
                return ind;
            }
        }

        // This line should never be reached but added for compiler safety
        return population.back();
    }

// default crossovers
Individual Population::simpleCrossover(const Individual &parent1, const Individual &parent2) {
    Individual offspring;
    size_t solutionLen = parent1.solution.size();
    size_t start = std::rand() % solutionLen;
    size_t end = std::rand() % solutionLen;

    if (start > end) {
        std::swap(start, end);
    }
    std::vector<int> segment(parent1.solution.begin() + start, parent1.solution.begin() + end);

    for (size_t i = 0, j = 0; i < solutionLen; ++i) {
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

// default mutations
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