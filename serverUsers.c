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

		user->files[i] = (struct Files*)malloc(sizeof(struct Files));
		user->files[i]->file = NULL;
		user->files[i]->friendname = NULL;
	}

	user->logged = 0;
	user->numFriends = 0;
	user->numSend = 0;
	user->numPending = 0;

	return user;
}

/* SIN HACER */
int userFree(User* user) {
	
	return 0;
}

/* FIN */
/* Comprueba si el usuario ya tiene al amigo agregado en la lista de amigos */
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

/* FIN */
/* Guarda en memoria, en la lista de solicitudes enviadas a un amigo */
int deliverReqfriend(User* usr,char* friendname){
	int i = 0;
	int j = -1;
	int found = 0;
	
	// Comprobar que el usuario no se quiera añadir a si mismo
	if(strcmp(usr->username,friendname) == 0) 
		return -1;
	char* aux;
	// Comprobar que el amigo no esté ya guardado en la lista de enviados
	while(i < MAXFRIENDS && found == 0){
		aux = usr->friends_request_send[i];
		if(aux != NULL) {
			if(strcmp(aux,friendname) == 0)
				found = 1;
		}
		else if(j == -1)
			j = i;
		i++;
	}
	// Guardar el amigo en la lista de enviados
	if(found == 0){
		usr->friends_request_send[j] = (char*)malloc(256*sizeof(char));
		strcpy(usr->friends_request_send[j] , friendname);
		usr->numSend++;
		//printf("%s\n",usr->friends[j]);
	}
	return found;
}

/* FIN */
/* Guarda en memoria la petición de amistad pendiente enviada por otro usuario*/
int deliverReqPending(User* usr,char* friendname) {
	int i = 0;
	int j = -1;
	int found = 0;

	if(strcmp(usr->username,friendname) == 0) 
		return -1;

	char* aux;
	while( i < MAXFRIENDS && found == 0) {
		aux = usr->friends_request_pending[i];
		if(aux != NULL)	{
			if(strcmp(aux,friendname) == 0)
				found = 1;
		}
		else if(j == -1)
			j = i;
		i++;
	}
	// Guardar el amigo en la lista de solicitudes pendientes
	if(found == 0){
		usr->friends_request_pending[j] = (char*)malloc(256*sizeof(char));
		strcpy(usr->friends_request_pending[j] , friendname);
		usr->numPending++;
		//printf("%s\n",usr->friends[j]);
	}
	return found;
}

/* FIN */
/* Añade un usuario a la lista de amigos aceptados */
int addFriend(User* user,char* friendname){
	int i = 0;
	int j = -1;
	int found = 0;

	// Un usuario no puede agregarse a si mismo
	if(strcmp(user->username, friendname) == 0){
		return -1; 
	}

	// Comprobar que el amigo no estaba agregado
	char* aux;
	while( i < MAXFRIENDS && found == 0){
		aux = user->friends[i];
		if(aux != NULL){
			if(strcasecmp(aux,friendname) == 0){
				found = 1;
			}
		} // Revisar
		else if(j == -1){
				j = i;
		}
		i++;
	}
	if(found == 0){
		user->friends[j] = (char*)malloc(256*sizeof(char));
		strcpy(user->friends[j] , friendname);
		user->numFriends++;
		//printf("%s\n",usr->friends[j]);
		printf("%s y %s son amigos\n", user->username, user->friends[j]);
	}

	return found;
}

/* FIN */
/* Elimina un usuario de la lista de solicitudes enviadas y 
 * actualiza el fichero correspondiente */
int deleteReqFriend(User* user,char* friendname) {
	int i = 0;
	int found = 0;
	char* aux;
	// Buscar la posicion del amigo en la lista de solicitudes enviadas
	while(i < MAXFRIENDS && found == 0){
		aux = user->friends_request_send[i];
		if(aux != NULL){
			if(strcmp(aux, friendname) == 0){
				found = 1;
			}
		}
		if(found == 0)
			i++;
	}
	if(found == 1){
		// Eliminar la solicitud de memoria
		free(user->friends_request_send[i]);
		user->friends_request_send[i] = NULL;
		user->numSend--;
		
		// Actualizar el fichero de solicitudes enviadas
		char* path = (char*)malloc(256*sizeof(char));
		sprintf(path,"%s%s%s/%s","rm ",DATA_PATH,user->username,"enviados");
		if(DEBUG_MODE) printf("removeFriendshipRequestSend -> Borrando fichero enviados path: %s\n",path);
		system(path);
		sprintf(path,"%s%s/%s",DATA_PATH,user->username,"enviados");
		FILE* file;
		if(DEBUG_MODE) printf("removeFriendshipRequestSend -> Rescribiendo fichero enviados path: %s\n",path);

		if((file = fopen(path, "w")) == NULL) perror("Error abriendo fichero");

		copyToFile(file,user->friends_request_send,user->numSend);

		if(fclose(file) == -1) perror("Error cerrando fichero");
		free(path);
	}
	return found;
}

/* FIN */
/* Elimina un usuario de la lista de solicitudes pendientes 
 * y actualiza el fichero correspondiente */
int deleteReqPending(User* user,char* friendname) {
	int i = 0;
	int found = 0;
	char* aux;
	// Buscar la posicion del amigo en la lista de solicitudes pendientes
	while(i < MAXFRIENDS && found == 0){
		aux = user->friends_request_pending[i];
		printf("%s  ", aux);
		if(aux != NULL){
			if(strcmp(aux,friendname) == 0){
				found = 1;
			}
		}
		if(found == 0)
			i++;
	}
	printf("deleteReqPending %d\n", found);
	if(found == 1){
		// Eliminar la solicitud en memoria
		free(user->friends_request_pending[i]);
		user->friends_request_pending[i] = NULL;
		user->numPending--;
		
		// Actualizar el fichero de solicitudes pendientes
		char* path = (char*)malloc(256*sizeof(char));
		sprintf(path,"%s%s%s/%s","rm ",DATA_PATH,user->username,"pendientes");
		if(DEBUG_MODE) printf("removeFriendshipRequestSend -> Borrando fichero pendientes path: %s\n",path);
		FILE* file;

		sprintf(path,"%s%s/%s",DATA_PATH,user->username,"pendientes");
		if(DEBUG_MODE) printf("removeFriendshipRequestSend -> Rescribiendo fichero pendientes path: %s\n",path);

		if((file = fopen(path, "w")) == NULL) perror("Error abriendo fichero");

		copyToFile(file,user->friends_request_pending,user->numPending);

		if(fclose(file) == -1) perror("Error cerrando fichero");

		system(path);
		free(path);
	}
	return found;
}

/* MEJORAR */ 
/* Elimina un amigo de la lista de amigos del usuario dado */
int rmFriend(User* user, char* friendname) {
	int i = 0;
	int found = 0;
	char* aux;
	while(i < MAXFRIENDS && found == 0){
		aux = user->friends[i];
		if(aux != NULL) {
			if(strcmp(aux, friendname) == 0){
				found = 1;
			}
		}
		//if(found == 0)
			i++;
	}
	if(found == 1){
		// Eliminar amigo en memoria
		free(user->friends[i-1]);
		user->friends[i-1] = NULL;
		user->numFriends--;
		// Eliminar el fichero del amigo
		char *path = (char*)malloc(256*sizeof(char));
		sprintf(path,"rm %s%s/%s",DATA_PATH, user->username, friendname);
		system(path);
		free(path);
	}
	return found;
	
}

int printUser(User* user){
	printf("Nombre: %s pass: %s logueado: %d numFriends: %d numEnviados %d numPendientes %d\n", user->username, user->password, user->logged, user->numFriends, user->numSend, user->numPending);

/*
	char* friends[MAXFRIENDS];//friends who accepted your friendship request
	char* friends_request_send[MAXFRIENDS];//friends that you've sent friendship request
	char* friends_request_pending[MAXFRIENDS];//friends who have sent friendship request to you
	*/
}
int copyToFile(FILE* file,char* friends[MAXFRIENDS],int num) {
	if(num > 0) {
		char* aux;
		int i;
		for(i = 0; i < MAXFRIENDS; i++) {
			aux = friends[i];
			if(aux != NULL) {
				fprintf(file,"%s\n",aux);
			}
		}
	}
	return 0;
}
int readAllFile(User* usr,char* friendname,int num,char* result)
{
	/*
	size_t needed = snprintf(NULL, 0, "%s: %s (%d)", msg, strerror(errno), errno);
	    char  *buffer = malloc(needed);
	    snprintf(buffer, needed, "%s: %s (%d)", msg, strerror(errno), errno);
	*/
	char *script;
	//int size = snprintf(NULL,0,"cat %s%s/%s | tail -n %d > %s%s/.temp",DATA_PATH,usr->nick,friend_nick,num,DATA_PATH,usr->nick);
	script = (char*)malloc(256*sizeof(char));
	sprintf(script,"cat %s%s/%s | tail -n %d > %s%s/.temp",DATA_PATH,usr->username,friendname,num,DATA_PATH,usr->username);
	system(script);


	char *path;
	//size = snprintf(NULL,0,"%s%s/.temp",DATA_PATH,usr->nick);
	path = (char*)malloc(256*sizeof(char));
	sprintf(path,"%s%s/.temp",DATA_PATH,usr->username);
	FILE *file;
	if((file = fopen(path, "r")) == NULL) perror("Error abriendo fichero");
	if(DEBUG_MODE) printf("readDownTo -> Fichero abierto\n");
	char* aux = (char*)malloc(256*sizeof(char));

	strcpy(result,"");

	while(!feof(file))
	{
		if(fgets(aux,255,file) != NULL)
		{
			//name[strlen(name)-1] = '\0';
			//if(DEBUG_MODE) printf("readDownTo -> Fichero linea %s\n",aux);
			//sprintf(result,"%s%s\n",result,aux);
			strcat(result,aux);
			//if(DEBUG_MODE) printf("readDownTo -> Result %s\n",result);
		}
	}
	free(aux);

	if(fclose(file) == -1) perror("Error cerrando fichero");

	char *script2;
	//size = snprintf(NULL,0,"rm %s%s/.temp",DATA_PATH,usr->nick);
	script2 = (char*)malloc(256*sizeof(char));
	sprintf(script2,"rm %s%s/.temp",DATA_PATH,usr->username);

	system(script2);
	free(script);
	free(path);
	free(script2);

	printf("Leyendo archivo de conversaciones %s\n",result);

	return 0;
}
