#include "soapH.h"
#include "imsService.nsmap"

#define DEBUG_MODE 1
#define NUM_MESSAGES 5
char *username, *password;

/* FIN */
/* Pedir al servidor iniciar sesion con un usuario existente */
int login(struct soap soap, char *serverURL){
	char* un;
	char* pass;
	int res = 1;
	
	un = (char*)malloc(256*sizeof(char));
	pass = (char*)malloc(256*sizeof(char));
	
	if(!DEBUG_MODE) system("clear");
	printf("Acceder a una cuenta ya existente\n");
	printf("	Introduce nombre de usuario: ");
	scanf("%s", un);
	printf("	Introduce contraseña: ");
	scanf("%s", pass);

	soap_call_ims__login(&soap, serverURL, "", un, pass, &res);

	switch(res) {
		case -2:
			printf("Ya estas logueado en otra maquina\n");
			free(un);
			free(pass);
			break;
		case -1:
			printf("Nombre de usuario o contraseña incorrectos\n");
			free(un);
			free(pass);
			break;
		case 1:
			printf("No hay conexion con el servidor\n");
			break;
		default:
			username = un;
			password = pass;
			printf("Logueado correctamente como %s\n", username);
			break;
	}
	
	return res;
}

/* FIN */
/* Pedir al servidor registrar un nuevo usuario */ 
void registerUser(struct soap soap, char *serverURL) {
	char* un;
	char* pass;
	int res = 1;
	
	un = (char*)malloc(256*sizeof(char));
	pass = (char*)malloc(256*sizeof(char));
	
	printf("Introduce nombre de usuario: ");
	scanf("%s", un);
	printf("Introduce contraseña: ");
	scanf("%s", pass);
	
	soap_call_ims__registerUser(&soap, serverURL, "", un, pass, &res);
	
	switch(res) {
		case 0:
			printf("Usuario registrado correctamente. Selecciona login para acceder a tu cuenta\n");
			break;
		case -1:
			printf("Se ha alcanzado el limite de usuarios. No puede registrarse\n");
			break;
		case -2:
			printf("Ya existe un usuario con el mismo nombre\n");
			break;
		default:
			printf("No hay conexion con el servidor\n");
			break;
	}
	free(un);
	free(pass);
}

/* FIN */
/* Baja de usuario. Pedir al servidor eliminar el usuario logueado */
int deleteUser(struct soap soap, char *serverURL){
	char conf;
	int error = 1;
	printf("¿Seguro que quieres darte de baja? Perderas todos los mensajes y amigos guardados (s/n)\n");
	scanf(" %c", &conf);
	if(conf == 's'){
		soap_call_ims__deleteUser(&soap, serverURL, "", username, &error);
		if(error == 0) {
			printf("Te has dado de baja\n");
			free(username);
			free(password);
			return 1;
		}
		else printf("No hay conexion con el servidor\n");
	}
	return 0;
}

/* FIN */
/* Pedir al servidor desconectar al usuario */
int logout(struct soap soap,char* serverURL) {
	int error = 1;
	soap_call_ims__logout(&soap, serverURL, "", username, &error);
	if(error == 0) {
		free(username);
		free(password);
		printf("Te has desconectado\n");
		return 1;
	}
	printf("No hay conexion con el servidor\n");
	return 0;
}

/* FIN */
/* Envía un mensaje a un amigo */
void sendMessage(struct soap soap,char *serverURL) {
	struct Message myMessage;
	myMessage.name = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	myMessage.msg = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	int error = 1;
	
	// Obtener amigo destinatario
	printf("¿A quien vas a escribir? ");
	scanf("%s", myMessage.name);
	// Obtener el mensaje
	printf("Escribe el mensaje:\n");
	while(getchar() != '\n');
	fgets(myMessage.msg, IMS_MAX_MSG_SIZE, stdin);
	// Quitar el salto de línea
	char *clean;
	clean = strchr(myMessage.msg, '\n');
	if (clean)
		*clean = '\0';
	// Enviar mensaje al servidor
	soap_call_ims__sendMessage(&soap, serverURL, "", username, myMessage, &error);
	// Gestión de errores
	switch(error) {
		case 0:
			printf("Mensaje enviado con exito\n");
			break;
		case 1:
			printf("No hay conexion con el servidor\n\n");
			break;
		case -1:
			printf("Solo puedes enviar mensajes a tus amigos\n");
			break;
		case -2:
			printf("No estas online.\n");
			break;
		default:
			break;
	}
	free(myMessage.name);
	free(myMessage.msg);

}

/* Muestra la conversación con un amigo */
void readMessage(struct soap soap,char *serverURL) {
	struct Message myMessage;
	myMessage.name = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	myMessage.msg = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	myMessage.operation = 1;

	printf("Escriba el nombre de su amigo: ");
	scanf("%s", myMessage.name);

	soap_call_ims__receiveMessage(&soap,serverURL,"", username,myMessage.name,NUM_MESSAGES,&myMessage);

	if(myMessage.operation == 0){
		printf("%s\n",myMessage.msg);
	}
	else if(myMessage.operation == 1){
		printf("Hay un problema con la conexion\n");
	}
	else if(myMessage.operation == -1){
		printf("No estas conectado\n");
	}
	else if(myMessage.operation == -2){
		printf("Ese no es tu amigo\n");
	}
	else if(myMessage.operation == -3){
		printf("No puedes leer mensajes de ti mismo, buscate un amigo\n");
	}
	free(myMessage.name);
	free(myMessage.msg);

}

/* FIN */
/* Envía una solicitud de amistad a otro usuario */
void sendReq(struct soap soap, char *serverURL) {
	int error;
	char* friendname=(char*)malloc(256*sizeof(char));
	printf("Introduce el nombre de usuario a agregar: \n");
	scanf("%s",friendname);
	soap_call_ims__sendReq(&soap, serverURL, "", username, friendname, &error);
	switch(error) {
		case 0:
			printf("Solicitud enviada\n");
			break;
		case -5:
			printf("Has alcanzado el limite de amigos\n");
			break;
		case -3:
			printf("Usuario no valido\n");
			break;
		case -4: 
			printf("Ese usuario ya es tu amigo\n");
			break;
		case 1:
			printf("No hay conexion con el servidor\n");
			break;
		case -6:
			printf("El otro usuario ha alcanzado el limite de amigos\n");
			break;
		default:
			break;
	}
	free (friendname);
}

/* FIN */
/* Acepta una solicitud de amistad enviada por otro usuario */
void acceptReq(struct soap soap,char *serverURL) {
	char* friendname;
	int res = 1;
	
	friendname = (char*)malloc(256*sizeof(char));
	
	printf("Introduce nombre de solicitud: ");
	scanf("%s", friendname);
	
	printf("¿Aceptas o rechazas la solicitud? (a/r/c): \n");
	char ar;
	scanf(" %c", &ar);
	
	if(ar == 'a') {
		soap_call_ims__acceptReq(&soap, serverURL,"",username,friendname,&res);
		if(res == 0)
			printf("%s agregado a amigos\n", friendname);
		else if(res == -2)
			printf("Usuario no valido\n");
		else
			printf("No hay conexion con el servidor\n");
	}
	else if(ar == 'r') {
		soap_call_ims__cancelReq(&soap, serverURL,"",username ,friendname,&res);
		if(res == 0)
			printf("Has rechazado la solicitud de %s\n", friendname);
		else
			printf("No hay conexion con el servidor\n");
	}
	else
		printf("Operacion cancelada\n");
	 
}

/*Solicitar la lista de peticiones de amistad al servidor*/
void listReq(struct soap soap, char *serverURL) {
	struct Char_vector* friends = (struct Char_vector*)malloc(sizeof(struct Char_vector));
	int i;
	int numRequestPending = -2;
	//comprobar si existe alguna peticion
	soap_call_ims__haveFriendshipRequest(&soap, serverURL,"",username,&numRequestPending);
	printf("numero de peticiones: %d\n", numRequestPending);
	if(numRequestPending > 0)
	{
		//si existen peticiones, imprimirlas por pantalla
		soap_call_ims__getFriendshipRequests(&soap, serverURL, "",username ,friends);
		printf("Lista de amistades sin aceptar:\n");

		for(i=0;i < 100;i++){
			if(friends->data[i] != NULL){
				printf("%d: %s\n",i,friends->data[i]);
			}
		}
	}
	else if(numRequestPending == -2){
		printf("Error del servidor\n");
	}
	else if(numRequestPending == -1){
			printf("No estas online\n");
	}
	else{
		printf("No tienes peticiones de amistad\n");
	}
	free(friends);

}

/* Borra a un amigo de la lista de amigos del usuario */
void deleteFriend(struct soap soap, char* serverURL) {
	char *friendname = (char*)malloc(256*sizeof(char));
	int error = 1;
	printf("Introduce el nombre del amigo que quieres borrar: \n");
	scanf("%s", friendname);

	soap_call_ims__deleteFriend(&soap, serverURL, "", username, friendname, &error);

	if(error == 1){
		printf("No hay conexion con el servidor\n");
	}
	else if (error == -1){
		printf("No estas conectado\n");
	}
	else if(error == -2){
		printf("Ese usuario no es tu amigo\n");
	}
	else if(error == 0){
		printf("%s ya no es tu amigo\n", friendname);
	}
}

/*Solicita la lista de amigos al servidor*/
void listFriends(struct soap soap, char *serverURL) {
	struct Char_vector* friends = (struct Char_vector*)malloc(sizeof(struct Char_vector));
	int i;
	int numFriends = -2;
	//comprobar el numero de amigos
	soap_call_ims__haveFriends(&soap, serverURL,"",username,&numFriends);
	if(numFriends > 0)	{
		//si existe uno o mas amigos mostrar por pantalla la lista
		soap_call_ims__getFriends(&soap, serverURL, "",username ,friends);

		printf("Lista de amigos:\n");
		for(i=0;i < 100;i++){
			if(friends->data[i] != NULL){
				printf("%s (%d)\n", friends->data[i], friends->msgcheck[i]);
			}
		}
	}else if (numFriends == 0)
	{
		//system("clear");
		printf("No se encontro nigun amigo :S\n");
	}
	else if (numFriends == -2){
		//system("clear");
		printf("Error del servidor\n");
	}
	else if (numFriends == -1){
		//system("clear");
		printf("No estas conectado \n");
	}
	free(friends);

}

/* Muestra el menú principal donde el usuario puede interactuar
 * con todas las funciones de la aplicación */
void show_menu(struct soap soap, char *serverURL) {
	int sel;
	int loop = 0;
	
	while(loop == 0) {
		printf("	Hola %s. Selecciona una opcion:\n", username);
		printf("	1) Enviar mensaje a otro usuario\n");
		printf("	2) Mostrar mensajes\n");
		printf("	3) Listar usuarios amigos\n");
		printf("	4) Enviar solicitud de amistad\n");
		printf("	5) Ver solicitudes de amistad pendientes\n");
		printf("	6) Aceptar solicitudes de amistad\n");
		printf("	7) Borrar amigo\n");
		printf("	8) Dar de baja esta cuenta\n");
		printf("	0) Salir\n");
		scanf("%d", &sel);
		system("clear");
		switch(sel) {
			case 1:
				sendMessage(soap, serverURL);
				break;
			case 2:
				readMessage(soap, serverURL);
				break;
			case 3:
				listFriends(soap, serverURL);
				break;
			case 4:
				sendReq(soap, serverURL);
				break;
			case 5:
				listReq(soap, serverURL);
				break;
			case 6:
				acceptReq(soap, serverURL);
				break;
			case 7:
				deleteFriend(soap, serverURL);
				break;
			case 8:
				loop = deleteUser(soap, serverURL);
				break;
			case 0:
				loop = logout(soap, serverURL);
				break;
			default:
				printf("Opcion no valida, vuelve a intentarlo\n");
				break;
		}
	}
}

/* Menú de bienvenida. 
 * Es el primero que se muestra y solicita
 * al usuario crear una cuenta nueva o iniciar sesión con una ya creada*/
void show_login(struct soap soap, char *serverURL) {
	int sel;
	int loop = 0;
	
	while(loop == 0) {
		printf("	Bienvenido. Selecciona una opcion: \n");
		printf("	1) Entrar\n");
		printf("	2) Registrarse\n");
		printf("	0) Salir\n");

		scanf("%d", &sel);
		system("clear");
		switch(sel) {
			case 1:
				//Si se loguea correctamente entrar al menú principal
				if(login(soap, serverURL) == 0) 
					show_menu(soap, serverURL);
				break;
			case 2:
				registerUser(soap,serverURL);
				break;
			case 0:
				printf("Adios!\n");
				loop = 1;
				break;
			default:
				break;
		}
	}
}

/* FIN */
int main(int argc, char **argv){
	struct soap soap;
	char *serverURL;
	int res;

	// Usage
	if (argc != 2) {
		printf("Usage: %s http://server:port\n",argv[0]);
		exit(0);
	}
	// Init gSOAP environment
	soap_init(&soap);
	// Obtain server address
	serverURL = argv[1];
	
	//Mostrar menú de bienvenida
	show_login(soap, serverURL);
	
	// Clean the environment
  	soap_end(&soap);
  	soap_done(&soap);
	
	return 0;
}
