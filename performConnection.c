#include "performConnection.h"

int 
performConnection(int socketFD, char *gameID){
  // benoetigte variablen
  char recv_msg[BUFLEN], *send_msg;
  send_msg = (char *) malloc(BUFLEN*sizeof(char));
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
  ssize_t size;
  
  // opening
  if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
    perror("error recv()ing in opening");
    return EXIT_FAILURE;
  }
  if(strcmp(recv_msg, SRV_OPENING) == 0){
    fprintf(stdout, "Der Server v1.0 akzeptiert Verbindungen.\n");
    size = strlen(CLT_OPENING);
    strncpy(send_msg, CLT_OPENING, size);
    fprintf(stdout, "Sende dem Server %s", send_msg);
    if(send(socketFD, send_msg, size, 0) == -1){
      perror("error send()ing");
      fprintf(stdout, "%s", send_msg);
      return EXIT_FAILURE;
    }
  } else{
    fprintf(stdout, "Unerwartete Nachricht vom Server bei Opening\n");
    return EXIT_FAILURE;
  }
  memset(recv_msg, 0, BUFLEN);
  memset(send_msg, 0, BUFLEN);
  
  // acceptance
  if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
    perror("error recv()ing in acceptance");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "%s", recv_msg);    // wohlformatierung sicherstellen :P
  if(strcmp(recv_msg, SRV_ACCEPTANCE) == 0){
    sprintf(send_msg, "%s %s\n", "ID", gameID);
    fprintf(stdout, "Sende dem Server die Game-%s", send_msg);
    if(send(socketFD, send_msg, strlen(send_msg), 0) == -1){
      perror("error send()ing");
      return EXIT_FAILURE;
    }
  } else{
    fprintf(stdout, "Unerwartete Nachricht vom Server bei Acceptance\n");
    return EXIT_FAILURE;
  }
  memset(recv_msg, 0, BUFLEN);
  memset(send_msg, 0, BUFLEN);
  
  // PLAYING empfangen
  if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
    perror("error recv()ing in PLAYING");
    return EXIT_FAILURE;
  }
  recv_msg[size] = '\0';
  stringSplit(recv_msg, argv);
  if(*argv[0] == '+' && strcmp(argv[1], "PLAYING") == 0){
    fprintf(stdout, "Wir spielen %s", argv[2]);
    memset(recv_msg, 0, BUFLEN);
  }
  
  // Spielname empfangen
  if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
    perror("error recv()ing in PLAYING (inner section)");
    return EXIT_FAILURE;
  }
  stringSplit(recv_msg, argv);
  if(*argv[0] == '+' && argv[1] != NULL){
    fprintf(stdout, "Der Name des Spiels lautet %s", argv[1]);
  } else{
      fprintf(stdout, "server schickt fehler\n");
      return EXIT_FAILURE;
  }
  memset(recv_msg, 0, BUFLEN);
    
  // PLAYER schicken
  strcpy(send_msg, "PLAYER\n");
  size = strlen(send_msg);
  fprintf(stdout, "Sende %s", send_msg);
  if(send(socketFD, send_msg, size, 0) == -1){
    perror("send()ing von PLAYER gescheitert");
    return EXIT_FAILURE;
  }
  memset(send_msg, 0, BUFLEN);
  
  // YOU empfangen
  if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
    perror("error recv()ing in YOU");
    return EXIT_FAILURE;
  }
//  fprintf(stdout, "%s", recv_msg);
  stringSplit(recv_msg, argv);
  if(*argv[0] == '+' && strcmp(argv[1], "YOU") == 0){
    fprintf(stdout, "Sie sind Spieler %i. Sie spielen die Farbe %s", atoi(argv[2]), argv[3]);
  } else{
      fprintf(stdout, "server schickt fehler\n");
      return EXIT_FAILURE;
  }
  memset(recv_msg, 0, BUFLEN);
    
  // TOTAL empfangen
  if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
    perror("error recv()ing in TOTAL");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "%s", recv_msg);
//  stringSplit(recv_msg, argv);
  // if(*argv[0] == '+' && strcmp(argv[1], "TOTAL") == 0){
  //   fprintf(stdout, "Gesamtzahl Spieler: %i\n", atoi(argv[2]));
  // } else{
  //     fprintf(stdout, "server schickt fehler\n");
  //     return EXIT_FAILURE;
  // }
  memset(recv_msg, 0, BUFLEN);
  
  // // anderen Spieler empfangen
  // if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
  //   perror("error recv()ing in TOTAL(1)");
  //   return EXIT_FAILURE;
  // }
  // fprintf(stdout, "%s", recv_msg);
  // stringSplit(recv_msg, argv);
  // if(*argv[0] == '+'){
  //   fprintf(stdout, "Spieler %i spielt %s und ist Bereit bei 1, Nicht Bereit bei 0: %i\n", atoi(argv[1]), argv[2], atoi(argv[3]));
  // } else{
  //     fprintf(stdout, "server schickt fehler\n");
  //     return EXIT_FAILURE;
  // }
  // memset(recv_msg, 0, BUFLEN);
  // 
  // // ENDPLAYERS empfangen
  // if((size = recv(socketFD, recv_msg, BUFLEN-1, 0)) == -1){
  //   perror("error recv()ing in ENDPLAYERS");
  //   return EXIT_FAILURE;
  // }
  // fprintf(stdout, "%s", recv_msg);
  // stringSplit(recv_msg, argv);
  // if(*argv[0] == '+' && strcmp(argv[1], "ENDPLAYERS\n") == 0){
  //   fprintf(stdout, "Prolog beendet.\n");
  //   return EXIT_SUCCESS;
  // } else{
  //     fprintf(stdout, "server schickt fehler\n");
  //     return EXIT_FAILURE;
  // }
  // memset(recv_msg, 0, BUFLEN);
  
  // fehlerbehandlung draussen
  if(size == -1){
    perror("error while recv()ing");
    return EXIT_FAILURE;
  }
  
  // server sendet fehlermessage
  if(recv_msg[0] == '-'){
    stringSplit(recv_msg, argv);
    if(strcmp(argv[1], "TIMEOUT") == 0){
      perror("server sendet TIMEOUT");
      return EXIT_FAILURE;
    } else{
      perror("server sendet fehler");
      return EXIT_FAILURE;
    }
  }
  
  return EXIT_SUCCESS;
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
