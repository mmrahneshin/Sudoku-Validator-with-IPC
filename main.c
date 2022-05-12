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

void remove_new_line(char *str);

void remove_space(char *str);

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

    int fd1, fd2, fd3, fd4;

    mkfifo(MYFIFO1, 0666);
    mkfifo(MYFIFO2, 0666);
    mkfifo(MYFIFO3, 0666);

    pid_t decoder, sotoon, satr, mostatil;
    // printf("%s\n", string);

    decoder = fork();
    if (decoder == 0)
    {
        char *args[] = {"./decoder", NULL};
        execvp(args[0], args);
    }
    sotoon = fork();
    if (sotoon == 0)
    {
        char *args[] = {"./sotoon", NULL};
        execvp(args[0], args);
    }

    fd1 = open(MYFIFO1, O_WRONLY);
    write(fd1, string, strlen(string) + 1);
    close(fd1);

    char str[20];
    fd2 = open(MYFIFO3, O_RDONLY);
    int index = read(fd2, str, sizeof(str));
    str[index] = '\0';
    close(fd2);

    printf("%s\n", str);
    return 0;
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