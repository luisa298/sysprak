#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include "service.h"
#include "thinker.h"

// rechnet dann spaeter Spielzuege aus; unimplemented
int
thinker(int pipe_fd[2]){
  int pipe_val = 0;
  int flag;
  const int n = sizeof(int);
  
  printf("thinker aufgerufen.\n\n");
  
  // Schreibseite der Pipe schliessen
  close(pipe_fd[1]);
  
  if((flag = read(pipe_fd[0], &pipe_val, n)) == -1){
    perror("error reading from pipe");
    return 0;
  } else if(flag == 0){
    fprintf(stderr, "nothing could be read form pipe in thinker\n");
    return 0;
  }
  
  if(pipe_val != 1){
    fprintf(stderr, "performConnection liefert fehler an thinker\n");
    return 0;
  }
  
  printf("thinker terminiert.\n\n");
  return 1;
}
