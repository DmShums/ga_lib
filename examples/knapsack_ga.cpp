#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

const int POPULATION_SIZE = 100;
const int maxWeight = 50;
const int weights[] = {10, 20, 30};
const int values[] = {60, 100, 120};
const int NUM_OF_ITEMS = sizeof(values) / sizeof(values[0]);

struct Individual {
    int chromosome[NUM_OF_ITEMS];
    int fitness;

    Individual() {
        for (int i = 0; i < NUM_OF_ITEMS; i++) {
            chromosome[i] = std::rand() % 2;
        }
        fitness = calculateFitness();
    }

    int calculateFitness() {
        int currWeight = 0, currValue = 0;
        for (int i = 0; i < NUM_OF_ITEMS; i++) {
            currWeight += chromosome[i] * weights[i];
            currValue += chromosome[i] * values[i];
        }
        if (currWeight > maxWeight) return -1;
        return currValue;
    }
};

bool operator<(const Individual &ind1, const Individual &ind2) {
    return ind1.fitness < ind2.fitness;
}

Individual population[POPULATION_SIZE];
Individual parents[POPULATION_SIZE];
Individual offsprings[POPULATION_SIZE];

void selection() {
    std::sort(population, population + POPULATION_SIZE);
    int i;
    for (i = 0; i < POPULATION_SIZE / 2; i++) {
        parents[i] = population[i];
    }
    for (; i < POPULATION_SIZE; i++) {
        parents[i] = population[POPULATION_SIZE - i - 1];
    }
}

void crossover() {
    for (int i = 0; i < POPULATION_SIZE; i += 2) {
        int crossover_point = std::rand() % NUM_OF_ITEMS;
        for (int j = 0; j < crossover_point; j++) {
            offsprings[i].chromosome[j] = parents[i].chromosome[j];
            offsprings[i + 1].chromosome[j] = parents[i + 1].chromosome[j];
        }
        for (int j = crossover_point; j < NUM_OF_ITEMS; j++) {
            offsprings[i].chromosome[j] = parents[i + 1].chromosome[j];
            offsprings[i + 1].chromosome[j] = parents[i].chromosome[j];
        }
        offsprings[i].fitness = offsprings[i].calculateFitness();
        offsprings[i + 1].fitness = offsprings[i + 1].calculateFitness();
    }
}

void mutation() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        int mutation_point = std::rand() % NUM_OF_ITEMS;
        offsprings[i].chromosome[mutation_point] = 1 - offsprings[i].chromosome[mutation_point];
        offsprings[i].fitness = offsprings[i].calculateFitness();
    }
}

int main() {
    std::srand(std::time(0));
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = Individual();
    }
    for (int generation = 0; generation < 100; generation++) {
        selection();
        crossover();
        mutation();
        for (int i = 0; i < POPULATION_SIZE; i++) {
            population[i] = offsprings[i];
        }
    }
    std::cout << "Maximum value in knapsack = " << population[POPULATION_SIZE - 1].fitness << std::endl;
    return 0;
}