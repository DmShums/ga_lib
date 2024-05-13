import subprocess
from random_graph import random_matrix

def run_command(command):
    output = subprocess.run(command.split(" "), stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True) 
    return output.stdout.decode('utf-8')

if __name__ == '__main__':
    results = {}
    for number in range(5, 26, 5):
        results[number] = {
            "ga": {"time": [], "results": []},
            "exact": {"time": [], "results": []},
        }

        for _ in range(10):
            random_matrix(number)
            ga = run_command('./cmake-build-release/ga_lib')
            exact = run_command('./cmake-build-release/TSP_GA')

            ga_time, ga_result = ga.split("\n")
            exact_time, exact_result = exact.split("\n")

            results[number]["ga"]["time"].append(float(ga_time))
            results[number]["ga"]["results"].append(float(ga_result))

            results[number]["exact"]["time"].append(float(exact_time))
            results[number]["exact"]["results"].append(float(exact_result))


        # random_matrix(number)
        # a = run_command('./cmake-build-release/ga_lib')
        # print(a)