#include <stdio.h>
#include <unistd.h>
#include <thread>
#include "func.h"

int main()
{
    setbuf(stdout, NULL);
    int result = OK;
    int choice = -1;
    while (choice && result == OK)
    {
        printf("\n\n\nВыберите номер команды\n");
        printf("1 - последовательный обсчет документной частоты DF\n");
        printf("2 - конвеерный обсчет документной частоты DF\n");
        printf("0 - выход\n\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        char ch = 1;
        while (ch != '\n')
            scanf("%c", &ch);
        switch (choice)
        {
        case 1:
            linear();
            choice = -1;
            break;
        case 2:
            parallel();
            choice = -1;
            break;
        case 0:
            break;
        
        default:
            printf("\nНеверный ввод, повторите попытку.\n\n");
            break;
        }
    }
    printf("Спасибо, что пользовались данным приложением!\n");
    return result;
}
