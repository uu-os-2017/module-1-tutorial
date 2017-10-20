/* Description:
 *   A small example program demonstrating some of the pitfalls when
 *   using fork(). A number of child processes is created. Both parent
 *   and children generates one random number each using the
 *   rand() function.
 * Author:
 *   Karl Marklund <karl.marklund@it.uu.se>
 * History:
 *   2011-01-22   1st version to use in the Operating Systems 1
 *                course (1DT044) at Uppsala University.
 */

#include <unistd.h>   // fork(), getpid(), getppid(), sleep()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h>		// NULL
#include <time.h>     // time()
#include <string.h>   // strcmp()

#define NUM_OF_CHILDREN_DEFAULT 5
#define MAX_RAND_NR_DEFAULT     99

#define RAND() rand() % (Max+1)
#define SEED() srand((unsigned) time(NULL) ^ getpid())

void help() {
  printf("\nUsage: random [n] [max]\n");
  printf("   n\tcreate <n> child processes (default %d).\n", NUM_OF_CHILDREN_DEFAULT);
  printf("   max\tboth parent and children generates one random\n");
  printf("   \tnumber each between 0 and <max> inclusive (default %d).\n", MAX_RAND_NR_DEFAULT);
  printf("   -h\tview this help message.\n\n");
  exit(EXIT_FAILURE);
}

void setup(int argc, char** argv, int* N, int* Max) {

  // Are there any valid arguments?

  if ( argc >1 &&  strcmp(argv[1], "-h") == 0 ) help();
  if ( argc >1 && !strtol(argv[1], NULL, 10)  ) help();
  if ( argc >2 && !strtol(argv[2], NULL, 10)  ) help();

  // If there are valid arguments, use these. Otherwise, use defaults.

  *N   = (argc > 1) ? atoi(argv[1]) : NUM_OF_CHILDREN_DEFAULT;
  *Max = (argc > 2) ? atoi(argv[2]) : MAX_RAND_NR_DEFAULT;

}


int main(int argc, char** argv) {

  int   N;   // Number of child processes.
  int   Max; // Generate random numbers between 0 and Max (inclusive).
  int   i;   // Loop index.
  pid_t pid; // Process ID used to check result of fork().

  // Use command line arguments to initialize N and M or use defaults. 

  setup(argc, argv, &N, &Max);

  // Seed the PRNG.

  srand(time(NULL) ^ getpid());

  printf("P <%ld> My random number is %d\n", (long) getpid(), RAND());

  // Use fork() to create N child processes.

  for (i=0; i<N; i++) {

    switch(pid=fork()) {

    case -1:

      perror("Could not create child process.");
      exit(EXIT_FAILURE);

    case 0:

      // The child goes here.

      printf("C <%ld> My random number is %d\n", (long) getpid(), RAND());

      exit(EXIT_SUCCESS);

    } // end-swithc
  } // end-if

  // The parent continues here.

  // Parent waits for all children to terminate.

  for (i=0;i<N;i++) wait(NULL);

  exit(EXIT_SUCCESS);
}
