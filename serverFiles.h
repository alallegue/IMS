#ifndef SERVERFILES_H
#define SERVERFILES_H_


#include "serverUsers.h"
#include "soapH.h"
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>


#define DEBUG_MODE 1
#define DATA_PATH "usuarios/"
#define MAX_FRIENDS 100
#define MAX_USERS 100


// Gestiona los ficheros del servidor
// Se encarga de crear, eliminar y modificar usuarios

// Número de usuarios en el servidor y lista de cada uno
int numUsers;
User *userlist[MAX_USERS];

int serverInit();

User* getUser(char* username);
int addUser(char* username, char* password);

int login(char* username, char* password);
int deleteUser(char* username);
int logout(char* username);

int makeReq(char* username, char* friendname);
int acceptReq(char *username, char *friendname);
int copyToFile(FILE* file, char* friends[MAXFRIENDS], int num);
int getFriendRequestsPending(User* usr,char* friends[MAX_FRIENDS]);
int haveReqs(char* user);
int getReqs(char* user,struct Char_vector *friends);
int deleteFriend(char* username, char* friendname);



#endif
