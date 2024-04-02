//
// Created by Victor Muryn on 01.04.2024.
//

#ifndef GA_LIB_GENETICALGORITHM_HPP
#define GA_LIB_GENETICALGORITHM_HPP

#pragma once

#include <vector>
#include <functional>
#include <random>
#include <thread>

#include "TPool.h"

template <typename T>
class Individual {
public:
    explicit Individual(std::vector<T> g) : genome(g), changed(true) {};
    Individual() : changed(true) {};

    double evaluateFitness(std::function<double(Individual)> fitness_function) {
        if (changed)
            fitness = fitness_function(*this);

        changed = false;
        return fitness;
    };

    void swap(int i, int j) {
        changed = true;
        std::swap(genome[i], genome[j]);
    }

    std::vector<T>& getGenome() { return genome; };
    double getFitness() { return fitness; };
private:
    std::vector<T> genome;
    double fitness = 0;
    bool changed = false;
};


template <typename T>
struct Config {
    int genome_size;
    int population_size = 1000;
    double mutation_rate = 0.01;
    double crossover_rate = 0.2;
    int max_generations = 1000;
    int tournament_size = 5;
    std::function<double(Individual<T>)> fitness_function;
};


template <typename T>
class GeneticAlgorithm {
public:
    explicit GeneticAlgorithm(Config<T> cfg);
    void initialize();
    void evaluate();
    Individual<T> select() const;
    Individual<T> crossover(Individual<T>& parent1, Individual<T>& parent2) const;
    void mutate(Individual<T>& individual) const;
//    void advanceGeneration();
    void evolve();

    Individual<T> getBestIndividual();
private:
    int genome_size;
    int population_size;
    double mutation_rate;
    double crossover_rate;
    int max_generations;
    int tournament_size = 5;
    std::vector<Individual<T>> population;
    std::function<double(Individual<T>)> fitness_function;

    std::vector<T> generateRandomPermutation();
};

#endif //GA_LIB_GENETICALGORITHM_HPP
