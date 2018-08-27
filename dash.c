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
#include <string.h>

void error() { // write error message

  printf("Something has gone horribly wrong.\n");

}

void exec_builtin(char line[]) {}

void parse_arg(char line[]) {

  const char *impl[3];
  char result[2][20];
  char *str1, *str2, *token, *subtoken, *saveptr1, *saveptr2;
  char args[2][100];
  int i, j, k;

  impl[0] = "exit";
  impl[1] = "cd";
  impl[2] = "path";

  for (j = 1, str1 = line; ; j++, str1 = NULL) {
    token = strtok_r(str1, "&|", &saveptr1);
    if (token == NULL) { break; }
    printf("%d: %s\n", j, token);
    
    for (i = 0, str2 = token; ; i++, str2 = NULL) {
      subtoken = strtok_r(str2, " ", &saveptr2);
      if (i > 1) { break; }
      if (subtoken == NULL) { break; }
      strcpy(args[i], subtoken);
      printf(" -- %d:  %s\n", i, args[i]);
    }

    for (k = 0; k < 3; k++) {
      if (strcmp(impl[k], token) == 0) { 
	switch(k) {
	  case 0:
	    exit(0);
	    break;
	  case 1:
	    chdir(args[1]);
	    break;
	  case 2:
	    printf("Change path to: %s\n", args[1]);
	    break;
	default:
	    printf("None of these.\n");
	}
      	strcpy(result[0], "Implementable!");
       	strcpy(result[1], args[1]);
	break;
      } else {
	strcpy(result[0], "Nonimplementable.");
	strcpy(result[1], args[1]);
      }
    }
    printf("%s\n", result[0]);
    printf("Arguments: %s\n", result[1]);
  }
  
}

void interactive_mode() {

  printf("I am in interactive mode!\n");
  char *line = NULL;
  size_t size;

  while(1) {
    line = NULL;
    printf("dash> ");
    getline(&line, &size, stdin);
    line[strcspn(line, "\n")] = '\0';
    parse_arg(line);
  }

}

int main (int argc, char **argv) {

  if (argc == 1) {
    interactive_mode();
  } else if (argc == 2) {
    printf("I am in batch mode! Your argument is: %s\n", argv[1]);    
  } else {
    error();
  }

  return 0;

}
