from time import process_time
from func import *
import matplotlib.pyplot as plt
import pandas as pd

MAX_SIZE = 10
sizes = range(2, 11, 2)
TIMES = 1

MAX_SIZE = 10
sizes = range(200, 1001, 100)

flag_rewrite_img = 1
std = [[],[]]
win = [[],[]]
wino = [[],[]]

def img_routine(plot, name):
    plot.legend()
    plot.grid()
    plt.ylabel("Затраченное время (мс)")
    plt.xlabel("Линейный размер матриц (ед.)")

    if flag_rewrite_img:
        plt.savefig('../docs/img/' + name + '.png')

def csv_routine(name, ind):
    if flag_rewrite_img:
        buf = {
            'len':[i + ind for i in sizes],
            'std' :[round(i, 4) for i in std[ind]],
            'win':[round(i, 4) for i in win[ind]],
            'wino':[round(i, 4) for i in wino[ind]]
                }
        df = pd.DataFrame(buf)
        df.to_csv("../docs/csv/" + name +".csv")

def print_matrix(m1:[[int]]):
    for i in range(len(m1)):
        print(*m1[i])

def getProcessTime(func, m1, m2, times):
    resultTime = 0
    for i in range(times):

        timeStart = process_time()
        func(m1, m2)
        timeEnd = process_time()

        resultTime += (timeEnd - timeStart) * 1000
    return resultTime / times


def test():
    global sizes
    global TIMES
    global flag_rewrite_img
    global std
    global win
    global wino

    for i in range(2):
        for size in sizes:
            n = size + i
            m = size + i
            s = size + i

            m1 = [[randint(0, MAX_SIZE) for _ in range(m)] for o in range(n)]
            m2 = [[randint(0, MAX_SIZE) for _ in range(s)] for o in range(m)]


            print(n)
            std[i].append(getProcessTime(standart, m1, m2, TIMES))
            win[i].append(getProcessTime(win_alg,m1,m2, TIMES))
            wino[i].append(getProcessTime(win_alg_opt,m1,m2, TIMES))
    

    # a = input("Rewrite images and csv? (y/n): ")
    # if a != 'y':
    #     flag_rewrite_img = 0

    name = ["even", "odd"]
    for i in range(2):
        fig = plt.figure(figsize=(10, 7))

        plot = fig.add_subplot()
        plot.plot([j+i for j in sizes], std[i],  label = "Стандартный алгоритм", c = 'b')
        plot.plot([j+i for j in sizes], win[i], ":", label="Алгоритм Винограда", c = 'g')
        plot.plot([j+i for j in sizes], wino[i], "--", label="Оптимизированный алгоритм Винограда", c = 'r')

        img_routine(plot, name[i])
        csv_routine(name[i], i)


    
    


if __name__ == '__main__':
    print(list(sizes))
    test()

    # plt.show() 