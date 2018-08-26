/* Angeera Naser
   CS 4348.003 -- Operating Systems
   25 August 2018 */

/* Create a shell in C.
   Do not implement built-in commands.
   Implement commands "exit", "cd", and "path".
   Implement "interactive" and "batch" modes. */

/* 1: Run program (no extra arguments = interactive, otherwise batch). 
   2a: If interactive, enter while loop, write prompt, take in input line.
   2b: If batch, open file, read in input line.
   3: Determine whether input function is built-in, implemented, or neither.
   4a: If built-in, access folder. Fork, run execvp process.
   4b: If implemented, run relevant method.
   4c: If neither, return error message.
   5: Repeat for all other lines of input supplied.
   6: Exit(1) upon exit command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error() { // write error message
  printf("Something has gone horribly wrong.\n");
}

void parse_arg(char *line) { // parse argument: determine whether it's built-in or implemented
  printf("%s", line);
  //printf("%p\n", (void *) &line);
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
	//parse_arg(line);
      }
    }
  } else if (argc == 2) {
    printf("I am in batch mode! Your argument is: %s\n", argv[1]);    
  } else {
    error();
  }

  return 0;
}
