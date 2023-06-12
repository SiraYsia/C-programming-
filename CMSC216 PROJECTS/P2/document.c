

#include "document.h"
#include <stdio.h>
#include <string.h>

int init_document(Document *doc, const char *name);
int reset_document(Document *doc);
int print_document(Document *doc);
int add_paragraph_after(Document *doc, int paragraph_number);
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line);
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines);
int append_line(Document *doc, int paragraph_number, const char *new_line);
int remove_line(Document *doc, int paragraph_number, int line_number);
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines);
int replace_text(Document *doc, const char *target, const char *replacement);
void replace_text_Aux2(const char *line, const char *target,
                       const char *replacement, char *replaced);
int highlight_text(Document *doc, const char *target);
int remove_text(Document *doc, const char *target);

/*
The purpose of this function is to intialize the document to be empty(0
paragraphs) and the document's name to the paramerer name. If doc and name are
NULL or the length of the name provided exceeds MAX_STR_Size FAILURE is
returned, otherwise SUCESS is returned.
*/

int init_document(Document *doc, const char *name) {
  /*name must exsist so its length shouldn't be less than 1*/
  if (doc == NULL || name == NULL || strlen(name) < 1 ||
      strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }
  doc->number_of_paragraphs = 0;
  strcpy(doc->name, name);
  return SUCCESS;
}
/*
This function sets the number of paragraphs to 0. FAILURE ois returned if doc is
NULL; otherwise SUCCESS is returned.
*/

int reset_document(Document *doc) {
  if (doc == NULL) {
    return FAILURE;
  }
  /*setting the number of paragraphs to empty(0)*/
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}
/*
This function prints the document’s name, number of paragraphs, followed by the
paragraphs. Each paragraph is separated by a blank line. FAILURE is returned if
doc is NULL; otherwise SUCCESS is returned.
*/
int print_document(Document *doc) {
  /*
  i - an index to go through the paragraph
  j - an index to go through each line at the specified paragraph
  */
  int i, j;

  if (doc != NULL) {
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    /*printing at each line of the paragraph
      paragraph[0].line[0]
      paragraph[0].line[1]

      paragraph[1].line[0]
      paragraph[1].line[1]
      */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
      for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
        printf("%s\n", doc->paragraphs[i].lines[j]);
      }
      /*a line to separate the paragraphs*/
      if (i < doc->number_of_paragraphs - 1) {
        printf("\n");
      }
    }
    return SUCCESS;
  }
  /*if document is NULL*/
  return FAILURE;
}

/*
This function adds a paragraph after the specified paragraph numbers. An
assumption is made about the start number of the paragraph, 1. FAILURE is only
returned if doc is NULL, the document already has the maximum number of
paragraphs allowed or the paragraph_number is larger than the number of
paragraphs available.
*/
int add_paragraph_after(Document *doc, int paragraph_number) {
  Paragraph *paragraph;
  int curr;

  if (doc == NULL) {
    return FAILURE;
  }

  if (doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
      paragraph_number >= MAX_PARAGRAPHS) {
    return FAILURE;
  }
  /* holds total numbers of paragraphs before the addition of a new one */
  curr = doc->number_of_paragraphs;

  /*paragraph number can't equal to or exceed the total number of paragraphs
  since it would exceed the range. Curr + 1 represents the total number of
  paragraphs after the addition of the new paragraph.
  */
  if (paragraph_number < curr + 1) {
    /*number of paragraphs must equal to the paragraph number plus 1*/
    doc->number_of_paragraphs = paragraph_number + 1;
    /*paragraph_is storing the address of the index the new paragraph is going
     * to be*/
    paragraph = &doc->paragraphs[paragraph_number];
    /*setting the lines to 0*/
    paragraph->number_of_lines = 0;
    return SUCCESS;
  }
  return FAILURE;
}
/*
This function adds a new line after the line with the specified line number.
Line numbers start at 1. The function returns FAILURE if doc is NULL, the
paragraph_number exceeds the number of paragraphs available, the paragraph
already has the maximum number of lines allowed, the line_number is larger than
the available number of lines, or new_line is NULL; otherwise, the function
returns SUCCESS.
*/

int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {

  int size;
  /*The paragraph where the line is going to be added*/
  Paragraph *target_paragraph;

  if (doc == NULL) {
    return FAILURE;
  }
  if (doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
      line_number >= MAX_PARAGRAPH_LINES) {
    return FAILURE;
  }
  if (new_line == NULL) {
    return FAILURE;
  }
  /*The length of the new line to be added*/
  size = strlen(new_line);
  if (size > MAX_STR_SIZE) {
    return FAILURE;
  }

  /*target_paragraph is now storing the address of the paragraph at the index
   *provided*/
  target_paragraph = &doc->paragraphs[paragraph_number - 1];

  /*The number of lines must always be less than the maximum allowed*/
  if (target_paragraph->number_of_lines < MAX_PARAGRAPH_LINES) {
    /*Adding a new line to the target_paragraph*/
    target_paragraph->number_of_lines = line_number + 1;
    /*copy the new_line after the line number specified */
    strncpy(target_paragraph->lines[line_number], new_line, size + 1);
    return SUCCESS;
  }
  /*returning failure if the target_paragraph already has the maximum number of
   * lines allowed*/
  return FAILURE;
}

/*
This function returns the number of lines in a paragraph. FAILURE is returned if
doc is NULL, number_of_lines is NULL or paragraph_number is larger than the
number of paragraphs available; otherwise, the function returns SUCCESS.
*/
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
  if (doc == NULL) {
    return FAILURE;
  }
  if (number_of_lines == NULL || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  /*paragraph_number is the index intended + 1 so the number of lines at the
   paragraph is the paragraph_number minus 1*/
  *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
  return SUCCESS;
}
/*
Appends a line to the specified paragraph. FAILURE should be returned if
doc is NULL, the paragraph number is out of boud or the number of lines
exceedes the max paragraph lines allowed. The function returns SUCCESS
if the line is appended.
*/
int append_line(Document *doc, int paragraph_number, const char *new_line) {

  int num_line;
  Paragraph *target_paragraph;

  /*new_line can't be null for this funcion to complete*/
  if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines >=
          MAX_PARAGRAPH_LINES ||
      new_line == NULL) {
    return FAILURE;
  }
  /*num_line storing the total number of lines at the specified paragraph*/
  num_line = doc->paragraphs[paragraph_number - 1].number_of_lines;
  /*target_paragraph pointing to the specidied paragraph*/
  target_paragraph = &doc->paragraphs[paragraph_number - 1];
  /*copying the new line at the end of the target_paragraph*/
  strcpy(target_paragraph->lines[num_line], new_line);
  /*incrmenting the number of lines by 1*/
  target_paragraph->number_of_lines++;
  return SUCCESS;
}

/*
This function removes the specified line from the paragraph. The function
returns FAILURE if doc is NULL, the paragraph_number exceeds the number of
paragraphs available, or line_number is larger than the number of lines in the
paragraph; otherwise the function returns SUCCESS.*/

int remove_line(Document *doc, int paragraph_number, int line_number) {
  int i, lines_in_para;

  if (doc == NULL) {
    return FAILURE;
  }
  if (paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  if (line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
    return FAILURE;
  }
  /*lines_in_para storing the total  number of lines in the paragraph based on
   * the paragraph number specified*/
  lines_in_para = doc->paragraphs[paragraph_number - 1].number_of_lines;

  /*i is acting as the index of the line number. */
  for (i = line_number - 1; i < lines_in_para; i++) {
    /*copying the next line present to the line that is to be removed*/
    strcpy(doc->paragraphs[paragraph_number - 1].lines[i],
           doc->paragraphs[paragraph_number - 1].lines[i + 1]);
  }
  /*delteting the extra line*/
  (doc->paragraphs[paragraph_number - 1].number_of_lines)--;
  return SUCCESS;
}
/*
This functions adds the first data_lines number of lines from the data array to
the document. An empty string in the array creates a new paragraph. Notice that
by default the function creates the first paragraph. Assume that if data_lines
is not zero, the appropriate number of lines is present in the data array. The
function returns FAILURE if doc is NULL, data is NULL, or data_lines is 0;
otherwise the function returns SUCCESS.
*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {

  int i = 0, num_par;

  if (doc == NULL || data == NULL || data_lines <= 0) {
    return FAILURE;
  }
  /*storing the total number of paragraphs in the document*/
  num_par = doc->number_of_paragraphs;
  /*adding a paragraph if the doc is empty*/
  if (doc->number_of_paragraphs == 0) {
    add_paragraph_after(doc, 0);
    num_par++;
  }
  while (i < data_lines) {
    /*in the presence of an empty string, add a new paragraph*/
    if (strcmp(data[i], "") == 0) {
      num_par++;
      add_paragraph_after(doc, doc->number_of_paragraphs);
    } else {
      /*adding a line at the end of the document*/
      append_line(doc, num_par, data[i]);
    }
    i++;
  }
  return SUCCESS;
}
/*
This function replaces the text target with the text replacement everywhere it
appears in the document. Assume that the replacement does not cause a line to
exceed the maximum line length. Its assumed that target is not empty. Failture
is returned if doc, target or replacement are NULL; otherwise the function
returns SUCCESS
*/
int replace_text(Document *doc, const char *target, const char *replacement) {

  int i, j;
  char replaced[MAX_STR_SIZE + 1];

  if (doc == NULL) {
    return FAILURE;
  }
  if (target == NULL || replacement == NULL) {
    return FAILURE;
  }
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      /*replacing text and storing in replaced*/
      replace_text_Aux2(doc->paragraphs[i].lines[j], target, replacement,
                        replaced);
      /*storing/copying the replaced line in to the the document current line*/
      strcpy(doc->paragraphs[i].lines[j], replaced);
    }
  }
  return SUCCESS;
}

/*
This is an auxullary functio  to help replace text in only a signle line.
Calling this recursivey in replace text function makes it easier to look for the
target line by line to replace a tex
t*/

void replace_text_Aux2(const char *line, const char *target,
                       const char *replacement, char *replaced) {

  int index = 0, i;

  for (i = 0; i < (int)strlen(line); i++) {
    /*if a match is found*/
    if (strstr(&line[i], target) == &line[i]) {
      /*copying the replacement string to replaced char*/
      strcpy(&replaced[index], replacement);
      replaced += strlen(replacement);
      i += strlen(target) - 1;
    } else {
      /*if its not a match, copy the line as it is to the replaced char*/
      replaced[index] = line[i];
      index++;
    }
  }
  /*Setting the last charachter to null*/
  replaced[index] = '\0';
}

/*
This funcion highlights the text associated with target everywhere it appears in
the document, by surrounding the text with the strings HIGHLIGHT_START_STR and
HIGHLIGHT_END_STR. An assumption that the highlight does not cause
a line to exceed the max line length and target is a non empty string is made.
FAILURE is returned if doc is NULL or target is NULL; otherwise the function
returns SUCCESS.
*/
int highlight_text(Document *doc, const char *target) {

  char replacement_target[MAX_STR_SIZE + 1] = "";

  if (doc == NULL) {
    return FAILURE;
  }
  if (target == NULL) {
    return FAILURE;
  } else {
    /*concatinating the 2 brackets with the targeet
    resulting in the format below
    [target]
    */
    strcat(replacement_target, HIGHLIGHT_START_STR);
    strcat(replacement_target, target);
    strcat(replacement_target, HIGHLIGHT_END_STR);
    replacement_target[strlen(target) + 2] = '\0';

    /*calling the replace_text function to replace the
     origional target with the highlighted text*/
    replace_text(doc, target, replacement_target);
    return SUCCESS;
  }

  return SUCCESS;
}

/*
This function removes the text target everywhere it appears in the document.
Target is not the empty string. The function returns FAILURE if doc is NULL or
target is NULL; otherwise the function returns SUCCESS.*/

int remove_text(Document *doc, const char *target) {
  if (doc == NULL) {
    return FAILURE;
  }
  if (target == NULL) {
    return FAILURE;
  }
  /*replacing the target with an empty string*/
  replace_text(doc, target, "");
  return SUCCESS;
}
