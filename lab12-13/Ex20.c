#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <execinfo.h>
#include <unistd.h>

static sigjmp_buf safe_point;

void segv_handler(int sig, siginfo_t *si, void *unused) {
    (void)sig;
    (void)si;
    (void)unused;
    //Demonstration of stack analysis
    void *buffer[10];
    int nptrs = backtrace(buffer, 10);
    fprintf(stderr, "\n[!] SIGSEGV caught. Stack trace:\n");
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);

    fprintf(stderr, "[*] Attempting to recover to a safe point...\n");
    siglongjmp(safe_point, 1);
}

void cause_segfault() {
    int *ptr = NULL;
    *ptr = 42; //SIGSEGV here
}

int main() {
    //Setting up a signal handler
    struct sigaction sa;
    sa.sa_sigaction = segv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
    if (sigsetjmp(safe_point, 1) == 0) {
        printf("==> Safe point set. Causing a segfault...\n");
        cause_segfault();
    } else {
        printf("==> Successfully recovered from segmentation fault!\n");
    }
    printf("==> Program continues after recovery.\n");
    return 0;
}
