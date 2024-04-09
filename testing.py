import random_graph
import subprocess
import statistics
import csv
import pandas as pd

best_permutation_result = []
total_distance_gen_result = []

total_distance_classic_result = []

for i in range(5, 26, 5):

    graph = random_graph.generate(i)
    # print(len(graph))
    random_graph.save_graph(graph)

    total_distance = 0
    
    for _ in range(10):
        result = subprocess.run(["./Release/TSP_GA"], capture_output=True, text=True)
        output = result.stdout.split("\n")
        # print(result)

        best_permutation = [int(x) for x in output[1].split()]
        total_distance += int(output[3])

    total_distance /= 10
    total_distance_gen_result.append(total_distance)
    

    result_classic = subprocess.run(["./Release/ga_lib"], capture_output=True, text=True)
    total_distance_classic_result.append(float(result_classic.stdout.split("\n")[1]))

    print(total_distance_gen_result)
    print(total_distance_classic_result)

print(total_distance_gen_result)
df = pd.DataFrame(total_distance_gen_result)
print(df)

df.to_csv('gen_result.csv', index=False)

df = pd.DataFrame(total_distance_classic_result)
print(df)
df.to_csv('classic_result.csv', index=False)

