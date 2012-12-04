#ifndef __service_h__
#define __service_h__

#define STRLEN 64
#define STDCONF "client.conf"
// #define STDCONF "local.conf"
#define VERSION "1.0"

typedef struct{
  char hostname[STRLEN];
  char portnumber[STRLEN];
  char gamekind[STRLEN];
} settings;
  
int checkArgs(int argc, char **argv, settings *toUse);
int readSettings(char *filename, settings *toUse);
int checkSettings(char *setting);
int receive(int socketFD, char *msg, char *argv[]);
int sendToServer(int socketFD, char *string);
int stringSplit(char *string, char *vector[], char *delim);

#endif
