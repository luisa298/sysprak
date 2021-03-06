#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "service.h"
#include "connector.h"
#include "performConnection.h"

// Netzwerkverbindung zum Server mit einem Socket herstellen, gibt den Socket Filedeskriptor zurück
int
connector(settings *toUse, char *gameID, int pipe_fd[2]){
  
  printf("connector aufgerufen.\n\n");
  
  // Leseseite der Pipe schließen
  close(pipe_fd[0]);
  int buffer = 0;
  int flag = 0;
  const int n = sizeof(int);
  
  // Socket vorbereiten
  int socketFD;                             // Socket FileDeskriptor
  struct addrinfo hints, *result;           // hints und result fuer getaddrinfo()
  memset(&hints, 0, sizeof(hints));         // Sicherstellen, dass struct leer ist
  hints.ai_family = AF_INET;                // struct ausfuellen
  hints.ai_socktype = SOCK_STREAM;

 // fprintf(stdout, "getaddrinfo()...\n");
  // getaddrinfo() aufrufen, um struct sockaddr zu befuellen
  if(getaddrinfo(toUse->hostname, toUse->portnumber, &hints, &result) != 0){
    perror("getaddrinfo() call failed");
    return 0;
  }

 // fprintf(stdout, "socket() call...\n");
  // socket() aufrufen, um den connection socket vorzubereiten
  if((socketFD = socket(result->ai_family, result->ai_socktype,
                  result->ai_protocol)) == -1){
    perror("socket() call failed");
    return 0;
  }

 // fprintf(stdout, "connect()...\n");
  // connect() aufrufen, um zum Server zu connecten
  if(connect(socketFD, result->ai_addr, result->ai_addrlen) == -1){
    close(socketFD);
    perror("connect() failed");
    return 0;
  }
  
  // Diese struct wird nicht mehr gebraucht; weg damit
  freeaddrinfo(result);
  
  // Ergebnis von performConnection (0 oder 1) in die Pipe verfrachten
  buffer = performConnection(socketFD, toUse->gamekind, gameID);
  if((flag = write(pipe_fd[1], &buffer, n)) == -1){
    perror("error writing to pipe");
    close(socketFD);
    return 0;
  }
  
  
  printf("connector terminiert.\n\n");
  
  // 
  close(socketFD);
  return buffer;
}
