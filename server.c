#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>


int main() { 
	int s;
	int cs;
	int conSize;
	int READSIZE;


	struct sockaddr_in server, client;
	char message[500];
	
	s = socket(AF_INET, SOCK_STREAM, 0);

	if(s == -1) { 
		printf("Could not create socket\n");
	} else { 
		printf("Socket successfully created!!\n");
	}

	//set sockaddr_in variables
	server.sin_port = htons(8801);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;



	//bind
	if(bind(s, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
		perror("Bind issue\n");	
		return 1;
	} else { 
		printf("bind complete\n");
	}
	
	//listen for a connection
	listen(s, 3);

	//accept any incoming connection
	printf("Waiting for incoming connection from client\n");
	conSize = sizeof(struct sockaddr_in);


	//accept connection from incoming client
	cs = accept(s, (struct sockaddr * ) &client, (socklen_t *) &conSize);

	if(cs < 0) { 
		perror("cant establish connection\n");
		return 1 ;
	} else  { 
		printf("connection from client accepted\n");
	}


	while(1) { 
		memset( message, 0, 500);

		READSIZE = recv(cs, message, 2000, 0);

		if(READSIZE == 0) {
			puts("client disconnected\n");
			fflush(stdout);
		} else  if (READSIZE == -1) { 
			perror("read error\n");
		}



		printf("Client said: %s\n", message);

		write(cs, "what??\n", strlen("what??\n"));
	}
	

	return 0;
}
