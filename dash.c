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
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

char path[512] = "/bin/";

void error() { // The only error message needed in this program.
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void execute_command(char* arg[][10]) { // Execute system (non-built-in) commands.
  int i, j, k, out, sys_out, sys_err;
  char execpath[20];
  char command[30];
  char *redirect[10];
  char *token;

  // Concatenate commands to later tokenize for file redirection operator.
  for (i=0; i<10; i++) {
    strcpy(command, "\0");
    if (arg[i][0] != '\0') {
      for (j=0; j<10; j++) {
	if (arg[i][j] != '\0') {
	  strcat(command, arg[i][j]);
	  strcat(command, " ");
	}
      }

      token = strtok(command, ">");
      redirect[0] = token;
      redirect[1] = '\0';
      
      token = strtok(NULL, "> ");
      if (token != NULL) {
	redirect[1] = token;
      }
      
      token = strtok(NULL, "> ");
      if (token != NULL) {
	error();
      }

      // Start handling file redirection.
      if (redirect[1] != NULL) {
	out = open(redirect[1], O_RDWR|O_CREAT|O_TRUNC, 0600);
	if (-1 == out) { error(); }
	sys_out = dup(fileno(stdout));
	sys_err = dup(fileno(stderr));
	if (-1 == dup2(out, fileno(stdout))) { error(); }
	if (-1 == dup2(out, fileno(stderr))) { error(); }
      }

      // Execute commands; check for & operator and execute in parallel if present.
      char *p = strtok(path, " \t");
      while(p != NULL) {

	strcpy(execpath, p);
	strcat(execpath, arg[i][0]);

	if(access(execpath, X_OK) == 0) {
	  int rc = fork();
	  if (rc < 0) {
	    exit(1);
	  } else if (rc == 0) {
	    char* temp[10];
	    j=0;
	    token = strtok(redirect[0], " \t");
	    while (token != NULL) {
	      temp[j++] = token;
	      token = strtok(NULL, " \t");
	    }
	    execv(execpath, temp);
	  } else {}
	  strcpy(p, path);
	  break;
	} else {
	  error();
	}
	p = strtok(NULL, " \t");
      }

      // Finish handling file redirection.
      if (redirect[1] != NULL) {
	fflush(stdout);
	fflush(stderr);
	close(out);

	dup2(sys_out, fileno(stdout));
	dup2(sys_err, fileno(stderr));

	close(sys_out);
	close(sys_err);
      }     
    }
  }

  // Wait for all child processes to finish before proceeding.
  for (i=0; i<10; i++) {
    wait(NULL);
  }

}

// Execute built-in commands: exit, change directory, and path.
void execute_builtin (int command, char* arg[][10]) {
  int i;
  int arg_num = -1;
  char p[512];

  for (i=0; i<10; i++) {
    if (arg[0][i] != '\0') {
      arg_num++;
    }
  }

  switch(command) {
  case 0: // Exit.
      if (arg_num < 0 || arg_num > 0) { 
	error(); 
      } else {
	exit(0);
      }
      break;
  case 1: // Change directory.
      if (arg_num < 1 || arg_num > 1) {
	error();
      } else {
	chdir(arg[0][1]);
      }
      break;
  case 2: // Path.
      strcpy(p, "\0");
      for (i=1; i<=arg_num; i++) {
	strcat(p, arg[0][i]);
	strcat(p, "/");
	strcat(p, " ");
      }
      strcpy(path, p);
  }

}

// Find out whether command is built-in or system, and execute accordingly.
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

// Tokenize by & first to handle parallel commands.
// Then tokenize by whitespace to separate arguments from command.
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
    commands[i][0] =  token;

    for (j = 0, str2 = token; ; j++, str2 = NULL) {
      subtoken = strtok_r(str2, " \t", &saveptr2);
      if (subtoken == NULL) { break; }
      commands[i][j] = subtoken;
    }
  }

  // Pass resulting array off to handling function.
  handle_commands(commands);

}

// Interactive mode: No other arguments in argv[], runs "dash> " prompt.
int interactive_mode() {

  char *command = NULL;
  size_t size;

  while(1) {
    command = NULL;
    printf("dash> ");
    getline(&command, &size, stdin);
    command[strcspn(command, "\n")] = '\0';
    parse_arg(command);
  }

}

// Batch mode: One argument (filename) in argv[].
// Parses the file line by line and offloads contents to handler.
int batch_mode(char* filename) {

  FILE *fp;
  char *command = NULL;
  size_t size;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    exit(1);
  }

  while((read = getline(&command, &size, fp)) != -1) {
    command[strcspn(command, "\n")] = '\0';
    parse_arg(command);
  }

  fclose(fp);

  if (command) {
    free(command);
  }

  exit(0);

}

// Main function: Determines whether shell is to be run in interactive mode or batch mode.
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
