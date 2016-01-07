#include "soapH.h"
#include "imsService.nsmap"

#define DEBUG_MODE 1

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
int registerUser(struct soap soap, char *serverURL) {
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
	
	return 0;
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

int sendMessage(struct soap soap, char *serverURL) {
	struct Message myMsgA, myMsgB;
	
	/*
	printf("Mensaje a enviar: ");
	scanf("%s", string);
	
	// Allocate space for the message field of myMsgA then copy into it
	myMsgA.msg = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	// Not necessary with strcpy since uses null-terminated strings
	// memset(myMsgA.msg, 0, IMS_MAX_MSG_SIZE); 
	strcpy (myMsgA.msg, msg);

	// Allocate space for the name field of myMsgA then copy into it
	myMsgA.name = (xsd__string) malloc (IMS_MAX_NAME_SIZE);
	// Not necessary with strcpy since uses null-terminated strings
	// memset(myMsgA.name, 0, IMS_MAX_NAME_SIZE);
	strcpy (myMsgA.name, username);

	myMsgA.operation = op;

	// Send the contents of myMsgA to the server
		soap_call_ims__sendMessage (&soap, serverURL, "", myMsgA, &res);
			
	// Check for errors...
	if (soap.error) {
		soap_print_fault(&soap, stderr); 
		exit(1);
	}
	else {
		printf("Exito\n");
	}

	// Receive a Message struct from the server into myMsgB
		//soap_call_ims__receiveMessage (&soap, serverURL, "", &myMsgB);

	// Check for errors...
	if (soap.error) {
		soap_print_fault(&soap, stderr); 
		exit(1);
	}
	else{}
		//printf ("Received from server: \n\tusername: %s \n\tmsg: %s\n", myMsgB.name, myMsgB.msg);    
		
	*/	
	return 0;
}

/* Envía una solicitud de amistad a otro usuario */
int sendReq(struct soap soap, char *serverURL) {
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
<<<<<<< HEAD
			printf("Ese usuario ya es tu amigo\n");break;
=======
			printf("Ese usuario ya es tu amigo\n");
			break;
>>>>>>> refs/remotes/origin/pruebas
		case 1:
			printf("No hay conexion con el servidor\n");
			break;
		case -6:
<<<<<<< HEAD
			printf("El otro usuario ha alcanzado el limite de amigos\n");break;
=======
			printf("El otro usuario ha alcanzado el limite de amigos\n");
			break;
>>>>>>> refs/remotes/origin/pruebas
		default:
			break;
	}
	free (friendname);
	return 1;
}

/* Acepta una solicitud de amistad enviada por otro usuario */
int acceptReq(struct soap soap,char *serverURL) {
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
		else
			printf("Algo ha fallado\n");
	}
	else if(ar == 'r') {
		soap_call_ims__cancelReq(&soap, serverURL,"",username ,friendname,&res);
		if(res == 0)
			printf("Solicitud de %s rechazada\n", friendname); 
	}
	else
		printf("Operacion cancelada\n");
	 
}

/* Menú de bienvenida. 
 * Es el primero que se muestra y solicita
 * al usuario crear una cuenta o iniciar con una ya creada*/
int show_login(struct soap soap, char *serverURL) {
	int op = -1;
	int res = -1;
	
	while(op != 0 && res != 0){
		if(!DEBUG_MODE) system("clear");
		printf("	Bienvenido. Selecciona una opcion: \n");
		printf("	1) Entrar\n");
		printf("	2) Registrarse\n");
		printf("	0) Salir\n");

		scanf("%d",&op);
		switch(op) {
			case 1: //Login
				res = login(soap, serverURL);
				//Si se loguea correctamente entrar al menú principal
				if(res == 0) 
					show_menu(soap, serverURL);
				res = 1;
				op = -1;
				break;
			case 2: //Registro
				registerUser(soap,serverURL);
				op = -1;
				res = -1;
				break;
			case 0:
				printf("Adios!\n");
				break;
			default:
				break;
		}
	}

	return res;
}
int listReq(struct soap soap, char *serverURL) {
	struct Char_vector* friends = (struct Char_vector*)malloc(sizeof(struct Char_vector));
	int i;
	int numRequestPending = -2;
	soap_call_ims__haveFriendshipRequest(&soap, serverURL,"",username,&numRequestPending);
	printf("numero de peticiones: %d", numRequestPending);
	if(numRequestPending > 0)
	{
		soap_call_ims__getFriendshipRequests(&soap, serverURL, "",username ,friends);
		printf("Lista de amistades sin aceptar:\n");

		for(i=0;i < 100;i++){
			if(friends->data[i] != NULL){
				printf("%d: %s\n",i,friends->data[i]);
			}
		}
	}
	else if(numRequestPending == -2){
		printf("Hay un problema con la conexion\n");
		return -1;

	}
	else if(numRequestPending == -1){
			printf("No estas online\n");
	}
	else{
		system("clear");
		printf("No tienes peticiones de amistad\n");
	}

	free(friends);

	printf("\n\n");
	return 0;
}

/* Muestra el menú principal donde el usuario puede interactuar
 * con todas las funciones de la aplicación
*/
int show_menu(struct soap soap, char *serverURL) {
	int q = 0, error;
	while(!q) {
		int in;
		if(!DEBUG_MODE) system("clear");
		printf("	Hola, %s. Selecciona una opcion:\n", username);
		printf("	1) Enviar mensaje a otro usuario\n");
		printf("	2) Mostrar nuevos mensajes entrantes\n");
		printf("	3) Listar usuarios amigos\n");
		printf("	4) Enviar solicitud de amistad\n");
		printf("	5) Ver solicitudes de amistad\n");
		printf("	6) Aceptar solicitudes de amistad\n");
		printf("	7) Dar de baja\n");
		printf("	0) Salir\n");
		scanf("%d", &in);
		
		switch(in) {
			case 1: //Enviar mensaje
				error = sendMessage(soap, serverURL);
				break;
			case 2: //Mostrar mensajes entrantes

				break;
			case 3: //Listar amigos
				error = listReq(soap, serverURL);
				break;
			case 4: //Solicitud de amistad
				error = sendReq(soap, serverURL);
				break;
			case 5: 
				break;
			case 6: // Aceptar solicitud
				error = acceptReq(soap, serverURL);
				break;
			case 7: //Baja
				q = deleteUser(soap, serverURL);
				break;
			case 0: //Salir
				q = logout(soap, serverURL);
				break;
			default:
				printf("Opcion no valida, vuelve a intentarlo\n");
				break;
		}
	}
	
	return 0;
}


/* FIN */
int main(int argc, char **argv){
	struct soap soap;
	char *serverURL;
	int res;

	// Usage
	if (argc != 2) {
		printf("Usage: %s http://server:port message\n",argv[0]);
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
