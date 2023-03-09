from multiprocessing.pool import ThreadPool
from time import *
import numpy as np


def solve_one_thread(containers, dists, start_index, step):
    n = len(dists)
    start_time = time()
    min_cost = np.inner(containers[:N], dists)
    for i in range(start_index, n, step):
        mn = np.inner(containers[i:i + n], dists)
        min_cost = min(min_cost, mn)

        if i % 10000 == 10:
            tm = n * (time() - start_time) / i / 60
            print(str(round(i / n * 100, 2)) + '% ; Прогноз : ' + str(round(tm, 2)) + ' минут')

    return min_cost


f = open('107_27_B.txt')
N = int(f.readline())
A = [int(f.readline()) for _ in range(N)]

bins = np.array(A + A, dtype=np.int64)
print('Длина списка мусорных баков :', len(bins))

center = (N - 1) // 2
distances = np.array([min(abs(center - i), N - abs(center - i)) for i in range(N)], dtype=np.int64)


print('Размер входа :', N)
print('Прогноз по количеству операций : ~', N**2)
start = time()

threads = 12
args = [(bins, distances, i, threads) for i in range(threads)]

pool = ThreadPool(processes=threads)

results = pool.starmap_async(solve_one_thread, args)
pool.close()

print('Ответ :', 3*min(results.get()))
print('Время :', round((time() - start)/60, 2), 'минут')

'''  471228, 49113954961677.
5.91 минут
'''