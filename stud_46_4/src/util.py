from time import process_time
import matplotlib.pyplot as plt
import pandas as pd

flag_rewrite_img = 1

def img_routine(plot, name):
    plot.legend()
    plot.grid()
    plt.ylabel("Затраченное время (мс)")
    plt.xlabel("Количество документов (ед.)")

    if flag_rewrite_img:
        plt.savefig('../docs/img/' + name + '.png')




def test():
    

    no_m = pd.read_csv('../docs/csv/nomutex.csv')
    print(no_m)
    m = pd.read_csv('../docs/csv/mutex.csv')

    

    name = ["nomutex", "mutex"]
    df = [no_m, m]
    for i in (0, 1):
        fig = plt.figure(figsize=(10, 7))

        a = range(2048, 8192 + 1, 512)
        plot = fig.add_subplot()
        plot.plot(a, df[i]['0'],  label = "0 потоков", c = 'b')
        plot.plot(a, df[i]['1'], ":", label="1 поток", c = 'g')
        plot.plot(a, df[i]['2'], "--", label="2 потока", c = 'r')
        plot.plot(a, df[i]['4'], "go-", label="4 потока", c = 'magenta')
        plot.plot(a, df[i]['8'], "go:", label="8 потоков", c = 'black')
        plot.plot(a, df[i]['16'], "go--", label="16 потоков", c = 'cyan')

        img_routine(plot, name[i])
    


if __name__ == '__main__':
    test()