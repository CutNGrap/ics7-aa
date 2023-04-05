#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    srand ( time(NULL) ); 
    setbuf(stdout, NULL);

    // int doc_size = DOCSIZE;
    int doc_size = 10;
    int docs_cnt = 10;
    char *docs = malloc(sizeof(char) * docs_cnt * doc_size);
    set_frequency(docs, docs_cnt, doc_size);

    int word_count[LETTERS_CNT];
    for (int i = 0; i < LETTERS_CNT; ++i)
        word_count[i] = 0;

    for (int i = 0; i < docs_cnt; ++i)
    {
        for (int j = 0; j < doc_size; ++j)
            printf("%c", docs[i * doc_size + j]);
        printf("\n");
    }

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
    printf("word_count:\n");

    for (int i = 0; i < LETTERS_CNT; ++i)
        printf("%d ", word_count[i]);
    
    printf("\nMost frequent terms:");
    for (int j = 0; j < 5; j++)
    {
        int maxi = 0;
        int max = word_count[maxi];
        for (int i = 0; i < LETTERS_CNT; ++i)
        {
            if (word_count[i] > max)
            {
                max = word_count[i];
                maxi = i;
            }
        }
        printf(" %c", 'a' + maxi);
        word_count[maxi] = -1;
    }

    free(docs);
    return 0;
}
