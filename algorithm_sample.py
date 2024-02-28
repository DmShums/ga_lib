from sys import maxsize 
from itertools import permutations
V = 4
 
def travellingSalesmanProblem(graph, s): 
 
    vertex = [] 
    for i in range(V): 
        if i != s: 
            vertex.append(i) 
 
    min_path = maxsize 
    next_permutation=permutations(vertex)
    for i in next_permutation:
 
        current_pathweight = 0
 
        k = s 
        for j in i: 
            current_pathweight += graph[k][j] 
            k = j 
        current_pathweight += graph[k][s] 
 
        min_path = min(min_path, current_pathweight) 
         
    return min_path 
 
 
if __name__ == "__main__": 
 
    # graph = [[0, 10, 15, 20], [10, 0, 35, 25], 
    #         [15, 35, 0, 30], [20, 25, 30, 0]] 
    
    with open('data/graph.csv', 'r', encoding="utf-8") as file:
        graph = [[int(j) for j in i.strip().split(',')] for i in file.readlines()]
    # print(graph)
    s = 0
    print(travellingSalesmanProblem(graph, s))