/* timer.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t alarm_count = 0; // Count of SIGALRM received
volatile sig_atomic_t running = 1;     // Flag to keep the program running

time_t start_time; // Variable to store the start time

void alarm_handler(int signum)
{ // SIGALRM handler
  alarm_count++; // Increment the alarm count
  // Schedule the next alarm after 1 second
  alarm(1);
}

void sigint_handler(int signum)
{ // SIGINT handler
  running = 0; // Set running to 0 to exit the main loop
}

int main(int argc, char * argv[])
{
  // Register signal handlers
  signal(SIGALRM, alarm_handler); // Handle SIGALRM
  signal(SIGINT, sigint_handler); // Handle SIGINT (CTRL-C)

  // Get the start time
  start_time = time(NULL);

  // Schedule the first alarm after 1 second
  alarm(1);

  printf("Timer started. Press CTRL-C to stop.\n");

  while (running)
  {
    // Main loop does nothing, just waits for signals
    pause(); // Suspend the process until a signal is received
  }

  // Calculate the total execution time
  time_t end_time = time(NULL);
  double total_time = difftime(end_time, start_time);

  printf("\nProgram executed for %.0f seconds.\n", total_time);
  printf("Number of SIGALRM signals received: %d\n", alarm_count);

  return 0;
}
