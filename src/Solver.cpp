#include "Solver.h"

Solver::Solver (SetUp setUp){
    genNum = setUp.generationsNum;
    crossoverPairsNum = setUp.crossoverPairsNum;
    mutationNum = setUp.mutationNum;
}

Individual Solver::solve(Population &population) {
    thread_pool pool(std::thread::hardware_concurrency());
//    thread_pool pool(2);

    for (size_t generation = 0; generation < genNum; ++generation) {
        std::vector<std::future<Individual>> fut_population;

        // crossover tasks
        for (size_t i = 0; i < crossoverPairsNum; ++i) {
            std::function<Individual()> task = [&population]() {
                Individual parent1 = population.selection();
                Individual parent2 = population.selection();
                Individual offspring = population.crossover(parent1, parent2);
                return offspring;
            };

            std::future<Individual> future = pool.submit(task);
            fut_population.push_back(std::move(future));
        }

        // mutation tasks
        for (size_t i = 0; i < mutationNum; ++i) {
            std::function<Individual()> task = [&population](){
                Individual parent = population.selection();
                Individual offspring = population.mutation(parent);
                return offspring;
            };

            std::future<Individual> future = pool.submit(task);
            fut_population.push_back(std::move(future));
        }

        // simple passing task
        size_t noChangeIndividualsNum = population.population.size() - mutationNum - crossoverPairsNum;
        for (size_t i = 0; i < noChangeIndividualsNum; ++i){
            std::function<Individual()> task = [&population](){
                return population.selection();
            };

            std::future<Individual> future = pool.submit(task);
            fut_population.push_back(std::move(future));
        }

        std::vector<Individual> new_population;
        for (auto &f: fut_population) {
            new_population.push_back(f.get());
        }
        population.population = new_population;

        std::cout << "Generation " << generation << std::endl;

        for(Individual ind : new_population){
            std::cout << ind.fitness << " ";
        }
        std::cout << std::endl;
    }

    return population.getBest();
}
