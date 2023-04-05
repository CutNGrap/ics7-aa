#include <stdio.h>
//#include <unistd.h>
#include <thread>
#include "func.h"
#include <iostream>
#include <cstdlib>  


using namespace std;

int main()
{
    system("chcp 1251");
    //setbuf(stdout, NULL);
    int result = 1;
    int choice = -1;
    while (choice && result == 1)
    {
        cout << "\n\n\nВыберите номер команды:\n";
        cout << "\
\t1 - полный перебор\
\n\t2 - муравьиный алгоритм\
\n\t3 - муравьиный алгоритм с потоками\
\n\t4 - время\
\n\t5 - параметризация\
\n\t9 - выход\n\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            parse_full_combinations();
            choice = -1;
            break;
        case 2:
            parse_ant_alg();
            choice = -1;
            break;
        case 3:
            parse_ant_alg_thr();
            choice = -1;
            break;
        case 4:
            my_time1();
            choice = -1;
            break;
        case 5:
            parametrization();
            choice = -1;
            break;
        case 0:
            break;

        default:
            choice = -1;
            printf("\nНеверный ввод, повторите попытку.\n\n");
            break;
        }
    }
    printf("Спасибо, что пользовались данным приложением!\n");
    return 0;
}
