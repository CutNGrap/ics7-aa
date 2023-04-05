from random import randint
from func import bubble_sort, bucket_sort, heap_sort

def main():
    a1 = []
    a2 = []
    a3 = [101]
    for i in range(10):
        a1.append(randint(0, 100))
        a2.append(randint(0, 100))
        a3.append(randint(0, 100))
    print(a1)
    a1 = bubble_sort(a1)
    print(a1)
    print(a2)
    a2 = heap_sort(a2)
    print(a2)
    print(a3)
    b = bucket_sort(a3)
    print(a3, b)

if __name__ == "__main__":
    main()