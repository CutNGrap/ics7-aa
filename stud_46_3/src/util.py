from time import process_time
from func import bubble_sort, heap_sort, bucket_sort
from copy import deepcopy
from random import randint
import matplotlib.pyplot as plt
import pandas as pd

TIMES = 10
sizes = [10, 20, 30, 40, 50]

TIMES = 1000
sizes = [100, 200, 300, 400, 500, 600, 700, 800]

flag_rewrite_img = 1

def getProcessTime(func, arr):
    resultTime = 0
    for i in range(TIMES):
        tmpArray = deepcopy(arr)

        timeStart = process_time()
        func(tmpArray)
        timeEnd = process_time()

        resultTime += (timeEnd - timeStart) * 1000
    return resultTime / TIMES


def test():
    global sizes
    bubble = [[],[],[],[]]
    heap = [[],[],[],[]]
    bucket = [[],[],[],[]]
    modes = [lambda x, y:randint(0, 1000), lambda x, y:x, lambda x, y:y - x, lambda x, y:randint(0, 1) if x != y - 1 else 1000000]

    for m in range(len(modes)):
        for size in sizes:
            print(m, size)
            a = []
            for i in range(size):
                a.append(modes[m](i, size))
            
            bubble[m].append(getProcessTime(bubble_sort, a))
            heap[m].append(getProcessTime(heap_sort, a))
            bucket[m].append(getProcessTime(bucket_sort, a))
    
    
    titles = ["для случайных массивов", "для отсортированных массивов", "для отсортированных по убыванию массивов", "для массивов с неравномерным распределением данных"]
    files = ["random", "sorted", "dec", "bad"]
    for i in range(len(modes)):
        fig = plt.figure(figsize=(10, 7))

        plot = fig.add_subplot()
        plot.plot(sizes, bubble[i], label = "Сортировка пузырьком")
        plot.plot(sizes, heap[i], ":", label="Пирамидальная сортировка")
        plot.plot(sizes, bucket[i], "--", label="Блочная сортировка")

        plot.legend()
        plot.grid()
        plt.title("Временные характеристики алгоритмов сортировок\n" + titles[i])
        plt.ylabel("Затраченное время (мс)")
        plt.xlabel("Длина массива")

        if flag_rewrite_img:
            plt.savefig('../docs/img/' + files[i] + '.png')

            buf = {
                    'len':sizes,
                    'bubble' :[round(i, 4) for i in bubble[i]],
                    'heap':[round(i, 4) for i in heap[i]],
                    'bucket':[round(i, 4) for i in bucket[i]],
                        }
            df = pd.DataFrame(buf)
            df.to_csv("../docs/csv/" + files[i] + ".csv")

    for i in range(4):
        fig = plt.figure(figsize=(10, 7))

        plot = fig.add_subplot()
        plot.plot(sizes, heap[i], label="Пирамидальная сортировка")
        plot.plot(sizes, bucket[i], "--", label="Блочная сортировка")

        plot.legend()
        plot.grid()
        plt.title("Временные характеристики алгоритмов сортировок\n" + titles[i])
        plt.ylabel("Затраченное время (мс)")
        plt.xlabel("Длина массива")
    
        if flag_rewrite_img:
            plt.savefig('../docs/img/' + files[i] + '_two.png')
    


if __name__ == '__main__':
    test()

    # plt.show() 