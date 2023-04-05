from random import randint

def standart(m1: [[int]], m2:[[int]]) ->[[int]] :
    if  len(m1[0]) != len(m2):
        return [[]]
    n = len(m1)
    m = len(m1[0])
    s = len(m2[0])
    res = [[0] * s for i in range(n)]
    for i in range(n):
        for k in range(s):
            for j in range(m):
                res[i][k] = res[i][k] + m1[i][j] * m2[j][k]
    return res

def win_alg(m1: [[int]], m2:[[int]]) ->[[int]] :
    if  len(m1[0]) != len(m2):
        print("Error: sizes doesn't match - multiplication impossible")
        return [[]]
    
    n = len(m1)
    m = len(m1[0])
    s = len(m2[0])

    res = [[0] * s for i in range(n)]
    row_sum = [0] * n
    col_sum = [0] * s

    for i in range(n):
        for j in range(m//2):
            row_sum[i] = row_sum[i] + m1[i][2 * j] * m1[i][2 * j + 1]

    for i in range(s):
        for j in range(m//2):
            col_sum[i] = col_sum[i] + m2[2 * j][i] * m2[2 * j + 1][i]

    for i in range(n):
        for j in range(s):
            res[i][j] = -row_sum[i] - col_sum[j]
            for k in range(0, m // 2):
                res[i][j] = res[i][j] + (m1[i][2 * k + 1] + m2[2 * k][j]) * (m1[i][2 * k] + m2[2 * k + 1][j])

    if (m % 2 == 1):
        for i in range(n):
            for j in range(s):
                res[i][j] = res[i][j] + m1[i][m - 1] * m2[m - 1][j]

    return res

def win_alg_opt(m1: [[int]], m2:[[int]]) ->[[int]] :
    if  len(m1[0]) != len(m2):
        print("Error: sizes doesn't match - multiplication impossible")
        return [[]]
    
    n = len(m1)
    m = len(m1[0])
    s = len(m2[0])

    res = [[0] * s for i in range(n)]
    row_sum = [0] * n
    col_sum = [0] * s

    m_05 = m >> 1
    for i in range(n):
        for j in range(m_05):
            t = j << 1
            row_sum[i] += m1[i][t] * m1[i][t + 1]

    for i in range(s):
        for j in range(m_05):
            t = j << 1
            col_sum[i] += m2[t][i] * m2[t + 1][i]

    for i in range(n):
        for j in range(s):
            res[i][j] = -row_sum[i] - col_sum[j]
            for k in range(0, m_05):
                t = k << 1
                t1 = t + 1
                res[i][j] += (m1[i][t1] + m2[t][j]) * (m1[i][t] + m2[t1][j])
    if (m % 2 == 1):
        for i in range(n):
            for j in range(s):
                t = m - 1
                res[i][j] += m1[i][t] * m2[t][j]

    return res


if __name__ == '__main__':
    n = randint(2, 5)
    m = randint(2, 5)
    s = randint(2, 5)

    m2 = [[randint(0, 10) for _ in range(s)] for o in range(m)]
    m1 = [[randint(0, 10) for _ in range(m)] for o in range(n)]


    print(m1)
    print(m2)

    print(standart(m1, m2))
    print(win_alg(m1, m2))
    print(win_alg_opt(m1, m2))