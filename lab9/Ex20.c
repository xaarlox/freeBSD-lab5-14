#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main() {
    printf("Before:\n");
    printf("UID = %d\n", getuid());
    printf("USER = %s\n", getenv("USER"));
    printf("HOME = %s\n", getenv("HOME"));
    setenv("USER", "bob", 1);
    setenv("HOME", "/home/bob", 1);
    setenv("SHELL", "/bin/sh", 1);

    printf("\nAfter:\n");
    printf("UID = %d\n", getuid());
    printf("USER = %s\n", getenv("USER"));
    printf("HOME = %s\n", getenv("HOME"));

    execlp("sh", "sh", "-c", "echo Hello from $USER, home $HOME", (char *)NULL);

    perror("execlp");
    return 1;
}
