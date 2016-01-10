SSL_LIBS=-lssl -lcrypto
SSL_FLAGS=-DWITH_OPENSSL

#SSL_LIBS=
#SSL_FLAGS=

all:	client server

client:	
	gcc $(SSL_FLAGS) -o client client.c soapC.c soapClient.c  -I$(GSOAP_INCLUDE) -lgsoap $(SSL_LIBS) -L$(GSOAP_LIB)

server:	
	gcc $(SSL_FLAGS) -o server serverUsers.c serverFiles.c server.c soapC.c soapServer.c -I$(GSOAP_INCLUDE) -lgsoap $(SSL_LIBS) -L$(GSOAP_LIB) -lpthread

clean:	
	rm client server *.xml *.nsmap *.wsdl *.xsd soapStub.h soapServerLib.c soapH.h soapServer.c soapClientLib.c soapClient.c soapC.c 
