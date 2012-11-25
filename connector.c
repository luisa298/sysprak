#include "connector.h"

int 
connector(){
  
  fprintf(stdout, "Client aufgerufen.\n\n");
  
  // Socket vorbereiten
  int socketFD;                             // Socket FileDeskriptor
  struct addrinfo hints, *result;           // hints und result fuer getaddrinfo()
  
  memset(&hints, 0, sizeof(hints));         // sicherstellen, dass struct leer ist
  hints.ai_family = AF_INET;                // struct ausfuellen
  hints.ai_socktype = SOCK_STREAM;

//  fprintf(stdout, "getaddrinfo()...\n");
  // getaddrinfo() aufrufen, um struct sockaddr zu befuellen
  if(getaddrinfo(HOSTNAME, PORTNUMBER, &hints, &result) != 0){
    perror("getaddrinfo() call failed");
    return EXIT_FAILURE;
  }

//  fprintf(stdout, "socket() call...\n");
  // socket() aufrufen, um den connection socket vorzubereiten
  if((socketFD = socket(result->ai_family, result->ai_socktype,
                  result->ai_protocol)) == -1){
    perror("socket() call failed");
    return EXIT_FAILURE;
  }

//  fprintf(stdout, "connect()...\n");
  // connect() aufrufen, um zum Server zu connecten
  if(connect(socketFD, result->ai_addr, result->ai_addrlen) == -1){
    close(socketFD);
    perror("connect() failed");
    return EXIT_FAILURE;
  }
  
  // diese struct wird nicht mehr gebraucht; weg damit
  freeaddrinfo(result);
  
  // Socket FileDeskriptor wird zurueckgegeben
  return socketFD;
}
