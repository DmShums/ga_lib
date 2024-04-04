//
// Created by Sahaidak on 04.04.2024.
//

#include <vector>
#include <future>

#ifndef GA_LIB_GENSOLVER_H
#define GA_LIB_GENSOLVER_H

class Individual{
public:
    virtual int estimate() const;

    bool operator < (const Individual& ind) const
    {
        return (estimate() < ind.estimate());
    }
};

typedef std::vector<Individual> Population;
typedef std::vector<std::future<Individual>> FuturePopulation;

typedef Individual selection_func_t(Population);
typedef Individual crossover_func_t(Individual, Individual);
// maybe will use future population not to wait all results from
// crossover but to path them to mutation. This might save some time
typedef Individual mutation_func_t(Individual);

struct Problem{
    Population firstPopulation;

    size_t generation_num;

    selection_func_t* selectionFunc;
    crossover_func_t* crossoverFunc;
    mutation_func_t* mutationFunc;

    size_t crossover_pairs_num;
    double mutation_rate;               // [0, 1]
};

class Solver{
public:
    virtual Individual solve(Problem problem);
};

#endif //GA_LIB_GENSOLVER_H
