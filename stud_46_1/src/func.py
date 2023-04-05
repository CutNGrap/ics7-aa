def levenshtein(s1:str, s2:str, n:int, m:int) -> int:
    if (n == 0):
        return m
    if (m == 0):
        return n 

    matrix = [[0 for j in range(m + 1)] for i in range (n + 1)]
    for i in range(n + 1):
        matrix[i][0] = i
    for j in range(m + 1):
        matrix[0][j] = j

    cost = 0
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            cost = (s1[i - 1] != s2[j - 1])
            matrix[i][j] = min(matrix[i-1][j] + 1, matrix[i][j-1] + 1, matrix[i-1][j-1] + cost)
    return matrix[n][m]

def damerau(s1:str, s2:str, n:int, m:int) -> int:
    if (n == 0):
        return m
    if (m == 0):
        return n 

    matrix = [[0 for j in range(m + 1)] for i in range (n + 1)]
    for i in range(n + 1):
        matrix[i][0] = i
    for j in range(m + 1):
        matrix[0][j] = j

    cost = 0
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            cost = (s1[i - 1] != s2[j - 1])
            matrix[i][j] = min(matrix[i-1][j] + 1, matrix[i][j-1] + 1, matrix[i-1][j-1] + cost)
            
            if (i > 1 and j > 1 and s1[i-1] == s2[j-2] and s1[i-2] == s2[j-1]):
                matrix[i][j] = min(matrix[i][j], matrix[i-2][j-2] + 1)
                
    return matrix[n][m]

def damerau_recursive(s1:str, s2:str, n:int, m:int) -> int:
    if (n * m == 0):
        return abs(n - m)
    
    res = min(damerau_recursive(s1, s2, n - 1, m), damerau_recursive(s1, s2, n, m-1)) + 1
    res = min(res, damerau_recursive(s1, s2, n - 1, m - 1) + (s1[n - 1] != s2[m - 1]))

    if (n > 1 and m > 1 and s1[n-1] == s2[m-2] and s1[n-2] == s2[m-1]):
        res = min(res, damerau_recursive(s1, s2, n - 2, m - 2) + 1)
    
    return res


def damer_rec_cache(s1:str, s2:str, n:int, m:int, cache:[]) -> int:
    if (n * m == 0):
        return abs(n - m)
    
    if (cache[n][m] > 0):
        return cache[n][m]

    res = min(damer_rec_cache(s1, s2, n - 1, m, cache), damer_rec_cache(s1, s2, n, m-1, cache)) + 1
    res = min(res, damer_rec_cache(s1, s2, n - 1, m - 1, cache) + (s1[n - 1] != s2[m - 1]))

    if (n > 1 and m > 1 and s1[n-1] == s2[m-2] and s1[n-2] == s2[m-1]):
        res = min(res, damer_rec_cache(s1, s2, n - 2, m - 2, cache) + 1)
    cache[n][m] = res
    return res

def damerau_recursive_cache(s1:str, s2:str, n:int, m:int) -> int:
    cache = [[-1 for j in range(m + 1)] for i in range (n + 1)]
    return damer_rec_cache(s1, s2, n, m, cache)

if __name__ == '__main__':
    damerau_recursive_cache("abc", 'bca', 3, 3)