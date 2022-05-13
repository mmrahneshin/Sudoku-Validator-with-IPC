#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int dimension[3];
int start;

void remove_new_line(char *str);

void remove_space(char *str);

void decoder(char *str);

void save_dimension(char *str);

void set_sudoku_array(char *str, char sudoku[dimension[0]][dimension[0]]);

int check_sotoon(char sudoku[dimension[0]][dimension[0]]);

int check_satr(char sudoku[dimension[0]][dimension[0]]);

int check_mostatil(char sudoku[dimension[0]][dimension[0]]);

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
    decoder(string);

    save_dimension(string);
    char sudoku[dimension[0]][dimension[0]];

    set_sudoku_array(string, sudoku);

    int sotoonbool = check_sotoon(sudoku);
    int satrbool = check_satr(sudoku);
    int mostatilbool = check_mostatil(sudoku);

    // for (int i = 0; i < 9; i++)
    // {
    //     for (int j = 0; j < 9; j++)
    //     {
    //         printf("%c ", sudoku[i][j]);
    //     }
    //     printf("\n");
    // }

    printf("%d\n", mostatilbool);
    return 0;
}

int check_sotoon(char sudoku[dimension[0]][dimension[0]])
{
    int sw = 0;
    for (int i = 0; i < dimension[0]; i++)
    {
        for (int j = 0; j < dimension[0]; j++)
        {
            for (int k = 0; k < dimension[0]; k++)
            {
                if (j != k)
                {
                    int diff = sudoku[j][i] - sudoku[k][i];
                    // printf("%c %c %d\n", sudoku[j][i], sudoku[k][i], diff);

                    if (diff == 0 || diff == -32 || diff == 32)
                    {
                        sw = 1;
                        return sw;
                    }
                }
            }
        }
    }
    return sw;
}

int check_satr(char sudoku[dimension[0]][dimension[0]])
{
    int sw = 0;
    for (int i = 0; i < dimension[0]; i++)
    {
        for (int j = 0; j < dimension[0]; j++)
        {
            for (int k = 0; k < dimension[0]; k++)
            {
                if (j != k)
                {
                    int diff = sudoku[i][j] - sudoku[i][k];
                    // printf("%c %c %d\n", sudoku[i][j], sudoku[i][k], diff);

                    if (diff == 0 || diff == -32 || diff == 32)
                    {
                        sw = 1;
                        return sw;
                    }
                }
            }
        }
    }
    return sw;
}

int check_mostatil(char sudoku[dimension[0]][dimension[0]])
{
    int sw = 0;
    for (int i = 0; i < dimension[0]; i += dimension[1])
    {
        for (int j = 0; j < dimension[0]; j += dimension[2])
        {
            for (int k = i; k < i + dimension[1]; k++)
            {
                for (int l = j; l < j + dimension[2]; l++)
                {
                    for (int m = i; m < i + dimension[1]; m++)
                    {
                        for (int n = j; n < j + dimension[2]; n++)
                        {
                            if (k != m || l != n)
                            {
                                int diff = sudoku[k][l] - sudoku[m][n];
                                if (diff == 0 || diff == -32 || diff == 32)
                                {
                                    sw = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return sw;
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

void decoder(char *str)
{
    char ch;
    int key = 2;

    // get the section that must be encrypte through pipe

    // decode by caesar cipher
    int i;
    for (i = 0; str[i] != '\0'; ++i)
    {
        ch = str[i];
        if (ch >= 'a' && ch <= 'z')
        {
            ch = ch - key;
            if (ch > 'z')
            {
                ch = ch - 'z' + 'a' - 1;
            }
            str[i] = ch;
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = ch - key;
            if (ch > 'Z')
            {
                ch = ch - 'Z' + 'A' - 1;
            }
            str[i] = ch;
        }
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