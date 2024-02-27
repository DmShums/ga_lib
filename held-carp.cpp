#include <vector>
#include <unordered_map>
#include <iostream>

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "timer.h"


// function to hash pair of 2 integers (this function is used in boost)
struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

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

typedef std::vector<std::vector<int>> CITIES_MAP;

int distance(int x_city, int y_city, const CITIES_MAP& cities_map) {
    return cities_map[y_city][x_city];
}

int vertexes_to_bits(const std::vector<int>& combination) {
    int bits = 0;

    for (auto i : combination) {
        bits |= 1 << i;
    }

    return bits;
}

int binary_without_vertex(int vertex, int binary) {
    return binary & ~(1 << vertex);
}

std::vector<int> held_carp(const CITIES_MAP& cities_map) {
    auto cities_count = (int)cities_map.size();

    std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash> distances;
    std::vector<int> vertexes_without_first(cities_count - 1);

    for (int i = 1; i < cities_count; ++i) {
        distances[{1 << i, i}] = {distance(0, i, cities_map), 0};
        vertexes_without_first[i - 1] = i;
    }

    for (int size = 2; size < cities_count; ++size) {
        std::vector<bool> v(cities_count - 1);
        std::fill(v.begin(), v.begin() + size, true);

        std::vector<int> combination;

        do {
            combination.clear();

            for (int i = 0; i < cities_count - 1; ++i) {
                if (v[i]) {
                    combination.emplace_back(i + 1);
                }
            }

            auto bits = vertexes_to_bits(combination);

            for (auto vertex : combination) {
                std::vector<std::pair<int, int>> local_shortest;

                for (auto i : combination) {
                    if (i == vertex) continue;

//                    get combination without vertex element
//                    we just set 0 in vertex-th place
                    auto prev = binary_without_vertex(vertex, bits);

                    auto distance_through_i = distances[{prev, i}].first;
                    distance_through_i += distance(i, vertex, cities_map);

                    local_shortest.emplace_back(distance_through_i, i);
                }

                distances[{bits, vertex}] = *std::min_element(
                    local_shortest.begin(),
                    local_shortest.end(),
                    [](std::pair<int, int> a, std::pair<int, int> b) {
                        return a.first < b.first;
                    }
                );
            }
        } while (std::prev_permutation(v.begin(), v.end()));
    }

    std::vector<std::pair<int, int>> local_shortest;
    auto full_cities_bits = vertexes_to_bits(vertexes_without_first);

    for (auto vertex : vertexes_without_first) {
        auto distance_through_vertex = distances[{full_cities_bits, vertex}].first;
        distance_through_vertex += distance(vertex, 0, cities_map);

        local_shortest.emplace_back(distance_through_vertex, vertex);
    }

//    reconstruct the shortest way
    auto shortest = *std::min_element(
        local_shortest.begin(),
        local_shortest.end(),
        [](std::pair<int, int> a, std::pair<int, int> b) {
            return a.first < b.first;
        }
    );

    auto shortest_distance = shortest.first;
    auto parent = shortest.second;

    std::vector<int> path;
    path.reserve(cities_count + 1);
    path.emplace_back(0);

    for (auto _ : vertexes_without_first) {
        path.emplace_back(parent);

        auto binary_path_new = binary_without_vertex(parent, full_cities_bits);
        parent = distances[{full_cities_bits, parent}].second;
        full_cities_bits = binary_path_new;
    }

    path.emplace_back(0);
    std::reverse(path.begin(), path.end());
    return path;
}


int main() {
    auto filename = "../data/graph.csv";
    auto cities_map = readCSVFile(filename);

    auto start_time = get_current_time_fenced();

    auto result = held_carp(cities_map);

    auto finish_time = get_current_time_fenced();
    auto time = finish_time - start_time;

    std::cout << to_us(time) << std::endl;

    for (auto i : result) {
        std::cout << i << " ";
    }

    return 0;
}
