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


#define MIN 10
#define MAX 19
#define TIMES 10


void my_time()
{
    //vector<string> names({ "s10.csv", "s12.csv" ,"s14.csv" ,"s16.csv" ,"s18.csv", "s20.csv",});
    vector<string> names({ "s10.csv" });
    vector<int> time_ant;
    vector<int> time_thread;

    vector<vector<double>> matrix;

    ifstream file(names[0]);
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

    for (int days = 100; days < 2000; days+=200)
    {
        cout << days << endl;
        auto start1 = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            ant_alg_thr1(matrix, 0.5, 0.5, 0.5, days, 4);
        }
        auto end1 = chrono::high_resolution_clock::now();
        time_thread.push_back((chrono::duration_cast<std::chrono::milliseconds>(end1 - start1)).count() / TIMES);

        auto start = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            ant_alg(matrix, 0.5, 0.5, 0.5, days);
        }
        auto end = chrono::high_resolution_clock::now();
        time_ant.push_back((chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / TIMES);

    }

    ofstream myfile("time_output.csv");
    for (int i = 0; i < time_thread.size(); ++i)
         myfile << 100 + 200 * i<< " & "<< time_thread[i]<<" & "<< time_ant[i]<<endl;
    myfile.close();
}

void my_time1()
{
    vector<string> names({ "s10.csv" });
    vector<int> time_ant;
    vector<int> time_thread;

    vector<vector<double>> matrix;

    ifstream file(names[0]);
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
        ant_alg(matrix, 0.5, 0.5, 0.5, 1000);
    }
    auto end = chrono::high_resolution_clock::now();
    time_ant.push_back((chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / TIMES);

    for (int th = 1; th < 32; th *= 2)
    {
        cout << th << endl;
        auto start1 = chrono::high_resolution_clock::now();
        for (int loop = 0; loop < TIMES; ++loop)
        {
            ant_alg_thr1(matrix, 0.5, 0.5, 0.5, 1000, th);
        }
        auto end1 = chrono::high_resolution_clock::now();
        time_thread.push_back((chrono::duration_cast<std::chrono::milliseconds>(end1 - start1)).count() / TIMES);


    }

    ofstream myfile("time_output_thr.csv");
    myfile << 0 <<" & " << time_ant[0] << endl;
    for (int i = 0; i < 5; ++i)
        myfile << pow(2, i) << " & " << time_thread[i] <<endl;
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