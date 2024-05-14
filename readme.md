# Genetic Algorithms Library

### Description

There are plenty of NP-complete algorithms, which
require a lot of time to be solved. The genetic algorithms allow
us to reach great results with significantly lower complexity. The
project’s main topic is the development of different techniques
for genetic algorithms and comparing them with each other. 
The details are in a [paper](https://www.overleaf.com/read/scdmmjntdcbd#daa60d).

### Installation
The easiest way to include our library is to clone this repository as a submodule to your project

```shell
git submodule add https://github.com/DmShums/ga_lib.git ./includes/ga_lib/
```

### Include

After that include library in your CmakeLists.txt

```cmake
target_include_directories(${PROJECT_NAME} PRIVATE "./include/ga_lib/include")
```

### Library API

#### Solver

This class as the only parameter accepts an SetUp structure.

- generationsNum: `size_t` — number of generations to calculate.
- crossoverRate: `double [0, 1]` — how many crossovers should be done in % to total population.
- mutationRate: `double [0, 1]` — how many mutations should be done in % to total population.

Important: `crossoverRate + mutationRate <= 1`. The reason is because `1 - crossoverRate - mutationRate` % of elements will forward-pass.

- sorted: `bool = false` — do you need to sort your population after each generation. Some selection algorithms might need to have a sorted population.

_Experimental features:_

- absError: `int = 0` — what absolute error you want to achieve. If algorithm achieved that precision, it will stop evolution.
- relError: `double = 0` — the same as `absError`, but in relative. 
- reachBothErrorBounds: `bool = false` — do we need to achieve only one error or two at the same time.

Also, the Solver class has a method `Individual solve(Population& population)` to get the solution of your problem. 
Also, this method has second optional argument: number of threads to run in parallel. By default, `std::thread::hardware_concurrency()`.

#### Individual
Abstract individual class. This class contains:

- `std::vector<size_t> solution` — the solution for this individual.
- `int fitness` — the number that tells how your Individual is good.

#### Population
The population is a main class to solve a problem. Here you should define main properties of Population.

- `std::vector<Individual> population` — the vector of individuals.
- `void evaluate(Individual&) const` — evaluate the individual. You should implement this method in your class.
- `bool isFirstBetterThanSecond(Individual& first, Individual& second)` — the function that tells how to compare two individuals. By default, it compares their fitnesses.

- `void setSelection(selection_t selection)` — set the selection algorithm.
- `void setMutation(mutation_t mutation)` — set the mutation algorithm.
- `void setCrossover(crossover_t crossover)` — set the crossover algorithm.

### Example

Here is an example of how to use the library to solve the Travelling Salesman Problem.

SalesmanPopulation.h
```cpp
#include <ga_lib/population.h>

class SalesmanPopulation : public Population {
private:
    std::vector<std::vector<int>> distMatrix;
public:
    SalesmanPopulation() = delete;
    SalesmanPopulation(const std::vector<std::vector<int>>& distMatrix,
                       size_t populationSize);

    void evaluate(Individual& ind) const override;
    bool isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const override;
};
```

SalesmanPopulation.cpp
```cpp
#include "SalesmanPopulation.h"

std::vector<size_t> getRandomPermutation(size_t len) {
    std::vector<size_t> permutation(len);

    for (size_t i = 0; i < len; ++i) {
        permutation[i] = i;
    }

    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    return permutation;
}

bool SalesmanPopulation::isFirstBetterThanSecond(const Individual& ind1, const Individual& ind2) const {
    return ind1 < ind2;
}

void SalesmanPopulation::evaluate(Individual& ind) const {
    int fitness = 0;
    size_t prev = ind.solution.front();

    for (size_t i = 1; i < ind.solution.size(); ++i) {
        size_t next = ind.solution[i];
        fitness += distMatrix[prev][next];
        prev = next;
    }

    fitness += distMatrix[ind.solution.back()][ind.solution.front()];
    ind.fitness = fitness;
}

SalesmanPopulation::SalesmanPopulation(const std::vector<std::vector<int>>& distMatrix,
                                       size_t populationSize) : distMatrix(distMatrix) {
    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind{};
        ind.solution = getRandomPermutation(distMatrix.size());
        evaluate(ind);
        population.emplace_back(ind);
    }
}
```
    
main.cpp
```cpp
#include <iostream>
#include <ga_lib/solver.h>
#include "SalesmanPopulation.h"
    
int main(int argc, char** argv) {
    auto path = argv[1];
    auto distance_matrix = readCSVFile(path);

    SalesmanPopulation population(distance_matrix, 1000);
    population.setMutation(Population::mutations::rotation);
    
    SetUp setUp = {
        .generationsNum = 1000,
        .crossoverRate = 0.5,
        .mutationRate = 0.4
    };

    Solver solver(setUp);

    Individual solution = solver.solve(population);
}
```

### License
This project is licensed under the MIT License - see the [LICENCE](LICENCE) file for details.

### Authors
- Dmytro Shumskyi
- Marta Samoilenko
- Victor-Mykola Muryn
- Yurii Sahaidak
- Roman Milischuk (team mentor)
