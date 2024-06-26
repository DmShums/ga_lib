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
//    the population is already sorted
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

    return population[selectedIndex];
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

// uniform crossover
std::vector<int> create_mask(size_t chrom_size, double px) {
    std::vector<int> mask(chrom_size, 0);

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<> distrFloat(0.0, 1.0);


    for (size_t i = 0; i < chrom_size; ++i){
        double prob = distrFloat(eng);

        if (prob < px) {
            mask[i] = 1;
        }
    }
    return mask;
}

Individual Population::uniformCrossover(const Individual &parent1, const Individual &parent2){
    Individual offspring{};
    size_t solutionLen = parent1.solution.size();

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<> distrFloat(0.0, 1.0);

    std::vector<int> mask = create_mask(solutionLen, distrFloat(eng));
    std::vector<bool> taken(solutionLen, false);

    for (size_t i = 0; i < solutionLen; ++i) {
        if (mask[i] == 1) {
            if (!taken[parent1.solution[i]]) {
                offspring.solution.emplace_back(parent1.solution[i]);
                taken[parent1.solution[i]] = true;
            } else {
                offspring.solution.emplace_back(std::numeric_limits<size_t>::max());
            }
        } else {
            if (!taken[parent2.solution[i]]) {
                offspring.solution.emplace_back(parent2.solution[i]);
                taken[parent2.solution[i]] = true;
            } else {
                offspring.solution.emplace_back(-1);
            }
        }
    }

    for (size_t i = 0; i < solutionLen; ++i) {
        if (offspring.solution[i] == std::numeric_limits<size_t>::max()) {
            for (size_t j = 0; j < solutionLen; ++j) {
                if (!taken[j]) {
                    offspring.solution[i] = j;
                    taken[j] = true;
                    break;
                }
            }
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

Individual Population::rotationMutation(const Individual& parent) {
    Individual offspring(parent);
    double mutationRate = 0.05;
    size_t solutionLen = parent.solution.size();

    if ((std::rand() / (double)RAND_MAX) < mutationRate) {
        size_t subsetSize = std::rand() % solutionLen;
        size_t startIndex = std::rand() % solutionLen;

        // Rotate the elements to the right
        std::rotate(offspring.solution.begin(), offspring.solution.begin() + startIndex, offspring.solution.end());
    }

    evaluate(offspring);
    return offspring;
}

Individual Population::inverseMutation(const Individual& parent) {
    Individual offspring(parent);
    std::reverse(offspring.solution.begin(), offspring.solution.end());
    evaluate(offspring);
    return offspring;
}
