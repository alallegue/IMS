#include "serverFiles.h"

/* Revisar */
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
	
	printf("serverInit\n");
	
	// Abrir el directorio server_data
	if ((dir = opendir(DATA_PATH)) == NULL){
		sprintf(aux_path,"mkdir %s",DATA_PATH);
		system(aux_path);
		//perror("opendir");
		return 0;
	}
	if(DEBUG_MODE) printf("serverInit -> Directorio: %s\n",DATA_PATH);
	
	// Buscar usuarios en el directorio del servidor y crear la lista de usuarios
	
	// Recorrer el directorio server_data
	while ((dit = readdir(dir)) != NULL){

		if(strcmp(dit->d_name,".") != 0 && strcmp(dit->d_name,"..") != 0){
			// LEER CLAVE
			num_user++;
			// Directorio del pass del usuario = aux_path
			sprintf(aux_path,"%s%s/password",DATA_PATH,dit->d_name);

			if((user_pass_file = fopen(aux_path, "r")) == NULL){
				perror("Error abriendo fichero");
				return 0;
			}

			pass = (char*)malloc(256*sizeof(char));
			fscanf( user_pass_file, "%s\\n", pass);

			addUser(dit->d_name,pass);
			if(DEBUG_MODE){
				printf("serverInit -> Se ha añadido: %s %s\n",dit->d_name,pass);
			}

			if(fclose(user_pass_file) == -1){
				perror("Error cerrando fichero");
			}

			// LEER peticiones enviadas

			// Directorio de la lista de enviados = aux_path
			sprintf(aux_path,"%s%s/enviados",DATA_PATH,dit->d_name);

			if((user_pass_file = fopen(aux_path, "r")) == NULL) perror("Error abriendo fichero");

			name = (char*)malloc(256*sizeof(char));

			while(!feof(user_pass_file))
			{
				if(fgets(name,100,user_pass_file) != NULL)
				{
					//fscanf(user_pass_file,"%s\\n",name);
					name[strlen(name)-1] = '\0';
					if(DEBUG_MODE) printf("serverInit -> Send: %s\n",name);
					user = userlist[num_user-1];// Coger el usuario actual
					//addFriendRequestSend(user,name); SIN HACER
				}
			}

			if(fclose(user_pass_file) == -1) perror("Error abriendo fichero");

			// LEER peticiones recibidas

			// Directorio de la lista de enviados = aux_path
			sprintf(aux_path,"%s%s/pendientes",DATA_PATH,dit->d_name);

			if((user_pass_file = fopen(aux_path, "r")) == NULL) perror("Error abriendo fichero");

			while(!feof(user_pass_file))
			{
				if(fgets(name,100,user_pass_file) != NULL)
				{
					//fscanf(user_pass_file,"%s\\n",name);
					name[strlen(name)-1] = '\0';
					if(DEBUG_MODE) printf("serverInit -> Pending: %s\n",name);
					user = userlist[num_user-1];// Coger el usuario actual
					//addFriendRequestPending(user,name); SIN HACER
				}
			}

			if(fclose(user_pass_file) == -1) perror("Error abriendo fichero");

			//LEER AMIGOS
			// Directorio del usuario = aux_path
			sprintf(aux_path,"%s%s/",DATA_PATH,dit->d_name);

			if ((dir_user = opendir(aux_path)) == NULL){
				perror("opendir");
				return 0;
			}
			//num_friends = 0;
			user = userlist[num_user-1];// Coger el usuario actual

			// Recorrer el directorio del usuario para añadir los amigos
			while ((dit_user = readdir(dir_user)) != NULL){
				if(strcmp(dit_user->d_name,".") != 0 && strcmp(dit_user->d_name,"..") != 0 && strcmp(dit_user->d_name,"password") != 0
				   && strcmp(dit_user->d_name,"pendientes") != 0 && strcmp(dit_user->d_name,"pendientes") != 0){
					//addFriend(user,dit_user->d_name); SIN HACER
					if(DEBUG_MODE){
						//printf("serverInit -> %s añade a %s\n",user->nick,dit_user->d_name);
					}
					//num_friends++;
				}

			}

			if (closedir(dir_user) == -1){
				perror("closedir");
				return 0;
			}
			free(pass);
			free(name);
			//user->numFriends = num_friends;
			//printf("\n%s", dit->d_name);
			//printf(" %d", dit->d_reclen);
		}

	 }
	
	free(aux_path);
	/* int closedir(DIR *dir);
	 *
	 * Close the stream to argv[1]. And check for errors. */
	if (closedir(dir) == -1){
		perror("closedir");
		return 0;
	}


	numUsers = num_user;

	if(DEBUG_MODE) printf("serverInit -> Exit Directorio: %s Users: %d\n",DATA_PATH,numUsers);

	return 0;
	
}

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

	printf("Usuario %s insertado en memoria\n",username);
	printf("Numero de usuarios en memoria: %d\n", numUsers);
	
	char path[50];
	// Crear el directorio del usuario
	sprintf(path, "%s%s", DATA_PATH, username);
	mkdir(path, 0777);
	printf("Directorio creado: %s\n",path);
	// Crear el fichero de mensajes enviados
	sprintf(path,"%s%s/%s", DATA_PATH,username,"enviados");
	creat(path, 0777);
	printf("Fichero creado: %s\n", path);
	// Crear el fichero de mensajes pendientes
	sprintf(path,"%s%s/%s",DATA_PATH,username,"pendientes");
	creat(path, 0777);
	printf("Fichero creado: %s\n", path);
	// Crear el fichero con la contraseña del usuario y almacenarla
	sprintf(path,"%s%s/%s",DATA_PATH,username,"password");
	FILE *file ;
	file= fopen(path,"w+");
	fprintf(file,"%s",password);
	printf("Fichero creado: %s\n", path);
	fclose(file);

	printf("Añadido usuario: %s con contraseña: %s al servidor\n", userlist[numUsers-1]->username, userlist[numUsers-1]->password);
	
	return 0;
}

int login(char* username, char* password) {
	int found = 0;
	int i = 0;
	User *user = NULL;
	while(i < numUsers && found == 0){
		if(strcmp(username, userlist[i]->username) == 0){
			found = 1;
			user = userlist[i];
		}
		i++;
	}
	// Si el usuario existe y las contraseñas coinciden, loguear al usuario
	if(found == 1 && strcmp(password, user->password) == 0){
		user->online = 1;
		printf("Usuario %s logueado correctamente\n", username);
		return 0;
	}
	
	printf("Login fallido\n");
	return -1;
}

