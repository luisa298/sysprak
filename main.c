#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "connector.h"
#include "performConnection.h"

#include <stdbool.h>

// Prüft die Game-ID auf syntaktische Gültigkeit
bool
checkForID(int argc, char *gameID){
  if(argc != 2){
    fprintf(stdout, "Bitte geben Sie nur eine 13-stellige GameID ohne Leerzeichen als Argument an.\n");
    return false;
  } else if(strlen(gameID)!=13){
    fprintf(stdout, "Game-ID hat falsche Laenge\n");
    return false;
  }
  else{
    return true;
  }
}

// Übergabeparameter ist die Game-ID
int
main(int argc, char **argv){
  int socketFD;
  
  // if(checkForID(argc, argv[1])){
    socketFD = connector();
    performConnection(socketFD, GAMEID);
    close(socketFD);
  // } else
  //   return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
