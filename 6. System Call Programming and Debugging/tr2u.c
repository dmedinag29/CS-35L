#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){

//first lets see if we have any errors

 if (argc != 3){
    fprintf(stderr, "There are an invalid number of arugments!");
    exit(1);
 }

 if (strlen(argv[1]) != strlen(argv[2])){
    fprintf(stderr, "The arguemnts are not of equal length!");
    exit(1);
 }
 
 const char* from = argv[1];
 const char* to = argv[2];
 int i, j;
 
 for(i = 0; i < strlen(from); i++){
    for(j = 0; j < strlen(from); j++ ){
     if(from[i] == from[j] && i != j){
      fprintf(stderr, "There is duplicate bytes in the first arguemnt!");
      exit(1);
      }
    }
 }

 char input[1];

 while( read(STDIN_FILENO, input, 1) != 0){
  int binary = 0; 
  for(i = 0; i < strlen(from) && binary == 0 ; i++ ){
   if(from[i] == (*input)){
    binary = 1;
    write(STDOUT_FILENO, &to[i], 1 );    
    }
   }
  if(binary == 0){
   write(STDOUT_FILENO, input, 1);
   } 
 }
 
 return 0;

}
