#include "serverFiles.h"

/* FIN */
/* Inicializa el servidor
 * Guarda cada usuario existente en la lista de usuarios
 * Guarda en memoria las solicitudes de amistad enviadas y pendientes 
 * Guarda en memoria los amigos de cada usuario */
int serverInit() {
	DIR *dir,*user_dir,*dir_user;
	FILE *passFile;
	struct dirent *dit,*dit_user;
	char *pass, *name;

	char path[100];
	int num_user = 0,num_friends = 0;
	
	//Inicializar un usuario predefinido
	User *user;
	//Inicializar numero usuarios en el servidor
	numUsers = 0;

	printf("serverInit:\n");
	// Abrir el directorio usuarios
	if ((dir = opendir(DATA_PATH)) == NULL){
		sprintf(path,"mkdir %s", DATA_PATH);
		system(path);
		dir = opendir(DATA_PATH);
		printf("Directorio %s creado\n", DATA_PATH);
		printf("serverInit: servidor inicializado correctamente sin usuarios\n");
		return 0;
	}
		
	// Buscar usuarios en el directorio del servidor y crear la lista de usuarios
	// Recorrer el directorio usuarios
	while ((dit = readdir(dir)) != NULL){
		if(strcmp(dit->d_name,".") != 0 && strcmp(dit->d_name,"..") != 0){
			num_user++;
			// Abrir fichero con la contraseña del usuario
			sprintf(path,"%s%s/password", DATA_PATH, dit->d_name);
			if((passFile = fopen(path, "r")) == NULL) {
				perror("Error abriendo fichero");
				return 0;
			}
			// Obtener la contraseña 
			pass = (char*)malloc(256*sizeof(char));
			fscanf(passFile, "%s\\n", pass);
			// Inicializar usuario
			addUser(dit->d_name, pass);
			if(fclose(passFile) == -1){
				perror("Error cerrando fichero");
			}

			// Buscar solicitudes enviadas
			sprintf(path, "%s%s/enviados", DATA_PATH, dit->d_name);
			if((passFile = fopen(path, "r")) == NULL) 
				perror("Error abriendo fichero");
			name = (char*)malloc(256*sizeof(char));
			while(!feof(passFile)) {
				if(fgets(name,100,passFile) != NULL) {
					name[strlen(name)-1] = '\0';
					user = userlist[num_user-1];// Coger el usuario actual
					deliverReqfriend(user,name);
				}
			}
			if(fclose(passFile) == -1) 
				perror("Error abriendo fichero");

			// Buscar peticiones recibidas sin aceptar
			sprintf(path,"%s%s/pendientes",DATA_PATH,dit->d_name);
			if((passFile = fopen(path, "r")) == NULL) 
				perror("Error abriendo fichero");
			while(!feof(passFile)) {
				if(fgets(name,100,passFile) != NULL) {
					name[strlen(name)-1] = '\0';
					user = userlist[num_user-1];// Coger el usuario actual
					deliverReqPending(user,name);
				}
			}
			if(fclose(passFile) == -1) perror("Error abriendo fichero");

			// Buscar amigos
			sprintf(path,"%s%s/",DATA_PATH,dit->d_name);
			if ((dir_user = opendir(path)) == NULL){
				perror("opendir");
				return 0;
			}
			user = userlist[num_user-1];

			// Recorrer el directorio del usuario para añadir los amigos
			while ((dit_user = readdir(dir_user)) != NULL){
				if(strcmp(dit_user->d_name,".") != 0 && strcmp(dit_user->d_name,"..") != 0 && 
					strcmp(dit_user->d_name,"password") != 0 && strcmp(dit_user->d_name,"enviados") != 0 && 
					strcmp(dit_user->d_name,"pendientes") != 0) {
					addFriend(user, dit_user->d_name);
				}
			}

			if (closedir(dir_user) == -1){
				perror("closedir");
				return 0;
			}
			free(pass);
			free(name);
		}
	}
	
	if (closedir(dir) == -1){
		perror("closedir");
		return 0;
	}

	numUsers = num_user;
	printf("serverInit: servidor inicializado correctamente con %d usuarios\n", numUsers);

	return 0;
}

/* FIN */
/* Obtiene un usuario por su nombre */
User* getUser(char* username){
	int found = 0;
	int i = 0;
	User *usr = NULL;
	//printf("numero:%d\n",numUsers);
	while(i < numUsers && found == 0){
		//printf("i:%d\n",i);
		//printf("nombre: %s\n",userlist[i]->username);
		if(strcmp(username,userlist[i]->username) == 0){
			found = 1;
			usr = userlist[i];
		}
		i++;
	}
	//printf("sale: %s\n",usr->username);
	return usr;
}

/* FIN */
/* Guarda un usuario en el servidor, en memoria y creando los ficheros necesarios */
int addUser(char* username, char* password) {
	printf("Insertar usuario %s\n", username);
	// Insertar usuario en memoria
	if(numUsers >= MAX_USERS){
		printf("El servidor ha alcanzado el límite de usuarios\n");
		return -1; 
	}
	
	int i = 0;
	while(i < numUsers){
		if(strcmp(username,userlist[i]->username) == 0){
			printf("El usuario ya estaba registrado\n");
			return -2;
		}
		i++;
	}

	User *user = userInit(username, password);
	userlist[numUsers] = user;
	numUsers++;

	printf("Usuario %s insertado en memoria\n", userlist[numUsers-1]->username);
	printf("Numero de usuarios en memoria: %d\n", numUsers);
	
	char path[100];
	// Crear el directorio del usuario
	sprintf(path,"%s%s%s","mkdir ",DATA_PATH,username);
	system(path);
	printf("Directorio creado: %s\n",path);
	// Crear el fichero de solicitudes enviadas
	sprintf(path,"%s%s%s/%s","touch ",DATA_PATH,username,"enviados");
	system(path);
	printf("Fichero creado: %s\n", path);
	// Crear el fichero de solicitudes pendientes de aceptar
	sprintf(path,"%s%s%s/%s","touch ",DATA_PATH,username,"pendientes");
	system(path);
	printf("Fichero creado: %s\n", path);
	// Crear el fichero con la contraseña del usuario y almacenarla
	sprintf(path,"%s%s/%s",DATA_PATH,username,"password");

	FILE *file ;
	if((file= fopen(path,"w+")) == NULL){
		perror("Error creando fichero de usuario");
	}
	fprintf(file,"%s",password);
	printf("Fichero creado: %s\n", path);
	if(fclose(file) == -1){
		perror("Error cerrando fichero de usuario");
	}
	printf("Añadido usuario: %s con contraseña: %s al servidor\n", username, password);

	return 0;
}

/* FIN */
/* Conecta un usuario al servidor */
int login(char* username, char* password) {
	User *user = getUser(username);
	if(user != NULL) {
		// Comprobar que no estaba previamente logueado
		if(user->logged == 1)
			return -2;
		// Si el usuario existe y las contraseñas coinciden, loguear al usuario
		if(strcmp(password, user->password) == 0) {
			user->logged = 1;
			printf("Usuario %s logueado correctamente\n", username);
			return 0;
		}
	}
	printf("No se encuentra el usuario: %s\n", username);
	return -1;
}

/* FIN */
/* Da de baja un usuario en el servidor, borrando toda su informacion */ 
int deleteUser(char* username) {
	int found = 0;
	int i;
	User *user;
	// Eliminar usuario en memoria y recolocar la lista de usuarios
	for(i = 0; i < numUsers; i++){
		if(found == 0) {
			if(strcmp(username, userlist[i]->username) == 0) {
				found = 1;
				user = userlist[i];
				userlist[i] = userlist[i+1];
			}
		}
		else {
			userlist[i] = userlist[i+1];
		}
	}
	userFree(user);
	numUsers--;
	printf("Usuario %s eliminado de memoria\n", username);
	
	//Borrar el usuario de las listas de amigos
	User *friend;
	for(i = 0; i < MAX_USERS; i++){
		friend = userlist[i];
		if(friend != NULL){
			if(rmFriend(friend, username))
				printf("%s borra de amigos a %s\n",friend->username, username);
			if(deleteReqPending(friend, username))
				printf("%s borra de pendientes a %s\n",friend->username, username);
			if(deleteReqFriend(friend, username))
				printf("%s borra de enviados a %s\n",friend->username, username);
		}
	}
	
	// Eliminar directorio de usuario
	char path[50];
	sprintf(path, "rm -R %s%s", DATA_PATH, username);
	system(path);
	printf("Directorio %s eliminado\n", username);
	
	return 0;
}

/* FIN */
/* Desconecta al usuario del servidor */
int logout(char* username){
	int found = 0;
	int i = 0;
	User *user = getUser(username);
	// Marcar desconectado, cerrar ficheros y liberar memoria
	user->logged = 0;
	closeFiles(user);
	//userFree(user);
	printf("Usuario %s desconectado\n", username);
	
	return 0;
}

/* El usuario hace una solicitud de amistad a otro usuario */
int makeReq(char* username, char* friendname){
	User *usr = getUser(username);
	User *friend = getUser(friendname);

	// Comprobar que hay hueco para nuevos amigos
	if(usr->numFriends == MAXFRIENDS)
		return -5;
	// Comprobar que el amigo existe	
	if(friend == NULL)
		return -3;
	//else if(usr->logged == 1) {
	
	// Comprobar que el amigo tiene hueco en la lista de amigos
	if(friend->numFriends == MAXFRIENDS)
		return -6;
	// Comprobar que el amigo no este en la lista de amigos	
	if(alreadyFriend(usr,friendname) >= 0)
		return -4;
	// Insertar amigo en la lista de solicitudes pendientes	
	if(deliverReqfriend(usr, friendname) == 0) {
		FILE *file;
		char path[100];
		sprintf(path,"%s%s/enviados",DATA_PATH,username);
		if((file = fopen(path, "a")) == NULL)
			perror("El fichero no existe");
		// Añadir al amigo en el fichero enviados
		fprintf(file,"%s\n",friendname);

		if(fclose(file) == -1)
			perror("El fichero no existe");
		
		// Insertar usuario en la lista de solicitudes pendientes del amigo
		if(deliverReqPending(friend, username) == 0) {
			sprintf(path,"%s%s/pendientes",DATA_PATH,friendname);
			if((file = fopen(path, "a")) == NULL)
				perror("El fichero no existe");
	
			// Añadir al usuario en el fichero de pendientes del amigo
			fprintf(file,"%s\n",username);

			if(fclose(file) == -1)
				perror("El fichero no existe");

			if(friend != NULL) printf("%s envio una peticion de amistad a %s\n",usr->username,friend->username);
		}
	}
	else 
		return -3; // El amigo ya estaba en la lista de enviados

	//}
	
	return 0;
}

/* FIN */
/* Acepta la solicitud de amistad de otro usuario */
int acceptReq(char *username, char *friendname) {
	User *user = getUser(username);
	User *friend = getUser(friendname);
	
	printf("Aceptar solicitud de %s a %s:\n", username, friendname);
	if(user != NULL && friend != NULL) {
		// Eliminar la solicitud pendiente del usuario
		if(deleteReqPending(user,friendname) == 1) {
			// Eliminar la solicitud enviada del amigo
			if(deleteReqFriend(friend,username) == 1) {
				// Añadir cada usuario a su lista de amigos
				addFriend(user, friendname);
				addFriend(friend, user->username);
				// Eliminar solicitudes de ambos usuarios
				deleteReqPending(friend, username);
				deleteReqFriend(user, friendname);
				char path[100];
				// Añadir un fichero con el nombre del amigo para cada usuario
				sprintf(path, "touch %s%s/%s", DATA_PATH, username, friendname);
				printf("Creando fichero: %s\n",path);
				system(path);
				sprintf(path, "touch %s%s/%s", DATA_PATH, friendname, username);
				printf("Creando fichero: %s\n",path);
				system(path);
				printf("    %s y %s ahora son amigos\n",user->username,friend->username);
			}
		}
	}
	else {
		printf("Usuario no valido\n");
		return -2;
	}
	return 0;
	
}


				
int getFriendRequestsPending(User* usr,char* friends[MAX_FRIENDS])
{
	if(usr->numPending > 0)
	{
		char* aux;
		int i;

		for(i = 0; i < MAX_FRIENDS; i++)
		{
			aux = usr->friends_request_pending[i];

			if(aux != NULL)
			{
				friends[i] = (char*)malloc(256*sizeof(char));
				strcpy(friends[i],aux);
			}
		}
	}
	return 0;
}


int haveReqs(char* user){
	User *usr = getUser(user);
	if(usr->logged == 1)
		return usr->numPending;
	return -1;
}

int getReqs(char* user, struct Char_vector *friends){

	User *usr = getUser(user);
	if(usr->logged == 1)
	{
		getFriendRequestsPending(usr,friends->data);
		printf("Enviando lista de peticiones de amistad de: %s\n",usr->username);
		return 0;
	}
	return -1;
}

/* FIN */
/* Borra un amigo de la lista de amistad */
int deleteFriend(char* username, char* friendname) {
	User* user = getUser(username);
	
	printf("%s va a borrar al amigo %s\n", username, friendname);
	// Obtener el usuario amigo y borrar de la lista de amigos de ambos usuarios
	if(alreadyFriend(user, friendname) >= 0) {
		User *friend = getUser(friendname);
		rmFriend(user, friendname);
		rmFriend(friend, username);
	}
	// No encontrado
	else {
		printf("%s y %s no eran amigos\n", username, friendname);
		return -2;
	}
	
	printf("%s ha borrado a %s\n", username, friendname);
	
	return 0;
}

/* username envía el mensaje myMessage.msg al usuario myMessage.name */
int sendMessage(char *username, struct Message myMessage) {
	User* user = getUser(username);
	User* friend = getUser(myMessage.name);
	printf("Usuario %s manda mensaje a %s\n", username, myMessage.name);
	if(user->logged == 1) {
		if(alreadyFriend(user, myMessage.name) >= 0) {
			char path[100];
			// Obtener la ruta del usuario con el fichero amigo
			sprintf(path, "%s%s/%s", DATA_PATH, username, myMessage.name);
									
			// Buscar el fichero de mensajes del amigo y abrirlo
			int pos = -1;
			FILE* file;					
			if(isFileOpen(user, myMessage.name, &pos) == 0) {
				if((file = fopen(path, "a+")) == NULL) 
					perror("Error abriendo fichero");

				user->files[pos]->friendname = (char*)malloc(256*sizeof(char));
				strcpy(user->files[pos]->friendname, friend->username);

				user->files[pos]->file = file;
			}else {
				file = user->files[pos]->file;
			}
			// Escribir el mensaje en el fichero
			fprintf(file, "%s : %s\n", username, myMessage.msg);
			fflush(file);
			printf("Escrito mensaje en el fichero %s\n", path);
						
			// Obtener la ruta del amigo con el fichero del usuario
			sprintf(path,"%s%s/%s", DATA_PATH, myMessage.name, username);
			
			// Buscar el fichero de mensajes del usuario y abrirlo
			pos = -1;
			if(isFileOpen(friend, username, &pos) == 0) {
				if((file = fopen(path, "a+")) == NULL) 
					perror("Error abriendo fichero");
				friend->files[pos]->friendname = (char*)malloc(256*sizeof(char));
				strcpy(friend->files[pos]->friendname,user->username);
				friend->files[pos]->file = file;
			}
			else {
				file = friend->files[pos]->file;
			}
			fprintf(file,"%s : %s\n", username, myMessage.msg);
			fflush(file);
			printf("Escrito mensaje en el fichero %s\n", path);
			
			// Doublecheck
			friend->doublecheck[pos]++;
			printf("%s tiene %d mensajes sin leer de %s\n", friend->username, friend->doublecheck[pos], username);
			
			return 0;
		}
		else {
			return -1;
		}
	}
	else {
		return -2;
	}

	
	
}


int isFileOpen(User* user,char* friendname, int *pos) {
	char* aux;
	int i = 0;
	int found = 0;
	*pos = -1;
	while( i < MAXFRIENDS && found == 0) {
		aux = user->files[i]->friendname;
		if(aux != NULL)	{
			if(strcasecmp(aux, friendname) == 0) {
				found = 1;
				*pos = i;
			}
		}else
		{
			if(*pos == -1)*pos = i;
		}
		i++;
	}
	return found;
	
}

int closeFiles(User* user) {
	printf("Cerrando ficheros abiertos de %s\n", user->username);
	char* aux;
	FILE *file;
	int i;
	for(i = 0; i < MAXFRIENDS; i++)	{
		aux = user->files[i]->friendname;
		if(aux != NULL)	{
			file = user->files[i]->file;
			if(fclose(file) == -1) 
				perror("Error cerrando fichero");
			free(user->files[i]->friendname);
		}
	}
	return 0;
	
}


int getFriends(char* username, struct Char_vector *friends){
	User *user = getUser(username);
	if(user->numFriends > 0) {
		char* aux;
		int i;

		for(i = 0; i < MAX_FRIENDS; i++) {
			aux = user->friends[i];
			if(aux != NULL)	{
				friends->data[i] = (char*)malloc(256*sizeof(char));
				strcpy(friends->data[i],aux);
				friends->msgcheck[i] = user->doublecheck[i];
			}
		}
	}
	return 0;
	
	printf("Enviando lista de amigos de: %s \n", username);
}


int haveFriends(char* user){
	User *usr = getUser(user);

	if(usr->logged == 1)
	{
		printf("%s tiene %d amigos \n",usr->username, usr->numFriends);
		return usr->numFriends;
	}
	else{
		return -1;
	}

}

int receiveMessage (char* user,int num,char* friendname,struct Message *myMessage){
	User *usr = getUser(user);
	User *friend = getUser(friendname);
	printf("Leer mensajes de %s\n",friendname);

	if(strcmp(user,friendname) == 0){
		return -3;
	}
	if(usr->logged == 1) {
		int friendpos = alreadyFriend(usr,friendname);
		if(friendpos >= 0) {
			myMessage->msg = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
			readAllFile(usr,friendname,num,myMessage->msg);
			
			// Doublecheck
			usr->doublecheck[friendpos] = 0;
			printf("%s tiene %d mensajes sin leer de %s\n", user, usr->doublecheck[friendpos], friendname);
			
			return 0;
		}else
		{
			return -2;// No es amigo
		}
	}else
	{
		return -1;// No esta online
	}


}

