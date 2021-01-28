#include<stdio.h>
#include<stdlib.h>
#define SP ' '


//our frobcmp function which compares arrays and works with qsort
int frobcmp(const void* a, const void* b){
  const char* first = *(const char**)a;
  const char* second = *(const char**)b;

//will compare our lines by iterating through character by character
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

//we call this funtion everytime we use getchar to check for input erros
void inputError(){
  if (ferror(stdin)){
    fprintf(stderr, "There was an error from reading from stdin!");
    exit(1);
  }
}

void sfrob(){
  int iter = 0;
  int twoDiter = 0;
  char* string = (char*)malloc(sizeof(char)); //creates our inital string
  char** twoDarray = (char**)malloc(sizeof(char*)); //creates our intial 2d-array
  char in = getchar();
  inputError();
  char nextin = getchar();
  inputError();

//while loop will take input and store it in line, then 2d array until EOF is reached
  while (in != EOF){ 
    string[iter] = in; //takes individual char
    iter++;
    char* resize = realloc(string, sizeof(char)*(iter+2)); //allocates more memory to our string
    //Will check if there was an error when allocating more memory
    if (resize == NULL){
      free(string);
      fprintf(stderr, "There was an error while reallocating memory!");
      exit(1);
    }
    string=resize;
    //this portion of the code runs only if string input is not complete
    if (in != SP){
      if (nextin == EOF){ in = SP; } //if the next character is EOF, it will add a space after our string
      else {
	in = nextin;
	nextin = getchar();
	inputError();
      }
    }
    else if (in == SP){ //runs only if our string input is complete
      twoDarray[twoDiter] = string; //adds our string to the 2Darray
      twoDiter++;
      char** twoDresize = realloc(twoDarray, sizeof(char*)*(twoDiter+2)); //resizes 2Darray
      if (twoDresize == NULL){
	free(twoDarray);
	fprintf(stderr, "There was an error while reallocating memory!");
	exit(1);
      }
      twoDarray = twoDresize;
      string = NULL; //will reset variables so it can be used again in while loop
      string = (char*)malloc(sizeof(char));
      iter = 0;
      //will break the loop if EOF is reached, else in will be set to the next character
      if (nextin == EOF)
	break;
      else if (nextin == SP){
	while (in == SP){
	  in = getchar();
	  inputError();
	}
	if (in == EOF)
	  break;
	nextin = getchar();
	inputError();
      }
      else {
	in = nextin;
	nextin = getchar();
	inputError();
      }
    } 
  } 
//will sort our 2D array using the frobcmp funtion we created
  qsort(twoDarray, twoDiter, sizeof(char*), frobcmp);
//prints sorted strings to the terminal
  int i,j;
  for (i = 0; i < twoDiter; i++){
    for (j = 0; ;j++){
      if (putchar(twoDarray[i][j]) == SP)
	break;
    }
  }
 //frees memorey
 int k;
  for (k = 0; k < twoDiter; k++){
    free(twoDarray[k]);
  }
  free(twoDarray);
  
}

int main(void){
  sfrob();
  exit(0);
}
