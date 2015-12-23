#ifndef SERVERFILES_H
#define SERVERFILES_H_


#include "serverUsers.h"
#include "soapH.h"
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>


#define DEBUG_MODE 1
#define DATA_PATH "server_data/"
#define MAXFRIENDS 100
#define  MAXUSER 100



//Gestiona los ficheros del servidor
//Se encarga de crear, eliminar y modificar usuarios

typedef struct {
	int numUser;
	User *listU[MAXUSER];
}UserList;

int serverInit(UserList* userlist);
int addUser(UserList* userlist, char* username, char* password);



#endif
