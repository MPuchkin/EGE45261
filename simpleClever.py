from time import *


def cost_point(a, dists, ind):
    result = 0
    for i in range(len(dists)):
        result += a[i+ind] * dists[i]
    return result


f = open('107_27_B.txt')
N = int(f.readline())
A = [int(f.readline()) for _ in range(N)]
A = A + A

center = (N - 1) // 2
distances = [min(abs(center - i), N - abs(center - i)) for i in range(N)]

minCost = cost_point(A, distances, 0)
count = 0
start = time()
print('Starting...')

for i in range(1, N):
    mn = cost_point(A, distances, i)
    if minCost > mn:
        minCost = mn

    if i % 100 == 10:
        tm = N * (time() - start) / i / 60
        print(str(round(i / N * 100, 2)) + '% ; Прогноз : ' + str(round(tm, 2)) + ' минут')

print('Ответ :', 3*minCost)
print('Время :', round((time() - start)/60, 2), 'минут')

''' 471228, 49113954961677. ~1.5 суток для файла 107_27_B.txt '''
