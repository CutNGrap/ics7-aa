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
        cout << "\n\n\n�������� ����� �������:\n";
        cout << "\
\t1 - ������ �������\
\n\t2 - ���������� ��������\
\n\t3 - ��� ���������\
\n\t4 - �����\
\n\t5 - ��������������\
\n\t0 - �����\n\n";
        cout << "��� �����: ";
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
            parse_all();
            choice = -1;
            break;
        case 4:
            my_time();
            choice = -1;
            break;
        case 0:
            break;

        default:
            printf("\n�������� ����, ��������� �������.\n\n");
            break;
        }
    }
    printf("�������, ��� ������������ ������ �����������!\n");
    return result;
}
