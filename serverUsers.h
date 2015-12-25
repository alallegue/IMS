#ifndef SERVERUSERS_H
#define SERVERUSERS_H_
#include "soapH.h"
#include <stdio.h>
#define DEBUG_MODE 1
#define DATA_PATH "usuarios/"
#define MAXFRIENDS 100



struct Files{
	FILE *file;
	char *friend_nick;
};

typedef struct {
	char* username;
	char* password;
	int online;

	//int numFriends;
	//int numSend;
	//int numPending;

	//struct Files *files[MAXFRIENDS];

	//char* friends[MAXFRIENDS];//friends who accepted your friendship request
	//char* friends_request_send[MAXFRIENDS];//friends that you've sent friendship request
	//char* friends_request_pending[MAXFRIENDS];//friends who have sent friendship request to you

}User;


User* userInit(char* username, char* password);



#endif
