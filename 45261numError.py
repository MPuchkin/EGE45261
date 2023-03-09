from time import *
import numpy as np

f = open('107_27_B.txt')
N = int(f.readline())
A = [int(f.readline()) for _ in range(N)]

bins = np.array(A + A)
print('Длина списка мусорных баков :', len(bins))

center = (N - 1) // 2
distances = np.array([min(abs(center - i), N - abs(center - i)) for i in range(N)])
print(distances)

print('Размер входа :', N)
print('Прогноз по количеству операций : ~', N**2)

minCost = np.inner(bins[:N], distances)
count = 0
start = time()
print('Starting...')
for i in range(1, N):
    mn = np.inner(bins[i:i+N], distances)
    # mn = bins[i:i + N] @ distances
    if minCost > mn:
        minCost = mn

    if i % 1000 == 10:
        tm = N * (time() - start) / i / 60
        print(str(round(i / N * 100, 2)) + '% ; Прогноз : ' + str(round(tm, 2)) + ' минут')

print('Ответ :', 3*minCost)
print('Время :', round((time() - start)/60,2), 'минут')

'''  471228, 49113954961677.
11 минут, переполнение
'''