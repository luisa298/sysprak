#include "performConnection.h"



int 
performConnection(int socketFD, char *gameID){
  // benoetigte variablen
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
  
  // Version und Game-ID mit dem Server abklaeren
  readyflag = openingHandler(socketFD, gameID, argv);
  
  // Spielerdaten mit dem Server austauschen und Prolog beenden
  while(readyflag && (readyflag = receive(socketFD, argv))){
    int playerflag = 0;
    char *farbe;
    for(i = 0; i < readyflag; i++){
      stringSplit(argv[i], subargv, " ");
      if(playerflag > 1)
        playerflag = printPlayers(playerflag, subargv);
      else{  
        switch(checkMsg(subargv[1])){
          case -1:
            fprintf(stdout, "Uebler Fehler.\n\n");
            return EXIT_FAILURE;
          break;
          case 0:
            fprintf(stdout, "\nServer sendet %s. Damit ist der Prolog beendet.\n\n", subargv[1]);
            return EXIT_SUCCESS;  // TEMPORARY SOLUTION
          break;
          case 1:
            if(strcmp(subargv[2], "Dame") != 0){
              fprintf(stdout, "Der Server will %s mit uns spielen. Das wollen wir nicht.\n", subargv[2]);
              return EXIT_FAILURE;
            }
            fprintf(stdout, "\nWir spielen %s. ", subargv[2]);
            if(receive(socketFD, argv)){
              stringSplit(argv[0], subargv, " ");
              fprintf(stdout, "Der Name des Spiels lautet %s.\n\n", subargv[1]);
              sendToServer(socketFD, "PLAYER\n");
            } else
              fprintf(stdout, "Fehler beim Empfangen des Spielnamens.\n");
          break;
          case 2:
            farbe = (strcmp(subargv[3], "white")) ? "Schwarz" : "Weiss";
            fprintf(stdout, "\nSie sind Spieler Nr. %i und spielen die Farbe %s.\n\n",
              atoi(subargv[2]), farbe);
          break;
          case 3:
            playerflag = atoi(subargv[2]);
            fprintf(stdout, "Insgesamt nehmen %i Spieler am Spiel teil:\n", playerflag);
          break;
          default:
            fprintf(stdout, "Fehler, weißte Bescheid unso\n");
          break;
        }
      }
    }
//    return EXIT_SUCCESS;
  }
  
  // Behandlung von Abbruchsignal durch Server
  if(readyflag == 0){
    fprintf(stdout, "Server schickt Fehler:\n%s\n", argv[0]);
    fprintf(stdout, "\nWir bitten, dies zu entschuldigen.\n\n\n");
    return EXIT_FAILURE;
  }
  
  free(argv);
  free(subargv);
  return EXIT_SUCCESS;
}

int
openingHandler(int socketFD, char *gameID, char **argv){
  char msg[BUFLEN];
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    perror("recv() in openingHandler gescheitert");
    return EXIT_FAILURE;
  }
  stringSplit(msg, argv, "\n");
  fprintf(stdout, "Empfangene Nachricht: %s\n", argv[0]);
  if(strcmp(argv[0], SRV_OPENING) == 0){
    sendToServer(socketFD, CLT_OPENING);
    fprintf(stdout, "\nDer Server hat unseren Client v1.0 akzeptiert.\n\n");
    memset(msg, 0, BUFLEN);
    return openingHandler(socketFD, gameID, argv);
  } else if(strcmp(argv[0], SRV_ACCEPTANCE) == 0){
    sprintf(msg, "%s %s\n", "ID", gameID);
    fprintf(stdout, "Server verlangt Game-ID. ");
    sendToServer(socketFD, msg);
    fprintf(stdout, "\nID gesendet.\n");
    memset(msg, 0, BUFLEN);
    return 1;
  } else
    return 0;
}

int
receive(int socketFD, char **argv){
  int flag;
  char msg[BUFLEN];
  memset(msg, 0, BUFLEN);
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    perror("recv() gescheitert");
    return EXIT_FAILURE;
  }
//  fprintf(stdout, "Empfangene Nachricht: %s", msg);
  flag = stringSplit(msg, argv, "\n");
  
  if(*argv[0] == '+')
    return flag;
  else if(*argv[0] == '-')
    return 0;
  else
    return EXIT_FAILURE;
  
  return EXIT_FAILURE;
}

int
sendToServer(int socketFD, char *msg){
  int size = strlen(msg);
  fprintf(stdout, "Sende %s", msg);
  if(send(socketFD, msg, size, 0) == -1){
    perror("send() gescheitert");
    return -1;
  }
  return size;
}

int
checkMsg(char *msg){
  if(strcmp(msg, "ENDPLAYERS") == 0)
    return 0;
  else if(strcmp(msg, "PLAYING") == 0)
    return 1;
  else if(strcmp(msg, "YOU") == 0)
    return 2;
  else if(strcmp(msg, "TOTAL") == 0)
    return 3; 
  else return -1;
}

int
printPlayers(int count, char **argv){
  int nr = count - 1;
  char *farbe, *bereitschaft;
  
  farbe = (strcmp(argv[2], "white")) ? "Schwarz" : "Weiss";
  bereitschaft = (atoi(argv[3])) ? "ist bereit" : "ist nicht bereit";
  
  fprintf(stdout, "Spieler Nr. %i spielt %s und %s.\n", nr, farbe, bereitschaft);
  return nr;
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
    strcpy(argv[i++], buffer);
    buffer = strtok(NULL, delim);
  }
  
  free(buffer);
  free(temp);
  
  return i;
}
