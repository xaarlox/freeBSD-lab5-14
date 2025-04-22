#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096

void* allocate_shared_memory(size_t size) {
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void deallocate_shared_memory(void* ptr, size_t size) {
    if (munmap(ptr, size) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }
}

void run_child_process(void* mem) {
    printf("Child says: %s", (char*)mem);
    _exit(0);
}

void run_parent_process(pid_t child_pid, void* mem) {
    waitpid(child_pid, NULL, 0);
    printf("Parent says: %s", (char*)mem);
    deallocate_shared_memory(mem, PAGE_SIZE);
}

int main(void) {
    void* mem = allocate_shared_memory(PAGE_SIZE);
    strcpy((char*)mem, "Hello from mmap!\n");
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        deallocate_shared_memory(mem, PAGE_SIZE);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        run_child_process(mem);
    } else {
        run_parent_process(pid, mem);
    }
    return 0;
}