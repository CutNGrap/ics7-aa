#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    srand ( time(NULL) ); 
    setbuf(stdout, NULL);

    FILE *f = fopen("no_mutex.csv", "w");
    FILE *g = fopen("mutex.csv", "w");
    fprintf(f, "docs,0,1,2,4,8,16,32,64\n");
    for (int docs_cnt = 2048; docs_cnt <= 4096 * 2; docs_cnt += 512)
    {
        fprintf(f, "%d,", docs_cnt);
        char *docs = malloc(sizeof(char) * docs_cnt * DOCSIZE);
        set_frequency(docs, docs_cnt);
        if (!f)
        {
            printf("Couldn't open file\n");
            return 1;
        }
        measure_time(0, f, docs, docs_cnt);
        for (int i = 1; i <= 64; i *= 2)
            measure_time(i, f, docs, docs_cnt);
        fprintf(f, "\n");
        free(docs);
    }

    fprintf(g, "docs,0,1,2,4,8,16,32,64\n");
    for (int docs_cnt = 2048; docs_cnt <= 4096 * 2; docs_cnt += 512)
    {
        fprintf(g, "%d,", docs_cnt);
        char *docs = malloc(sizeof(char) * docs_cnt * DOCSIZE);
        set_frequency(docs, docs_cnt);
        if (!f)
        {
            printf("Couldn't open file\n");
            return 1;
        }
        measure_time_mutex(0, g, docs, docs_cnt);
        for (int i = 1; i <= 64; i *= 2)
            measure_time_mutex(i, g, docs, docs_cnt);
        fprintf(g, "\n");
        free(docs);
    }
    fclose(f);
    fclose(g);
    return 0;
}
