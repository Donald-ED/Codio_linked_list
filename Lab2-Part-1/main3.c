#include <stdio.h>
#include <stdlib.h>     // For exit(), srand(), rand()
#include <unistd.h>     // For fork(), getpid(), getppid(), sleep()
#include <sys/types.h>  // For pid_t
#include <sys/wait.h>   // For wait()

#define MAX_ITERATIONS 30
#define MAX_SLEEP_TIME 10

void child_process();   // Child process prototype
void parent_process(pid_t, pid_t);  // Parent process prototype

int main(void) {
    pid_t pid1, pid2;

    // Seed the random number generator
    srandom(time(NULL));

    // Create first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Failed to fork first child");
        exit(1);
    } else if (pid1 == 0) {
        // In first child process
        child_process();
        exit(0);
    }

    // Create second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("Failed to fork second child");
        exit(1);
    } else if (pid2 == 0) {
        // In second child process
        child_process();
        exit(0);
    }

    // In parent process
    parent_process(pid1, pid2);

    return 0;
}

void child_process() {
    pid_t pid = getpid();
    pid_t ppid = getppid();

    // Random number of iterations (1 to 30)
    int iterations = (random() % MAX_ITERATIONS) + 1;

    for (int i = 0; i < iterations; i++) {
        printf("Child Pid: %d is going to sleep!\n", pid);

        // Sleep for a random time (1 to 10 seconds)
        int sleep_time = (random() % MAX_SLEEP_TIME) + 1;
        sleep(sleep_time);

        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", pid, ppid);
    }
}

void parent_process(pid_t pid1, pid_t pid2) {
    int status;
    pid_t child_pid;

    // Wait for first child to terminate
    child_pid = wait(&status);
    printf("Child Pid: %d has completed\n", child_pid);

    // Wait for second child to terminate
    child_pid = wait(&status);
    printf("Child Pid: %d has completed\n", child_pid);
}
