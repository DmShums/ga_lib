#include <iostream>
#include <sstream>
#include <fstream>
#include <random> // For random number generation

#include "Solver.h"
#include "KnapsackPopulation.h"
#include "timer.h"

std::vector<std::vector<int>> readCSVFile(const std::string& filename) {
    std::vector<std::vector<int>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> row;
        std::string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

int main(int argc, char* argv[]) {
    size_t threads = std::stol(argv[1]);

    std::vector<int> weights = {10, 20, 30};
    std::vector<int> values = {60, 100, 120};
    int maxWeight = 50;
    size_t populationSize = 100;

    KnapsackPopulation knapsackPopulation(weights, values, maxWeight, populationSize);

    SetUp setUp {
        .generationsNum = 1000,
        .crossoverRate = 0.5,
        .mutationRate = 0.2
    };

    Solver solver(setUp);

    auto start_time = get_current_time_fenced();

    Individual solution = solver.solve(knapsackPopulation, threads);

    auto finish_time = get_current_time_fenced();
    auto time = finish_time - start_time;

    std::cout << to_us(time) << std::endl;

    std::cout
        << "Total value: " << solution.fitness << std::endl
        << "Items: " << std::endl;

    for (auto a : solution.solution) {
        std::cout << a << " ";
    }

    std::cout << std::endl;

    return 0;
}
