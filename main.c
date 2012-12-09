#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "service.h"
#include "connector.h"

// #define GAMEID "50c0d56e226a8"

int
main(int argc, char **argv){
  int v, ret_code;
  const int n = sizeof(int);
  char check[n];
  int fd[2];
  settings *toUse = (settings *) malloc(sizeof(settings));
  
  pid_t pid = 0;
  fd[0] = fd[1] = 0;
  check[0] = check[1] = check[2] = check[3] = 'x';
  
  if((ret_code = pipe(fd)) < 0){
    perror("Fehler beim pipe()-Aufruf");
    return EXIT_FAILURE;
  }
  if((pid = ret_code = fork()) < 0){
    perror("Fehler beim fork()-Aufruf");
    return EXIT_FAILURE;
  }
  if(pid > 0)
    thinker(fd);
  else if(checkArgs(argc, argv, toUse)){
    connector(toUse, argv[1], fd);
   } else
     return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

