#include <iostream>
#include <sstream>
#include <fstream>

#include "Solver.h"
#include "SalesmanPopulation.h"
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
    auto threads = atoi(argv[1]);
    auto filename = "./data/graph.csv";
    auto distance_matrix = readCSVFile(filename);

    SetUp setUp {
        .generationsNum = 1000,
        .crossoverRate = 0.5,
        .mutationRate = 0.2
    };

    Solver solver(setUp);

    SalesmanPopulation population(distance_matrix, 1000);

    //  if you want to change in-build type, use this constructions
//    population.setSelection(Population::selections::simple);
//    population.setCrossover(Population::crossovers::uniform);

    auto start_time = get_current_time_fenced();

    Individual solution = solver.solve(population, threads);

    auto finish_time = get_current_time_fenced();
    auto time = finish_time - start_time;

    std::cout << to_us(time) << std::endl;

    std::cout
        << "distance: " << solution.fitness << std::endl
        << "path: " << std::endl;

    for (auto a : solution.solution) {
        std::cout << a << " -> ";
    }

    std::cout << solution.solution.front() << std::endl;
}