#ifndef __performConnection_h__
#define __performConnection_h__

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

#define GAMEKINDNAME Dame

#define BUFLEN 128
#define ARGS 6

#define SRV_OPENING "+ MNM Gameserver v1.0 accepting connections"
#define CLT_OPENING "VERSION 1.0\n"
#define SRV_ACCEPTANCE "+ Client version accepted - please send Game-ID to join"

int performConnection(int socketFD, char *gameID);
int openingHandler(int socketFD, char *gameID, char **argv);
int receive(int socketFD, char **argv);                       // empfaengt und splittet
int sendToServer(int socketFD, char *msg);                    // sendet und fehlert bei Fehlschlag
int checkMsg(char *msg);                                      // checkt Servernachricht
int printPlayers(int count, char **argv);                     // printet Spielerinfo
int stringSplit(char *string, char **argv, char *delim);      // returnt argc

#endif
