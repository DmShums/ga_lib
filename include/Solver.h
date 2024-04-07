
#include "Population.h"
#include "TPool.h"

#ifndef GA_LIB_SOLVER_H
#define GA_LIB_SOLVER_H

struct SetUp{
    size_t generationsNum;
    size_t crossoverPairsNum;
    size_t mutationNum;
};

class Solver{
public:
    size_t genNum;
    size_t crossoverPairsNum;
    size_t mutationNum;

    explicit Solver(SetUp setUp);

    Individual solve(Population& population);
};

#endif //GA_LIB_SOLVER_H
