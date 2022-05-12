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

    FILE *fdecoder = fopen("decoder.txt", "a");

    fprintf(fdecoder, "%s\n", str);
    fclose(fdecoder);

    // Send the result through pipe to finder
    fd2 = open(MYFIFO2, O_WRONLY);
    write(fd2, str, strlen(str) + 1);
    close(fd2);
}
