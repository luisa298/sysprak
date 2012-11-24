#ifndef __connector_h__
#define __connector_h__

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

#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define PORTNUMBER "1357"

int connector();

#endif
