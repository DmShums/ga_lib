#include <utility>
#include <vector>
#include <iostream>

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <thread>

#include "oneapi/tbb/concurrent_unordered_map.h"

#include "TreadSafeQ.h"
#include "timer.h"


// function to hash a pair of 2 integers (this function is used in Boost)
struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

typedef oneapi::tbb::concurrent_unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash> distances_table_t;

typedef std::vector<std::vector<int>> CITIES_MAP;



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

void consumer(
        ThreadSafeQ<std::vector<bool>>& tsq,
        std::atomic<size_t>& ready,
        distances_table_t& distances,
        const CITIES_MAP& cities_map,
        const int combinations_per_thread
) {
    std::vector<int> combination;

    while (true) {
        auto v = tsq.pop();

//        poisson pill
        if (v.empty()) break;

        for (int k = 0; k < combinations_per_thread; ++k) {
            combination.clear();

            for (size_t i = 0; i < cities_map.size() - 1; ++i) {
                if (v[i]) {
                    combination.emplace_back(i + 1);
                }
            }

            auto bits = vertexes_to_bits(combination);

            for (auto vertex : combination) {
                std::pair<int, int> local_shortest{INT32_MAX, 0};

                for (auto i : combination) {
                    if (i == vertex) continue;

//                   get combination without vertex element
//                   we just set 0 in vertex-th place
                    auto prev = binary_without_vertex(vertex, bits);

                    auto distance_through_i = distances[{prev, i}].first;
                    distance_through_i += cities_map[i][vertex];

                    if (local_shortest.first > distance_through_i) {
                        local_shortest = {distance_through_i, i};
                    }
                }

                distances[{bits, vertex}] = local_shortest;
            }

            if (!std::prev_permutation(v.begin(), v.end())) break;
        }

        ++ready;
    }
}

std::pair<std::vector<int>, int> held_carp(
        const CITIES_MAP& cities_map,
        int threads_number = 1,
        int combinations_per_thread = 100
) {
    distances_table_t distances;

    auto cities_count = (int)cities_map.size();
    std::vector<int> vertexes_without_first(cities_count - 1);

    for (int i = 1; i < cities_count; ++i) {
        distances[{1 << i, i}] = {distance(0, i, cities_map), 0};
        vertexes_without_first[i - 1] = i;
    }

    ThreadSafeQ<std::vector<bool>> tsq;
    std::atomic<size_t> ready = 0;

    std::vector<std::thread> threads;
    threads.reserve(threads_number);

    for (int i = 0; i < threads_number; ++i) {
        threads.emplace_back(
            consumer,
            std::ref(tsq),
            std::ref(ready),
            std::ref(distances),
            std::ref(cities_map),
            combinations_per_thread
        );
    }

    for (int size = 2; size < cities_count; ++size) {
        std::vector<bool> v(cities_count - 1);
        std::fill(v.begin(), v.begin() + size, true);

        std::vector<int> combination;

        size_t count = 0;
        ready = 0;
        int combinations = 0;

        do {
            if (combinations == 0) {
                tsq.push(v);
                ++count;
            }

            ++combinations;

            if (combinations >= combinations_per_thread) {
                combinations = 0;
            }
        } while (std::prev_permutation(v.begin(), v.end()));

        while (ready != count) {}
    }

    for (int i = 0; i < threads_number; ++i) {
        tsq.push({});
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto full_cities_bits = vertexes_to_bits(vertexes_without_first);
    int shortest_distance = INT32_MAX, parent = 0;

    for (auto vertex : vertexes_without_first) {
        auto distance_through_vertex = distances[{full_cities_bits, vertex}].first;

        distance_through_vertex += distance(vertex, 0, cities_map);

        if (shortest_distance > distance_through_vertex) {
            shortest_distance = distance_through_vertex;
            parent = vertex;
        }
    }

    std::vector<int> path;
    path.reserve(cities_count + 1);

    for (size_t i = 0; i < vertexes_without_first.size(); ++i) {
        path.emplace_back(parent);

        auto binary_path_new = binary_without_vertex(parent, full_cities_bits);
        parent = distances[{full_cities_bits, parent}].second;

        full_cities_bits = binary_path_new;
    }

    path.emplace_back(0);
    std::reverse(path.begin(), path.end());
    path.emplace_back(0);

    return {path, shortest_distance};
}


int main() {
    auto filename = "../data/graph.csv";
    auto cities_map = readCSVFile(filename);

    int threads_number = 4;
    int combinations_per_thread = 300;

    auto start_time = get_current_time_fenced();

    auto [path, distance] = held_carp(cities_map, threads_number, combinations_per_thread);

    auto finish_time = get_current_time_fenced();
    auto time = finish_time - start_time;

    std::cout << to_us(time) << std::endl;
    std::cout << distance << std::endl;

    for (auto i : path) {
        std::cout << i << " ";
    }

    return 0;
}
