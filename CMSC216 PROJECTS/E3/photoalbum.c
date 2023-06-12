

#include "photoalbum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Returns a dynamically-allocated Photo structure initialized based on the
provided parameters. If de- scription is not NULL, the function dynamically
allocates memory for the description and copies the description. If description
is NULL, no memory allocation takes place and the description field is ini-
tialized to NULL. The function returns NULL if a memory allocation fails.*/

Photo *create_photo(int id, const char *description) {
  char *p_disc;
  int length;

/*new Photo object*/
  Photo *photo = (Photo*)malloc(sizeof(Photo));

/*if memory allocation fails*/
  if (!photo) {
    return NULL;
  }
  /*if memory allocation doesn't fail*/
  if (description != NULL) {

    length = strlen(description) + 1;
    p_disc = (char *)malloc(sizeof(char)*length);
    memcpy(p_disc, description, length);
  } else {
    p_disc = NULL;
  }
  photo->id = id;
  photo->description = p_disc;
  return photo;
}

/*Prints a photo id and description. If the description is NULL, print the
description message "None". The function performs no task if the photo parameter
is NULL. See the public tests for information regarding output format.*/

void print_photo(Photo *photo) {
  char *p_disc;
  int id;

  if (photo == NULL) {
    return;
  }
    id = photo->id;
    p_disc = photo->description;
    if (p_disc == NULL) {
      printf("Photo Id: %d, Description: None\n", id);
    } else {
      printf("Photo Id: %d, Description: %s\n", id, p_disc);
    }
    return;
}
/*Deallocates any dynamically-allocated memory associated with parameter photo.
The function per- forms no task if the photo parameter is NULL.*/
void destroy_photo(Photo *photo) {
  if (photo == NULL) {
    return;
  }
  free(photo->description);
  free(photo);
  photo->description = NULL;
  photo = NULL;
  return;
}
/*Initializes the album size to 0. Assume this function is not called on an
album that has already been initialized. The function performs no task if the
album parameter is NULL.*/
void initialize_album(Album *album) {
  if (album == NULL || !album) {
    return;
  }
  album->size = 0;
}
/*Prints the contents of the album. If the album has no photos, the message
"Album has no photos." is printed – a newline char should be printed at the end
of the message. The function performs no task if the album parameter is NULL.
See the public tests for information regarding output format.*/
void print_album(const Album *album) {
  int size;
  int i;
  Photo *photo;

  if (album == NULL) {
    return;
  }
  size = album->size;
  if (size == 0) {
    printf("Album has no photos.\n");
  } else {
    /*lopoing the all_photos array*/
    for (i = 0; i < size; i++) {
      photo = album->all_photos[i];
      print_photo(photo);
    }
  }
  return;
}
/*Deallocates any dynamically-allocated memory associated with the album and
sets the album size to 0. The function performs no task if the album parameter
is NULL.*/
void destroy_album(Album *album) {
  int size;
  int i;
  Photo *photo;

  if (album == NULL) {
    return;
  }
  size = album->size;
  for (i = 0; i < size; i++) {
    photo = album->all_photos[i];
    destroy_photo(photo);
  }
  album->size = 0;
  return;
}

/*Appends (to the end of the array) a photo if there is enough space (if the
album size is less than MAX_ALBUM_SIZE). No photo is added if a photo cannot be
created. The function performs no task if the album parameter is NULL.*/
void add_photo_to_album(Album *album, int id, const char *description) {
  int size;
  Photo *photo;
  if(album == NULL) { 
    return; 
    }
  size = album->size;
  if (size >= MAX_ALBUM_SIZE) {
    return;
  }
  if (album == NULL) {
    return;
  }
  photo = create_photo(id, description);
  if (photo == NULL) {
    return;
  } else {
    album->all_photos[size] = photo;
    size ++;
    album->size = size;
  }
}
