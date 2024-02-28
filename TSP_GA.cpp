#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "timer.h"
#include <cstdlib>
#include <random>


int NUM_CITIES;
const int POPULATION_SIZE = 100;
const int NUM_GENERATIONS = 1000;
const double MUTATION_RATE = 0.01;

std::vector<std::vector<int>> distance_matrix;
//std::vector<std::vector<int>> distance_matrix = {
//        {0,12,20,19,13,14,12,12,11,17,12,17,2,15,19,18,9,1,15,7,6},
//        {12,0,5,11,3,18,13,13,7,13,8,14,8,1,13,7,15,20,3,19,5},
//        {20,5,0,9,2,9,8,3,10,3,5,17,9,7,6,12,15,6,14,7,18},
//        {19,11,9,0,20,21,3,5,21,17,16,7,9,1,14,12,18,3,18,15,5},
//        {13,3,2,20,0,6,7,10,21,15,2,6,2,4,6,14,19,2,9,17,8},
//        {14,18,9,21,6,0,11,10,12,5,21,18,5,1,12,18,14,13,16,17,19},
//        {12,13,8,3,7,11,0,17,6,11,9,11,13,16,5,17,17,9,10,17,8},
//        {12,13,3,5,10,10,17,0,4,2,20,3,1,16,21,20,8,4,6,5,18},
//        {11,7,10,21,21,12,6,4,0,7,1,5,16,12,14,7,11,6,20,16,2},
//        {17,13,3,17,15,5,11,2,7,0,5,16,17,15,13,1,5,14,9,15,18},
//        {12,8,5,16,2,21,9,20,1,5,0,5,4,4,14,5,21,6,17,12,3},
//        {17,14,17,7,6,18,11,3,5,16,5,0,1,13,10,5,21,8,11,5,19},
//        {2,8,9,9,2,5,13,1,16,17,4,1,0,16,3,7,17,12,11,8,19},
//        {15,1,7,1,4,1,16,16,12,15,4,13,16,0,14,8,4,12,15,1,16},
//        {19,13,6,14,6,12,5,21,14,13,14,10,3,14,0,2,9,19,5,19,6},
//        {18,7,12,12,14,18,17,20,7,1,5,5,7,8,2,0,18,9,21,2,14},
//        {9,15,15,18,19,14,17,8,11,5,21,21,17,4,9,18,0,8,1,18,9},
//        {1,20,6,3,2,13,9,4,6,14,6,8,12,12,19,9,8,0,5,5,5},
//        {15,3,14,18,9,16,10,6,20,9,17,11,11,15,5,21,1,5,0,15,9},
//        {7,19,7,15,17,17,17,5,16,15,12,5,8,1,19,2,18,5,15,0,3},
//        {6,5,18,5,8,19,8,18,2,18,3,19,19,16,6,14,9,5,9,3,0}
//};

std::vector<std::vector<int>> readCSVFile(const std::string& filename) {
    std::vector<std::vector<int>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> row;
        std::string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

struct Individual {
    std::vector<int> permutation;
    int fitness;

    Individual() {
        fitness = 0;
    }
};

std::vector<int> generateRandomPermutation() {
    std::vector<int> permutation(NUM_CITIES);
    for (int i = 0; i < NUM_CITIES; ++i) {
        permutation[i] = i;
    }
    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    return permutation;
}

int calculateTotalDistance(const std::vector<int>& permutation) {
    int total_distance = 0;
    for (int i = 0; i < NUM_CITIES - 2; ++i) {
        total_distance += distance_matrix[permutation[i]][permutation[i + 1]];
    }
    total_distance += distance_matrix[permutation[NUM_CITIES - 1]][permutation[0]];
    return total_distance;
}

std::vector<Individual> initializePopulation() {
    std::vector<Individual> population(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].permutation = generateRandomPermutation();
        population[i].fitness = calculateTotalDistance(population[i].permutation);
    }
    return population;
}

Individual tournamentSelection(const std::vector<Individual>& population) {
    int tournament_size = 5;
    Individual best_individual = population[std::rand() % POPULATION_SIZE];
    for (int i = 1; i < tournament_size; ++i) {
        Individual contender = population[std::rand() % POPULATION_SIZE];
        if (contender.fitness < best_individual.fitness) {
            best_individual = contender;
        }
    }
    return best_individual;
}

Individual crossover(const Individual& parent1, const Individual& parent2) {
    Individual offspring;
    int start = std::rand() % NUM_CITIES;
    int end = std::rand() % NUM_CITIES;
    if (start > end) {
        std::swap(start, end);
    }
    std::vector<int> segment(parent1.permutation.begin() + start, parent1.permutation.begin() + end);

    for (int i = 0, j = 0; i < NUM_CITIES; ++i) {
        if (i >= start && i < end) {
            offspring.permutation.push_back(parent1.permutation[i]);
        } else {
            while (std::find(segment.begin(), segment.end(), parent2.permutation[j]) != segment.end()) {
                ++j;
            }
            offspring.permutation.push_back(parent2.permutation[j]);
            ++j;
        }
    }
    offspring.fitness = calculateTotalDistance(offspring.permutation);
    return offspring;
}

void mutate(Individual& individual) {
    for (int i = 0; i < NUM_CITIES; ++i) {
        if ((std::rand() / (double)RAND_MAX) < MUTATION_RATE) {
            int j = std::rand() % NUM_CITIES;
            std::swap(individual.permutation[i], individual.permutation[j]);
        }
    }
    individual.fitness = calculateTotalDistance(individual.permutation);
}

std::vector<Individual> geneticAlgorithm() {
    std::vector<Individual> population = initializePopulation();
    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        std::vector<Individual> new_population;
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            Individual parent1 = tournamentSelection(population);
            Individual parent2 = tournamentSelection(population);
            Individual offspring = crossover(parent1, parent2);
            mutate(offspring);
            new_population.push_back(offspring);
        }
        population = new_population;
    }
    return population;
}

int main() {
    auto filename = "../data/graph.csv";
    distance_matrix = readCSVFile(filename);
    NUM_CITIES = distance_matrix.size();
    std::cout << NUM_CITIES << std::endl;

    std::srand(time(NULL));
    std::vector<Individual> final_population = geneticAlgorithm();
    Individual best_individual = *std::min_element(final_population.begin(), final_population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
    });
    std::cout << "Best permutation: ";
    for (int i = 0; i < NUM_CITIES; ++i) {
        std::cout << best_individual.permutation[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Total distance: " << best_individual.fitness << std::endl;
    return 0;
}
