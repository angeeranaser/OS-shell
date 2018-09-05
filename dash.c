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
#include <sys/wait.h>

void error() {
  printf("Something has gone horribly wrong.\n");
}

void execute_command(char* arg[][10]) {
  int i, j;
  int command_num, arg_num;
  char path[20] = "/bin/";
  char command[30];
  char output[20];
  for (i=0; i<10; i++) {
    strcpy(command, "\0");
    command_num, arg_num = 0;
    if (arg[i][0] != '\0') {
      command_num++;
      for (j=0; j<10; j++) {
	if (arg[i][j] != '\0') {
	  arg_num++;
	  strcat(command, arg[i][j]);
	}
      }

      /*strcat(path, arg[i][0]);
      if (access(path, X_OK) == 0) {
	//printf("Execute process here.\n");
	//printf("Hello world (pid:%d)\n", (int) getpid());
	int rc = fork();
	if (rc < 0) {
	  printf("Fork failed.\n");
	  exit(1);
	} else if (rc == 0) {
	  // printf("Hello! I am child (pid:%d)\n", (int) getpid());
	  execv(path, arg[i]);
	} else {
	  int rc_wait = wait(NULL);
	  //printf("Hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
	}
	} else { error(); }*/
      printf("Command is: %s\n", command);
      printf("Command number %d has %d arguments.\n", command_num, arg_num);
      //      printf("Access: %d\n", access(path, X_OK));
    }
    //printf("Command number %d has %d arguments.\n", command_num, arg_num);
  }
}

void execute_builtin (int command, char* arg[][10]) {
  int i;
  int arg_num = -1;
  char path[30];

  for (i=0; i<10; i++) {
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
	chdir(arg[0][1]);
      }
      break;
    case 2:
      for (i=1; i<=arg_num; i++) {
	strcat(path, arg[0][i]);
	strcat(path, " ");
      }
      printf("Change path to: %s\n", path);
  }

}

int handle_commands(char* commands[][10]) {
  int k, built_in = 0;
  const char *impl[3] = {"exit", "cd", "path"};
  for (k=0; k<3; k++) {
    if (strcmp(impl[k], commands[0][0]) == 0) {
      execute_builtin(k, commands);
      built_in = 1;
      break;
    }
  }

  if (built_in != 1) {
    execute_command(commands);
  }
}

void parse_arg(char* command) {

  int i, j, k;
  int implemented = 0;
  char *str1, *str2, *token, *subtoken, *saveptr1, *saveptr2, *filepath;
  char *commands[10][10];

  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) {
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
      subtoken = strtok_r(str2, " \t", &saveptr2);
      if (subtoken == NULL) { break; }
      commands[i][j] = subtoken;
      printf(" -- %d: %s\n", j, subtoken);
    }
  }

  handle_commands(commands);

  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) {
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
