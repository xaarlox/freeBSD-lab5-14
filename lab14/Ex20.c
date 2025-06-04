#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define TIMEOUT_MINUTES 1
#define TIMEOUT_SECONDS (TIMEOUT_MINUTES * 60)

timer_t timerid;

void logout_handler(int sig, siginfo_t *si, void *uc) {
    (void)sig; (void)si; (void)uc;
    printf("\n[!] No activity for %d minute(s). Logging out.\n", TIMEOUT_MINUTES);
    exit(0);
}

void reset_timer() {
    struct itimerspec its = {
        .it_value = { .tv_sec = TIMEOUT_SECONDS, .tv_nsec = 0 },
        .it_interval = { .tv_sec = 0, .tv_nsec = 0 } //Non-periodic
    };
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(1);
    }
}

int main() {
    struct sigaction sa = {0};
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = logout_handler;
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    struct sigevent sev = {0};
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(1);
    }
    reset_timer(); //Starting the timer for the first time
    char line[256];
    while (1) {
        printf("cli> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n[!] EOF received. Exiting.\n");
            break;
        }
        line[strcspn(line, "\n")] = '\0'; //Remove '\n'
        if (strcmp(line, "exit") == 0)
            break;
        printf("You typed: %s\n", line);
        reset_timer(); //Activity - timer restart
    }
    timer_delete(timerid);
    return 0;
}
