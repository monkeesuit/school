//  Head.c
//  Sam Mishra - sam640@nyu.edu - N12140743
//  Print the first few lines of a file, as specified by the user.

#include "types.h"
#include "stat.h"
#include "user.h"

char buffer[1024];

/* From the file described by (fd),
   read the first number of lines 
   where the number is specified by 
   the user and passed to this function 
   by the variale (read_lines) */
  
int head(int fd, int read_lines) {

  int i, j, n;
  int num_lines, bytes;

  num_lines=bytes=0;

  while((n = read(fd, buffer, sizeof(buffer))) > 0) {
    for(i=0; i<n; i++){
      if(buffer[i]=='\n') { //  Whenever a '\n' is encountered the 
        num_lines++;	    //  num_lines variable is incremented by 1
        if(num_lines==read_lines) {
          for(j=0; j<i; j++) {
            printf(1, "%c", buffer[j]);
          }
	  memset(buffer,'\0',sizeof(buffer));
          printf(1, "\n");
          return 1;
        }
        
      }
      
    }
    printf(1, "%s", buffer);
    memset(buffer,'\0',sizeof(buffer));
  }
  printf(1, "\n");
  return 1;  
}

int main(int argc, char *argv[]) {
  
  int i;
  int num_file_arguments, num_line_arguments;
  int fd, lines_argument;
  int default_lines, default_file;
  char *option;

  default_lines = 10;
  default_file = 0;

  num_file_arguments = num_line_arguments = 0;

  /* If One Argument is Supplied, head, then read 10 lines from stdinput */
  if(argc == 1){
    head(default_file,default_lines);
    close(default_file);
    exit();
  }

  /* If two Arguments are specified we have to */
  /* check which argument the user specified   */
  else if(argc == 2){

    /* If filename is missing - open default file = stdinput */
    option = strchr(argv[1],'-');
    if(option!=0){
      lines_argument = atoi(option+1);
      head(default_file,lines_argument);
      close(default_file);
    }
  
    /* If num_lines is missing - set num_lines to default value = 10 */
    else{
      if((fd = open(argv[1],0)) < 0){
        printf(1, "ERROR: Trying to Open File\n");
        exit();
      }
      head(fd,default_lines);
      close(fd);
    }
    exit();
  }

  /* Extract num_lines and filename from arguments
     provided by user and pass them to head function */
  else if(argc == 3){

    for(i=1; i<3; i++){
      option = strchr(argv[i],'-');

      if(option!=0){
        lines_argument = atoi(option+1);
        num_line_arguments++;
      }
      else{
        if((fd = open(argv[i],0)) < 0){
          printf(1, "ERROR: Trying to Open File\n");
          exit();
        }
        num_file_arguments++;
      }
    }

    if((num_file_arguments > 1) || (num_line_arguments > 1)){
      printf(1, "ERROR: head -NUM filename\n");
      exit();
    }
    else{
      head(fd, lines_argument);
      close(fd);
      exit();
    }
  }
  else{
    /* Usage Error: Display usage "head -NUM filename" */
    printf(1, "ERROR: head -NUM filename\n");
    exit();
  }
}
