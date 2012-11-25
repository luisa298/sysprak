#include "performConnection.h"

int 
performConnection(int socketFD, char *gameID){
  // benoetigte variablen
//  char recv_msg[BUFLEN], *send_msg;
//  send_msg = (char *) malloc(BUFLEN*sizeof(char));
  int i;
  char **argv;
  // argv alloziieren
  if((argv = (char **) malloc(ARGS * sizeof(char *))) == NULL){
    fprintf(stdout, "out of memory\n");
    abort();
  }
  for(i = 0; i < 6; i++){
    if((argv[i] = (char *) malloc(BUFLEN * sizeof(char))) == NULL){
      fprintf(stdout, "out of memory\n");
      abort();      
    }
  }
//  ssize_t size;
  int readyflag;
  
  readyflag = openingHandler(socketFD, argv, gameID);
  fprintf(stdout, "%i\n", readyflag);
  
  // while(recvFrServer(socketFD, recv_msg, argv) > 0){
  //   
  //   
  //   memset(recv_msg, 0, BUFLEN);
  // }
  
  return EXIT_SUCCESS;
}

int
openingHandler(int socketFD, char **argv, char *gameID){
  char msg[BUFLEN];
  size_t size;
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    perror("recv() in openingHandler gescheitert");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Empfangene Nachricht: %s", msg);
  if(strcmp(msg, SRV_OPENING) == 0){
    if(send(socketFD, CLT_OPENING, strlen(CLT_OPENING), 0) == -1){
      perror("send() in openingHandler gescheitert");
      return EXIT_FAILURE;
    }
    fprintf(stdout, "hello\n");
    return openingHandler(socketFD, argv, gameID);
  } else if(strcmp(msg, SRV_ACCEPTANCE) == 0){
    sprintf(msg, "%s %s\n", "ID", gameID);
    size = strlen(msg);
    if(send(socketFD, msg, size, 0) == -1){
      perror("send() in openingHandler gescheitert");
      return EXIT_FAILURE;
    }
    return 1;
  } else
    return 0;
  
  return size;
}

size_t
recvFrServer(int socketFD, char *msg, char **argv){
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    perror("recv() gescheitert");
    return EXIT_FAILURE;
  }
  stringSplit(msg, argv);
  
  if(*argv[0] == '+')
    return 1;
  else if(*argv[0] == '-')
    return 0;
  else
    return EXIT_FAILURE;
  
  return EXIT_FAILURE;
}

size_t
sendToServer(int socketFD, char *msg){
  size_t size = strlen(msg);
  fprintf(stdout, "Sende %s", msg);
  if(send(socketFD, msg, size, 0) == -1){
    perror("send() gescheitert");
    return -1;
  }
  memset(msg, 0, BUFLEN);
  return size;
}

int
stringSplit(char *string, char **argv){   // argv muss schon alloziiert sein
  int i = 0;
  
  char *temp;
  temp = (char *) malloc(BUFLEN*sizeof(char));
  strcpy(temp, string);
  
  char *buffer;
  buffer = (char *) malloc(BUFLEN*sizeof(char));
  
  char *delim;
  delim = " ";
  
  buffer = strtok(temp, delim);
  
  while(buffer != NULL){
    strcpy(argv[i], buffer);
    i++;
    buffer = strtok(NULL, delim);
  }
  
  free(buffer);
  free(temp);
  
  return i;
}
