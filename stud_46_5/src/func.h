#ifndef FUNC
#define FUNC

#include <pthread.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <queue>

#define OK 0

#define WORDS_TO_CHOOSE 5

#define DOC_CNT 30
// #define TIMES 1000
#define DOCSIZE 20
#define RAND_LEN rand() % 3 + 2

#define TIMES 15
// #define DOC_CNT 800
// #define DOCSIZE 1
// #define RAND_LEN rand() % 2 + 1
using namespace std;
struct request
{
public:
    request(){}

    vector<vector<string>> files;
    map<string, int> dict;
    timespec p1_start;
    timespec p1_end;
    timespec p2_start;
    timespec p2_end;
    timespec p3_start;
    timespec p3_end;
};

struct pool_result_t
{
public:
    pool_result_t()=default;
    bool operator < (const pool_result_t &other) const
    {
        return this->nanosec < other.nanosec;
    }

    pool_result_t &operator = (const pool_result_t &other)
    {
        this->message = other.message;
        this->nanosec = other.nanosec;
        return *this;
    }

    long long nanosec;
    string message;
};

void linear();
void parallel();
request *proc_1(int doc_count, int word_count);
void proc_2(vector<vector<string>> &files, map<string, int> &dict);
void proc_3(map<string, int> &dict, string filename);
void print_pool(vector<request *> &pool, const string &filename);

void thr_1(int n, queue<request *> &q1to2);
void thr_2(int n, queue<request *> &q1to2, queue<request *> &q2to3);
void thr_3(int n, queue<request *> &q2to3, vector<request *> &pool);


#endif