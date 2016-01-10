#ifndef SERVERUSERS_H
#define SERVERUSERS_H_
#include "soapH.h"
#include <stdio.h>
#define DEBUG_MODE 1
#define DATA_PATH "usuarios/"
#define MAXFRIENDS 100


struct Files{
	FILE *file;
	char *friendname;
};

typedef struct {
	char* username;
	char* password;
	int logged;

	int numFriends;
	int numSend;
	int numPending;

	struct Files *files[MAXFRIENDS];

	char* friends[MAXFRIENDS];//friends who accepted your friendship request
	char* friends_request_send[MAXFRIENDS];//friends that you've sent friendship request
	char* friends_request_pending[MAXFRIENDS];//friends who have sent friendship request to you
	
	int doublecheck[MAXFRIENDS];

}User;


User* userInit(char* username, char* password);
void userFree(User* user);

int alreadyFriend(User* usr,char *friendname);
int deliverReqfriend(User* usr,char* friendname);
int deliverReqPending(User* usr,char* friendname);
int addFriend(User* usr,char* friendname);

int deleteReqFriend(User* user, char* friendname);
int deleteReqPending(User* user, char* friendname);
int deleteReq(char* username, char* friendname);

int rmFriend(User* user, char* friendname);
int printUser(User* user);
int writeToFile(FILE* file, char* friends[MAXFRIENDS], int num);
int readAllFile(User* usr,char* friend_nick,int num,char* result);
#endif
