/* Yeabsira Tamiru
117857740
*/

#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#define TOTAL_VALID_CHAR 1024

/*function prototype*/
static int generate_command(Document *doc, const char *command,
                            const char *line);
static int read_add_paragraph_after(Document *doc, const char *line);
static int read_print_document(Document *doc, const char *line);
static int read_quit(const char *command);
static int read_append_line(Document *doc, const char *line);
static int read_add_line_after(Document *doc, const char *line);
static int read_remove_line(Document *doc, const char *line);
static int read_load_file(Document *doc, const char *line);
static int read_replace_text(Document *doc, const char *line);
static int read_highlight_text(Document *doc, const char *line);
static int read_remove_text(Document *doc, const char *line);
static char *remove_quotes(char *command, char *result);
static int read_save_document(Document *doc, const char *line);
static int read_reset_document(Document *doc, const char *line);

/*
This functions reads the array of char line
and returns SUCESS if add_paragraph is sucessful and FAILURE otherwise, along
with a failure message.
*/
static int read_add_paragraph_after(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  int para_num;
  int read;

  read = sscanf(line, " %s %d%s", command_output, &para_num, ext);
  /*read should only be 2 because if additional information is provided after
  the PARAGRAPH_NUMBER failed message is printed*/
  if (read == 2 && para_num >= 0) {
    if (add_paragraph_after(doc, para_num) == FAILURE) {
      printf("add_paragraph_after failed\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if print_document is sucessful and FAILURE otherwise, along
with a failure message.
*/
static int read_print_document(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  int read;

  read = sscanf(line, " %s%s", command_output, ext);
  if (read == 1) {
    if (print_document(doc) == FAILURE) {
      printf("print_document failed\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}
/*
This functions reads if either quit or exit is inputed and terminates the
program if any data appears after quit or exit invalid command is genrated.
*/
static int read_quit(const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  int read;

  read = sscanf(line, " %s %s", command_output, ext);
  if (read == 1) {
    return SUCCESS;
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if append_line is sucessful and FAILURE otherwise, along
with a failure message.
*/
static int read_append_line(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char new_line[MAX_STR_SIZE];
  char ext[MAX_STR_SIZE + 1];
  int read, para_num = 0;

  read = sscanf(line, " %s%d%s", command_output, &para_num, ext);
  if (read == 3 && para_num > 0) {
    /*if the * is missing, the command fails */
    if (strchr(line, '*') != NULL) {
      strcpy(new_line, strchr(line, '*') + 1);

      if (append_line(doc, para_num, new_line) == FAILURE) {
        printf("append_line failed\n");
      }
      return SUCCESS;
    }
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if add_line_after is sucessful and FAILURE otherwise, along
with a failure message.
*/
static int read_add_line_after(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char new_line[MAX_STR_SIZE];
  char ext[MAX_STR_SIZE + 1];
  int read, para_num, line_num;

  read = sscanf(line, " %s%d%d%s", command_output, &para_num, &line_num, ext);
  /*finding the first instance of the quote*/

  if (read == 4 && para_num > 0 && line_num >= 0) {
    /*if the * is missing, the command fails */
    if (strchr(line, '*') != NULL) {
      strcpy(new_line, strchr(line, '*') + 1);
      if (add_line_after(doc, para_num, line_num, new_line) == FAILURE) {
        printf("add_line_after failed\n");
      }
      return SUCCESS;
    }
  }
  return FAILURE;
}

/*
This functions reads the array of char line
and returns SUCESS if remove_line is sucessful and FAILURE otherwise, along
with a failure message.
*/
static int read_remove_line(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  int para_num, line_num;
  int read;

  read = sscanf(line, " %s%d%d%s", command_output, &para_num, &line_num, ext);

  /*can't exceed 3 read values also param_num and line_num can't be negative*/
  if (read == 3 && para_num > 0 && line_num > 0) {
    if (remove_line(doc, para_num, line_num) == FAILURE) {
      printf("remove_line failed\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if load_file is sucessful and FAILURE otherwise, along
with a failure message.
*/

static int read_load_file(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char file[MAX_STR_SIZE + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  int read;
  read = sscanf(line, " %s%s%s", command_output, file, ext);

  /*can't exceed 2 or it means there is something else after file name */
  if (read == 2) {
    if (load_file(doc, file) == FAILURE) {
      printf("load_file failed\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if replace_text is sucessful and FAILURE otherwise, along
with a failure message.

The function remove quotes could also be used to assist with this function
*/

static int read_replace_text(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR], *first_quote, *second_quote,
      *third_quote, *fourth_quote;
  char target[MAX_STR_SIZE], replacement[MAX_STR_SIZE];
  char tar[MAX_STR_SIZE + 1], rep[MAX_STR_SIZE + 1];
  int read;

  read = sscanf(line, " %s%s%s", command_output, tar, rep);
  /*the first instance of the quote*/
  first_quote = strstr(line, "\"");

  /*if quotations are missing the command fails so they can't be null*/

  if (read == 3 && first_quote != NULL) {
    /*start from the first quote and let second_quote point to the last quote of
     * the target */

    second_quote = strstr(first_quote + 1, "\"");
    /*first quote of replacement*/
    if (second_quote != NULL) {
      third_quote = strstr(second_quote + 1, "\"");

      /*last quote of replacement*/
      if (third_quote != NULL) {
        fourth_quote = strstr(third_quote + 1, "\"");
        if (fourth_quote != NULL) {
          /*ex: target = CS*/
          strncpy(target, first_quote + 1, second_quote - first_quote);
          /*ex: CS/0*/
          target[second_quote - (first_quote + 1)] = '\0';

          /* copying the replacement without a quote
           and adding a null at the end of the replacement */
          strncpy(replacement, third_quote + 1, fourth_quote - third_quote);
          replacement[fourth_quote - (third_quote + 1)] = '\0';

          if (replace_text(doc, target, replacement) == FAILURE) {
            printf("replace_text failed\n");
          }
          return SUCCESS;
        }
      }
    }
  }
  return FAILURE;
}
/*
This purpose of this function is to remove quotes from a string.
It is used to help with the processing of remove and highlight text functions.
*/
static char *remove_quotes(char *command, char *result) {
  char *first, *second;

  first = strstr(command, "\"");
  if (first != NULL) {
    second = strstr(first + 1, "\"");

    if (second != NULL) {
      strncpy(result, first + 1, second - first);
      result[second - (first + 1)] = '\0';
      return result;
    }
  }
  return NULL;
}
/*
This functions reads the array of char line
and returns SUCESS if highlight_text is sucessful and FAILURE otherwise.
*/
static int read_highlight_text(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR];
  char target[MAX_STR_SIZE + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  char *inst, *final_target;

  sscanf(line, " %s%s", command_output, ext);
  /*finding the instance of the quote*/
  inst = strstr(line, "\"");

  if (inst != NULL) {
    /*removing the quote before calling highlight_text*/
    if ((final_target = remove_quotes(inst, target)) != NULL) {
      highlight_text(doc, final_target);
      return SUCCESS;
    }
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if remove_text is sucessful and FAILURE otherwise.
*/
static int read_remove_text(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR];
  char target[MAX_STR_SIZE + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  char *str, *final_target;

  sscanf(line, " %s%s", command_output, ext);
  str = strstr(line, "\"");

  if (str != NULL) {
    /*removing the quote of the target to be removed and calls remove_text*/
    if ((final_target = remove_quotes(str, target)) != NULL) {
      remove_text(doc, final_target);
      return SUCCESS;
    }
  }
  return FAILURE;
}
/*
This functions reads the array of char line
and returns SUCESS if save_document is sucessful and FAILURE along with a
message otherwise.
*/
static int read_save_document(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char file[MAX_STR_SIZE + 1];
  char ext[TOTAL_VALID_CHAR + 1];
  int read;

  read = sscanf(line, " %s%s%s", command_output, file, ext);

  /*no data should come after FILENAME*/
  if (read == 2) {
    if (save_document(doc, file) == FAILURE) {
      printf("save_document failed\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}
/*
This functions reading the array of char line
and returns SUCESS if reset_document is sucessful and FAILURE along with a
message otherwise.
*/
static int read_reset_document(Document *doc, const char *line) {
  char command_output[TOTAL_VALID_CHAR + 1];
  char ext[TOTAL_VALID_CHAR + 1];

  int read;
  read = sscanf(line, " %s%s", command_output, ext);

  if (read == 1) {
    if (reset_document(doc) == SUCCESS) {
      return SUCCESS;
    }
  }
  return FAILURE;
}

/*
This functions compares the command inputed by the user to the corrosponding
functions. Depedning on the result of the comparasion, if a match wasn't found
it prints "Invalid command." It returns -1 if commanded to either quit or exit
byt the user.
*/

static int generate_command(Document *doc, const char *command,
                            const char *line) {
  int output = FAILURE;
  /*keeps track of how  many times "invalid command" has been printed*/
  int yes = 0;

  /*comparing the strings from the command to check if its valid and
   storing the return*/
  if (strcmp(command, "add_paragraph_after") == 0) {
    output = read_add_paragraph_after(doc, line);
  } else if (strcmp(command, "add_line_after") == 0) {
    output = read_add_line_after(doc, line);
  } else if (strcmp(command, "print_document") == 0) {
    output = read_print_document(doc, line);
  } else if (strcmp(command, "append_line") == 0) {
    output = read_append_line(doc, line);
  } else if (strcmp(command, "remove_line") == 0) {
    output = read_remove_line(doc, line);
  } else if (strcmp(command, "load_file") == 0) {
    output = read_load_file(doc, line);
  } else if (strcmp(command, "replace_text") == 0) {
    output = read_replace_text(doc, line);
  } else if (strcmp(command, "highlight_text") == 0) {
    output = read_highlight_text(doc, line);
  } else if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
    output = read_quit(line);
    if (output == SUCCESS) {
      /*since quit/exit means the program is terminated*/
      return -1;
    }
  } else if (strcmp(command, "remove_text") == 0) {
    output = read_remove_text(doc, line);
  } else if (strcmp(command, "save_document") == 0) {
    output = read_save_document(doc, line);
  } else if (strcmp(command, "reset_document") == 0) {
    output = read_reset_document(doc, line);

  } else {
    printf("Invalid Command\n");
    yes = 1;
  }
  if (output == FAILURE && yes != 1) {
    /*if output fails and the Invalid Command hasn't been printed, do so*/
    printf("Invalid Command\n");
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int read;
  Document doc;
  FILE *input_stream;
  char line[TOTAL_VALID_CHAR + 1];
  char command_output[TOTAL_VALID_CHAR + 1];

  /*input from the standard input*/
  if (argc == 1) {
    input_stream = stdin;
  } else if (argc == 2) {
    input_stream = fopen(argv[1], "r");
    if (input_stream == NULL) {
      fprintf(stderr, "%s cannot be opened\n", argv[1]);
      exit(EX_OSERR);
    }
  } else {

    /*If the command line has more than one argument */
    fprintf(stderr, "Usage: user_interface\n");
    fprintf(stderr, "Usage: user_interface <filename>\n");
    exit(EX_USAGE);
  }
  /*initizlizing a document to start*/
  init_document(&doc, "main_document");
  if (input_stream == stdin) {
    printf("> ");
  }
  while (fgets(line, TOTAL_VALID_CHAR + 1, input_stream) != NULL) {
    /* remove the new line charachter*/
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    read = sscanf(line, " %s", command_output);

    /*the first non-whitespace character is a hash symbol*/
    if (command_output[0] != '#' && read != EOF) {

      if (generate_command(&doc, command_output, line) == -1) {
        fclose(input_stream);
        exit(EXIT_SUCCESS);
      }
    }
    if (input_stream == stdin) {
      printf("> ");
    }
  }
  printf("\n");
  return 0;
}
