

#include "calendar.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
This function initializes a Calendar struct based on the parameters provided.
The out parameter calendar provides access to the new Calendar struct. The total
number of events is set to zero. The function returns FAILURE if calendar and/or
name are NULL, if the number of days is less than 1, or if any memory allocation
fails. Otherwise the function returns SUCCESS.
*/
int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar) {

  Calendar *cal;

  if (calendar == NULL || name == NULL || days < 1) {
    return FAILURE;
  }

  /*Allocating memory for calender struct, name, events*/
  cal = malloc(sizeof(Calendar));

  cal->name = malloc(strlen(name) + 1);
  /* assigning memory to an array of pointers to Event structs*/
  cal->events = calloc(days, sizeof(Event *));

  /*if memory allocation fails for any allocations return failure*/
  if (cal == NULL || cal->name == NULL || cal->events == NULL) {
    return FAILURE;
  }

  cal->comp_func = comp_func;
  /*settin total nuber of events to 0*/
  cal->total_events = 0;
  cal->days = days;
  cal->free_info_func = free_info_func;
  strcpy(cal->name, name);
  *calendar = cal;
  return SUCCESS;
}
/*
This function prints, to the designated output stream, the calendar’s name,
days, and total number of events if print_all is true; otherwise this
information is not printed. Information about each event (name, start time, and
duration) is printed regardless the value of print_all. See public tests output
for format information. Notice that the heading "**** Events ****" is always
printed. This function returns FAILURE if calendar and/or output_stream are
NULL; otherwise the function returns SUCCESS.
*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {

  int i;
  FILE *fp;

  Event *new_event;
  if (calendar == NULL || output_stream == NULL) {
    return FAILURE;
  }

  fp = output_stream;
  /*if print_all is 1 then it means the following is printed based on the PubT
   * output*/
  if (print_all == 1) {
    fprintf(fp, "Calendar's Name: \"%s\"\n", calendar->name);
    fprintf(fp, "Days: %d\n", calendar->days);
    fprintf(fp, "Total Events: %d\n", calendar->total_events);
    /*new line*/
    fprintf(fp, "\n");
  }
  fprintf(fp, "%s", "**** Events ****\n");

  if (calendar->total_events == 0) {
    return SUCCESS;
  }
  /*looping through the days to get the events*/
  for (i = 0; i < calendar->days; i++) {

    fprintf(fp, "Day %d\n", i + 1);
    new_event = calendar->events[i];

    while (new_event) {
      /* Event's Name: "lunch", Start_time: 1200, Duration: 45*/
      /*prints each events name*/
      fprintf(fp, "Event's Name: \"%s\",Start_time: %d, Duration: %d\n",
              new_event->name, new_event->start_time,
              new_event->duration_minutes);
      new_event = new_event->next;
    }
  }
  return SUCCESS;
}

/*  This function adds the specified event to the list associated with the day
parameter. The event is added in increasing sorted order using the comparison
function (comp_func) that allows comparison of two events. The function
allocates memory for the new event and for the event’s name. Other fields of the
event struct are initialized based on the parameter values. This function
returns FAILURE if calendar and/or name are NULL, start time is invalid,
duration_minutes is less than or equal to 0, day is less than 1 or greater than
the number of calendar days, if the event already exist, or if any memory a
llocation fails. Otherwise the function will return SUCCESS.*/

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {

  Event *new_event, *curr;
  Event *prev = NULL;

  /*checking for NULL cases*/
  if (calendar == NULL || name == NULL || start_time < 0 || start_time > 2400 ||
      duration_minutes <= 0 || day < 1 || day > calendar->days) {
    return FAILURE;
  }
  /*it must return Failure if an attempt to add an event that already exsists is
   * made*/
  if (find_event(calendar, name, &new_event) == SUCCESS) {
    return FAILURE;
  }
  /*allocating memory for the event to be added*/
  new_event = malloc(sizeof(Event));
  new_event->name = malloc(strlen(name) + 1);
  /*if memory allocation fails incase make sure to return FAILURE*/
  if (new_event == NULL || new_event->name == NULL) {
    return FAILURE;
  }
  /*initiazlizing events based on parameter to create the new event*/
  strcpy(new_event->name, name);
  new_event->start_time = start_time;
  new_event->duration_minutes = duration_minutes;
  new_event->info = info;

  /*pointing at the head -> the first day*/
  curr = calendar->events[day - 1];

  /* while the new event is bigger than curr keep looking for the next day*/
  while (curr && calendar->comp_func(new_event, curr) > 0) {
    prev = curr;
    curr = curr->next;
  }
  /* if prev is null, or only a head is present, set the new event to be
  the head*/
  if (prev == NULL) {
    new_event->next = calendar->events[day - 1];
    /*making the head be the new event*/
    calendar->events[day - 1] = new_event;

  } else {
    /*add in the right order*/
    new_event->next = curr;
    prev->next = new_event;
  }
  /*increment total events*/
  (calendar->total_events)++;
  return SUCCESS;
}
/*
This function returns a pointer (via the out parameter event) to the event with
the specified name. If the event parameter is NULL, no pointer is
returned. The out parameter event should not be modified unless the event
is found. The function returns FAILURE if calendar and/or name are NULL. The
function returns SUCCESS if the event was found and FAILURE otherwise.
*/

int find_event(Calendar *calendar, const char *name, Event **event) {

  int day;

  if (calendar && name) {
    /*looping through each day*/
    for (day = 0; day < calendar->days; day++) {
      /*if the evenent is found in a specfic day then it means the event is
       * found*/
      if (find_event_in_day(calendar, name, day + 1, event) == SUCCESS) {
        return SUCCESS;
      }
    }
  }
  return FAILURE;
}

/*
This function returns a pointer (via the out parameter event) to the event with
the specified name in the specified day (if such event exists). If the event
parameter is NULL, no pointer is returned. The out parameter event is
not modified unless the event is found. The function returns FAILURE if
calendar and/or name are NULL, or if the day parameter is less than 1 or greater
than the number of calendar days. The function returns SUCCESS if the event is
found and FAILURE otherwise.
*/

int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {

  Event *curr;

  if (calendar && name && day >= 1 && day <= calendar->days &&
      calendar->total_events != 0) {
    /*curr -> the first day*/
    curr = calendar->events[day - 1];
    while (curr) {
      if (strcmp(curr->name, name) == 0) {
        /*event is found. Check if event is null before modifiying the event*/
        if (event && curr) {
          *event = curr;
        }
        return SUCCESS;
      }
      /*if name hasn't matched keep looking */
      curr = curr->next;
    }
  }
  return FAILURE;
}

/*
This function removes the specified event from the calendar, returning any
memory allocated for the event. If the event has an info field other than NULL
and a free_info_func exists, the function is called on the info field. The
number of calendar events is adjusted accordingly. This function returns FAILURE
if calendar and/or name are NULL or if the event cannot be found; otherwise the
function returns SUCCESS.
*/
int remove_event(Calendar *calendar, const char *name) {

  Event *curr, *prev = NULL;
  int i;

  if (calendar == NULL || name == NULL) {
    return FAILURE;
  }

  /*must return failure if the event to removed isn't there*/
  if (find_event(calendar, name, &curr) == FAILURE) {
    return FAILURE;
  }
  /*try looping through each day*/
  for (i = 0; i < calendar->days; i++) {
    /*first event in a specific day*/
    curr = calendar->events[i];
    /*as long as we haven't reached the end and a match hasn't been found*/
    while (curr && strcmp(curr->name, name) != 0) {
      /*keep looping*/
      prev = curr;
      curr = curr->next;
    }
    /*when the event to be reomved is found*/
    if (curr && strcmp(curr->name, name) == 0) {
      /*and its at the head*/
      if (prev == NULL) {
        /*basically making the event after curr, the head(first) event*/
        calendar->events[i] = curr->next;
      } else {
        /*else jump it*/
        prev->next = curr->next;
      }
      /*checking the name isn't empty before freeing it and setting it to NULL*/
      if (curr->name) {
        free(curr->name);
        curr->name = NULL;
      }
      /*per direction chcking if currr has an  info field and free_info_func
       * exsists*/
      if (curr->info && calendar->free_info_func) {
        /*calling the function on curr*/
        calendar->free_info_func(curr->info);
      }
      /*we can finally free curr now*/
      free(curr);
      curr = NULL;
      /*decrement total_events by 1*/
      (calendar->total_events)--;
      return SUCCESS;
    }
  }
  return FAILURE;
}
/*
This function returns the info pointer associated with the specified event. The
function returns NULL if the event is not found. Assume the calendar and name
parameters are different than NULL.
*/
void *get_event_info(Calendar *calendar, const char *name) {
  /*pointer to an event*/
  Event *curr;
  /*no need to check for NULL parameter - assumed*/

  /*no need to loop throuh each day and event, the function find_event already
  does that for us*/

  /*if event is found return the info associated with the pointer to the EVENT*/
  if (find_event(calendar, name, &curr) == SUCCESS) {
    if (curr->info) {
      return curr->info;
    }
  }
  return NULL;
}
/*This function removes all the event lists associated with the calendar and set
them to empty lists. The array holding the event lists will not be
removed. The total number of events is set to 0. If an event has an info field
other than NULL and a free_info_func exists, the function is called on the info
field. This function returns FAILURE if calendar is NULL; otherwise the function
returns SUCCESS.*/
int clear_calendar(Calendar *calendar) {

  int i;

  if (calendar == NULL) {
    return FAILURE;
  }
  /*clearing each day since its the same as setting the events associated with
  it to empty*/
  for (i = 1; i <= calendar->days; i++) {
    clear_day(calendar, i);
  }
  return SUCCESS;
}

/*
This function removes all the events for the specified day, setting the event
list to empty. The total number of events is adjusted accordingly. If an event
has an info field other than NULL and a free_info_func exists, the function is
called on the info field. This function returns FAILURE if calendar is NULL, or
if the day is less than 1 or greater than the calendar days; otherwise the
function returns SUCCESS.
*/
int clear_day(Calendar *calendar, int day) {

  Event *prev, *curr;

  if (calendar == NULL || day < 1 || day > calendar->days) {
    return FAILURE;
  }
  /*there should be atleast one event to be cleared*/
  if (calendar->total_events > 0) {
    /*curr is poitning at first day*/
    curr = calendar->events[day - 1];
    while (curr) {
      prev = curr;
      /*curr is pointing at the event next to prev*/
      curr = curr->next;
      /*removing and freeing prev as we go*/
      free(prev->name);
      if (calendar->free_info_func) {
        if (prev->info) {
          /*freeing info*/
          calendar->free_info_func(prev->info);
        }
      }
      /*free prev at last*/
      free(prev);
      /*decrement events*/
      (calendar->total_events)--;
    }
    /*setting the head(first day) to NULL(empty)*/
    calendar->events[day - 1] = NULL;
    return SUCCESS;
  }
  return FAILURE;
}

/*
This function frees memory that was dynamically-allocated for the calendar. If
an event has an info field other than NULL and a free_info_func exists, the
function is called on the info field. This function returns FAILURE if calendar
is NULL; otherwise the function returns SUCCESS.
*/
int destroy_calendar(Calendar *calendar) {

  if (calendar == NULL) {
    return FAILURE;
  }
  /*removing all the events*/
  clear_calendar(calendar);
  /*freeing the name of the calender*/
  free(calendar->name);
  /*freeing the all the events associated with the calender*/
  free(calendar->events);
  /*then freeing the calander at last*/
  free(calendar);

  return SUCCESS;
}
