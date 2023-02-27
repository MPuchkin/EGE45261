from time import *


def cost_point(a, ind):
    result = 0
    for i in range(len(a)):
        result += 3 * a[i] * min(abs(ind - i), len(a) - abs(ind - i))
    return result


f = open('107_27_B.txt')
N = int(f.readline())
A = [int(f.readline()) for _ in range(N)]

start = time()
print('Starting...')

minCost = cost_point(A, 0)
for i in range(1, len(A)):
    mn = cost_point(A, i)
    if minCost > mn:
        minCost = mn

    if i % 100 == 10:
        tm = len(A) * (time() - start) / i / 60
        print(str(round(i / len(A) * 100, 2)) + '% ; Прогноз : ' + str(round(tm, 2)) + ' минут');

print('Ответ :', minCost)
print('Время :', round((time() - start)/60,2), 'минут')


'''
471228, 49113954961677.
7000 минут
'''