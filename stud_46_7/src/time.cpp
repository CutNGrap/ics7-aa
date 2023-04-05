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

void 

//int main()
//{
//    time();
//    return 0;
//}