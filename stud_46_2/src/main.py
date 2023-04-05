
from func import *
from random import randint

def print_matrix(m1):
    n = len(m1)
    m = len(m1[0])
    for i in range(n):
        print(*m1[i])

def main():
    n, m, s = list(map(int, input("Insert 3 matrix sizes: ").split()))


    c = input("Do you want to fill martix random? (y/n) \n ")
    if c == 'y':
        m1 = [[randint(1, 20) for j in range(m)] for i in range(n)]
        m2 = [[randint(1, 20) for j in range(s)] for i in range(m)]
    else:
        m1 = [[0] * m for i in range(n)]
        m2 = [[0] * s for i in range(m)]
        print("Input first matrix (%dx%d): "%(n, m))
        i = 0
        while i < n * m:
            sp = list(map(int, input().split()))
            for j in range(len(sp)):
                m1[i // m][i % m] = sp[j]
                i += 1

        print("Input second matrix (%dx%d): "%(m, s))
        i = 0
        while i < m * s:
            sp = list(map(int, input().split()))
            for j in range(len(sp)):
                m2[i // s][i % s] = sp[j]
                i += 1
    print("\nFirst matrix:")
    print_matrix(m1)
    print("\nSecond matrix:")
    print_matrix(m2)

    print("\nStandart algorithm:")
    print_matrix(standart(m1, m2))
    print("\nWinograd algorithm:")
    print_matrix(win_alg(m1, m2))
    print("\nOptimized Winograd algorithm:")
    print_matrix(win_alg_opt(m1, m2))



if __name__ == "__main__":
    main()
