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
    auto filename = "../data/graph.csv";
    auto distance_matrix = readCSVFile(filename);

    SetUp setUp;
    setUp.generationsNum = std::stoi(argv[1]);
    SalesmanPopulation population(distance_matrix, 1000);

    if (argv[2] == "crossover"){
        setUp.crossoverRate = 0.5;
        setUp.mutationRate = 0.4;

        if (argv[3] == "uniform"){
            population.setCrossover(Population::crossovers::uniform);
        }
    } else if (argv[2] == "mutation"){
        setUp.crossoverRate = 0.2;
        setUp.mutationRate = 0.5;

        if (argv[3] == "inverse"){
            population.setMutation(Population::mutations::inverse);
        } else if (argv[3] == "rotation") {
            population.setMutation(Population::mutations::rotation);
        }
    } else {
        setUp.crossoverRate = 0.5;
        setUp.mutationRate = 0.4;

        if (argv[3] == "rank"){
            population.setSelection(Population::selections::rank);
        } else if (argv[3] == "boltzmann"){
            population.setSelection(Population::selections::boltzmann);
        } else if (argv[3] == "proportional"){
            population.setSelection(Population::selections::proportional);
        }
    }

    Solver solver(setUp);

    auto start_time = get_current_time_fenced();
    Individual solution = solver.solve(population);
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