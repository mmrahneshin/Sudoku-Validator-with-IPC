#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_new_line(char *str);

void remove_space(char *str);

void save_dimension(char *str);

int main()
{
    FILE *ptr;
    int sudoku, tool, arz;
    char string[1000];
    ptr = fopen("test.txt", "r");

    if (NULL == ptr)
    {
        printf("file can't be opened \n");
    }

    int i = 0;
    while (!feof(ptr))
    {
        string[i] = fgetc(ptr);
        i++;
    }
    fclose(ptr);

    remove_new_line(string);
    remove_space(string);
    string[strlen(string) - 1] = '\0';
    printf("%s", string);

    return 0;
}

// void save_dimension(char *str)
// {
//     int i;
//     int number = 0;
//     for (i = 0; str[i]; i++)
//     {
//         number += str[i] - '0' + number * 10;
//         if (str[i] == '*')
//         {
//             number = 0;
//         }
//     }
// }

void remove_new_line(char *str)
{
    int count = 0, i;
    for (i = 0; str[i]; i++)
        if (str[i] != '\n')
            str[count++] = str[i];
    str[count] = '\0';
}

void remove_space(char *str)
{
    int count = 0, i;
    for (i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
    str[count] = '\0';
}