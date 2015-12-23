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
#define MAX_FRIENDS 100
#define MAX_USERS 100


// Gestiona los ficheros del servidor
// Se encarga de crear, eliminar y modificar usuarios

/*
typedef struct {
	int numUser;
	User *listU[MAXUSER];
}UserList;
*/

// Número de usuarios en el servidor y lista de cada uno
int numUsers;
User *userlist[MAX_USERS];


int serverInit();
int addUser(char* username, char* password);



#endif
