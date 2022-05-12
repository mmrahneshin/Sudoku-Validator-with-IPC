#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

#define MYFIFO2 "/tmp/myfifo2"
#define MYFIFO3 "/tmp/myfifo3"

int dimension[3];
int start;

void save_dimension(char *str);

void set_sudoku_array(char *str, char sudoku[dimension[0]][dimension[0]]);

void check_sotoon(char sudoku[dimension[0]][dimension[0]], char sw[]);

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

    char sw[20] = "true";
    check_sotoon(sudoku, sw);

    FILE *fdecoder = fopen("sotoon.txt", "a");

    fprintf(fdecoder, "%s\n", sw);
    fclose(fdecoder);

    fd2 = open(MYFIFO3, O_WRONLY);
    write(fd2, sw, strlen(sw) + 1);
    close(fd2);
}

void check_sotoon(char sudoku[dimension[0]][dimension[0]], char sw[])
{
    for (int i = 0; i < dimension[0]; i++)
    {
        for (int j = 0; j < dimension[0]; j++)
        {
            for (int k = 0; k < dimension[0]; k++)
            {
                if (j != k)
                {
                    int diff = sudoku[i][j] - sudoku[i][k];
                    if (diff == 0 || diff == -20 || diff == 20)
                    {
                        sw = 'false';
                    }
                }
            }
        }
    }
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