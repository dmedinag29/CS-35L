#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

#define SP ' '
char flg = 'f'; 
//our frobcmp function which compares arrays and works with qsort
int frobcmp(const void* a, const void* b){
  const char* first = *(const char**)a;
  const char* second = *(const char**)b;

//will compare our lines by iterating through character by character
//will also check for f flag
 if(flg == 't'){
   while(1){
     if(*first == SP || toupper((unsigned char)(*first^42)) < toupper((unsigned char)(*second^42)))
        return -1;
     else if (*first == SP && *second == SP)
        return 0;
     else if (*second == SP || toupper((unsigned char)(*first^42)) > toupper((unsigned char)(*second^42)))
        return 1;
  
     first++;
     second++;
    }
 }
  else{ 
   while(1){
      if (*first == SP || (*first^42) < (*second^42))
         return -1;
      else if (*first == SP && *second == SP)
         return 0;
      else if (*second == SP || (*first^42) > (*second^42))
         return 1;
 
      first++;
      second++;
     }
  }
}

//called when input comes from a file
char* readFile(int filesize, int* buffsize){
    char* input = malloc(filesize);
    char in = '\0';
    int syscall = read(STDIN_FILENO, &in, sizeof(in));
    int iter = 0;
    while(syscall > 0){
        input[iter] = in;
        iter++;
        syscall = read(STDIN_FILENO, &in, sizeof(in));
        //the following only executes if file is actively growing and syscall has not reached EOF
        if(iter == filesize && syscall > 0){
          filesize++;
          input = realloc(input, filesize); 
        }
   }
   *buffsize = filesize;
   return input;
}
//called when input comes from stdin
char* readSTDIN(int* buffsize){
     char* input = malloc(8192);
     char in = '\0';
     int syscall = read(STDIN_FILENO, &in, sizeof(in));
     int iter = 0;
     int inputsize = 8192;
     while (syscall > 0){
     input[iter] = in;
     iter++;
     syscall = read(STDIN_FILENO, &in, sizeof(in));
        if (syscall > 0 && iter+1 == inputsize ){
           inputsize = inputsize*2;
           input = realloc(input, inputsize);
    
         }
     }
    *buffsize = iter;
    return input;
}


void sfrob(){
  int filesize = 0;
  int sizebuff = 0;
  int* sizeptr;
  sizeptr = &sizebuff;
  char* buffer;
  struct stat file_buff;
  fstat(0, &file_buff);
  if (S_ISREG(file_buff.st_mode)){
    filesize = file_buff.st_size;
    buffer = readFile(filesize, sizeptr);
  }
  else{
    buffer = readSTDIN(sizeptr);
  }
  int iter = 0;
  int twoDiter = 0;
  char* string = (char*)malloc(sizeof(char)); //creates our inital string
  char** twoDarray = (char**)malloc(sizeof(char*)); //creates our intial 2d-array
  char in;
  char nextin;
  int i = 0;
  int j = 1;
//while loop will take buffer and store it in string, then 2d array until end of buffer is reached
  in = buffer[i];
  while (j <= sizebuff){ 
    string[iter] = in; //takes individual char
    iter++;
    char* resize = realloc(string, sizeof(char)*(iter+2)); //allocates more memory to our string
    //Will check if there was an error when allocating more memory
    if (resize == NULL){
      free(string);
      char errdes[] = "There was an error while reallocating memory!\n";
      write(2, errdes, sizeof(errdes)-1);
      exit(1);
    }
    string=resize;
    //this portion of the code runs only if string input is not complete
    if (in != SP){
      if (j == sizebuff){ 
          in = SP;
      } //if the next character is EOF, it will add a space after our string
      else {
	i++;
        j++;
        in = buffer[i];
      }
    }
    else if (in == SP){ //runs only if our string input is complete
        twoDarray[twoDiter] = string; //adds our string to the 2Darray
        twoDiter++;
        char** twoDresize = realloc(twoDarray, sizeof(char*)*(twoDiter+2)); //resizes 2Darray
             if (twoDresize == NULL){
	           free(twoDarray);
	           char errdes[] =  "There was an error while reallocating memory!\n";
                   write(2, errdes, sizeof(errdes)-1);
       	           exit(1);
              }
         twoDarray = twoDresize;
         string = NULL; //will reset variables so it can be used again in while loop
         string = (char*)malloc(sizeof(char));
         iter = 0;
         //will break the loop if EOF is reached, else in will be set to the next character
             if (j == sizebuff){
                 j++;
                 break;
             }
             else if (buffer[j] == SP){
	        while (in == SP && j <= sizebuff ){
	          i++;
                  j++;
	          in = buffer[i];
	        }
	        if (in == SP && j > sizebuff)
	           break;
             }
             else {
	        i++;
                j++;
                in = buffer[i];
             }
    } 
  } 
//will sort our 2D array using the frobcmp funtion we created
  qsort(twoDarray, twoDiter, sizeof(char*), frobcmp);
//prints sorted strings to the terminal
  for (i = 0; i < twoDiter; i++){
    for (j = 0; ;j++){
      write(STDOUT_FILENO, &twoDarray[i][j], sizeof(char));
      if (twoDarray[i][j] == SP)
	break;
    }
  }
 //frees memorey
 int k;
  for (k = 0; k < twoDiter; k++){
    free(twoDarray[k]);
  }
  free(twoDarray);
  free(string);
  free(buffer);  
}

int main(int argc, char* argv[]){
//this part of the code will check for valid # of arudemnts, as well as if f flag was called 
 if (argc > 2){
     char errorm[] = "An invalid number of arugemnts was passed!";
     write(2, errorm, sizeof(errorm)-1);
     exit(1);
  }
  else if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'f'){
   flg = 't';    
  }
  
  sfrob();
  exit(0);
}
