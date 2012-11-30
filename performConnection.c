#include "performConnection.h"

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

/*
Fehlerbehandlung mit __FILE__ und __LINE__ (und __METHOD__)
strace verwenden
*/

int 
performConnection(int socketFD, char *gameID){
  // benoetigte variablen
  // int i;
  char *argv[BUFLEN] = {0};
  // char *subargv[BUFLEN];
  int readyflag = 0;
  
  // Version und Game-ID mit dem Server abklaeren
  readyflag = openingHandler(socketFD, gameID);
  
  readyflag = receive(socketFD, argv);
  printf("%d: %s\n", readyflag, argv[0]);
  
  readyflag = receive(socketFD, argv);
  printf("%d: %s\n", readyflag, argv[0]);
  
  // Spielerdaten mit dem Server austauschen und Prolog beenden
  // while(readyflag > 0 && (readyflag = receive(socketFD, argv)) > 0){
  //     int argc;
  //     int playerflag = 0;
  //     printf("%s\n", argv[0]);
  //     argc = stringSplit(argv[0], subargv, " ");
  //     printf("%s\n", subargv[0]);
  //     printf("%s\n", subargv[1]);
  //       char *farbe;
  //       for(i = 0; i < readyflag; i++){
  //         argc = stringSplit(argv[i], subargv, " ");
  //         if(playerflag > 1)
  //           playerflag = printPlayers(playerflag, subargv);
  //         else{
  //           for(int j = 0; j < argc; j++) 
  //             printf("%s\n", subargv[j]);
  //           switch(checkMsg(subargv[1])){
  //             case -1:
  //               fprintf(stdout, "Uebler Fehler.\n\n");
  //               return EXIT_FAILURE;
  //             break;
  //             case 0:
  //               fprintf(stdout, "\nServer sendet %s. Damit ist der Prolog beendet.\n\n", subargv[1]);
  //               return EXIT_SUCCESS;  // TEMPORARY SOLUTION
  //             break;
  //             case 1:
  //               if(strcmp(subargv[2], GAMEKINDNAME) != 0){
  //                 fprintf(stdout, "Der Server will %s mit uns spielen. Das wollen wir nicht.\n", subargv[2]);
  //                 return EXIT_FAILURE;
  //               }
  //               fprintf(stdout, "\nWir spielen %s. ", subargv[2]);
  //               if(receive(socketFD, argv)){
  //                 stringSplit(argv[0], subargv, " ");
  //                 fprintf(stdout, "Der Name des Spiels lautet %s.\n\n", subargv[1]);
  //                 sendToServer(socketFD, "PLAYER\n");
  //               } else
  //                 fprintf(stdout, "Fehler beim Empfangen des Spielnamens.\n");
  //             break;
  //             case 2:
  //               farbe = (strcmp(subargv[3], "white")) ? "Schwarz" : "Weiss";
  //               fprintf(stdout, "\nSie sind Spieler Nr. %i und spielen die Farbe %s.\n\n",
  //                 atoi(subargv[2]), farbe);
  //             break;
  //             case 3:
  //               playerflag = atoi(subargv[2]);
  //               fprintf(stdout, "Insgesamt nehmen %i Spieler am Spiel teil:\n", playerflag);
  //             break;
  //             default:
  //               fprintf(stdout, "Fehler, weißte Bescheid unso\n");
  //             break;
  //           }
  //         }
  //       }
  // }
  
  // Behandlung von Fehlermeldung vom Server
  if(readyflag == 0){
    fprintf(stdout, "Server schickt Fehler:\n%s\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  // Behandlung von Abbruchsignal
  if(readyflag == -1){
    fprintf(stdout, "Server schickt Abbruch:\n%s\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

int
openingHandler(int socketFD, char *gameID){
  char msg[BUFLEN] = {0};
  char *argv[BUFLEN] = {0};
  
  // Version
  if(receive(socketFD, argv) > 0){
    sprintf(msg, "%s %s\n", "VERSION", VERSION);
    sendToServer(socketFD, msg);
  } else    
    return -1;
  
  //GameID
  if(receive(socketFD, argv) > 0){
    sprintf(msg, "%s %s\n", "ID", gameID);
    sendToServer(socketFD, msg);
  } else    
    return -1;
  
  return 1;
}

int
receive(int socketFD, char *argv[]){
  int flag;
  char msg[BUFLEN] = {0};
  if(recv(socketFD, msg, BUFLEN-1, 0) == -1){
    // fprintf(stdout, "error in %s at %d", __FILE__, __LINE__);
    perror("perror");
    return -1;
  }
  
  // for(int i = 0; msg[i] != '\0'; i++)
  //   printf("%c", msg[i]);
  
  flag = stringSplit(msg, argv, "\n\0");
  
  for(int i = 0; i < flag; i++)
    fprintf(stdout, "Empfangene Nachricht: %s\n", argv[i]);
  
  if(*argv[0] == '+')
    return flag;
  else if(*argv[0] == '-')
    return 0;
  else
    return -1;
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
printPlayers(int count, char *argv[]){
  int nr = count - 1;
  char *farbe, *bereitschaft;
  
  farbe = (strcmp(argv[2], "white")) ? "Schwarz" : "Weiss"; // errorhandling
  bereitschaft = (atoi(argv[3])) ? "ist bereit" : "ist nicht bereit";
  
  fprintf(stdout, "Spieler Nr. %i spielt %s und %s.\n", atoi(argv[1]), farbe, bereitschaft);
  return nr;
}

int
stringSplit(char *string, char *argv[], char *delim){ 
  int i = 0;
  argv[i] = strtok(string, delim);
  while(argv[i] != NULL)
    argv[++i] = strtok(NULL, delim);
  return i;
}
