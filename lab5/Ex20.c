#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096

void* allocate_memory(size_t size) {
    void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANON, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void write_message(void* mem) {
    sprintf((char*)mem, "Temporary message\n");
}

void run_child() {
    _exit(0);
}

void run_parent() {
    wait(NULL);
    exit(0);
}

int main() {
    void* mem = allocate_memory(PAGE_SIZE);
    write_message(mem);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        munmap(mem, PAGE_SIZE);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        run_child();
    } else {
        run_parent();
    }
    return 0;
}