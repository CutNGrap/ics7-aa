import numpy as np
from random import *

def update_file():
	try:
		filename = input("\nВведите имя файла: ")
		size = int(input("\nВведите размер матрицы: "))
		rand_start = int(input("\nВведите начальное число рандома: "))
		rand_end = int(input("\nВведите конечное число рандома: "))
		print(generate_matrix_file(filename, size, rand_start, rand_end))
		print("\n\nУспешно обновлен список файлов\n")
	except:
		print("\nНеверный ввод")


def generate_matrix_file(filename, size, rand_start, rand_end):
    matrix = generate_matrix(size, rand_start, rand_end)
    file = open(filename, "w")

    file.write(str(size) + '\n')
    for i in range(size):
        string = ""
        for j in range(size):
            string += str(matrix[i][j])
            string += " "
        string += "\n"

        file.write(string)

    file.close()

    return f'File {filename} generated\n'

def generate_matrix(size, rand_start, rand_end):
    matrix = np.zeros((size, size), dtype = int)

    for i in range(size):
        for j in range(size):
            if (i == j):
                num = 0
            else:
                matrix[i][j] = randint(rand_start, rand_end)
                matrix[j][i] = randint(rand_start, rand_end)

    return matrix


update_file()