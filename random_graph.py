""" generate random matrix """
import random

result = []
number = 21

for _ in range(number):
    result.append(["0"] * number)

for i in range(number):
    for j in range(number):
        if i == j:
            continue

        result[i][j] = str(random.randint(1, number))
        result[j][i] = result[i][j]

print('writing')
with open('graph.csv', 'w', encoding='utf-8') as file:
    file.write("\n".join(",".join(row) for row in result))
