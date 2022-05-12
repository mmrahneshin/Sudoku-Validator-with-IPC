 fd2 = open(MYFIFO2, O_WRONLY);
    write(fd2, str, strlen(str) + 1);
    close(fd2);