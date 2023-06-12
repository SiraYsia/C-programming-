

#include "executor.h"
#include "command.h"
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>

/*static void print_tree(struct tree *t);*/

int execute(struct tree *t) {
  /*print_tree(t);*/
  /*lets handle the case where there is no cojuction first*/
  /*access the conjuction from command.h */
  if (t->conjunction == NONE) {
    /*first case is cd*/
    if (!strcmp(t->argv[0], "cd")) {
      /*if there is no second argument after cd*/
      if (t->argv[1] == NULL) {
        /*change the directory to home*/
        /* cd alone = home
         get the environment of Home and replace the directory with it*/
        if (chdir(getenv("HOME")) != 0) {
          perror(getenv("HOME"));
          fflush(stdout);
          return 1;
        }
        /*if there is a second argument(directory) then change the cwd to that
         * one*/
      } else {
        /*if chdir is unsucessful*/
        if (chdir(t->argv[1]) != 0) {
          perror(t->argv[1]);
          fflush(stdout);
          return 1;
        }
        /*return 0 because successful*/
        return 0;
      }
      /*terminating*/
    } else if (!strcmp(t->argv[0], "exit")) {
      exit(0);
    } else { /*for any other command*/
      pid_t pid;
      /*i belive fd may be reused once closed so no need for fd2 but just to be
       * safe*/
      int fd, fd2; /*input, output and dup */ /*input, output and dup */

      /*handling error with fork */
      if ((pid = fork()) < 0) {
        perror("fork");
        exit(EX_OSERR);

      } else if (pid == 0) { /*child */
                             /*checking input redirection*/
        if (t->input) {
          /*returns a file descriptor if sucess*/
          if ((fd = open(t->input, O_RDONLY)) < 0) {
            perror("dup2");
            exit(EX_OSERR);
          }
          /*redirecting standard input(getting input from the input(file))*/
          if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2");
            exit(EX_OSERR);
          }
          /*if close fails*/
          if (close(fd) != 0) {
            perror("close");
            exit(EX_OSERR);
          }
        }

        /*output redirection is not null*/
        if (t->output) {

          /*returns a file descriptor if sucess*/
          /*If a file is created by output redirection, the file should be
           * created using permissions 0664. */
          if ((fd2 = open(t->output, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0) {
            perror("open");
            exit(EX_OSERR);
          }
          if (dup2(fd2, STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(EX_OSERR);
          }
          /*if close fails*/
          if (close(fd2) != 0) {
            perror("close");
            exit(EX_OSERR);
          }
        }

        /*executig the command */
        /*check for errors and print error to stderr*/
        if (execvp(t->argv[0], t->argv) == -1) {
          /*wouldn't reach here unless it fails */
          /*
  printf("Failed to execute %s/n", t->argv[0]);
  fflush(stderr); why terminal prinitng?
  exit(EX_OSERR);
  */
          fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
          fflush(stdout); /*still*/
          exit(EX_OSERR);
        }

      } else { /*parent*/ /*should just wait for the child
                             and return the exit status */
        int status;
        /*waits till the child process is terminated*/
        wait(&status);
        return WEXITSTATUS(status); /*exit 0 or failure*/
      }
    }

    /**********************AND***********************/
  } else if (t->conjunction == AND) {
    /*executing the left node. execute the right as well if it equals to 0*/
    if (execute(t->left) == 0) {
      return execute(t->right); /*just call execute to executre the rest*/
    }
    return 1;

    /********************PIPE**************************/

  } else if (t->conjunction == PIPE) {
    pid_t pid2;
    int child_pipe2[2];

    /*Handle Ambigious redirect*/
    /* asking the shell to output both to the outputfile and through the pipe.
    Rather than choosing one.*/
    /*ls | wc < lexer.c*/
    /*if there exsists output redirection in the left sub tree*/
    if ((t->left)->output) {
      printf("Ambiguous output redirect.\n");
      fflush(stdout);
      return 1;
    }
    /*same case except now checking that there doesn't exsist input redirection
    in the right. drawing to illustrate the tree on puruple notebook*/
    if ((t->right)->input) {
      printf("Ambiguous output redirect.\n");
      fflush(stdout);
      return 1;
    }

    if (pipe(child_pipe2) < 0) {
      perror("pipe");
    }

    if ((pid2 = fork()) < 0) {
      perror("fork");
      exit(EX_OSERR);
    }
    /*could either make the child do the reading and parent writting
    or could fork 2 times making one of the child execute the reaidng and
    another the writting and the parent could close both end and reap all child
    processes. */

    if (pid2 == 0) {
      if (close(child_pipe2[1]) < 0) {
        perror("close");

      } /*closing the write end*/
      if (dup2(child_pipe2[0], STDIN_FILENO) < 0) {
        perror("dup2");
      } /*read from the child pipe*/
      if (close(child_pipe2[0]) < 0) {
        perror("close");

      }                        /*close read end*/
      exit(execute(t->right)); /*to acess exit staus of the children*/

    } else {
      int pid3;
      if ((pid3 = fork()) < 0) {
        perror("fork");
        exit(EX_OSERR);
      }
      if (pid3 == 0) {
        /*Closing read end again just to be safe*/
        if (close(child_pipe2[0]) < 0) {
          perror("close");
          exit(EX_OSERR);
        }
        /* redirecting output to the child pipe. Writting
                                    to the child pipe*/
        if (dup2(child_pipe2[1], STDOUT_FILENO) < 0) {
          perror("dup2");
        }
        /*close write end*/
        if (close(child_pipe2[1]) < 0) {
          perror("close");
          exit(EX_OSERR);
        }
        exit(execute(t->left)); /*execute the left*/
      } else {
        int status_fc, status_sc;
        if (close(child_pipe2[0]) < 0) {
          perror("close");
          exit(EX_OSERR);
        }
        if (close(child_pipe2[1]) < 0) {
          perror("close");
          exit(EX_OSERR);
        }
        wait(&status_fc);
        wait(&status_sc);
        /*without any fault*/
        if (WIFEXITED(status_fc) && WIFEXITED(status_sc)) {
          /*returns 0 if excited normally*/
          if (!WEXITSTATUS(status_fc) && !WEXITSTATUS(status_sc)) {
            return 0; /*exit(0)*/
          } else {
            return 1;
          }
        }
      }
    }

    /********************SUBSHELL**************************/

  } else if (t->conjunction == SUBSHELL) {
    pid_t pid4;

    if ((pid4 = fork()) < 0) {
      perror("fork");
      exit(EX_OSERR);
    }
    if (pid4 == 0) {
      exit(execute(t->left)); /*execute the subshell in its own process*/
    } else {                  /*parent process*/
      int status_r;
      wait(&status_r);
      if (WIFEXITED(status_r)) {
        /*returns 0 if excited normally*/
        if (!WEXITSTATUS(status_r)) {
          return 0; /*exit(0)*/
        } else {
          return 1;
        }
      }
    }
  }
  return 0;
}

/*static void print_tree(struct tree *t) {
  if (t != NULL) {
    print_tree(t->left);

    if (t->conjunction == NONE) {
      printf("NONE: %s, ", t->argv[0]);
    } else {
      printf("%s, ", conj[t->conjunction]);
    }
    printf("IR: %s, ", t->input);
    printf("OR: %s\n", t->output);

    print_tree(t->right);
  }
}*/
