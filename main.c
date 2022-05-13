#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MYFIFO1 "/tmp/myfifo1"
#define MYFIFO2 "/tmp/myfifo2"
#define MYFIFO3 "/tmp/myfifo3"
#define MYFIFO4 "/tmp/myfifo4"
#define MYFIFO5 "/tmp/myfifo5"
#define MYFIFO6 "/tmp/myfifo6"
#define MYFIFO7 "/tmp/myfifo7"

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
    char string[1000];

    pid_t decode, sotoon, satr, mostatil;

    decode = fork();
    if (decode > 0)
    {
        sotoon = fork();
        if (sotoon > 0)
        {
            satr = fork();
            if (satr > 0)
            {
                mostatil = fork();
            }
        }
    }

    if (decode == 0)
    {
        char str[1000];
        mkfifo(MYFIFO1, 0666);

        int f1 = open(MYFIFO1, O_RDONLY);
        read(f1, str, 1000);
        close(f1);
        // printf("%s decode\n", str);

        decoder(str);

        mkfifo(MYFIFO2, 0666);
        f1 = open(MYFIFO2, O_WRONLY);
        write(f1, str, strlen(str) + 1);
        close(f1);

        mkfifo(MYFIFO3, 0666);
        f1 = open(MYFIFO3, O_WRONLY);
        write(f1, str, strlen(str) + 1);
        close(f1);

        mkfifo(MYFIFO4, 0666);
        f1 = open(MYFIFO4, O_WRONLY);
        write(f1, str, strlen(str) + 1);
        close(f1);
    }
    else if (sotoon == 0)
    {
        char str[2000];
        mkfifo(MYFIFO2, 0666);

        int f1 = open(MYFIFO2, O_RDONLY);
        read(f1, str, 2000);
        close(f1);

        save_dimension(str);

        char sudoku[dimension[0]][dimension[0]];
        set_sudoku_array(str, sudoku);

        int sotoonbool = check_sotoon(sudoku);

        mkfifo(MYFIFO5, 0666);
        f1 = open(MYFIFO5, O_WRONLY);

        if (sotoonbool == 0)
        {
            write(f1, "true", strlen("true") + 1);
        }
        else
        {
            write(f1, "false", strlen("false") + 1);
        }
        close(f1);
    }

    else if (satr == 0)
    {
        char str[2000];
        mkfifo(MYFIFO3, 0666);

        int f1 = open(MYFIFO3, O_RDONLY);
        read(f1, str, 2000);
        close(f1);

        save_dimension(str);

        char sudoku[dimension[0]][dimension[0]];
        set_sudoku_array(str, sudoku);

        int satrbool = check_satr(sudoku);

        mkfifo(MYFIFO7, 0666);
        f1 = open(MYFIFO7, O_WRONLY);

        if (satrbool == 0)
        {
            write(f1, "true", strlen("true"));
        }
        else
        {
            write(f1, "false", strlen("false"));
        }
        close(f1);
    }
    else if (mostatil == 0)
    {

        char str[2000];
        mkfifo(MYFIFO4, 0666);

        int f1 = open(MYFIFO4, O_RDONLY);
        read(f1, str, 2000);
        close(f1);

        save_dimension(str);

        char sudoku[dimension[0]][dimension[0]];
        set_sudoku_array(str, sudoku);
        int mostatilbool = check_mostatil(sudoku);

        mkfifo(MYFIFO6, 0666);
        f1 = open(MYFIFO6, O_WRONLY);

        if (mostatilbool == 0)
        {
            write(f1, "true", strlen("true"));
        }
        else
        {
            write(f1, "false", strlen("false"));
        }
        close(f1);
        // printf("mostatil\n");
    }
    else
    {
        // parent ---------------------------------------------------
        FILE *ptr;
        ptr = fopen("test.txt", "r");

        if (NULL == ptr)
        {
            printf("file can't be opened \n");
            return 0;
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

        mkfifo(MYFIFO1, 0666);
        int f1 = open(MYFIFO1, O_WRONLY);
        write(f1, string, strlen(string) + 1);
        close(f1);

        char sot[5];
        mkfifo(MYFIFO5, 0666);
        int f2 = open(MYFIFO5, O_RDONLY);
        read(f2, sot, 5);
        close(f2);
        // printf("%s sotoon\n", sot);

        mkfifo(MYFIFO7, 0666);
        char sat[6];
        int f3 = open(MYFIFO7, O_RDONLY);
        read(f3, sat, 6);
        close(f3);
        // printf("%s satr\n", sat);

        mkfifo(MYFIFO6, 0666);
        char mos[6];
        int f4 = open(MYFIFO6, O_RDONLY);
        read(f4, mos, 6);
        close(f4);
        // printf("%s mostatil\n", mos);

        if (strstr(mos, "true") != NULL && strstr(sat, "true") != NULL && strstr(sot, "true") != NULL)
        {
            printf("%s\n", "Sudoku Puzzle constraints satisfied!!!");
        }
        else
        {
            printf("%s\n", "Sudoku Puzzle is Wrong!!");
        }

        // parent ---------------------------------------------------
    }

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
        // printf("%c\n", str[k]);
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