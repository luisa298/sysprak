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
  settings *toUse = (settings *) malloc(sizeof(settings));

  if(argc < 3){
	  shm("client.config");
  } else if(argc == 3) {
	  shm(argv[2]);
  }
  
  if(checkArgs(argc, argv, toUse)){
    connector(toUse, argv[1]);
   } else
     return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

