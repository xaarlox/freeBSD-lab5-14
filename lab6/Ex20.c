#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void buffer_overflow() {
    char *buf = malloc(10);
    buf[10] = 'x'; //heap-buffer-overflow
    free(buf);
}

void use_after_free() {
    char *buf = malloc(10);
    free(buf);
    buf[0] = 'y'; //heap-use-after-free
}

void heap_leak() {
    char *buf = malloc(100); //memory leak
    strcpy(buf, "memory leak");
    //free(buf);
}

int main() {
    buffer_overflow();
    use_after_free();
    heap_leak();
    return 0;
}