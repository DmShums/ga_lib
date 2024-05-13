#include <iostream>
#include <sstream>
#include <fstream>

#include "timer.h"
#include "Solver.h"
#include "SalesmanPopulation.h"

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

int main(int argc, char* argv[]){
    //  ./structured_ga.exe generations testFucntion type

    SetUp setUp;
    auto path = argv[1];
    auto distance_matrix = readCSVFile(path);

    SalesmanPopulation population(distance_matrix, 1000);
    population.setCrossover(Population::crossovers::simple);
    population.setMutation(Population::mutations::rotation);
    population.setSelection(Population::selections::simple);

    setUp.generationsNum = std::stoi(argv[2]);
    setUp.crossoverRate = 0.5;
    setUp.mutationRate = 0.4;

    size_t threads = std::stoi(argv[3]);

    Solver solver(setUp);

    auto start_time = get_current_time_fenced();
    Individual solution = solver.solve(population, threads);
    auto finish_time = get_current_time_fenced();
    auto time = finish_time - start_time;

    std::cout << to_us(time) << std::endl;
    std::cout << solution.fitness << std::endl;
//            << "path: " << std::endl;

//    for (auto a : solution.solution) {
//        std::cout << a << " -> ";
//    }
//
//    std::cout << solution.solution.front() << std::endl;
}