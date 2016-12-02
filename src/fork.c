#include <stdio.h>  // printf(), perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h> // fork()

int main(void) {

  pid_t pid;

  printf("P ==> My PID is <%ld>\n", (long) getpid());

  switch (pid = fork()) {

  case -1:

    // On error fork() returns -1.

    perror("fork() failed");
    exit(EXIT_FAILURE);

  case 0:

    // On success fork() returns 0 in the child.

    printf("   C ==> Hello!\n");
    printf("   C ==> My PID is <%ld>\n", (long) getpid());
    printf("   C ==> Goodbye!\n");

    exit(EXIT_SUCCESS);

  default:

    // On success fork() returns the pid of the child to the parent.

    printf("P ==> My child got PID <%ld>\n", (long) pid);
    printf("P ==> Godbye!\n");

    exit(EXIT_SUCCESS);
  }
}
