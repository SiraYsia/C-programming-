

You should start by creating a loop that reads lines and displays them. Then you
should begin to process each command (starting with the exit and cd commands).
You are free to process each line any way you want, however, reading a whole
line using fgets and then processing the line using sscanf could make things
simpler. Keep in mind that if sscanf cannot read a value into a string variable,
it will not change the variable. This could help you identify when a command has
an argument or not.

*/
#define MAX_LEN 1024
#include <err.h> /*err*/
#include <stdio.h>
#include <stdlib.h> /*fgets*/
#include <string.h> /*strcmp*/
#include <sys/types.h>
#include <sys/wait.h>/*wait*/
#include <sysexits.h>/*EX_OSERR*/
#include <unistd.h>  /*chdir*/

int main() {
  pid_t pid;

  char line[MAX_LEN + 1];
  const char *command = NULL;
  char cmd[MAX_LEN + 1]; /*argv[0]*/

  char directory[MAX_LEN + 1]; /*argv[1]*/
  char buffer[MAX_LEN + 1];
  char *argv[3] = {NULL, NULL,
                   NULL}; /*a vector list for the excevp - later use*/

  int read = 0;   /*total values read from sscanf*/
  int status = 0; /*chdir*/

  /*creating a loop that reads line from standard input*/
  /*getting the command from the user*/
  /*returns a pointer to the inputed command*/

  /*getting user input*/
  command = fgets(line, MAX_LEN + 1, stdin);
  printf("shell_jr: ");
  fflush(stdout);

  /*while the line read isn't NULL*/
  while (command != NULL) {
    /*processing the line*/
    /*process the input, (cmd, directory, buffer)*/
    read = sscanf(command, "%s %s %s", cmd, directory, buffer);

    /*assigning the command, directory and buffer to argv[index]*/
    if (read >= 1) {
      /*the first index is always the command*/
      argv[0] = cmd;
    }
    /*  ex:cd /tmp -> tmp is the directory and cd is the cmd*/
    if (read == 2) {
      argv[1] = directory;
    } /*ignoring for anything greater than 2*/

    /*if command match */
    if (!strcmp(cmd, "exit")) {
      printf("See you\n");
      fflush(stdout);
      exit(0);
    } else if (!strcmp(cmd, "hastalavista")) {
      printf("See you\n");
      fflush(stdout);
      exit(0);
    } else if (!strcmp(cmd, "cd")) {
      /*changing the current working directory to the argv[1] directory*/
      /*and should return either -1(fails) or 0(succeds)*/
      status = chdir(argv[1]);
      if (status == -1) {
        printf("Cannot change to directory %s\n", directory);
        fflush(stdout);
      }
    }
    /*for any other commands, fork */
    else {
      /*pwd -> outputs the path took starting from home to current dire
       which is the absolute path*/
      if ((pid = fork()) < 0) {
        /*failed*/
        err(EX_OSERR, "fork error");
      } else if (pid > 0) {
        /*waits till the child process is terminated*/
        wait(NULL);
      } else { /*child*/
        /*executing the command. pwd. basically jumpng to it (my analogy)*/
        execvp(argv[0], argv);
        /*if it continues from here its only because execvp failed*/
        printf("Failed to execute %s", command);
        fflush(stdout);
        exit(EX_OSERR);
      }
    }
    /*when done excuting with one command, reset argv[0] */
    strcpy(argv[0], "");
    if (read == 2) {
      strcpy(argv[1], "");
    }
    command = fgets(line, 1024, stdin);
    /*prompt must appear again*/
    printf("shell_jr: ");
    fflush(stdout);
  }
  return 0;
}
