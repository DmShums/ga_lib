#include "GeneticAlgorithm.hpp"

template <class T>
GeneticAlgorithm<T>::GeneticAlgorithm(Config<T> cfg) :
    population_size(cfg.population_size), mutation_rate(cfg.mutation_rate), crossover_rate(cfg.crossover_rate),
    max_generations(cfg.max_generations), fitness_function(cfg.fitness_function), genome_size(cfg.genome_size)
{}

template <class T>
std::vector<T> GeneticAlgorithm<T>::generateRandomPermutation()  {
    std::vector<T> permutation(genome_size);

    /// TODO: one standard population
    for (int i = 0; i < genome_size; ++i) {
        permutation[i] = i;
    }

    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    return permutation;
}

template <class T>
void GeneticAlgorithm<T>::initialize() {
    population.reserve(population_size);

    for (int i = 0; i < population_size; ++i) {
        population.emplace_back( generateRandomPermutation() );
        population[i].evaluateFitness(fitness_function);
    }
}

template <class T>
void GeneticAlgorithm<T>::evaluate() {
    for (Individual<T>& individual : population) {
        individual.evaluateFitness(fitness_function);
    }
}

template <class T>
Individual<T> GeneticAlgorithm<T>::select() const {
    auto best_individual = population[std::rand() % population_size];

    for (int i = 0; i < tournament_size; ++i) {
        auto contender = population[std::rand() % population_size];

        if (contender.getFitness() < best_individual.getFitness()) {
            best_individual = contender;
        }
    }

    return best_individual;
}

template <class T>
Individual<T> GeneticAlgorithm<T>::crossover(Individual<T>& parent1, Individual<T>& parent2) const {
    std::vector<T> offspring_genome;
    offspring_genome.reserve(genome_size);

    int start = std::rand() % genome_size;
    int end = std::rand() % genome_size;

    if (start > end) {
        std::swap(start, end);
    }

    std::vector<int> segment(parent1.getGenome().begin() + start, parent1.getGenome().begin() + end);

    for (int i = 0, j = 0; i < genome_size; ++i) {
        if (i >= start && i < end) {
            offspring_genome.emplace_back(parent1.getGenome()[i]);
        } else {
            while (std::find(segment.begin(), segment.end(), parent2.getGenome()[j]) != segment.end()) {
                ++j;
            }

            offspring_genome.emplace_back(parent2.getGenome()[j]);
            ++j;
        }
    }

    Individual<T> offspring{offspring_genome};
    offspring.evaluateFitness(fitness_function);
    return offspring;
}

template <class T>
void GeneticAlgorithm<T>::mutate(Individual<T>& individual) const {
    for (int i = 0; i < genome_size; ++i) {
        if ((std::rand() / (double)RAND_MAX) < mutation_rate) {
            int j = std::rand() % genome_size;

            individual.swap(i, j);
        }
    }

    individual.evaluateFitness(fitness_function);
}


template <class T>
void GeneticAlgorithm<T>::evolve() {
    thread_pool pool(std::thread::hardware_concurrency());

    std::function<Individual<T>()> task = [this]() {
        auto parent1 = select();
        auto parent2 = select();
        auto offspring = crossover(parent1, parent2);

        mutate(offspring);
        return offspring;
    };

    for (int generation = 0; generation < max_generations; ++generation) {
        std::vector<Individual<T>> new_population;
        std::vector<std::future<Individual<T>>> fut_population;

        new_population.reserve(population_size);
        fut_population.reserve(population_size);

        for (int i = 0; i < population_size; ++i) {
            fut_population.emplace_back( std::move(pool.submit(task)) );
//            auto parent1 = select();
//            auto parent2 = select();
//            auto offspring = crossover(parent1, parent2);
//
//            mutate(offspring);
//            new_population.emplace_back(offspring);
        }

        for (auto &f : fut_population) {
            new_population.emplace_back(f.get());
        }

        population = new_population;
    }
}

template <class T>
Individual<T> GeneticAlgorithm<T>::getBestIndividual() {
    return *std::min_element(population.begin(), population.end(), [](Individual<T> a, Individual<T> b) {
        return a.getFitness() < b.getFitness();
    });
}