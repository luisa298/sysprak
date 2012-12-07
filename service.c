#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <xlocale.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "service.h"

int
checkArgs(int argc, char **argv, settings *toUse){
  if(argc == 1){
    
    fprintf(stderr, "Bitte geben Sie eine 13stellige GameID als Argument an.\n");
    fprintf(stderr, "Ausserdem kann eine Konfigurationsdatei angegeben werden.\n");
    return 0;
    
  } else if(argc > 1 && strlen(argv[1]) != 13){
    
    fprintf(stderr, "GameID muss 13stellig sein.\n");
    return 0;
    
  }
  
  if(argc == 2 && !readSettings(STDCONF, toUse)){
    
    fprintf(stderr, "Konfigurationsdatei konnte nicht gelesen werden.\n");
    return 0;
    
  }
  
  if(argc == 3 && !readSettings(argv[2], toUse)){
    
    fprintf(stderr, "Konfigurationsdatei konnte nicht gelesen werden.\n");
    return 0;
    
  }
  
  return 1;
}

int
readSettings(char *filename, settings *toUse){
  int argc;
  char buffer[STRLEN], *vec[STRLEN];
  FILE *datei;
  
  if((datei = fopen(filename, "r")) == NULL){
    printf("Fehler beim Oeffnen der Datei.\n");
    return 0;
  }
  
  while(fgets(buffer, STRLEN, datei) != NULL){
    argc = stringSplit(buffer, vec, " =\n");
    if(argc > 2) return 0;
    switch(checkSettings(vec[0])){
      case 0:
        strcpy(toUse->hostname, vec[1]);
        break;
      case 1:
        strcpy(toUse->portnumber, vec[1]);
        break;
      case 2:
        strcpy(toUse->gamekind, vec[1]);
        break;
      default:
        break;
    }
  }
  fclose(datei);
  return 1;
}

int
checkSettings(char *setting){
  if(strcmp(setting, "hostname") == 0)
    return 0;
  else if(strcmp(setting, "portnumber") == 0)
    return 1;
  else if(strcmp(setting, "gamekind") == 0)
    return 2;
  else return -1;
}

int
receive(int socketFD, char *msg, char *argv[]){
  int size;
  memset(msg, 0, STRLEN);
  if((size = recv(socketFD, msg, STRLEN-1, 0)) == -1){
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

// shared memory segment
void
shm(char *conf) {
    key_t ShmKEY;
    int ShmID;
    struct gameInfos  *gameInfo;
    struct playerInfos *playerInfo;

    // gameInfo anbinden
    ShmKEY = ftok(conf, 'x');
    ShmID = shmget(ShmKEY, sizeof(struct gameInfos), IPC_CREAT | 0666);

    if (ShmID < 0) {
         printf("shmget error\n");
         exit(1);
    }

    gameInfo = (struct gameInfos *) shmat(ShmID, NULL, 0);
    if ((int) gameInfo == -1) {
         printf("*** shmat error ***\n");
         exit(1);
    }
    printf("gameInfos an Shared Memory Segment anbinden\n");

    //playerInfo anbinden
    ShmKEY = ftok(conf, 'y');
        ShmID = shmget(ShmKEY, sizeof(struct playerInfos), IPC_CREAT | 0666);

        if (ShmID < 0) {
             printf("shmget error\n");
             exit(1);
        }

        playerInfo = (struct playerInfos *) shmat(ShmID, NULL, 0);
        if ((int) gameInfo == -1) {
             printf("*** shmat error ***\n");
             exit(1);
        }
        printf("playerInfos an Shared Memory Segment anbinden\n");
}

int
stringSplit(char *string, char *vector[], char *delim){ 
  int i = 0;
  vector[i] = strtok(string, delim);
  while(vector[i] != NULL)
    vector[++i] = strtok(NULL, delim);
  return i;
}
