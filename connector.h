#ifndef __connector_h__
#define __connector_h__

#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de" // Hostname des Spielservers
//#define HOSTNAME "127.0.0.1"
#define PORTNUMBER "1357" // Portnummer des Spielservers

#define GAMEID "50b4aec484805"

// Netzwerkverbindung zum Server mit einem Socket herstellen
int connector();

#endif
