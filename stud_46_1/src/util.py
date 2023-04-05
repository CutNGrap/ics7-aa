from time import process_time
from string import ascii_letters
from func import *
from random import choice
import matplotlib.pyplot as plt
import pandas as pd

MAX_SIZE = 10


TIMES_UNREC = 100000
TIMES_REC = 100

# TIMES_UNREC = 100
# TIMES_REC = 1

flag_rewrite_img = 1

sizes = range(MAX_SIZE)

def getProcessTime(func, s1, s2, n, times):
    resultTime = 0
    for i in range(times):

        timeStart = process_time()
        func(s1, s2, n, n)
        timeEnd = process_time()

        resultTime += (timeEnd - timeStart) * 1000
    return resultTime / times


def test():
    global sizes
    global TIMES_REC
    global TIMES_UNREC
    global flag_rewrite_img
    global flag_rewrite_csv
    leven = []
    damer = []
    damer_rec = []
    damer_cache = []

    for size in sizes:
        a = ""
        b = ""

        for i in range(size):
            a += choice(ascii_letters)
            b += choice(ascii_letters)
        print(size, a, b)

        if size > 3:
            TIMES_UNREC = 10000
        if size > 7:
            TIMES_REC = 50
        if size > 8:
            TIMES_REC = 10
        
        leven.append(getProcessTime(levenshtein, a, b, size, TIMES_UNREC))
        damer.append(getProcessTime(damerau, a, b, size, TIMES_UNREC))
        damer_rec.append(getProcessTime(damerau_recursive, a, b, size, TIMES_REC))
        damer_cache.append(getProcessTime(damerau_recursive_cache, a, b, size, TIMES_UNREC))
    

    a = input("Rewrite images and csv? (y/n): ")
    if a != 'y':
        flag_rewrite_img = 0

    for i in range(1):
        fig = plt.figure(figsize=(10, 7))

        plot = fig.add_subplot()
        plot.plot(sizes, leven, label = "Левенштейн")

        plot.legend()
        plot.grid()
        plt.ylabel("Затраченное время (мс)")
        plt.xlabel("Длина строк")

        if flag_rewrite_img:
            plt.savefig('../docs/img/graph_levenshtein.png')


    for i in range(1):
        fig = plt.figure(figsize=(10, 7))

        plot = fig.add_subplot()
        plot.plot(sizes, damer, ":", label = "Дамерау-Левенштейн (матрично)", c = 'b')
        plot.plot(sizes, damer_rec, label="Дамерау-Левенштейн (рекурсивно)", c = 'g')
        plot.plot(sizes, damer_cache, "--", label="Дамерау-Левенштейн (рекурсивно с кэшем)", c = 'r')

        plot.legend()
        plot.grid()
        plt.ylabel("Затраченное время (мс)")
        plt.xlabel("Длина строк")

        if flag_rewrite_img:
            plt.savefig('../docs/img/graph_damer.png')

        if flag_rewrite_img:
            buf = {
                'len':sizes,
                'lev' :[round(i, 4) for i in leven],
                'dam':[round(i, 4) for i in damer],
                'damr':[round(i, 4) for i in damer_rec],
                'damc':[round(i, 4) for i in damer_cache]
                    }
            df = pd.DataFrame(buf)
            df.to_csv("../docs/csv/res.csv")
    
    for i in range(1):
        fig = plt.figure(figsize=(10, 7))

        plot = fig.add_subplot()
        plot.plot(sizes, damer, ":", label="Дамерау-Левенштейн (матрично)", c = 'b')
        plot.plot(sizes, damer_cache, "--", label="Дамерау-Левенштейн (рекурсивно с кэшем)", c = 'r')

        plot.legend()
        plot.grid()
        plt.ylabel("Затраченное время (мс)")
        plt.xlabel("Длина строк")

        if flag_rewrite_img:
            plt.savefig('../docs/img/graph_damer2.png')

    
    


if __name__ == '__main__':
    test()

    # plt.show() 