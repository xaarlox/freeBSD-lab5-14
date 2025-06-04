#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_DEPTH 5

void create_process_tree(int level) {
    if (level >= MAX_DEPTH) {
        printf("Leaf process at depth %d with PID %d\n", level, getpid());
        return;
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        //Child process
        printf("Child at depth %d with PID %d, parent PID %d\n", level, getpid(), getppid());
        create_process_tree(level + 1);
    } else {
        //Parent process
        printf("Parent at depth %d with PID %d created child %d\n", level, getpid(), pid);
        //We don't wait for descendants - each process is responsible only for its own child
        sleep(1); //Give time to the child process
        printf("Parent at depth %d with PID %d exiting\n", level, getpid());
        exit(0);
    }
}

int main() {
    create_process_tree(0);
    return 0;
}
