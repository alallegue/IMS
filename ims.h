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

typedef struct
{
	char* str;
}String;

struct Char_vector {
	char* data[100];
};


int ims__sendMessage (struct Message myMessage, int *result);
int ims__receiveMessage (struct Message *myMessage);

int ims__login(char* username, char* password, int *error);
int ims__registerUser(char* username, char* password, int *error);
int ims__deleteUser(char* username, int *error);
int ims__logout(char* username, int *error);

int ims__sendReq(char* username, char* friendname, int *error);
int ims__getFriendshipRequests(char* user , struct Char_vector *friends);
int ims__haveFriendshipRequest(char* user,int *result);
//int ims__getFriendshipRequest(char* user,String* friend_nick);
