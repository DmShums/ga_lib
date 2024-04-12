
There are plenty of NP-complete algorithms, which
require a lot of time to be solved. The genetic algorithms allow
us to reach great results with significantly lower complexity. The
project’s main topic is the development of different techniques
for genetic algorithms and comparing them with each other. 
The details are in a [paper](https://www.overleaf.com/project/65e82e6d956e437be5db1335).

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

