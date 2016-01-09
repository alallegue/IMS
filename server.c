
#include "soapH.h"
#include "imsService.nsmap"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "serverFiles.h"

#define DEBUG_MODE 1

int main(int argc, char **argv){ 
	int m, s;
	struct soap soap;
  	if (argc < 2) {
    	printf("Usage: %s <port>\n",argv[0]); 
		exit(-1); 
  	}
	// Init environment
  	soap_init(&soap);
	// Bind to the specified port	
	m = soap_bind(&soap, NULL, atoi(argv[1]), 100);
	// Check result of binding		
	if (m < 0) {
  		soap_print_fault(&soap, stderr); exit(-1); 
	}

	//Inicializar servidor
	serverInit(userlist);

	// Listen to next connection
	while (1) { 
		// accept
	  	s = soap_accept(&soap);    
	  	if (s < 0) {
			soap_print_fault(&soap, stderr); exit(-1);
	  	}
		// Execute invoked operation
	  	soap_serve(&soap);
		// Clean up!
	  	soap_end(&soap);
	}

  return 0;
}


int ims__sendMessage (struct soap *soap, char* username, struct Message myMessage, int *result){
	*result = sendMessage(username, myMessage);
	return SOAP_OK;
}


int ims__receiveMessage(struct soap *soap, char* username, char* friendname, int numMensajes, struct Message *myMessage) {
	myMessage->operation=receiveMessage(username,numMensajes, friendname, myMessage);
	return SOAP_OK;
}


int ims__registerUser(struct soap *soap, char* username, char* password, int *error) {
	*error = addUser(username, password);
	return SOAP_OK;
}

int ims__login(struct soap *soap, char* username, char* password, int *error) {
	*error = login(username, password);
	return SOAP_OK;
}

int ims__deleteUser(struct soap *soap, char* username, int *error) {
	*error = deleteUser(username);	
	return SOAP_OK;
}

int ims__logout(struct soap *soap, char* username, int *error) {
	*error = logout(username);
	return SOAP_OK;
}

int ims__sendReq(struct soap* soap, char* username, char* friendname, int *error){
	*error = makeReq(username,friendname);
	return SOAP_OK;
}

int ims__haveFriendshipRequest(struct soap *soap, char* user,int *result)
{
	*result=haveReqs(user);
	return SOAP_OK;
}
int ims__getFriendshipRequests(struct soap *soap, char* user,struct Char_vector *friends)
{
	getReqs(user,friends);
	return SOAP_OK;
}

int ims__acceptReq(struct soap* soap, char* username, char* friendname, int *error) {
	*error = acceptReq(username, friendname);
	return SOAP_OK;
}

int ims__cancelReq(struct soap* soap, char* username, char* friendname, int *error) {
	*error = deleteReq(username, friendname);
	return SOAP_OK;
}

int ims__deleteFriend(struct soap* soap, char* username, char* friendname, int *error) {
	*error = deleteFriend(username, friendname);
	return SOAP_OK;
}

int ims__haveFriends(struct soap *soap, char* user,int *result){
	*result=haveFriends(user);
	return SOAP_OK;
}

int ims__getFriends(struct soap *soap, char* user ,struct Char_vector* friends){
	getFriends(user, friends);
	return SOAP_OK;
}
