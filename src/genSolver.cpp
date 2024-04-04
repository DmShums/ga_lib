//
// Created by Sahaidak on 04.04.2024.
//
#include <thread>
#include <random>

#include "genSolver.h"
#include "TPool.h"

Individual Solver::solve(Problem problem) {
    // getting data from problem struct;
    Population population = problem.firstPopulation;

    size_t genNum = problem.generation_num;
    size_t popSize = population.size();

    crossover_func_t* crossoverFunc = problem.crossoverFunc;
    mutation_func_t* mutationFunc = problem.mutationFunc;
    selection_func_t* selectionFunc = problem.selectionFunc;

    size_t XOverPairsNum = problem.crossover_pairs_num;
    double mutRate = problem.mutation_rate;

    thread_pool tPool(std::thread::hardware_concurrency());

    //generation loop
    for (size_t gen = 0; gen < genNum; ++gen){
        FuturePopulation futPop;

        // crossover
        for (size_t i = 0; i < XOverPairsNum; ++i) {
            std::function<Individual()> task = [&population, &selectionFunc, &crossoverFunc]() {
                Individual parent1 = selectionFunc(population);
                Individual parent2 = selectionFunc(population);

                Individual offspring = crossoverFunc(parent1, parent2);
                return offspring;
            };
            /*
             * it was like this in last code
            std::future<Individual> future = pool.submit(task);
            futPop.push_back(std::move(future));
             */
            futPop.push_back(std::move(tPool.submit(task)));
        }

        // mutation
        for (size_t iter = 0; iter < popSize; ++iter){
            if(std::rand() <= mutRate * RAND_MAX){
                Individual ind = futPop[iter].get();

                std::function<Individual()> task = [&ind, &mutationFunc]() {
                    return mutationFunc(ind);
                };

                futPop[iter] = std::move(tPool.submit(task));
            }
        }

        Population nextPopulation;
        // getting results
        for (std::future<Individual> &futInd : futPop){
            nextPopulation.push_back(futInd.get());
        }

        /*
         * if we will add abs/rel errors the check should be here
         */

        population = nextPopulation;
    }

    // sorts in ascending order
    std::sort(population.begin(), population.end());
    return population[popSize - 1];
}