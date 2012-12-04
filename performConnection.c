#include "performConnection.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*
Fehlerbehandlung mit __FILE__ und __LINE__ (und __METHOD__)
strace verwenden
*/

// msg, argv, subargv werden von so gut wie allen Funktionen verwendet
char msg[BUFLEN];
char *argv[BUFLEN];
char *subargv[BUFLEN];

int 
performConnection(int socketFD, char *gameID){
  // benoetigte Variablen
  int i, readyflag, playerflag;
  char *farbe;
  readyflag = playerflag = 1;
  
  // Version und Game-ID mit dem Server abklaeren
  readyflag = openingHandler(socketFD, gameID);
  
  // Spielerdaten mit dem Server austauschen und Prolog beenden
  while(readyflag > 0 && (readyflag = receive(socketFD)) > 0){
    for(i = 0; i < readyflag; i++){
      if(!stringSplit(argv[i], subargv, " ")){
        fprintf(stderr, "fehler in line %d\n", __LINE__);
        readyflag = -1;
        break;
      }
      if(playerflag > 1)
        playerflag = printPlayers(playerflag, subargv);
      else{
        switch(checkMsg(subargv[1])){
          case -1:
            fprintf(stderr, "Unerwarte Nachricht vom Server oder Protokollfehler.\n\n");
            readyflag = -1;
          break;
          case 0:
            fprintf(stdout, "\nServer sendet %s. Damit ist der Prolog beendet.\n\n", subargv[1]);
            return EXIT_SUCCESS;  // TEMPORARY SOLUTION
          break;
          case 1:
            if(strcmp(subargv[2], GAMEKINDNAME) != 0){
              fprintf(stderr, "Server schickt %s.\n", subargv[2]);
              readyflag = -1;
              break;
            }
            fprintf(stdout, "\nWir spielen %s. ", subargv[2]);
            if(argv[++i] == NULL){
              if(receive(socketFD) > 0)
                fprintf(stdout, "Der Name des Spiels lautet %s.\n\n", argv[0]+2);
              else
                fprintf(stderr, "Fehler beim Empfangen des Spielnamens.\n");
            }
            else if(argv[i] != NULL)
              fprintf(stdout, "Der Name des Spiels lautet %s.\n\n", argv[i]+2);
            else{
              fprintf(stderr, "fehler in line %d\n", __LINE__);
              readyflag = -1;
              break;
            }
            sendToServer(socketFD, "PLAYER\n");
          break;
          case 2:
            if(strcmp(subargv[3], "white") == 0)
              farbe = "Weiss";
            else if(strcmp(subargv[3], "black") == 0)
              farbe = "Schwarz";
            else{
              fprintf(stderr, "Protokollfehler. %d\n", __LINE__);
              readyflag = -1;
              break;
            }
            if(isdigit(*subargv[2]))
              fprintf(stdout, "\nSie sind Spieler Nr. %i und spielen die Farbe %s.\n\n",
                atoi(subargv[2]), farbe);
            else
              readyflag = -1;
          break;
          case 3:
            if(isdigit(*subargv[2]))
              playerflag = atoi(subargv[2]);
            else{
              fprintf(stderr, "Protokollfehler. %d\n", __LINE__);
              readyflag = -1;
              break;
            }
            fprintf(stdout, "Insgesamt nehmen %i Spieler am Spiel teil:\n", playerflag);
          break;
          default:
            fprintf(stderr, "Fehler, weißte Bescheid unso\n");
          break;
        }
      }
    }
  }
  
  // Behandlung von Fehlermeldung vom Server
  if(readyflag == 0){
    fprintf(stderr, "Server schickt Fehler:\n%s\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  // Behandlung von Abbruchsignal
  if(readyflag == -1){
    fprintf(stderr, "Server schickt Abbruch:\n%s\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

int
openingHandler(int socketFD, char *gameID){
  int flag;
  
  // Version
  if((flag = receive(socketFD)) > 0){
    sprintf(msg, "%s %s\n", "VERSION", VERSION);
    sendToServer(socketFD, msg);
  } else    
    return flag;
  
  //GameID
  if((flag = receive(socketFD)) > 0){
    sprintf(msg, "%s %s\n", "ID", gameID);
    sendToServer(socketFD, msg);
  }
  
  return flag;
}

int
receive(int socketFD){
  int size;
  memset(msg, 0, BUFLEN);
  if((size = recv(socketFD, msg, BUFLEN-1, 0)) == -1){
    perror("perror");
    return -1;
  }
  
  size = stringSplit(msg, argv, "\n");
  
//  for(int i = 0; i < size; i++)
//    fprintf(stdout, "Empfangene Nachricht: %s\n", argv[i]);
    
  if(*argv[0] == '+')
    return size;
  else if(*argv[0] == '-')
    return 0;
  else
    return -1;
}

int
sendToServer(int socketFD, char *string){
  int size = strlen(string);
  fprintf(stdout, "Sende %s", string);
  if(send(socketFD, string, size, 0) == -1){
    perror("send() gescheitert");
    return -1;
  }
  return size;
}

int
checkMsg(char *string){
  if(strcmp(string, "ENDPLAYERS") == 0)
    return 0;
  else if(strcmp(string, "PLAYING") == 0)
    return 1;
  else if(strcmp(string, "YOU") == 0)
    return 2;
  else if(strcmp(string, "TOTAL") == 0)
    return 3; 
  else return -1;
}

int
printPlayers(int count, char *vector[]){
  int nr = count - 1;
  char *farbe, *bereitschaft;
  
  if(strcmp(vector[2], "white") == 0)
    farbe = "Weiss";
  else if(strcmp(vector[2], "black") == 0)
    farbe = "Schwarz";
  else
    return -1;
  
  if(isdigit(*vector[3]) && (atoi(vector[3]) == 0 || atoi(vector[3]) == 1))
    bereitschaft = (atoi(vector[3])) ? "ist bereit" : "ist nicht bereit";
  else
    return -1;
  
  if(isdigit(*vector[1]))
    fprintf(stdout, "Spieler Nr. %i spielt %s und %s.\n", atoi(vector[1]), farbe, bereitschaft);
  else
    return -1;
  
  return nr;
}

int
stringSplit(char *string, char *vector[], char *delim){ 
  int i = 0;
  vector[i] = strtok(string, delim);
  while(vector[i] != NULL)
    vector[++i] = strtok(NULL, delim);
  return i;
}
