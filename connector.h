#ifndef __connector_h__
#define __connector_h__

// Netzwerkverbindung zum Server mit einem Socket herstellen
int connector(settings *toUse, char *gameID, int pipe_fd[2]);

#endif
