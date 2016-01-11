#include "soapH.h"
#include "imsService.nsmap"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "serverFiles.h"

// Concurrencia 
pthread_mutex_t mutex;
struct soap soapptr;
void signal_kill_handler(int sig);

void *serve_clients(struct soap *soap){
	struct soap *aux_soap = soap_copy(soap);

	pthread_mutex_lock (&mutex);
  	soap_serve(aux_soap);
	pthread_mutex_unlock (&mutex);
	soap_end(aux_soap);

}

void signal_kill_handler(int sig)
{

	printf("Cerrando el servidor por Ctrl+C\n");

	pthread_mutex_destroy(&mutex);
	//serverFree();

	soap_end(&soapptr);

	exit(1);
}

int main(int argc, char **argv){ 
	int m, s;
	struct soap soap;
	struct soap soap_aux;
	
	pthread_t idHilo;
	signal(SIGINT, signal_kill_handler);
	pthread_mutex_init (&mutex, NULL);
	
  	if (argc < 2) {
    	printf("Usage: %s <port>\n",argv[0]); 
		exit(-1); 
  	}
	// Init environment
  	soap_init(&soap);
  	soapptr=soap;
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
	  	
	  	// Concurrencia
	  	soap_aux = soap;
	  	pthread_create (&idHilo, NULL, serve_clients, &soap_aux);
	  	
		// Execute invoked operation
	  	soap_serve(&soap);
		// Clean up!
	  	soap_end(&soap);
	}
	
	pthread_mutex_destroy(&mutex);
	//serverFree();
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

int ims__haveMessages(struct soap *soap, char* user, int *result) {
	*result = haveMessages(user);
	return SOAP_OK;
}
