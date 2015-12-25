#include "serverUsers.h"
#include <string.h>

User* userInit(char* username,char* password){
	User *user = (User*)malloc(sizeof(User));
	user->username = (char*)malloc(256*sizeof(char));
	strcpy (user->username, username);
	user->password = (char*)malloc(256*sizeof(char));
	strcpy (user->password, password);

	/*
	int i;
	for(i = 0;i < MAXFRIENDS;i++){
		user->friends[i] = NULL;
		user->friends_request_pending[i] = NULL;
		user->friends_request_send[i] = NULL;

		user->files[i] = (struct Files*)malloc(sizeof(struct Files));
		user->files[i]->file = NULL;
		user->files[i]->friend_nick = NULL;
	}
	*/
	user->online = 0;
	//user->numFriends = 0;
	//user->numSend = 0;
	//user->numPending = 0;

	return user;
}

int userFree(User* user) {
	
	return 0;
}
