#include <stdio.h>
#include <string.h>
#include <stdlib.h>



//*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/\\



int *intcsv(char *rawdata) {
  int i,j;
  int leftbound, rightbound, counter;
  int digit, entry;
  int *buffer = malloc(10000*sizeof(int*));
  char number[16];
  
  leftbound = -1;
  rightbound = 0;
  counter = 0;
  
  digit = 0;
  entry = 0;
  
  for (i=0; i < strlen(rawdata); i++) {
    if (rawdata[i] != ','){
      counter += 1;
      
    } else if (rawdata[i] == ',') {
      counter += 1;
      
      rightbound = leftbound + counter;
      
      memset(number, 0, sizeof(number));
      for (j = leftbound + 1; j < rightbound; j++) {
        number[digit] = rawdata[j];
        digit += 1;
      }
      digit = 0;
      
      leftbound = rightbound;
      counter = 0;
      
      buffer[entry] = atoi(number);
      entry += 1;
    }
  }
  return buffer;
}



//*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/\\



char *input(char *prompt) {
  char *buffer = malloc(10000 * sizeof(char*));
  
  printf("%s", prompt);
  fgets(buffer, 10000, stdin);
  buffer[strlen(buffer)-1] = '\0';
  
  return buffer;
}



//*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/\\



char *readline(char *data, int start) {
  int i,j,k;
  int end;
  int counter;
  
	char *buffer = malloc(128 * sizeof(char*));
  
  k = 0;
  counter = 0;
  
  for (i=start + 1; i < strlen(data); i++) {
		printf("%c", data[i]);

    if (data[i] != '\n') {
      counter += 1;
			printf("Hi\n");
			
    } else if (data[i] == '\n') {
      counter += 1;
      end = start + counter;
      
      for (j=start + 1; j < end; j++) {
        buffer[k] = data[j];
        k += 1;
      }
			
      return buffer;
    }
  }
}



//*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/*\*/\\





char *read(char *filename) {
  char *buffer = malloc(1000000 * sizeof(char*));
  int i = 0;
  FILE *file;
  
  file = fopen(filename, "r");
  
  if(file != NULL);{
    i = fread(buffer, sizeof(char), 1000000 * sizeof(char*), file);
  }
  
  fclose(file);
  
  return buffer;
}




/*
int main(void) {
  char *data = malloc(10000 * sizeof(char*));
  char *buffer = malloc(128 * sizeof(char*));
  int start = -1;

  data = read("key.txt");
  do {
		//printf("%s",data);

    buffer = readline(data,start);
    printf("%s",buffer);
    start += strlen(buffer);
  } while(strlen(buffer));
}
*/


















