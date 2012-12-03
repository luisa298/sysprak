#ifndef __performConnection_h__
#define __performConnection_h__

#define GAMEKINDNAME "Dame"
#define BUFLEN 64
#define VERSION "1.0"

int performConnection(int socketFD, char *gameID);
int openingHandler(int socketFD, char *gameID);
int receive(int socketFD);            // empfaengt und splittet
int sendToServer(int socketFD, char *string);                    // sendet und fehlert bei Fehlschlag
int checkMsg(char *string);                                      // checkt Servernachricht
int printPlayers(int count, char *vector[]);                     // printet Spielerinfo
int stringSplit(char *string, char *vector[], char *delim);      // returnt argc

#endif
