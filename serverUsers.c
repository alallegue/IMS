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
		
		user->doublecheck[i] = 0;
	}

	user->logged = 0;
	user->numFriends = 0;
	user->numSend = 0;
	user->numPending = 0;


	return user;
}

/* FIN */
/* Libera la memoria ocupada por un usuario */
void userFree(User* user) {
	int i;
	char* aux;
	printf("Liberando memoria del usuario: %s\n", user->username);
	for(i = 0; i < MAXFRIENDS; i++) {
		aux = user->friends[i];
		if(aux != NULL)
			free(aux);
		aux = user->friends_request_pending[i];
		if(aux != NULL)
			free(aux);
		aux = user->friends_request_send[i];
		if(aux != NULL)
			free(aux);
		free(user->files[i]);
	}
	free(user->username);
	free(user->password);
	free(user);

}

/* FIN */
/* Comprueba si el usuario ya tiene al amigo agregado en la lista de amigos 
 * Devuelve la posición del amigo en la lista de amigos si se encuentra en ella
 * ó -1 si no está */
int alreadyFriend(User* user,char *friendname){
	int found = 0;
	int i = 0;
	char* aux;
	
	while(found == 0 && i < MAXFRIENDS){
		if(user->friends[i] != NULL){
			aux = user->friends[i];
			if(strcmp(aux,friendname) == 0){
				found = 1;
			}
		}
		i++;
	}
	if(found == 1)
		return --i;
	else
		return -1;
		
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
	printf("Usuario %s ha enviado solicitud de amistad a %s\n", usr->username, friendname);
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
	printf("Usuario %s tiene solicitud pendiente de %s\n", usr->username, friendname);
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
		system(path);
		sprintf(path,"%s%s/%s",DATA_PATH,user->username,"enviados");
		FILE* file;
		if((file = fopen(path, "w")) == NULL) 
			perror("Error abriendo fichero");
		copyToFile(file,user->friends_request_send,user->numSend);
		printf("Actualizado fichero: %s%s/%s\n", DATA_PATH, user->username, "enviados");
		if(fclose(file) == -1) 
			perror("Error cerrando fichero");
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
		if(aux != NULL){
			if(strcmp(aux,friendname) == 0){
				found = 1;
			}
		}
		if(found == 0)
			i++;
	}
	if(found == 1){
		// Eliminar la solicitud en memoria
		free(user->friends_request_pending[i]);
		user->friends_request_pending[i] = NULL;
		user->numPending--;
		
		// Actualizar el fichero de solicitudes pendientes
		char* path = (char*)malloc(256*sizeof(char));
		sprintf(path,"%s%s%s/%s","rm ",DATA_PATH,user->username,"pendientes");
		FILE* file;

		sprintf(path,"%s%s/%s",DATA_PATH,user->username,"pendientes");
		printf("Actualizado fichero: %s%s/%s\n", DATA_PATH, user->username, "pendientes");
		
		if((file = fopen(path, "w")) == NULL) 
			perror("Error abriendo fichero");

		copyToFile(file,user->friends_request_pending,user->numPending);

		if(fclose(file) == -1) 
			perror("Error cerrando fichero");

		system(path);
		free(path);
	}
	return found;
}

/* FIN */
/* Descarta una petición de amistad de otro usuario */
int deleteReq(char* username, char* friendname) {
	User* user = getUser(username);
	User* friend = getUser(friendname);

	printf("Rechazar solicitud de amistad:\n");
	//if(user->online == 1) {
	if(deleteReqPending(user, friendname) == 1) {
		if(deleteReqFriend(friend, username) == 1) {
			// Borrar ficheros enviados y pendientes
			char path[100];
			sprintf(path,"%s%s%s/%s","rm ",DATA_PATH,username,"pendientes");
			system(path);
			printf("    Borrado fichero: %s\n",path);
			sprintf(path,"%s%s%s/%s","rm ",DATA_PATH,friendname,"enviados");
			system(path);
			printf("    Borrado fichero: %s\n",path);
			
			// Volver a crear enviados y pendientes con las listas de ambos usuarios
			FILE* file;
			sprintf(path,"%s%s/%s",DATA_PATH,username,"pendientes");
			printf("    Creado fichero: %s\n",path);

			if((file = fopen(path, "w")) == NULL) 
				perror("Error abriendo fichero");
			// Rellenar pendientes del usuario
			copyToFile(file,user->friends_request_pending, user->numPending);
			if(fclose(file) == -1) 
				perror("Error cerrando fichero");

			sprintf(path,"%s%s/%s",DATA_PATH,friendname,"enviados");
			printf("    Creado fichero: %s\n",path);

			if((file = fopen(path, "w")) == NULL) 
				perror("Error abriendo fichero");
			// Rellenar enviados del amigo
			copyToFile(file,friend->friends_request_send,friend->numSend);
			if(fclose(file) == -1) perror("Error cerrando fichero");

			printf("%s ha rechazado la solicitud de %s\n",username, friendname);
		}
	}
	//}
	
	return 0;
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

/* Debug */
int printUser(User* user){
	printf("Nombre: %s pass: %s logueado: %d numFriends: %d numEnviados %d numPendientes %d\n", 
		user->username, user->password, user->logged, user->numFriends, user->numSend, user->numPending);
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

int readAllFile(User* usr,char* friendname,int num,char* result) {
	printf("Leer %d mensajes del fichero de conversacion entre %s y %s\n", num, usr->username, friendname);
	char *script;
	script = (char*)malloc(256*sizeof(char));
	sprintf(script,"cat %s%s/%s | tail -n %d > %s%s/.temp",DATA_PATH,usr->username,friendname,num,DATA_PATH,usr->username);
	system(script);

	char *path;
	path = (char*)malloc(256*sizeof(char));
	sprintf(path,"%s%s/.temp",DATA_PATH,usr->username);
	FILE *file;
	if((file = fopen(path, "r")) == NULL) perror("Error abriendo fichero");
	char* aux = (char*)malloc(256*sizeof(char));

	strcpy(result,"");

	while(!feof(file)) {
		if(fgets(aux,255,file) != NULL)	{
			strcat(result,aux);
		}
	}
	free(aux);

	if(fclose(file) == -1) 
		perror("Error cerrando fichero");

	char *script2;
	script2 = (char*)malloc(256*sizeof(char));
	sprintf(script2,"rm %s%s/.temp",DATA_PATH,usr->username);

	system(script2);
	free(script);
	free(path);
	free(script2);

	printf("Obtenida conversacion\n");

	return 0;
}
