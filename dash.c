/* Angeera Naser
   CS 4348.003 -- Operating Systems
   25 August 2018 */

/* Create a shell in C.
   Do not implement built-in commands.
   Implement commands "exit", "cd", and "path".
   Implement "interactive" and "batch" modes. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error() {
  printf("Something has gone horribly wrong.\n");
}

void checkExit(char *line) {
  printf("%s", line);
  //printf("%p\n", (void *) &line);
  access("bin/ls", X_OK);
}

int main (int argc, char **argv) {

  if (argc == 1) {
    printf("I am in interactive mode!\n");
    
    while(1) { // continually prints a prompt to screen, takes in arg
      printf("dash > ");
      char *line = NULL;
      size_t size;
      if (getline(&line, &size, stdin) == -1) {
	printf("No line -- try again.");
      } else {
	int x = (line == 'exit');
	printf("%s", line);
	printf("%d\n", x);
	//checkExit(line);
      }
    }
  } else if (argc == 2) {
    printf("I am in batch mode! Your argument is: %s\n", argv[1]);    
  } else {
    error();
  }

  return 0;
}
