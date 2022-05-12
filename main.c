#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int dimension[3];
int start;

void remove_new_line(char *str);

void remove_space(char *str);

void save_dimension(char *str);

void set_sudoku_array(char *str, char sudoku[dimension[0]][dimension[0]]);

int main()
{
    FILE *ptr;
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

    save_dimension(string);
    char sudoku[dimension[0]][dimension[0]];

    set_sudoku_array(string, sudoku);

    // for (int i = 0; i < 9; i++)
    // {
    //     for (int j = 0; j < 9; j++)
    //     {
    //         printf("%c ", sudoku[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}

void set_sudoku_array(char *str, char sudoku[dimension[0]][dimension[0]])
{
    int i = 0, j = 0, k;

    for (k = start; str[k]; k++)
    {
        if (str[k] == '#')
        {
            j = 0;
            i++;
            continue;
        }

        sudoku[i][j] = str[k];
        j++;
    }
}

void save_dimension(char *str)
{
    int i;
    int number = 0;
    int count = 0;
    for (i = 0; str[i]; i++)
    {
        if (str[i] == '*')
        {
            dimension[count] = number;
            number = 0;
            count++;
            continue;
        }

        if (!isdigit(str[i]) && str[i] != '*')
        {
            dimension[count] = number;
            start = i;
            break;
        }
        number += str[i] - '0' + number * 10;
    }
}

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