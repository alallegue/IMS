
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

int procesaMensaje(struct Message message) {
	char usuario[200];	
	switch(message.operation) {
		case 1: //Alta usuario
			strcpy(usuario, message.name);
			printf("%s\n", usuario);
			break;
		case 2: //Baja usuario
			
			break;
		default:break;
	}

	return 0;
}

int ims__sendMessage (struct soap *soap, struct Message myMessage, int *result){
	*result = procesaMensaje(myMessage);	
	//printf ("Received by server: \n\tusername:%s \n\tmsg:%s\n toperation:%d\n", myMessage.name, myMessage.msg, myMessage.operation);
	return SOAP_OK;
}

int ims__receiveMessage (struct soap *soap, struct Message *myMessage){

	// Allocate space for the message field of the myMessage struct then copy it
	myMessage->msg = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	// Not necessary with strcpy since uses null-terminated strings
	// memset(myMessage->msg, 0, IMS_MAX_MSG_SIZE);
	strcpy (myMessage->msg, "Invoking the remote function receiveMessage simply retrieves this standard message from the server"); // always same msg

	// Allocate space for the name field of the myMessage struct then copy it
	myMessage->name = (xsd__string) malloc (IMS_MAX_NAME_SIZE);
	// Not necessary with strcpy since uses null-terminated strings
	// memset(myMessage->name, 0, IMS_MAX_NAME_SIZE);  
	strcpy(myMessage->name, "aServer");	

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
<<<<<<< HEAD
/*int ims__listReq(struct soap* soap,char* user ,cString* fl,int *result)
{
	*result= listReqs(user ,fl);
	return SOAP_OK;
}*/
int ims__haveFriendshipRequest(struct soap *soap, char* user,int *result)
{
	*result=haveReqs(user);
	return SOAP_OK;
}
int ims__getFriendshipRequests(struct soap *soap, char* user,struct Char_vector *friends)
{
	printf("llego la peticion");
	getReqs(user,friends);
	return SOAP_OK;
}

/*int ims__getFriendshipRequest(struct soap *soap, char* user,String* friend_nick)
{
	User *usr = getUser(user);
	if(usr->logged == 1)
	{
		//int found =
		//char* aux = (char*)malloc(sizeof(char*));
		getFriendRequestPending(usr,&friend_nick->str);
		//friend_nick->name = aux;
		if(DEBUG_MODE) printf("ims__getFriendshipRequest -> Peticion %s\n",friend_nick->str);
		//*friend_nick = *aux;
		//printf("Primera peticion %s\n",friend_nick);
	}
	return SOAP_OK;
}*/
=======

int ims__acceptReq(struct soap* soap, char* username, char* friendname, int *error) {
	*error = acceptReq(username, friendname);
	return SOAP_OK;
}

int ims__cancelReq(struct soap* soap, char* username, char* friendname, int *error) {

	return SOAP_OK;
}
>>>>>>> refs/remotes/origin/pruebas

