#ifndef FUNC
#define FUNC

#include <pthread.h>
#include <stdio.h>

#define OK 0
#define LETTERS_CNT 26
#define DOC_CNT 256
#define TIMES 1000
#define DOCSIZE 30

typedef struct
{
    int start_doc; 
    int end_doc; 
    pthread_mutex_t *mutex;
    int *word_count; // DF counter
    char *docs;
} thread_args_t;

int measure_time(int threads, FILE *f, char *docs, int docs_cnt);
int measure_time_mutex(int threads, FILE *f, char *docs, int docs_cnt);
long int my_time();
void set_frequency(char *frequency, int doc_cnt, int doc_size);
int *allocate_flag_arr(int m);
void free_flag_arr(int *arr);


#endif