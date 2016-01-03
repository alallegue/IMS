#include "serverUsers.h"
#include <string.h>

User* userInit(char* username,char* password){
	User *user = (User*)malloc(sizeof(User));
	user->username = (char*)malloc(256*sizeof(char));
	strcpy (user->username, username);
	user->password = (char*)malloc(256*sizeof(char));
	strcpy (user->password, password);


	int i;
	for(i = 0;i < MAXFRIENDS;i++){
		user->friends[i] = NULL;
		user->friends_request_pending[i] = NULL;
		user->friends_request_send[i] = NULL;

		/*user->files[i] = (struct Files*)malloc(sizeof(struct Files));
		user->files[i]->file = NULL;
		user->files[i]->friend_nick = NULL;*/
	}

	user->logged = 0;
	user->numFriends = 0;
	user->numSend = 0;
	user->numPending = 0;

	return user;
}

int userFree(User* user) {
	
	return 0;
}

int alreadyFriend(User* usr,char *friendname){
	int found = 0;
	int i = 0;
	char* aux;

	while(found == 0 && i < MAXFRIENDS){
		if(usr->friends[i] != NULL){
			aux = usr->friends[i];
			if(strcmp(aux,friendname) == 0){
				found = 1;
			}
		}
		i++;
	}
	return found;
}

int deliverReqfriend(User* usr,char* friendname){
	int i = 0;
	int j = -1;
	int found = 0;

	if(strcmp(usr->username,friendname) == 0) return -1;
	char* aux;
	while( i < MAXFRIENDS && found == 0)
	{
		aux = usr->friends_request_send[i];
		if(aux != NULL)
		{
			if(strcmp(aux,friendname) == 0){
				found = 1;
			}
		}else if(j == -1)
		{
			j = i;
		}
		i++;
	}
	if(found == 0){
		usr->friends_request_send[j] = (char*)malloc(256*sizeof(char));
		strcpy(usr->friends_request_send[j] , friendname);
		usr->numSend++;
		//printf("%s\n",usr->friends[j]);
	}
	return found;
}

int deliverReqPending(User* usr,char* friendname)
{
	int i = 0;
	int j = -1;
	int found = 0;

	if(strcmp(usr->username,friendname) == 0) return -1;

	char* aux;
	while( i < MAXFRIENDS && found == 0)
	{
		aux = usr->friends_request_pending[i];
		if(aux != NULL)
		{
			if(strcmp(aux,friendname) == 0){
				found = 1;
			}
		}else if(j == -1)
		{
				j = i;
		}
		i++;
	}
	if(found == 0){
		usr->friends_request_pending[j] = (char*)malloc(256*sizeof(char));
		strcpy(usr->friends_request_pending[j] , friendname);
		usr->numPending++;
		//printf("%s\n",usr->friends[j]);
	}
	return found;
}
