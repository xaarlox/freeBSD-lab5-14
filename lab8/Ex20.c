#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *filename = "test_shared.txt";
    int fd_init = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_init == -1) {
        perror("open init");
        exit(1);
    }
    const char *initial_data = "ABCDEFGHIJ";
    write(fd_init, initial_data, strlen(initial_data));
    close(fd_init);
    int fd1 = open(filename, O_RDONLY);
    int fd2 = open(filename, O_RDONLY);
    if (fd1 == -1 || fd2 == -1) {
        perror("open");
        exit(1);
    }
    char buf[2] = {0};
    read(fd1, buf, 1);
    printf("fd1 read: %s\n", buf);
    read(fd2, buf, 1);
    printf("fd2 read: %s\n", buf);
    read(fd1, buf, 1);
    printf("fd1 read again: %s\n", buf);
    close(fd1);
    close(fd2);
    return 0;
}