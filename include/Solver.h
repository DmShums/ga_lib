#ifndef GA_LIB_SOLVER_H
#define GA_LIB_SOLVER_H

#include "Population.h"
#include "TPool.h"

struct SetUp {
    size_t generationsNum{};
    double crossoverRate{};
    double mutationRate{};

    int absError = 0;
    double relError = 0;
    bool reachBothErrorBounds = false;

    bool sorted = false;
};

class Solver {
private:
    double relError;
    int absError;
    bool checkError;
    bool reachBothErrorBounds;

    bool reachedErrorBounds(const Individual &prevBest,
                            const Individual &newBest,
                            bool useAnd);
public:
    size_t genNum;
    bool sorted;

    double crossoverRate;
    double mutationRate;

    size_t crossoverPairsNum{};
    size_t mutationNum{};

    explicit Solver(SetUp setUp);

    Individual solve(Population& population, size_t threads = std::thread::hardware_concurrency());
};

#endif //GA_LIB_SOLVER_H
