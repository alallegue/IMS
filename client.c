#include "soapH.h"
#include "imsService.nsmap"

#define DEBUG_MODE 1

char *username, *password;

/* nueva versión*/ 

/*Menú de bienvenida. 
 * Es el primero que se muestra y solicita
 * al usuario crear una cuenta o iniciar con una ya creada*/
int show_login(struct soap soap, char *serverURL) {
	int op = -1;
	int res = -1;
	
	while(op != 0 && res != 0){
		//system("clear");
		printf("	Bienvenido. Selecciona una opcion: \n");
		printf("	1) Entrar.\n");
		printf("	2) Registrarse.\n");
		printf("	0) Salir.\n");

		scanf("%d",&op);
		switch(op) {
			case 1: //Login
				res = login(soap, serverURL);
				if(res == 0){ //Si se loguea correctamente entrar al menú principal
					show_menu(soap, serverURL);
				}
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

int login(struct soap soap,char *serverURL){
	char* nick,*pass;
	int res = 1;

	nick = (char*)malloc(256*sizeof(char));
	pass = (char*)malloc(256*sizeof(char));

	//user* usr;

	printf("\nNombre\n");
	scanf("%s",nick);
	printf("Contraseña\n");
	scanf("%s",pass);

	//soap_call_ims__login(&soap, serverURL, "",nick,pass,&res);

	if(res == -1){
		printf("Nombre de usuario o contraseña incorrectos\n");
		free(nick);
		free(pass);
	}
	else if (res == 1){
		printf("Hay un problema con la conexion\n");
	}
	else{
		username = nick;
		password = pass;
	}
	//free(pass);

	return res;
}

int registerUser(struct soap soap,char *serverURL) {
	char* un;
	char* pass;
	int res = 1;
	
	un = (char*)malloc(256*sizeof(char));
	pass = (char*)malloc(256*sizeof(char));
	
	printf("Introduce nombre de usario: ");
	scanf("%s", un);
	printf("Introduce contraseña: ");
	scanf("%s", pass);
	
	//Gestión de errores
	
	soap_call_ims__registerUser(&soap, serverURL, "", un, pass, &res);
	
	//Gestión de errores
	
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

/* Muestra el menú principal donde el usuario puede interactuar
 * con todas las funciones de la aplicación
*/
int show_menu(struct soap soap, char *serverURL) {
	int q = 0, error;
	while(!q) {
		int in;
		printf("	Elige una opcion:\n");
		printf("	1) Enviar mensaje a usuario\n");
		printf("	2) Mostrar nuevos mensajes entrantes\n");
		printf("	3) Listar usuarios amigos\n");
		printf("	4) Enviar solicitud de amistad\n");
		printf("	5) Ver solicitudes de amistad\n");
		printf("	4) Dar de baja\n");
		printf("	0) Salir\n");
		scanf("%d", &in);
		
		switch(in) {
			case 1: //Enviar mensaje
				error = sendMessage(soap, serverURL);
				break;
			case 2: //Mostrar mensajes entrantes

				break;
			case 3: //Listar amigos
				
				break;
			case 4:

				break;
			case 5: 

				break;
			case 0: //Salir
				q = 1;
				break;
			default:
				break;
		}
	}
	
	return 0;
}

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
	// Debug?
	if (DEBUG_MODE){
		printf ("Server to be used by client: %s\n", serverURL);
	}
	
	//Mostrar menú de bienvenida
	show_login(soap, serverURL);
	
	// Clean the environment
  	soap_end(&soap);
  	soap_done(&soap);
	
	return 0;
}
