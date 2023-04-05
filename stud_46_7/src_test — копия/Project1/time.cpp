#include <stdio.h>
#include <thread>
#include "func.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include "func.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <utility>
#include <limits>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <time.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <format>

using namespace std;


#define MIN 2
#define MAX 10
#define TIMES 10


void my_time()
{
    string names[] = { "s2.csv","s3.csv" ,"s4.csv" ,"s5.csv" ,"s6.csv" ,"s7.csv" ,
    "s8.csv" ,"s9.csv", "s10.csv"};
    vector<int> time_full;
    vector<int> time_ant;
    for (int size = MIN; size <= MAX; ++size)
    {
        cout << size << endl;

        vector<vector<double>> matrix;
        vector<int> places(size);
        iota(begin(places), end(places), 0);

        ifstream file(names[size - MIN]);
        int count;
        file >> count;
        double t;
        for (int i = 0; i < count; i++)
        {
            vector<double> v;
            for (int j = 0; j < count; j++)
            {
                file >> t;
                v.push_back(t);
            }
            matrix.push_back(v);
        }
        file.close();

        auto start = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            ant_alg(matrix, 0.5, 0.5, 0.5, 200);
        }
        auto end = chrono::high_resolution_clock::now();
        time_ant.push_back((chrono::duration_cast<std::chrono::microseconds>(end - start)).count() / TIMES);

        auto start1 = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            full_combinations(matrix);
        }
        auto end1 = chrono::high_resolution_clock::now();
        time_full.push_back((chrono::duration_cast<std::chrono::microseconds>(end1 - start1)).count() / TIMES);
        
    }

    ofstream myfile("time_output.csv");
    for (int i = MIN; i <= MAX; ++i)
         myfile << i << " & "<<time_full[i - MIN]<<" & "<< time_ant[i - MIN]<<endl;
    myfile.close();
}

void my_time(int a)
{
    string names[] = { "s2.csv","s3.csv" ,"s4.csv" ,"s5.csv" ,"s6.csv" ,"s7.csv" ,
    "s8.csv" ,"s9.csv", "s10.csv" };
    vector<int> time_full;
    vector<int> time_ant;
    for (int size = MIN; size <= MAX; ++size)
    {
        cout << size << endl;

        vector<vector<double>> matrix;
        vector<int> places(size);
        iota(begin(places), end(places), 0);

        ifstream file(names[size - MIN]);
        int count;
        file >> count;
        double t;
        for (int i = 0; i < count; i++)
        {
            vector<double> v;
            for (int j = 0; j < count; j++)
            {
                file >> t;
                v.push_back(t);
            }
            matrix.push_back(v);
        }
        file.close();

        auto start = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            ant_alg(matrix, 0.5, 0.5, 0.5, 200);
        }
        auto end = chrono::high_resolution_clock::now();
        time_ant.push_back((chrono::duration_cast<std::chrono::microseconds>(end - start)).count() / TIMES);

        auto start1 = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            full_combinations(matrix);
        }
        auto end1 = chrono::high_resolution_clock::now();
        time_full.push_back((chrono::duration_cast<std::chrono::microseconds>(end1 - start1)).count() / TIMES);

    }

    ofstream myfile("time_output1.csv");
    for (int i = MIN; i <= MAX; ++i)
        myfile << i << "," << time_full[i - MIN] << "," << time_ant[i - MIN] << endl;
    myfile.close();
}

void parametrization()
{
    vector<double> alpha_arr({ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 });
    vector<double> ro_arr({ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 });
    vector<int> days_arr({ 1, 3, 5, 10, 50, 100, 300, 500 });

    int size = 9;

    auto matrix1 = read_matrix("mat9_lowdif.csv");
    auto matrix2 = read_matrix("mat9_highdif.csv");

    auto optimal1 = full_combinations(matrix1);
    auto optimal2 = full_combinations(matrix2);

    ofstream file1("parametrization_class1.txt");
    ofstream file2("parametrization_class2.txt");

    int count = 0;
    int count_all = alpha_arr.size() * ro_arr.size();

    file1 << "alpha, ro, days, res, dif\n";
    file2 << "alpha, ro, days, res, dif\n";
    for (auto alpha : alpha_arr)
    {
        double beta = 1 - alpha;
        for (auto ro : ro_arr)
        {
            ++count;
            for (auto days : days_arr)
            {
                double d1 = 0;
                double d2 = 0;
                cout << alpha << ',' << ro << ',' << days << endl;
                for (int i = 0; i < TIMES; ++i)
                { 
                    auto res1 = ant_alg(matrix1, alpha, beta, ro, days);
                    auto res2 = ant_alg(matrix2, alpha, beta, ro, days);

                    if (res1.first - optimal1.first > d1)
                        d1 = res1.first - optimal1.first;
                    if (res2.first - optimal2.first > d2)
                        d2 = res2.first - optimal2.first;
                }
                file1 << alpha << ',' << ro << ',' << days << ',' << optimal1.first << ',' << d1 << endl;
                file2 << alpha << ',' << ro << ',' << days << ',' << optimal2.first << ',' << d2 << endl;
            }
        }
    }
}

//int main()
//{
//    time();
//    return 0;
//}