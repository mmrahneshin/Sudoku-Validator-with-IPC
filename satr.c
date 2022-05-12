#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

#define MYFIFO2 "/tmp/myfifo2"
#define MYFIFO4 "/tmp/myfifo4"

int dimension[3];
int start;

void save_dimension(char *str);

void set_sudoku_array(char *str, char sudoku[dimension[0]][dimension[0]]);

int check_satr(char sudoku[dimension[0]][dimension[0]]);

int main()
{
    int fd1, fd2;
    char str[2000];

    fd1 = open(MYFIFO2, O_RDONLY);
    int index = read(fd1, str, sizeof(str));
    str[index] = '\0';
    close(fd1);

    save_dimension(str);
    char sudoku[dimension[0]][dimension[0]];

    set_sudoku_array(str, sudoku);

    int sw = check_satr(sudoku);

    // for (int i = 0; i < 9; i++)
    // {
    //     for (int j = 0; j < 9; j++)
    //     {
    //         printf("%c ", sudoku[i][j]);
    //     }
    //     printf("\n");
    // }

    FILE *fdecoder = fopen("satr.txt", "a");
    fd2 = open(MYFIFO4, O_WRONLY);
    if (sw == 0)
    {
        fprintf(fdecoder, "%s\n", "true");
        write(fd2, "true", strlen("true") + 1);
    }
    else
    {
        fprintf(fdecoder, "%s\n", "false");
        write(fd2, "false", strlen("false") + 1);
    }

    fclose(fdecoder);

    close(fd2);
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
                    int diff = sudoku[j][i] - sudoku[j][i];
                    if (diff == 0 || diff == -20 || diff == 20)
                    {
                        sw = 1;
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