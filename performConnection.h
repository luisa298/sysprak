#ifndef __performConnection_h__
#define __performConnection_h__

#define GAMEKINDNAME "Dame"
#define BUFLEN 64
#define ARGS 8
#define VERSION "1.0"

int performConnection(int socketFD, char *gameID);
int openingHandler(int socketFD, char *gameID);
int receive(int socketFD, char *argv[]);                       // empfaengt und splittet
int sendToServer(int socketFD, char *msg);                    // sendet und fehlert bei Fehlschlag
int checkMsg(char *msg);                                      // checkt Servernachricht
int printPlayers(int count, char *argv[]);                     // printet Spielerinfo
int stringSplit(char *string, char *argv[], char *delim);      // returnt argc

#endif
