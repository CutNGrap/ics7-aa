#define LET 26
#define DOC 30
#define DOC_LEN 30

//LET --- количество букв в алфавите
//DOC_LEN --- количество слов в документе
//DOC --- число документов в выборке
//w_count[i] --- количество документов, содержащих слово i
//flag--- массив флагов 

int w_count[LET]; //-1
char docs[DOC][DOC_LEN]; //0 
for (int i = 0; i < DOC; ++i) //1
{
    int flag[LET]; //2
    for (int j = 0; j < DOC_LEN; ++j) //3
    {   
        int n = (docs)[i*DOC + j] - 'a';//4
        flag[n] = 1; //5
    }

    for (int k = 0; k < LET; ++k) //6
            w_count[k] += flag[k]; //7
}

int sw = 1; //8
for (int m = 0; m < LET && sw; ++m) //9
{
    sw = 0;//10
    for (int q = 0; q < LET - 1; ++q) //11
    {
        if (w_count[q] > w_count[q + 1]) //12
        {
            swap(w_count[q], w_count[q + 1]);//13
            ++sw; //14
        }
    }
}

