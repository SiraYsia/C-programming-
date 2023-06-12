#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
  return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/******************************************************************************
This test mainly checks what happens when trying to find event that doesn't
exsist. Testted functions: init_calendar, add_event, find_event,
destroy_calendar
*******************************************************************************/
static int test1() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;
  Event *event;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    add_event(calendar, "Hello", 939, 33, NULL, 1);
    add_event(calendar, "there", 334, 22, NULL, 3);
    add_event(calendar, "Hello2", 939, 33, NULL, 1);
    add_event(calendar, "Helloooo", 939, 33, NULL, 1);

    if (find_event(calendar, "HOLA", &event) == FAILURE &&
        find_event(calendar, "Bonjur", &event) == FAILURE) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == SUCCESS) {
      truth = SUCCESS;
    }
  }
  return truth;
}

/******************************************************************************
This test mainly checks for the output when attemtpting to remove an event
from the head and tail
Testted functions: init_calendar, add_event, remove_event, destroy_calendar
*******************************************************************************/

static int test2() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    add_event(calendar, "Hello", 939, 33, NULL, 1);
    add_event(calendar, "there", 334, 22, NULL, 3);
    add_event(calendar, "Hello2", 939, 33, NULL, 1);
    add_event(calendar, "Helloooo", 939, 33, NULL, 1);

    if (remove_event(calendar, "Helloooo") == SUCCESS &&
        remove_event(calendar, "Hello") == SUCCESS) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = SUCCESS;
    }
  }
  return truth;
}

/******************************************************************************
This test mainly checks for the output when attemtpting to find an event
from an empty calander
Testted functions: init_calendar, find_event, destroy_calendar
*******************************************************************************/
static int test3() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;
  Event *event;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (find_event(calendar, "find", &event) == FAILURE)
      truth = SUCCESS;

    if (destroy_calendar(calendar) == FAILURE) {
      truth = SUCCESS;
    }
  }
  return truth;
}
/******************************************************************************
This test mainly checks for the output when attemtpting to find an event that
doesn't exsits in the day its going to be looked for. finding in a wrong day.
Testted functions: init_calendar, add_event, find_event_in_day, destroy_calendar
*******************************************************************************/
static int test4() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;
  Event *event;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (add_event(calendar, "look for", 303, 50, NULL, 6) == SUCCESS &&
        find_event_in_day(calendar, "look for", 5, &event) == FAILURE &&
        find_event_in_day(calendar, "look---for", 6, &event) == FAILURE &&
        find_event_in_day(calendar, "look for", 6, &event) == SUCCESS) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}
/******************************************************************************
This test mainly checks for the output when attemtpting to remove an event from
an empty calander.
Testted functions: init_calendar, find_event_in_day, destroy_calendar
*******************************************************************************/
static int test5() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (remove_event(calendar, "find") == FAILURE)
      truth = SUCCESS;

    if (destroy_calendar(calendar) == FAILURE) {
      truth = SUCCESS;
    }
  }
  return truth;
}

/******************************************************************************
This test mainly checks for the output when attemtpting to clear a calendar
Testted functions: init_calendar, add_event,clear_calandar, destroy_calendar
*******************************************************************************/
static int test6() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    add_event(calendar, "Hello", 939, 33, NULL, 1);
    add_event(calendar, "there", 334, 22, NULL, 3);
    add_event(calendar, "Hello2", 939, 33, NULL, 4);
    add_event(calendar, "Helloooo", 939, 33, NULL, 2);

    if (clear_calendar(calendar) == SUCCESS) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}

/******************************************************************************
This test simply checks for the output when attemtpting to add an event for
every day in the calander. Testted functions: init_calendar, add_event,
destroy_calendar
*******************************************************************************/

static int test7() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (add_event(calendar, "Hello", 939, 33, NULL, 1) == SUCCESS &&
        add_event(calendar, "there", 334, 22, NULL, 2) == SUCCESS &&
        add_event(calendar, "Hello2", 939, 33, NULL, 3) == SUCCESS &&
        add_event(calendar, "Helloooo", 939, 33, NULL, 4) == SUCCESS &&
        add_event(calendar, "fddd", 939, 33, NULL, 5) == SUCCESS &&
        add_event(calendar, "wewe", 939, 33, NULL, 6) == SUCCESS &&
        add_event(calendar, "erer", 939, 33, NULL, 7) == SUCCESS) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}

/******************************************************************************
This test mainly checks for the output when attemtpting to add an event that
already exsits.
Testted functions: init_calendar, find_event_in_day, destroy_calendar
*******************************************************************************/
static int test8() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (add_event(calendar, "Hello", 939, 33, NULL, 1) == SUCCESS &&
        add_event(calendar, "there", 334, 22, NULL, 2) == SUCCESS &&
        add_event(calendar, "Hello2", 939, 33, NULL, 3) == SUCCESS &&
        add_event(calendar, "Helloooo", 939, 33, NULL, 4) == SUCCESS &&
        add_event(calendar, "fddd", 939, 33, NULL, 5) == SUCCESS &&
        add_event(calendar, "Hello", 939, 33, NULL, 1) == FAILURE &&
        add_event(calendar, "there", 334, 22, NULL, 5) == FAILURE) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}
/******************************************************************************
This test mainly checks for the output when attemtpting to remove an event from
the middle of the linked list.
Testted functions: init_calendar, add_event, remove_event, destroy_calendar
*******************************************************************************/
static int test9() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    add_event(calendar, "Hello", 939, 33, NULL, 1);
    add_event(calendar, "there", 334, 22, NULL, 2);
    add_event(calendar, "Hello2", 939, 33, NULL, 3);
    add_event(calendar, "Helloooo", 939, 33, NULL, 1);
    add_event(calendar, "fddd", 939, 33, NULL, 1);

    if (remove_event(calendar, "Hello2") == SUCCESS) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}

/******************************************************************************
This test mainly checks for the output when attemtpting clear days from a
list of events.
Testted functions: init_calendar, add_event, clear_day, destroy_calendar
*******************************************************************************/
static int test10() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    add_event(calendar, "Hello", 939, 33, NULL, 1);
    add_event(calendar, "there", 334, 22, NULL, 2);
    add_event(calendar, "Hello2", 939, 33, NULL, 3);
    add_event(calendar, "Helloooo", 939, 33, NULL, 4);
    add_event(calendar, "fddd", 939, 33, NULL, 5);

    if (clear_day(calendar, 4) == SUCCESS &&
        clear_day(calendar, 5) == SUCCESS &&
        clear_day(calendar, 2) == SUCCESS) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}

/******************************************************************************
This test mainly checks for the output when attemtpting clear days from a
list of events when day is greater than the total calander days
Testted functions: init_calendar, add_event, clear_day, destroy_calendar
*******************************************************************************/
static int test11() {
  int days = 7, truth = FAILURE;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    add_event(calendar, "Hello", 939, 33, NULL, 1);
    add_event(calendar, "there", 334, 22, NULL, 2);
    add_event(calendar, "Hello2", 939, 33, NULL, 3);
    add_event(calendar, "Helloooo", 939, 33, NULL, 4);
    add_event(calendar, "fddd", 939, 33, NULL, 5);

    if (clear_day(calendar, 4) == SUCCESS &&
        clear_day(calendar, 5) == SUCCESS &&
        clear_day(calendar, 8) == FAILURE) {
      truth = SUCCESS;
    }
    if (destroy_calendar(calendar) == FAILURE) {
      truth = FAILURE;
    }
  }
  return truth;
}

int main() {
  int result = SUCCESS;

  /***** Starting memory checking *****/
  start_memory_check();
  /***** Starting memory checking *****/

  /**********************************************/
  if (test1() == FAILURE) {
    printf("1FAILED");
    result = FAILURE;
  } else {
    printf("1PASSED ");
  }
  /**********************************************/

  if (test2() == FAILURE) {
    printf("2FAILED");
    result = FAILURE;
  } else {
    printf("2PASSED ");
  }
  /**********************************************/
  if (test3() == FAILURE) {
    printf("3FAILED");
    result = FAILURE;
  } else {
    printf("3PASSED ");
  }
  /**********************************************/
  if (test4() == FAILURE) {
    printf("4FAILED");
    result = FAILURE;
  } else {
    printf("4PASSED ");
  }
  /**********************************************/
  if (test5() == FAILURE) {
    printf("5FAILED");
    result = FAILURE;
  } else {
    printf("5PASSED ");
  }

  /**********************************************/
  if (test6() == FAILURE) {
    printf("6FAILED");
    result = FAILURE;
  } else {
    printf("6PASSED ");
  }
  /**********************************************/
  if (test7() == FAILURE) {
    printf("7FAILED");
    result = FAILURE;
  } else {
    printf("7PASSED ");
  }
  /**********************************************/
  if (test8() == FAILURE) {
    printf("8FAILED");
    result = FAILURE;
  } else {
    printf("8PASSED ");
  }
  /**********************************************/
  if (test9() == FAILURE) {
    printf("9FAILED");
    result = FAILURE;
  } else {
    printf("9PASSED ");
  }
  /**********************************************/
  if (test10() == FAILURE) {
    printf("10FAILED");
    result = FAILURE;
  } else {
    printf("10PASSED ");
  }
  /**********************************************/
  if (test11() == FAILURE) {
    printf("11FAILED");
    result = FAILURE;
  } else {
    printf("11PASSED ");
  }

  /**********************************************/

  /****** Gathering memory checking info *****/
  stop_memory_check();
  /****** Gathering memory checking info *****/

  if (result == FAILURE) {
    exit(EXIT_FAILURE);
  }
  printf("\nSUCCESS! YOU PASSED ALL THE TESTS");
  printf("\n");
  return EXIT_SUCCESS;
}
