#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define CYCLE_TIME_MS 100

void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <CPU_percent>\n", argv[0]);
        return 1;
    }
    pid_t pid = atoi(argv[1]);
    int cpu_percent = atoi(argv[2]);
    if (cpu_percent < 1 || cpu_percent > 100) {
        fprintf(stderr, "CPU_percent must be between 1 and 100\n");
        return 1;
    }
    printf("Throttling process %d to %d%% CPU usage...\n", pid, cpu_percent);
    int on_time = CYCLE_TIME_MS * cpu_percent / 100;
    int off_time = CYCLE_TIME_MS - on_time;
    int total_time_ms = 10000;
    int elapsed = 0;
    while (elapsed < total_time_ms) {	
	    kill(pid, SIGCONT);
	    sleep_ms(on_time);
	    elapsed += on_time;
	    
	    kill(pid, SIGSTOP);
	    sleep_ms(off_time);
	    elapsed += off_time;
    }
    return 0;
}
