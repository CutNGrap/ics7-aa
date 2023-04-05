from time import process_time
import matplotlib.pyplot as plt
import pandas as pd

flag_rewrite_img = 1

def img_routine(plot, name):
    plot.legend()
    plot.grid()
    plt.ylabel("Затраченное время, мс")
    plt.xlabel("Линейный размер матрицы, ед")

    plt.show()




def test():
    

    data = pd.read_csv('time_output1.csv')
    print(data)
    fig = plt.figure(figsize=(10, 7))

    plot = fig.add_subplot()
    plot.plot(data['size'][:-1], [i/1000 for i in data['full'][:-1]],  label = "Полный перебор", c = 'b')
    plot.plot(data['size'][:-1], [i/1000 for i in data['ant'][:-1]], ":", label="Муравьиный метод", c = 'g')

    img_routine(plot, "sus")
    


if __name__ == '__main__':
    test()