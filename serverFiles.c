#include "serverFiles.h"

/* FIN */
/* Inicializa el servidor
 * Guarda cada usuario existente en la lista de usuarios
 * Guarda en memoria las solicitudes de amistad enviadas y pendientes 
 * Guarda en memoria los amigos de cada usuario */
int serverInit() {
	DIR *dir,*user_dir,*dir_user;
	FILE *user_pass_file;
	struct dirent *dit,*dit_user;
	char *aux_path,*pass, *name;

	aux_path = (char*)malloc(256*sizeof(char));
	//char path[100];
	int num_user = 0,num_friends = 0;
	
	//Inicializar un usuario predefinido
	User *user;
	
	//Inicializar la lista de usuarios
	numUsers = 0;
	
	printf("serverInit:\n");
	
	// Abrir el directorio usuarios
	if ((dir = opendir(DATA_PATH)) == NULL){
		sprintf(aux_path,"mkdir %s", DATA_PATH);
		system(aux_path);
		dir = opendir(DATA_PATH);
		printf("	Directorio %s creado\n", DATA_PATH);
		return 0;
	}
		
	// Buscar usuarios en el directorio del servidor y crear la lista de usuarios
	// Recorrer el directorio usuarios
	while ((dit = readdir(dir)) != NULL){
		if(strcmp(dit->d_name,".") != 0 && strcmp(dit->d_name,"..") != 0){
			printf("Leyendo usuario %s\n", dit->d_name);
			num_user++;
			// Abrir fichero con la contraseña del usuario
			sprintf(aux_path,"%s%s/password", DATA_PATH, dit->d_name);
			if((user_pass_file = fopen(aux_path, "r")) == NULL) {
				perror("Error abriendo fichero");
				return 0;
			}
			// Obtener la contraseña 
			pass = (char*)malloc(256*sizeof(char));
			fscanf(user_pass_file, "%s\\n", pass);
			// Inicializar usuario
			addUser(dit->d_name, pass);
			printf("	Añadido usuario %s con contraseña %s\n", dit->d_name, pass);
			if(fclose(user_pass_file) == -1){
				perror("Error cerrando fichero");
			}

			// Buscar solicitudes enviadas
			sprintf(aux_path, "%s%s/enviados", DATA_PATH, dit->d_name);
			if((user_pass_file = fopen(aux_path, "r")) == NULL) 
				perror("Error abriendo fichero");
			name = (char*)malloc(256*sizeof(char));
			while(!feof(user_pass_file)) {
				if(fgets(name,100,user_pass_file) != NULL) {
					name[strlen(name)-1] = '\0';
					user = userlist[num_user-1];// Coger el usuario actual
					deliverReqfriend(user,name);
					printf("	Usuario %s solicitud enviada a %s\n", user->username, name);
				}
			}
			if(fclose(user_pass_file) == -1) perror("Error abriendo fichero");

			// Buscar peticiones recibidas
			sprintf(aux_path,"%s%s/pendientes",DATA_PATH,dit->d_name);
			if((user_pass_file = fopen(aux_path, "r")) == NULL) 
				perror("Error abriendo fichero");
			while(!feof(user_pass_file)) {
				if(fgets(name,100,user_pass_file) != NULL) {
					name[strlen(name)-1] = '\0';
					user = userlist[num_user-1];// Coger el usuario actual
					printf("	Usuario %s solicitud pendiente de %s\n", user->username, name);
					deliverReqPending(user,name);
				}
			}
			if(fclose(user_pass_file) == -1) perror("Error abriendo fichero");

			// Buscar amigos
			sprintf(aux_path,"%s%s/",DATA_PATH,dit->d_name);
			if ((dir_user = opendir(aux_path)) == NULL){
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
					printf("	%s y %s son amigos\n", user->username, dit_user->d_name);
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
	
	free(aux_path);
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
	sprintf(path,"%s/%s/%s",DATA_PATH,username,"password");

	FILE *file ;
	if((file= fopen(path,"w+")) == NULL){
		perror("Error creando fichero de usuario");
	}
	if(DEBUG_MODE) printf("ims__addUser -> Path: %s\n",path);
	fprintf(file,"%s",password);
	if(fclose(file) == -1){
		perror("Error cerrando fichero de usuario");
	}
		printf("Añadido usuario: %s con contraseña: %s al servidor\n", username, password);

	return 0;
}

/* FIN */
/* Conecta un usuario al servidor */
int login(char* username, char* password) {
	int found = 0;
	int i = 0;
	User *user;
	while(i < numUsers && found == 0) {
		if(strcmp(username, userlist[i]->username) == 0) {
			found = 1;
			user = userlist[i];
		}
		i++;
	}
	// Si el usuario existe y las contraseñas coinciden, loguear al usuario
	if(found == 1 && strcmp(password, user->password) == 0) {
		user->logged = 1;
		printf("Usuario %s logueado correctamente\n", username);
		return 0;
	}
	
	printf("Login fallido\n");
	return -1;
}

/* Da de baja un usuario en el servidor, borrando toda su informacion */ 
int deleteUser(char* username) {
	int found = 0;
	int i;
	User *user;
	// Eliminar usuario en memoria
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
	//AMIGOS SIN HACER
	User *friend;
	/*for(i = 0; i < MAX_USERS; i++){
		friend = userlist[i];
		if(friend != NULL){
			if(DEBUG_MODE) printf("removeUser ->%s le borra de amigos\n",friend->nick);
			removeFriend(friend,nick);
			if(DEBUG_MODE) printf("removeUser ->%s le borra de pendientes\n",friend->nick);
			removeFriendRequestPending(friend,nick);
			if(DEBUG_MODE) printf("removeUser ->%s le borra de enviados\n",friend->nick);
			removeFriendRequestSend(friend,nick);
		}
	}*/
	
	// Eliminar directorio de usuario
	char path[50];
	sprintf(path, "rm -R %s%s", DATA_PATH, username);
	system(path);
	printf("Directorio %s eliminado\n", username);
	
	return 0;
}

/* Desconecta al usuario del servidor */
int logout(char* username){
	int found = 0;
	int i = 0;
	User *user;
	// Obtener el usuario
	while(i < numUsers && found == 0){
		if(strcmp(username, userlist[i]->username) == 0){
			found = 1;
			user = userlist[i];
		}
		i++;
	}
	// Marcar desconectado
	user->logged = 0;
	closeFiles(user);
	userFree(user);
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
	if(alreadyFriend(usr,friendname) == 1)
		return -4;
	// Insertar amigo en la lista de solicitudes pendientes	
	if(deliverReqfriend(usr, friendname) == 0) {
		FILE *file;
		char path[100];
		sprintf(path,"%s%s/enviados",DATA_PATH,username);

		if(DEBUG_MODE) printf("ims__sendFriendshipRequiest -> Path: %s\n",path);

		if((file = fopen(path, "a")) == NULL)
			perror("El fichero no existe");
		// Añadir al amigo en el fichero enviados
		fprintf(file,"%s\n",friendname);

		if(fclose(file) == -1)
			perror("El fichero no existe");
		
		// Insertar usuario en la lista de solicitudes pendientes del amigo
		if(deliverReqPending(friend, username) == 0) {
			sprintf(path,"%s%s/pendientes",DATA_PATH,friendname);

			if(DEBUG_MODE) printf("ims__sendFriendshipRequiest -> Path: %s\n",path);

			if((file = fopen(path, "a")) == NULL)
				perror("El fichero no existe");
	
			// Añadir al usuario en el fichero de pendientes del amigo
			fprintf(file,"%s\n",username);

			if(fclose(file) == -1)
				perror("El fichero no existe");

			if(DEBUG_MODE && friend != NULL) printf("ims__sendFriendshipRequiest -> %s envia peticion de amistad a %s\n",usr->username,friend->username);
		}
	}
	else 
		return -3; // El amigo ya estaba en la lista de enviados

	//}
	
	return 0;
}


/* Acepta la solicitud de amistad de otro usuario */
int acceptReq(char *username, char *friendname) {
	User *user = getUser(username);
	User *friend = getUser(friendname);
	
	if(user->logged == 1) {
		printf("Aceptar solicitud de %s a %s\n:", username, friendname);
		// Eliminar la solicitud pendiente 
		printf("Este usuario :\n");
		printUser(user);
		printf("Amigo: \n");
		printUser(friend);
		int found = deleteReqPending(user,friendname);
		if(found == 1) {
			// Eliminar la solicitud enviada
			found = deleteReqFriend(friend,user->username);
			if(found == 1)	{
				// Añadir cada usuario a su lista de amigos
				addFriend(user, friendname);
				addFriend(friend, user->username);
				char path[100];
				// Añadir un fichero con el nombre del amigo para cada usuario
				sprintf(path, "touch %s%s/%s", DATA_PATH, username, friendname);
				if(DEBUG_MODE) printf("    Creando fichero amigos path: %s\n",path);
				system(path);
				sprintf(path, "touch %s%s/%s", DATA_PATH, friendname, username);
				if(DEBUG_MODE) printf("    Creando fichero amigos path: %s\n",path);
				system(path);
				if(DEBUG_MODE) printf("    %s y %s ahora son amigos\n",user->username,friend->username);
			}
		}
	}
	else
		return -1;
	return 0;
	
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
	printf("peta aqui");
	if(usr->logged == 1)
		return usr->numPending;
	return -1;
}

int getReqs(char* user, struct Char_vector *friends){

	User *usr = getUser(user);
	if(usr->logged == 1)
	{
		getFriendRequestsPending(usr,friends->data);
		if(DEBUG_MODE)
			printf("ims__getFriendshipRequiests -> %s quiere su lista de peticiones pendientes\n",usr->username);
	}
}

/* FIN */
int deleteFriend(char* username, char* friendname) {
	User* user = getUser(username);
	
	printf("%s va a borrar al amigo %s\n", username, friendname);
	
	//if(strcmp(user,friendname) == 0){
		//return -3; 
	//}
	//if(user->logged == 1)
	//{
		// Obtener el usuario amigo y borrar de la lista de amigos de ambos usuarios
		if(alreadyFriend(user, friendname) == 1) {
			User *friend = getUser(friendname);
			rmFriend(user, friendname);
			rmFriend(friend, username);
		}
		// No encontrado
		else {
			printf("No son amigos\n");
			return -2;
		}
	//}else
	//{
		//return = -1;
	//}
	
	printf("%s ha borrado a %s\n", username, friendname);
	
	return 0;
}

int sendMessage(char *username, struct Message myMessage) {
	User* user = getUser(username);
	User* friend = getUser(myMessage.name);
	int is_friend = 0;
	printf("Usuario %s manda mensaje a %s\n", username, myMessage.name);
	if(user->logged == 1) {
		is_friend = alreadyFriend(user, myMessage.name);
		if(is_friend == 1) {
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
	printf("Cerrados\n");
	return 0;
	
}
int getFriends(char* username, struct Char_vector *friends){
	User *usr = getUser(username);
		if(usr->logged == 1)
		{
			if(usr->numFriends > 0)
			{
				char* aux;
				int i;

				for(i = 0; i < MAX_FRIENDS; i++)
				{
					aux = usr->friends[i];
					if(aux != NULL)
					{
						friends->data[i] = (char*)malloc(256*sizeof(char));
						strcpy(friends->data[i],aux);
					}
				}
			}
			return 0;
			if(DEBUG_MODE)
				printf("ims__getFriends -> %s quiere su lista de amigos\n",usr->username);
		}

		return -1;
}
int haveFriends(char* user){
	User *usr = getUser(user);

	if(usr->logged == 1)
	{
		return usr->numFriends;
	}
	else{
		return -1;
	}

}

