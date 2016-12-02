#include <signal.h> // signal()
#include <stdio.h>  // printf()
#include <unistd.h> // getpid(), pause()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

#define N 3 // Number of child processes

static volatile int PARENT_SIGNAL = 0;
static volatile int CHILD_SIGNAL = 0;

void parent_signal_handler(int sig) {
  PARENT_SIGNAL = sig;
}

void child_signal_handler(int sig) {
  CHILD_SIGNAL = sig;
}

void parent();
void child();

int main(){

  pid_t pid;

  for (int i = 0; i < N; i++) {

    switch (pid = fork()) {

    case -1:
      perror("Error");
      exit(EXIT_FAILURE);

    case 0:
      child();
      exit(EXIT_SUCCESS);
    }
  }

  parent();

  exit(EXIT_SUCCESS);
}

void parent() {

  pid_t pid;

  // Install the same signal handler for SIGINT and SIGUSR1.

  signal(SIGTERM, parent_signal_handler);
  signal(SIGUSR1, parent_signal_handler);

  printf("P <%ld> Hello! I will now spin and wait for the SIGTERM or SIGUSR1 signal ...\n",
         (long) getpid());

  for (int i = 0; i < N; i++) {

    // Block spinning until SIGTERM och SIGUSR1 is received.

    while (PARENT_SIGNAL == 0);

    // Take action on the received signal.

    switch (PARENT_SIGNAL) {

    case SIGUSR1:

      printf("P <%ld> SIGUSR1 received, now waiting for one of my children to terminate ...\n",
             (long) getpid());

      pid = wait(NULL);

      printf("P <%ld> My child <%ld> terminated\n", (long) getpid(), (long) pid);

      // To catch future SIGUSR1 signals, the SIGUSR1 handler must be re-installed.
      signal(SIGUSR1, parent_signal_handler);

      break;

    case SIGTERM:

      printf("P <%ld> SIGTERM received, I will now terminate\n", (long) getpid());
      exit(EXIT_SUCCESS);

    }

    PARENT_SIGNAL = 0;

  }
}

void child() {

  printf("C <%ld> Hello! I will now spin and wait for the SIGTERM signal ...\n",
         (long) getpid());

  signal(SIGTERM, child_signal_handler);

  // Suspend execution until receiving a signal.
  pause();

  if (CHILD_SIGNAL == SIGTERM) {
    printf("C <%ld> SIGTERM received, goodbye!\n", (long) getpid());

    exit(EXIT_SUCCESS);
  }
}
