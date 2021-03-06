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

struct Char_vector {
	char* data[100];
	int msgcheck[100];
};


int ims__sendMessage (char *username, struct Message myMessage, int *error);
int ims__receiveMessage(char* username, char* friendname, int numMensajes,struct Message *myMessage);
int ims__login(char* username, char* password, int *error);
int ims__registerUser(char* username, char* password, int *error);
int ims__deleteUser(char* username, int *error);
int ims__logout(char* username, int *error);

int ims__sendReq(char* username, char* friendname, int *error);
int ims__getFriendshipRequests(char* user , struct Char_vector *friends);
int ims__haveFriendshipRequest(char* user,int *result);
int ims__acceptReq(char* username, char* friendname, int *error);
int ims__cancelReq(char* username, char* friendname, int *error);
int ims__deleteFriend(char* username, char* friendname, int *error);

int ims__getFriends(char* user ,struct Char_vector *friends);
int ims__haveFriends(char* user,int *result);
int ims__haveMessages(char* user, int *result);
