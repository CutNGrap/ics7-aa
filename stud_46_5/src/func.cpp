#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "func.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <mutex>
#include <thread>
#include <ctime>
#include <unistd.h>
#include <set>


using namespace std;

mutex mutex_q1to2;
mutex mutex_q2to3;

string gen_random(const int len) {
    static const char alphanum[] ="abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

request *proc_1(int doc_count, int word_count)
{
    request *r = new request();
    for (int i = 0; i < doc_count; ++i)
    {
        vector<string> doc;
        for (int j = 0; j < word_count; ++j)
            doc.push_back(gen_random(RAND_LEN));
        r->files.push_back(doc);
    }
    return r;
}

void proc_2(vector<vector<string>> &files, map<string, int> &dict)
{
    for (auto file:files)
    {
        set<string> sett;
        for (auto word : file)
            sett.insert(word);
        for (auto s : sett)
        {
            if (dict.count(s) == 0)
                dict.insert({s, 1});
            else
                ++dict[s];
        }
    }
}

void proc_3(map<string, int> &dict, string filename)
{
    ofstream myfile;
    myfile.open (filename);
    myfile<<"Наиболее частые слова:\n";
    for (int j = 0; j < WORDS_TO_CHOOSE; j++)
    {
        string maxi = dict.begin()->first;
        int max = dict.begin()->second;
        for (auto i:dict)
        {
            if (i.second > max)
            {
                max = i.second;
                maxi = i.first;
            }
        }
        dict[maxi] = -1;
        myfile<<maxi<<' '<<max<<endl;
    }
    myfile.close();
}

void print_pool(vector<request *> &pool, const string &filename)
{
    vector<pool_result_t> pool_sort(pool.size() * 6);
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
        snprintf(buf, 50, "Request %llu start generating: %lld ns", i, nanosec);
        pool_sort[i * 6].message = string(buf);
        pool_sort[i * 6].nanosec = nanosec;
        
        nanosec = pool[i]->p1_end.tv_sec * 1000000000 + pool[i]->p1_end.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu end generating: %lld ns", i, nanosec);
        pool_sort[i * 6 + 1].message = string(buf);
        pool_sort[i * 6 + 1].nanosec = nanosec;
        
        nanosec = pool[i]->p2_start.tv_sec * 1000000000 + pool[i]->p2_start.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu start counting: %lld ns", i, nanosec);
        pool_sort[i * 6 + 2].message = string(buf);
        pool_sort[i * 6 + 2].nanosec = nanosec;
        
        nanosec = pool[i]->p2_end.tv_sec * 1000000000 + pool[i]->p2_end.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu end counting: %lld ns", i, nanosec);
        pool_sort[i * 6 + 3].message = string(buf);
        pool_sort[i * 6 + 3].nanosec = nanosec;
        
        nanosec = pool[i]->p3_start.tv_sec * 1000000000 + pool[i]->p3_start.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu start writing most frequent: %lld ns", i, nanosec);
        pool_sort[i * 6 + 4].message = string(buf);
        pool_sort[i * 6 + 4].nanosec = nanosec;
        
        nanosec = pool[i]->p3_end.tv_sec * 1000000000 + pool[i]->p3_end.tv_nsec;
        nanosec -= min_nano;
        buf = new char[50];
        snprintf(buf, 50, "Request %llu end writing most frequent: %lld ns", i, nanosec);
        pool_sort[i * 6 + 5].message = string(buf);
        pool_sort[i * 6 + 5].nanosec = nanosec;
    }
    sort(pool_sort.begin(), pool_sort.end());
    ofstream fout;
    fout.open(filename);
    for (size_t i = 0; i < pool_sort.size(); ++i)
    {
        fout << pool_sort[i].message << endl;
    }
    fout.close();

}

void thr_1(int n, queue<request *> &q1to2)
{
    timespec time;
    for (int i = 0; i < n; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &time);
        request *r = proc_1(DOC_CNT, DOCSIZE);
        r->p1_start = time;

        mutex_q1to2.lock();
        clock_gettime(CLOCK_REALTIME, &(r->p1_end));
        q1to2.push(r);
        mutex_q1to2.unlock();
    }
}

void thr_2(int n, queue<request *> &q1to2, queue<request *> &q2to3)
{
    for (int i = 0; i < n; ++i)
    {
        while (q1to2.empty());
        mutex_q1to2.lock();
        request *r = q1to2.front();
        q1to2.pop();
        mutex_q1to2.unlock();
        clock_gettime(CLOCK_REALTIME, &(r->p2_start));
        proc_2(r->files, r->dict);

        mutex_q2to3.lock();
        clock_gettime(CLOCK_REALTIME, &(r->p2_end));
        q2to3.push(r);
        mutex_q2to3.unlock();
    }
}

void thr_3(int n, queue<request *> &q2to3, vector<request *> &pool)
{
    for (int i = 0; i < n; ++i)
    {
        while (q2to3.empty());
        mutex_q2to3.lock();
        request *r = q2to3.front();
        q2to3.pop();
        mutex_q2to3.unlock();
        clock_gettime(CLOCK_REALTIME, &(r->p3_start));
        proc_3(r->dict, "file.txt");
        clock_gettime(CLOCK_REALTIME, &(r->p3_end));
        pool[i] = r;
    }
}


void linear()
{
    int n = TIMES;
    vector<request *> pool(n);
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
    print_pool(pool, "linear.txt");
    for (size_t i = 0; i < pool.size(); ++i)
        delete pool[i];
}

void parallel()
{
    int n = TIMES;
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
    print_pool(pool, "parallel.txt");
    for (size_t i = 0; i < pool.size(); ++i)
        delete pool[i];
}