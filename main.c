#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "service.h"
#include "connector.h"
#include "thinker.h"

// #define GAMEID "50c0d56e226a8"

int
main(int argc, char **argv){
  int thinkerflag, connectorflag;
  thinkerflag = connectorflag = 0;
  const int n = sizeof(int);
  int fd[2];
  
  settings *toUse = (settings *) malloc(sizeof(settings));
  
  pid_t pid = 0;
  fd[0] = fd[1] = 0;
  
  fprintf(stdout, "\nClient aufgerufen.\n\n");
  
  if(pipe(fd) < 0){
    perror("Fehler beim pipe()-Aufruf");
    return EXIT_FAILURE;
  }
  if((pid = fork()) < 0){
    perror("Fehler beim fork()-Aufruf");
    return EXIT_FAILURE;
  }
  
  // Elternprozess ist der thinker
  if(pid > 0)
    thinkerflag = thinker(fd);
  // Kindprozess ist der Connector
  else{
    if(checkArgs(argc, argv, toUse))
      connectorflag = connector(toUse, argv[1], fd);
    else{
      close(fd[0]);
      if(write(fd[1], &connectorflag, n) == -1){
        perror("error writing to pipe");
        return 0;
      }
      return EXIT_FAILURE;
    }
  }
  
  if(thinkerflag || connectorflag){
    printf("Alles gut.\n");
    return EXIT_SUCCESS;
  }
  else{
    printf("Failure.\n");
    return EXIT_FAILURE;
  }
}

