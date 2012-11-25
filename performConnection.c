#include "performConnection.h"

int 
performConnection(int socketFD, char *gameID){
  // benoetigte variablen
//  char *send_msg = (char *) malloc(BUFLEN*sizeof(char));
  int i;
  // argv alloziieren
  char **argv;
  if((argv = (char **) malloc(ARGS * sizeof(char *))) == NULL){
    fprintf(stdout, "out of memory\n");
    abort();
  }
  for(i = 0; i < ARGS; i++){
    if((argv[i] = (char *) malloc(BUFLEN * sizeof(char))) == NULL){
      fprintf(stdout, "out of memory\n");
      abort();      
    }
  }
  // subargv alloziieren
  char **subargv;
  if((subargv = (char **) malloc(ARGS * sizeof(char *))) == NULL){
    fprintf(stdout, "out of memory\n");
    abort();
  }
  for(i = 0; i < ARGS; i++){
    if((subargv[i] = (char *) malloc(BUFLEN * sizeof(char))) == NULL){
      fprintf(stdout, "out of memory\n");
      abort();      
    }
    }
  int readyflag;
  
  readyflag = openingHandler(socketFD, argv, subargv, gameID);
  readyflag = receive(socketFD, argv, subargv);
  // while(readyflag && (readyflag = receive(socketFD, recv_msg, argv, subargv))){
  //   fprintf(stdout, "%s\n", subargv[1]);
  //   
  //   memset(recv_msg, 0, BUFLEN);
  //   return EXIT_SUCCESS;
  // }
  i = 0;
  while(argv[i] != NULL)
    fprintf(stdout, "%s\n", argv[i++]);
  
  // Behandlung von Abbruchsignal durch Server
  if(readyflag == 0){
    fprintf(stdout, "Server schickt Fehler:\n%s\n", argv[0]);
    fprintf(stdout, "\nWir bitten, dies zu entschuldigen.\n\n\n");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

int
openingHandler(int socketFD, char **argv, char **subargv, char *gameID){
  char msg[BUFLEN];
  size_t size;
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    perror("recv() in openingHandler gescheitert");
    return EXIT_FAILURE;
  }
  stringSplit(msg, argv, "\n");
  fprintf(stdout, "Empfangene Nachricht: %s\n", argv[0]);
  if(strcmp(argv[0], SRV_OPENING) == 0){
    fprintf(stdout, "Sende dem Server die Nachricht %s", CLT_OPENING);
    if(send(socketFD, CLT_OPENING, strlen(CLT_OPENING), 0) == -1){
      perror("send() in openingHandler gescheitert");
      return EXIT_FAILURE;
    }
    fprintf(stdout, "\nDer Server hat unseren Client v1.0 akzeptiert.\n");
    return openingHandler(socketFD, argv, subargv, gameID);
  } else if(strcmp(argv[0], SRV_ACCEPTANCE) == 0){
    sprintf(msg, "%s %s\n", "ID", gameID);
    fprintf(stdout, "Server verlangt Game-ID.\nSende dem Server die Game-%s", msg);
    size = strlen(msg);
    if(send(socketFD, msg, size, 0) == -1){
      perror("send() in openingHandler gescheitert");
      return EXIT_FAILURE;
    }
    fprintf(stdout, "\nID gesendet.\n");
    return 1;
  } else{
    int i = 0;
    while(stringSplit(argv[i++], subargv, " "));
    return 0;
  }
  
  return size;
}

int
receive(int socketFD, char **argv, char **subargv){
  char msg[BUFLEN];
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    perror("recv() gescheitert");
    return EXIT_FAILURE;
  }
  stringSplit(msg, argv, "\n");
  if(*argv[0] == '+')
    return 1;
  else if(*argv[0] == '-')
    return 0;
  else
    return EXIT_FAILURE;
  
  return EXIT_FAILURE;
}

int
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
stringSplit(char *string, char **argv, char *delim){   // argv muss schon alloziiert sein
  int i = 0;
  
  char *temp;
  temp = (char *) malloc(BUFLEN*sizeof(char));
  strcpy(temp, string);
  
  char *buffer;
  buffer = (char *) malloc(BUFLEN*sizeof(char));
  
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
