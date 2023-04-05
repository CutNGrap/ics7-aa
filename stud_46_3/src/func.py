from random import randint

def bubble_sort(array):
    n = len(array)
    for i in range(n): 
        for j in range(1, n - i):  
            if array[j - 1] > array[j]:
                t = array[j-1]
                array[j-1] = array[j]
                array[j] = t
    return array

def bucket_sort(array):
    max_value = max(array)
    min_value = min(array)
    n = len(array)
    if n == 0:
        return array
    size = (max_value - min_value)/n
    if size == 0:
        return array

    buckets_list= []
    for i in range(n): 
        buckets_list.append([])

    for i in range(n):
        j = int ((array[i] - min_value)/ size)
        if j != n:
            buckets_list[j].append(array[i])
        else:
            buckets_list[-1].append(array[i])

    for z in range(n):
        insertion_sort(buckets_list[z])
            
    final_output = []
    for x in range(n):
        final_output = final_output + buckets_list[x]
    return final_output

def insertion_sort(array):
    for i in range(1,len(array)):   
        key = array[i]
        j = i-1
        while array[j] > key and j >= 0:
            array[j+1] = array[j]
            j -= 1
        array[j+1] = key
    return array



def heapify(array, n, i):
    largest = i
    l = 2 * i + 1
    r = 2 * i + 2
    if l < n and array[i] < array[l]:
        largest = l
    if r < n and array[largest] < array[r]:
        largest = r
    if largest != i:
        t = array[i]
        array[i] = array[largest]
        array[largest] = t
        heapify(array, n, largest)
        
def heap_sort(array):
    n = len(array)
    for i in range(n//2, -1, -1):   
        heapify(array, n, i)
    for i in range(n-1, 0, -1): 
        t = array[i]
        array[i] = array[0]
        array[0] = t
        heapify(array, i, 0)
    return array

if __name__ == "__main__":
    a1 = []
    for i in range(10):
        a1.append(randint(-10, 10))
    print(a1)
    b = bucket_sort(a1)
    print(b)