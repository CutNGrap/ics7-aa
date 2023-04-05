#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <processthreadsapi.h>
#include <pthread.h>
#include <conio.h>
#include <time.h>





long int my_time()
{
    struct timespec stamp;
    clock_gettime(CLOCK_REALTIME, &stamp);
    return stamp.tv_nsec ; // в dwLowDateTime время с запуска процесса в единицах 1e-7 сек
}


void set_frequency(char *frequency, int doc_cnt, int doc_size)
{
    for (int i = 0; i < doc_cnt; ++i)
        for (int j = 0; j < doc_size; ++j)
            frequency[doc_size * i + j] = rand() % LETTERS_CNT + 'a';
}

int *allocate_flag_arr(int m)
{
    return calloc(m, sizeof(int));
}

void free_flag_arr(int *arr)
{
    free(arr);
}

 
void *thread_work_mutex(void *args)
{
    thread_args_t *arg = args;

    int start = arg->start_doc;
    int end = arg->end_doc;
    pthread_mutex_t *mutex = arg->mutex;
    char *docs = arg->docs;
    int *word_count = arg->word_count;
    for (int i = start; i < end; ++i)
    {
        int *flag_arr = allocate_flag_arr(LETTERS_CNT);
        for (int j = 0; j < DOCSIZE; ++j)
            ++(flag_arr[(docs)[i*DOCSIZE + j] - 'a']);

        for (int i = 0; i < LETTERS_CNT; ++i)
        {
            if (flag_arr[i])
            {
                pthread_mutex_lock((mutex) + i);
                    ++(word_count[i]);
                pthread_mutex_unlock((mutex) + i);
            }
        }
        free_flag_arr(flag_arr);
    }

    return NULL;
}

void *thread_work(void *args)
{
    thread_args_t *arg = args;

    int start = arg->start_doc;
    int end = arg->end_doc;
    char *docs = arg->docs;
    int *word_count = arg->word_count;
    for (int i = start; i < end; ++i)
    {
        int *flag_arr = allocate_flag_arr(LETTERS_CNT);
        for (int j = 0; j < DOCSIZE; ++j)
            ++(flag_arr[(docs)[i*DOCSIZE + j] - 'a']);

        for (int i = 0; i < LETTERS_CNT; ++i)
        {
            if (flag_arr[i])
                ++(word_count[i]);
        }
        free_flag_arr(flag_arr);
    }

    return NULL;
}

 
int measure_time(int threads, FILE *f, char *docs, int docs_cnt)
{
    int word_count[LETTERS_CNT];
    for (int i = 0; i < LETTERS_CNT; ++i)
        word_count[i] = 0;
    
    if (threads)
    {
        struct timespec start;
        struct timespec end;
        clock_gettime(CLOCK_REALTIME, &start);
        for (int k = 0; k < TIMES; ++k)
        {
            int *word_count = calloc(threads * LETTERS_CNT, sizeof(int));
            pthread_t *tid = malloc(threads * sizeof(pthread_t));
            thread_args_t *args = malloc(threads * sizeof(thread_args_t));
            if (tid && args)
            {
                int delta_doc = docs_cnt / threads;
                int last = docs_cnt % threads;
                int start = 0;
                int end = delta_doc + last;
                for (int thread = 0; thread < threads; ++thread)
                {
                    (args + thread)->start_doc = start;
                    (args + thread)->end_doc = end;
                    // (args + thread)->mutex = mutex;
                    (args + thread)->docs = docs;
                    (args + thread)->word_count = word_count + thread * LETTERS_CNT;
                    pthread_create(tid + thread, NULL, &thread_work, args + thread);
                    start = end;
                    end += delta_doc;
                }
                for (int thread = 0; thread < threads; ++thread)
                    pthread_join(tid[thread], NULL);
            }
            else
                printf("Не удалось выделить память для аргументов потока");
            free(tid);
            free(args);
            free(word_count);
        }
        clock_gettime(CLOCK_REALTIME, &end);
        long int ttime = ((end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec) / TIMES / 1000;
        printf("%d %d it took %ld msec\n", docs_cnt, threads, ttime); 
        fprintf(f, "%ld", ttime);
        if (threads !=64)
            fprintf(f, ",");
    }
    else
    {
        struct timespec start;
        struct timespec end;
        clock_gettime(CLOCK_REALTIME, &start);
        for (int k = 0; k < TIMES; ++k)
        {

            for (int i = 0; i < docs_cnt; ++i)
            {
                int *flag_arr = allocate_flag_arr(LETTERS_CNT);
                for (int j = 0; j < DOCSIZE; ++j)
                    ++(flag_arr[(docs)[i*DOCSIZE + j] - 'a']);

                for (int i = 0; i < LETTERS_CNT; ++i)
                {
                    if (flag_arr[i])
                        ++word_count[i];
                }
                free_flag_arr(flag_arr);
            }

        }
        clock_gettime(CLOCK_REALTIME, &end);
        long int ttime = ((end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec) / TIMES / 1000;
        printf("%d %d it took %ld msec\n", docs_cnt, threads, ttime); 
        fprintf(f, "%ld", ttime);
        if (threads !=64)
            fprintf(f, ",");
    }

    return 0;
}


 
int measure_time_mutex(int threads, FILE *f, char *docs, int docs_cnt)
{
    int word_count[LETTERS_CNT];
    for (int i = 0; i < LETTERS_CNT; ++i)
        word_count[i] = 0;
 
    // Инициализация мьютекса
    pthread_mutex_t mutex[LETTERS_CNT];
    for (int i = 0; i < LETTERS_CNT; ++i)
        pthread_mutex_init(mutex + i, NULL);
    
    if (threads)
    {
        struct timespec start;
        struct timespec end;
        clock_gettime(CLOCK_REALTIME, &start);
        for (int k = 0; k < TIMES; ++k)
        {
            int *word_count = calloc(threads * LETTERS_CNT, sizeof(int));
            pthread_t *tid = malloc(threads * sizeof(pthread_t));
            thread_args_t *args = malloc(threads * sizeof(thread_args_t));
            if (tid && args)
            {
                int delta_doc = docs_cnt / threads;
                int last = docs_cnt % threads;
                int start = 0;
                int end = delta_doc + last;
                for (int thread = 0; thread < threads; ++thread)
                {
                    (args + thread)->start_doc = start;
                    (args + thread)->end_doc = end;
                    (args + thread)->mutex = mutex;
                    (args + thread)->docs = docs;
                    (args + thread)->word_count = word_count + thread * LETTERS_CNT;
                    pthread_create(tid + thread, NULL, &thread_work_mutex, args + thread);
                    start = end;
                    end += delta_doc;
                }
                for (int thread = 0; thread < threads; ++thread)
                    pthread_join(tid[thread], NULL);
            }
            else
                printf("Не удалось выделить память для аргументов потока");
            free(tid);
            free(args);
            free(word_count);
        }
        clock_gettime(CLOCK_REALTIME, &end);
        long int ttime = ((end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec) / TIMES / 1000;
        printf("%d %d it took %ld msec\n", docs_cnt, threads, ttime); 
        fprintf(f, "%ld", ttime);
        if (threads !=64)
            fprintf(f, ",");
    }
    else
    {
        struct timespec start;
        struct timespec end;
        clock_gettime(CLOCK_REALTIME, &start);
        for (int k = 0; k < TIMES; ++k)
        {

            for (int i = 0; i < docs_cnt; ++i)
            {
                // int *flag_arr = allocate_flag_arr(LETTERS_CNT);
                int flag_arr[LETTERS_CNT] = {0};
                for (int j = 0; j < DOCSIZE; ++j)
                {
                    int number = (docs)[i*DOCSIZE + j] - 'a';
                    flag_arr[number] = 1;
                }

                for (int i = 0; i < LETTERS_CNT; ++i)
                        word_count[i] += flag_arr[i];
                // free_flag_arr(flag_arr);
            }

        }
        clock_gettime(CLOCK_REALTIME, &end);
        long int ttime = ((end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec) / TIMES / 1000;
        printf("%d %d it took %ld msec\n", docs_cnt, threads, ttime); 
        fprintf(f, "%ld", ttime);
        if (threads !=64)
            fprintf(f, ",");
    }


    for (int i = 0; i < LETTERS_CNT; ++i)
    {
        // printf("%d ", word_count[i] / TIMES);
        pthread_mutex_destroy(mutex + i);
    }
    return 0;
}
