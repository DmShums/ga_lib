""" generate random matrix """
import random

number = 50

def random_matrix(number):
    result = []

    for _ in range(number):
        result.append(["0"] * number)

    for i in range(number):
        for j in range(number):
            if i == j:
                continue

            result[i][j] = str(random.randint(1, number))
            result[j][i] = result[i][j]

    print('writing')
    with open('./data/graph.csv', 'w', encoding='utf-8') as file:
        file.write("\n".join(",".join(row) for row in result))


if __name__ == "__main__":
    random_matrix(number)
