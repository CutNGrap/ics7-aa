#include <stdio.h>
#include <unistd.h>
#include <thread>
#include "func.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;


#define MAX_REQUESTS 100
#define LOOPS 100

int num = 1;
long long sum = 0;

void print_pool1(std::vector<request *> &pool, const std::string &filename)
{
    std::vector<pool_result_t> pool_sort(pool.size() * 6);
    timespec min = pool[0]->p1_start;
    for (size_t i = 0; i < pool.size(); ++i)
    {
        if (min.tv_sec > pool[i]->p1_start.tv_sec || (min.tv_sec == pool[i]->p1_start.tv_sec && min.tv_nsec > pool[i]->p1_start.tv_nsec))
            min = pool[i]->p1_start;
    }
    long long min_nano = min.tv_sec * 1000000000 + min.tv_nsec;
    for (size_t i = 0; i < pool.size(); ++i)
    {
        long long nanosec = pool[i]->p1_start.tv_sec * 1000000000 + pool[i]->p1_start.tv_nsec;
        nanosec -= min_nano;
        char *buf = new char[50];
        snprintf(buf, 50, "Request %llu start creating: %lld ns", i, nanosec);
        pool_sort[i * 6].message = std::string(buf);
        pool_sort[i * 6].nanosec = nanosec;
        
        nanosec = pool[i]->p1_end.tv_sec * 1000000000 + pool[i]->p1_end.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu end creating: %lld ns", i, nanosec);
        pool_sort[i * 6 + 1].message = std::string(buf);
        pool_sort[i * 6 + 1].nanosec = nanosec;
        
        nanosec = pool[i]->p2_start.tv_sec * 1000000000 + pool[i]->p2_start.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu start sum: %lld ns", i, nanosec);
        pool_sort[i * 6 + 2].message = std::string(buf);
        pool_sort[i * 6 + 2].nanosec = nanosec;
        
        nanosec = pool[i]->p2_end.tv_sec * 1000000000 + pool[i]->p2_end.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu end sum: %lld ns", i, nanosec);
        pool_sort[i * 6 + 3].message = std::string(buf);
        pool_sort[i * 6 + 3].nanosec = nanosec;
        
        nanosec = pool[i]->p3_start.tv_sec * 1000000000 + pool[i]->p3_start.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu start unpack: %lld ns", i, nanosec);
        pool_sort[i * 6 + 4].message = std::string(buf);
        pool_sort[i * 6 + 4].nanosec = nanosec;
        
        nanosec = pool[i]->p3_end.tv_sec * 1000000000 + pool[i]->p3_end.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu end unpack: %lld ns", i, nanosec);
        pool_sort[i * 6 + 5].message = std::string(buf);
        pool_sort[i * 6 + 5].nanosec = nanosec;
    }
    std::sort(pool_sort.begin(), pool_sort.end());

    std::ofstream fout;
    fout.open(filename, std::ios_base::app);
    sum += pool_sort[pool_sort.size() - 1].nanosec;
    // printf("%s %lld\n", pool_sort[pool_sort.size() - 1].message.c_str(), pool_sort[pool_sort.size() - 1].nanosec);
    if (num % LOOPS == 0)
    {
        fout << sum / LOOPS / 1000 / 1000<< std::endl;
        sum = 0;
    }
    ++num;
    fout.close();

}

void time_linear()
{
    for (int j = 10; j <= MAX_REQUESTS; j+=10)
    {
        cout<<"linear "<<j<<endl;
        std::ofstream fout;
        fout.open("linear_measure.txt", std::ios_base::app);
        fout << j << ' ';
        fout.close();
        for (int loop = 0; loop < LOOPS; ++loop)
        {
            int n = j;
            std::vector<request *> pool(n);
            timespec time;
            for (int i = 0; i < n; ++i)
            {
                clock_gettime(CLOCK_REALTIME, &time);
                request *r = proc_1(DOC_CNT, DOCSIZE);
                r->p1_start = time;
                clock_gettime(CLOCK_REALTIME, &(r->p1_end));
                clock_gettime(CLOCK_REALTIME, &(r->p2_start));
                proc_2(r->files, r->dict);
                clock_gettime(CLOCK_REALTIME, &(r->p2_end));
                clock_gettime(CLOCK_REALTIME, &(r->p3_start));
                proc_3(r->dict, "file.txt");
                clock_gettime(CLOCK_REALTIME, &(r->p3_end));
                pool[i] = r;
            }
            print_pool1(pool, "linear_measure.txt");
            for (size_t i = 0; i < pool.size(); ++i)
                delete pool[i];
        }
    }
}

void time_parallel()
{
    
    for (int i = 10; i <= MAX_REQUESTS; i+=10)
    {
        cout<<"parallel "<<i<<endl;
        std::ofstream fout;
        fout.open("parallel_measure.txt", std::ios_base::app);
        fout << i << ' ';
        fout.close();
        for (int loop = 0; loop < LOOPS; ++loop)
        {
            int n = i;
            string s = "file.txt";
            vector<request *> pool(n);
            queue<request *> q1to2;
            queue<request *> q2to3;
            thread t_1(thr_1, n, ref(q1to2));
            thread t_2(thr_2, n, ref(q1to2), ref(q2to3));
            thread t_3(thr_3, n, ref(q2to3), ref(pool));
            t_1.join();
            t_2.join();
            t_3.join();
            print_pool1(pool, "parallel_measure.txt");
            for (size_t i = 0; i < pool.size(); ++i)
                delete pool[i];
        }
    }
}

int main()
{
    std::ofstream fout;
    fout.open("parallel_measure.txt");
    fout.close();
    fout.open("linear_measure.txt");
    fout.close();
    time_linear();
    time_parallel();
    return 0;
}