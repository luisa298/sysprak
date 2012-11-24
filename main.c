#include "connector.h"
#include "performConnection.h"

#include <stdbool.h>

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

int
main(int argc, char **argv){
  int socketFD;
  
  if(checkForID(argc, argv[1])){
    socketFD = connector();
    performConnection(socketFD, argv[1]);
  } else
    return EXIT_FAILURE;
  
  close(socketFD);
  return EXIT_SUCCESS;
}
