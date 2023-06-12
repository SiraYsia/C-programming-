
#include <stdio.h>
#include <string.h>
#include "text_manipulation.h"

int remove_spaces(const char *source, char *result, int *num_spaces_removed) {  
    int j, i = 0, curr, beg, end;
    int length = 0;

    if(source == NULL){
        return FAILURE;
    }
    
    length = strlen(source);
    if(length == 0 ){
        return FAILURE;
    }
     if(num_spaces_removed != NULL){
        *num_spaces_removed = 0;
     }
        while(source[i] != '\0'){
        i++;
        end = i;
    
     }
    i = 0;
    while(source[i] == ' '){
        if(num_spaces_removed != NULL){
        (*num_spaces_removed)++;
     }
        i++;
    }

    beg = i;
    i = end;
    
    while(source[i - 1] == ' '){
        if(num_spaces_removed != NULL){
        (*num_spaces_removed)++;
      }
         i--;
    }
    curr = i;
    i = beg;
    length = curr - beg;
    for(j = 0; j < length; j++){
        result[j] = source[beg + j];
        
    }
    result[j] = '\0';
    return SUCCESS;

}

int center(const char *source, int width, char *result) {     
    int  i, index = 0;
    int spaces =0;
    int len = 0;


     if(source == NULL){ 
        return FAILURE;
    }
        len = strlen(source);
        spaces = (width - len)/2;

        if(strlen(source) == 0 || width < len){
            return FAILURE;
        }

    while(index < spaces){
        result[index] =' ';
        index++;
    }
    for(i = 0; i< len; i++){
        result[i + spaces] = source[i];
    }
     for(i = 0; i< spaces; i++){
        result[len + spaces + i] = ' ';
    }

           result[spaces + spaces + len] = '\0';
           return SUCCESS;

 
    }



