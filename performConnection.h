#ifndef __performConnection_h__
#define __performConnection_h__

int performConnection(int socketFD, char *gameKind, char *gameID);
int openingHandler(int socketFD, char *gameID); 
int checkMsg(char *string);                                      // checkt Servernachricht
int printPlayers(int count, char *vector[]);                     // printet Spielerinfo

#endif
