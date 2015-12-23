#include "serverFiles.h"


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
	
	/* Buscar usuarios en el directorio del servidor y crear la lista de usuarios
	
	// Recorrer el directorio server_data
	while ((dit = readdir(dir)) != NULL){

		if(strcmp(dit->d_name,".") != 0 && strcmp(dit->d_name,"..") != 0){
			//LEER CLAVE
			num_user++;
			// Directorio del pass del usuario = aux_path
			sprintf(aux_path,"%s%s/.pass",DATA_PATH,dit->d_name);

			if((user_pass_file = fopen(aux_path, "r")) == NULL){
				perror("Error abriendo fichero");
				return 0;
			}

			pass = (char*)malloc(256*sizeof(char));
			fscanf( user_pass_file, "%s\\n", pass);

			addUsers(luser,dit->d_name,pass);
			if(DEBUG_MODE){
				printf("serverInit -> Se ha añadido: %s %s\n",dit->d_name,pass);
			}

			if(fclose(user_pass_file) == -1){
				perror("Error cerrando fichero");
			}

			// LEER peticiones enviadas

			// Directorio de la lista de enviados = aux_path
			sprintf(aux_path,"%s%s/.send",DATA_PATH,dit->d_name);

			if((user_pass_file = fopen(aux_path, "r")) == NULL) perror("Error abriendo fichero");

			name = (char*)malloc(256*sizeof(char));

			while(!feof(user_pass_file))
			{
				if(fgets(name,100,user_pass_file) != NULL)
				{
					//fscanf(user_pass_file,"%s\\n",name);
					name[strlen(name)-1] = '\0';
					if(DEBUG_MODE) printf("serverInit -> Send: %s\n",name);
					user = luser->listU[num_user-1];// Coger el usuario actual
					addFriendRequestSend(user,name);
				}
			}

			if(fclose(user_pass_file) == -1) perror("Error abriendo fichero");

			// LEER peticiones recibidas

			// Directorio de la lista de enviados = aux_path
			sprintf(aux_path,"%s%s/.pending",DATA_PATH,dit->d_name);

			if((user_pass_file = fopen(aux_path, "r")) == NULL) perror("Error abriendo fichero");

			while(!feof(user_pass_file))
			{
				if(fgets(name,100,user_pass_file) != NULL)
				{
					//fscanf(user_pass_file,"%s\\n",name);
					name[strlen(name)-1] = '\0';
					if(DEBUG_MODE) printf("serverInit -> Pending: %s\n",name);
					user = luser->listU[num_user-1];// Coger el usuario actual
					addFriendRequestPending(user,name);
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
			user = luser->listU[num_user-1];// Coger el usuario actual

			// Recorrer el directorio del usuario para añadir los amigos
			while ((dit_user = readdir(dir_user)) != NULL){
				if(strcmp(dit_user->d_name,".") != 0 && strcmp(dit_user->d_name,"..") != 0 && strcmp(dit_user->d_name,".pass") != 0
				   && strcmp(dit_user->d_name,".pending") != 0 && strcmp(dit_user->d_name,".send") != 0){
					addFriend(user,dit_user->d_name);
					if(DEBUG_MODE){
						printf("serverInit -> %s añade a %s\n",user->nick,dit_user->d_name);
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
	 */
	
	free(aux_path);
	/* int closedir(DIR *dir);
	 *
	 * Close the stream to argv[1]. And check for errors. */
	if (closedir(dir) == -1){
		perror("closedir");
		return 0;
	}


	//userlist->numUser = num_user;

	if(DEBUG_MODE) printf("serverInit -> Exit Directorio: %s Users: %d\n",DATA_PATH,numUsers);

	return 0;
	
}

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
			return -1;
		}
		i++;
	}

	User *usr = userInit(username, password);
	userlist[numUsers] = usr;
	numUsers++;

	if(DEBUG_MODE) {
		printf("addUser -> Usuario %s creado en memoria\n",username);
		printf("num usuarios: %d\n", numUsers);
	}
	
	// Crear directorio y ficheros para el usuario:
	// Enviados, pendientes y contraseña
	//char *path = (char*)malloc(sizeof(char*));
	char path[100];

	sprintf(path,"%s%s%s","mkdir ",DATA_PATH, username);
	if(DEBUG_MODE) printf("ims__addUser -> Path: %s\n",path);
	system(path);

	sprintf(path,"%s%s%s/%s","touch ",DATA_PATH,username,".send");
	if(DEBUG_MODE) printf("ims__addUser -> Path: %s\n",path);
	system(path);

	sprintf(path,"%s%s%s/%s","touch ",DATA_PATH,username,".pending");
	if(DEBUG_MODE) printf("ims__addUser -> Path: %s\n",path);
	system(path);

	sprintf(path,"%s/%s/%s",DATA_PATH,username,".pass");
	FILE *file ;
	if((file= fopen(path,"w+")) == NULL){
		perror("Error creando fichero de usuario");
	}
	if(DEBUG_MODE) printf("ims__addUser -> Path: %s\n",path);
	fprintf(file,"%s",password);
	if(fclose(file) == -1){
		perror("Error cerrando fichero de usuario");
	}

	if(DEBUG_MODE){
		printf("ims__addUser -> Añadido: %s %s\n",userlist[numUsers-1]->username,userlist[numUsers-1]->password);
	}
	
	return 0;
}
