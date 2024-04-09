#include "Solver.h"

#define DEBUG false

Solver::Solver (SetUp setUp) {
    genNum = setUp.generationsNum;
    crossoverRate = setUp.crossoverRate;
    mutationRate = setUp.mutationRate;
    absError = setUp.absError;
    relError = setUp.relError;
    reachBothErrorBounds = setUp.reachBothErrorBounds;
}

bool Solver::reachedErrorBounds(const Individual &prevBest, const Individual &newBest, bool useAnd){
    bool reachedAbsErr = (prevBest.fitness - newBest.fitness) < absError;
    bool reachedRelErr = (double)(prevBest.fitness - newBest.fitness) / (double)prevBest.fitness < relError;

    return reachedAbsErr && reachedRelErr || (useAnd && (reachedAbsErr || reachedRelErr));
}

Individual Solver::solve(Population &population) {
    thread_pool pool(std::thread::hardware_concurrency());

    if (mutationRate + crossoverRate > 1) {
        std::cerr << "Wrong inputs. mutationRate + crossoverRate must be <= 1" << std::endl;
        return Individual{};
    }

    size_t noChangeIndividualsNum = population.population.size() * (1 - mutationRate - crossoverRate);

    mutationNum = population.population.size() * mutationRate;
    crossoverPairsNum = population.population.size() * crossoverRate;

    std::function<Individual()> crossoverTask = [&population]() {
        Individual parent1 = population.selection();
        Individual parent2 = population.selection();
        Individual offspring = population.crossover(parent1, parent2);
        return offspring;
    };

    std::function<Individual()> mutationTask = [&population](){
        Individual parent = population.selection();
        Individual offspring = population.mutation(parent);
        return offspring;
    };

    std::function<Individual()> selectionTask = [&population](){
        return population.selection();
    };

    for (size_t generation = 0; generation < genNum; ++generation) {
        std::vector<std::future<Individual>> fut_population;
        ++population.generationNumber;

        // crossover tasks
        for (size_t i = 0; i < crossoverPairsNum; ++i) {
            std::future<Individual> future = pool.submit(crossoverTask);
            fut_population.emplace_back(std::move(future));
        }

        // mutation tasks
        for (size_t i = 0; i < mutationNum; ++i) {
            std::future<Individual> future = pool.submit(mutationTask);
            fut_population.emplace_back(std::move(future));
        }

        // simple passing task
        for (size_t i = 0; i < noChangeIndividualsNum; ++i){
            std::future<Individual> future = pool.submit(selectionTask);
            fut_population.emplace_back(std::move(future));
        }

        // get all results from ThreadPool
        std::vector<Individual> new_population;
        for (auto &f: fut_population) {
            new_population.emplace_back(f.get());
        }

        Individual prevBest = population.getBest();
        population.population = new_population;
        Individual newBest = population.getBest();

//        if (reachedErrorBounds(prevBest, newBest, reachBothErrorBounds)) break;

#if DEBUG
        std::cout << "Generation " << generation << std::endl;
        for (Individual ind : new_population){
            std::cout << ind.fitness << " ";
        }
        std::cout << std::endl;
#endif
    }

    return population.getBest();
}
