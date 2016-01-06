//gsoap ims service name: imsService
//gsoap ims service style: rpc
//gsoap ims service encoding: encoded
//gsoap ims service namespace: urn:imsService
#define IMS_MAX_MSG_SIZE 256
#define IMS_MAX_NAME_SIZE 256

typedef char *xsd__string;

struct Message{
	xsd__string name;
	xsd__string msg;
	int operation;
};

int ims__sendMessage (struct Message myMessage, int *result);
int ims__receiveMessage (struct Message *myMessage);

int ims__login(char* username, char* password, int *error);
int ims__registerUser(char* username, char* password, int *error);
int ims__deleteUser(char* username, int *error);
int ims__logout(char* username, int *error);

int ims__sendReq(char* username, char* friendname, int *error);
//int ims__getNumReq(char* username, int* numReq);
//int ims__getReq(char* username, char* friendname);
int ims__acceptReq(char* username, char* friendname, int *error);
int ims__cancelReq(char* username, char* friendname, int *error);
