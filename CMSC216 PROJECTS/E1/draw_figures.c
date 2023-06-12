#include <stdio.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define COLOR_END "\33[0m"

int valid_character (char symbol);
void draw_rectangle(  char symbol, int width, int length);
void draw_triangle( char symbol, int size );

int main() {

  int shape;
  char symbol2;
  
 while(1){

  printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
  scanf("%d" ,&shape);

  if(shape ==0){
    printf("Bye Bye.\n");
    break;

 }else if(shape ==1){
  int width;
  int length;
   printf("Enter character, width and length: ");
    scanf(" %c %d %d", &symbol2, &width, &length );
    
    if(valid_character (symbol2) && width > 0 && length >0){
      draw_rectangle(symbol2, width, length);
    }else{
      printf("Invalid data provided.\n");
      continue;
    }

 }else if(shape == 2){
       char symbol3;
       int size;

     printf("Enter character and size: ");

    scanf(" %c %d", &symbol3, &size );

    if(valid_character (symbol3) && size >0){
      draw_triangle(symbol3, size);
    }else{
      printf("Invalid data provided.\n");
      continue;
    }

 }else if(shape ==3){
    printf("Not implmented.\n");
 }else{
        printf( "Invalid choice.\n");
        continue;
 }
 }
 return 0;

}

 int valid_character (char symbol){
  
  if(symbol=='*' || symbol == '%' || symbol =='#'){
    return 1;
  }
  return 0;
}   

void draw_rectangle( char character, int width, int length) {
  
       int i;
       int j;
       for (i = 0; i < width; i++){
        for (j = 0; j < length; j++){
            printf("%c" , character);
        }
        printf("\n");
    }
}
void draw_triangle( char character, int size ) {
         int i;
         int j;
         int k;

         for(i=0; i<size;i++){
          for(j=0; j< (size -i) -1; j++){
                printf(" ");
          }
          for(k=0;k<2*(i+1)-1;k++){
            printf("%c" , character);

          }
                  printf("\n");

         }
}
    
        
