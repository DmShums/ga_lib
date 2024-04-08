
#include "Population.h"
#include "TPool.h"

#ifndef GA_LIB_SOLVER_H
#define GA_LIB_SOLVER_H

struct SetUp{
    size_t generationsNum;
    double crossoverRate;
    double mutationRate;
};

class Solver{
public:
    size_t genNum;

    double crossoverRate;
    double mutationRate;

    size_t crossoverPairsNum{};
    size_t mutationNum{};

    explicit Solver(SetUp setUp);

    Individual solve(Population& population);
};

#endif //GA_LIB_SOLVER_H
