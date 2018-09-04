/* Angeera Naser
   CS 4348.003 -- Operating Systems
   25 August 2018

   Create a shell in C.
   Do not implement built-in commands.
   Implement commands "exit", "cd", and "path".
   Implement "interactive" and "batch" modes.
   Implement redirection.
   Implement parallel processes. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

void error() {
  printf("Something has gone horribly wrong.\n");
}

void execute_impl (int command, char* arg[][5]) {
  int i;
  int arg_num = -1;

  for (i=0; i<5; i++) {
    if (arg[0][i] != '\0') {
      arg_num++;
    }
  }

  printf("Number of arguments: %d\n", arg_num);

  switch(command) {
    case 0:
      if (arg_num < 0 || arg_num > 0) { 
	error(); 
      } else {
	exit(0);
      }
      break;
    case 1:
      if (arg_num < 1 || arg_num > 1) {
	error();
      } else {
	printf("Change directory to: %s\n", arg[0][1]);
      }
      break;
    case 2:
      printf("Change path to: %s\n", arg[0][1]);
  }

}

void parse_arg(char* command) {

  int i, j, k;
  int implemented = 0;
  char *str1, *str2, *token, *subtoken, *saveptr1, *saveptr2;
  char *commands[20][5];
  const char *impl[3] = {"exit", "cd", "path"};

  for (i=0; i<20; i++) {
    for (j=0; j<5; j++) {
	commands[i][j] = '\0';
      }
  }

  for (i = 0, str1 = command; ; i++, str1 = NULL) {
    token, subtoken, saveptr1, saveptr2 = NULL;
    implemented = 0;
    token = strtok_r(str1, "&", &saveptr1);
    if (token == NULL) { break; }
    commands[i][0] = token;
    printf("%d: %s\n", i, token);

    for (j = 0, str2 = token; ; j++, str2 = NULL) {
      subtoken = strtok_r(str2, " ", &saveptr2);
      if (subtoken == NULL) { break; }
      commands[i][j] = subtoken;
      printf(" -- %d: %s\n", j, subtoken);
    }
  }

  for (k=0; k<3; k++) {
    if (strcmp(impl[k], commands[0][0]) == 0) {
      execute_impl(k, commands);
      implemented = 1;
      break;
    }
  }



  //execute_builtin(commands);
  printf("Implemented: %d\n", implemented);

  for (i=0; i<20; i++) {
    for (j=0; j<5; j++) {
      printf("%s ", commands[i][j]);
    }
    printf("\n");
  }

}

int interactive_mode() {

  printf("Interactive mode.\n");
  char *command = NULL;
  size_t size;

  while(1) {
    command = NULL;
    printf("dash> ");
    getline(&command, &size, stdin);
    command[strcspn(command, "\n")] = '\0';
    printf("Your command was: %s\n", command);
    parse_arg(command);
  }

}

int batch_mode(char* command) {

  printf("Batch mode. Argument is: %s\n", command);

}

int main (int argc, char **argv) {

  if (argc == 1) {
    interactive_mode();
  } else if (argc == 2) {
    batch_mode(argv[1]);
  } else {
    error();
  }

  return 0;

}
