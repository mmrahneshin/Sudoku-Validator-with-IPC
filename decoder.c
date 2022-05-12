#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

#define MYFIFO1 "/tmp/myfifo1"
#define MYFIFO2 "/tmp/myfifo2"

int dimension[3];
int start;

void save_dimension(char *str);

void set_sudoku_array(char *str, char sudoku[dimension[0]][dimension[0]]);

int main()
{
    char ch;
    int key = 3;
    int fd1, fd2;
    char str[2000];

    // get the section that must be encrypte through pipe
    fd1 = open(MYFIFO1, O_RDONLY);
    int index = read(fd1, str, sizeof(str));
    str[index] = '\0';
    close(fd1);

    // decode by caesar cipher
    int i;
    for (i = 0; str[i] != '\0'; ++i)
    {
        ch = str[i];
        if (ch >= 'a' && ch <= 'z')
        {
            ch = ch + key;
            if (ch > 'z')
            {
                ch = ch - 'z' + 'a' - 1;
            }
            str[i] = ch;
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = ch + key;
            if (ch > 'Z')
            {
                ch = ch - 'Z' + 'A' - 1;
            }
            str[i] = ch;
        }
    }

    // printf("converted : %s \n",message);

    // Save the Result in file

    save_dimension(str);
    char sudoku[dimension[0]][dimension[0]];

    set_sudoku_array(str, sudoku);

    // for (int i = 0; i < 9; i++)
    // {
    //     for (int j = 0; j < 9; j++)
    //     {
    //         printf("%c ", sudoku[i][j]);
    //     }
    //     printf("\n");
    // }

    FILE *fdecoder = fopen("decoder.txt", "a");

    fprintf(fdecoder, "%s\n", str);
    fclose(fdecoder);

    // Send the result through pipe to finder
    fd2 = open(MYFIFO2, O_WRONLY);
    write(fd2, str, strlen(str) + 1);
    close(fd2);
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