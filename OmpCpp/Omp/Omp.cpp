#include <iostream>
#include <iterator>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>
#include <limits>
#include <ctime>
#include <omp.h>
#include <execution>

using namespace std;

using valuetype = long long;

valuetype CostInPoint(const vector<short> &A, int ind) {
    valuetype Result = 0;
    for (size_t i = 0; i < A.size(); i++)
        Result += valuetype(A[i]) * min<valuetype>(labs(ind - i), A.size() - labs(ind - i));
    return Result;
}

valuetype DotProduct(const vector<short>& A, const vector<valuetype> & Dists, int ind) {
    valuetype Result = 0;
    for (size_t i = 0; i < Dists.size(); i++)
        Result += valuetype(A[i + ind]) * Dists[i];
    return Result;
}

valuetype Solve1(const vector<short> &A) {
    size_t N = A.size();

    size_t count = 0;
    valuetype MinCost = numeric_limits<valuetype>::max();
    auto start = clock();

    //#pragma omp parallel for
    for (int i = 0; i < A.size(); i++) {
        valuetype mn = CostInPoint(A, i);

        count++;
        //#pragma omp critical
        {
            if (MinCost > mn)
                MinCost = mn;
        }

        if (count % 2500 == 99) {
            std::cout << ((double(clock() - start) / CLOCKS_PER_SEC) * N) / (count * 60) << " minutes" << std::endl;
            std::cout << (double(count) / A.size()) * 100 << " %" << endl;
        }
    }
    return 3 * MinCost;
}

valuetype Solve2(vector<short> A) {
    size_t N = A.size();
    
    //  Удваиваем вектор мусорников
    A.reserve(2 * N);
    for (size_t i = 0; i < N; i++)
        A.push_back(A[i]);

    //  Строим вектор расстояний
    vector<valuetype> dist;
    dist.reserve(N);
    size_t center = (N - 1) / 2; //  10 -> 4,  9 -> 4
    for (size_t i = 0; i < N; i++) {
        int forw = static_cast<int>(labs(center - i));
        int back = static_cast<int>(N - forw);
        dist.push_back(min(forw, back));
    }

    //  Засекаем время
    auto start = clock();

    //  Счётчик обработанных элементов - при распараллеливании на счётчик цикла полагаться нельзя
    size_t count = 0;

    //  Явно указываем количество потоков по числу логических ядер процессора
    const int threadNum = 12;
    omp_set_num_threads(threadNum);
    //  Вектор минимумов, чтобы каждый поток обращался к своему минимуму
    vector<int64_t> mins(threadNum, numeric_limits<int64_t>::max());
    //valuetype minCost = numeric_limits<int64_t>::max();
    //  Параллельный цикл
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int64_t mn = DotProduct(A, dist, i);

        //  Результат вычислений записываем в соответствующий минимум массива
        const int tnum = omp_get_thread_num();
        if (mn < mins[tnum])
            mins[tnum] = mn;
        //minCost = min(minCost, mn);
        count++;

        if (count % 10000 == 99) {
            //  Вывод информации один раз на каждые 10000 перемножений векторов
            std::cout << ((double(clock() - start) / CLOCKS_PER_SEC) * N) / (count * 60) << " minutes" << std::endl;
            std::cout << (double(count) / N) * 100 << " %" << endl;
        }
    }
    // return 3 * minCost;
    return 3 * *min_element(mins.cbegin(), mins.cend());
}

valuetype Solve3(vector<short> A) {
    //  Удваиваем вектор
    size_t N = A.size();
    A.reserve(2 * N);
    for (size_t i = 0; i < N; i++)
        A.push_back(A[i]);

    //  Строим вектор расстояний
    vector<int> dist;
    dist.reserve(N);
    size_t center = (N - 1) / 2; //  10 -> 4,  9 -> 4
    for (size_t i = 0; i < N; i++) {
        int forw = static_cast<int>(labs(center - i));
        int back = static_cast<int>(N - forw);
        dist.push_back(min(forw, back));
    }

    //  Засекаем время
    auto start = clock();

    //  Счётчик обработанных элементов - при распараллеливании на счётчик цикла полагаться нельзя
    size_t count = 0;

    //  Явно указываем количество потоков по числу логических ядер процессора
    const int threadNum = 12;
    omp_set_num_threads(threadNum);
    //  Вектор минимумов, чтобы каждый поток обращался к своему минимуму
    vector<int64_t> mins(threadNum, numeric_limits<int64_t>::max());

    //  Параллельный цикл
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        /*int64_t mn = dot_product(bins, dist, i);
        if (mn < minCost)
            minCost = mn;*/
            //  Параллельное скалярное произведение
        int64_t mn = transform_reduce(std::execution::par_unseq, dist.cbegin(), dist.cend(), A.cbegin() + i, 0LL);
        //  Результат вычислений записываем в соответствующий минимум массива
        const int tnum = omp_get_thread_num();
        if (mn < mins[tnum])
            mins[tnum] = mn;
        count++;

        if (count % 10000 == 99) {
            //  Вывод информации один раз на каждые 10000 перемножений векторов
            std::cout << ((double(clock() - start) / CLOCKS_PER_SEC) * N) / (count * 60) << " minutes" << std::endl;
            std::cout << (double(count) / N) * 100 << " %" << endl;
        }
    }

    return 3 * *min_element(mins.cbegin(), mins.cend());
}

int main()
{
    ifstream in("107_27_B.txt");
    size_t N;
    in >> N;
    std::cout << "Elems count : " << N << endl;
    
    vector<short> A;
    copy(istream_iterator<short>(in), istream_iterator<short>(), back_inserter(A));
    
    auto start = clock();

    valuetype result = Solve3(A);
    
    std::cout << "Total time : " << (double(clock() - start) / CLOCKS_PER_SEC) / 60 << " minutes\n";
    std::cout << "Result : " << result << endl;
    int x;
    std::cin >> x;
    return 0;
}
/* Solve1 - 60/25 min
*  Solve2 (single) - 20 min
*  Solve2 - 8.2/15 min
*  Solve2(P) - 3.5 min
*  Solve3 - 2.5 min
*/
