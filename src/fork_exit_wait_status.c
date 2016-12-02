#include <stdio.h>    // printf()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>   // fork(), getpid(), getppid(), sleep()
#include <sys/wait.h> // WIFEXITED, WEXITSTATUS

#define SLEEP             5
#define CHILD_EXIT_STATUS 127

int main(void) {

  pid_t pid;
  int status;

  printf("P ==> My PID is <%ld>\n", (long) getpid());

  // Parent uses the fork() system call to create a child process.

  switch(pid=fork()) {

  case -1: // ERROR

    perror("Could not create child process.");

    exit(EXIT_FAILURE);

  case 0: // CHILD PROCESS

    printf("   C ==> Hello!\n");
    printf("   C ==> My PID is <%ld>\n", (long) getpid());
    printf("   C ==> My parents PID is <%ld>\n", (long) getppid());
    printf("   C ==> I will now sleep for %d seconds ...\n", SLEEP);
    sleep(SLEEP);
    printf("   C ==> Godbye, I´ll terminate with exit status %d!\n", CHILD_EXIT_STATUS);

    exit(CHILD_EXIT_STATUS);

  default: // PARENT PROCESS

    printf("P ==> Child got PID <%ld>\n", (long) pid);
    printf("P ==> Waiting for my child to terminate ...\n");
    
    pid = wait(&status);

    printf("P ==> My child <%ld> terminated :(\n", (long) pid);

    if (WIFEXITED(status))
      printf("P ==> Child terminated with exit code %d\n", WEXITSTATUS(status));
    else
      printf("P ==> Child did not terminate with exit\n");
  }

  printf("P ==> Godbye!\n");

  exit(EXIT_SUCCESS);
}
