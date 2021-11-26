#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) { 
	int SID;
	struct sockaddr_in server;
	char clientMessage[500];
	char serverMessage[500];	


	//create socket
	SID = socket(AF_INET, SOCK_STREAM, 0);

	if(SID == -1) { 
		printf("error creating socket\n");
	} else { 
		printf("socket created\n");
	}

	server.sin_port = htons (8801);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	

	//connect to server
	if(connect(SID, (struct  sockaddr*)&server, sizeof(server)) < 0 ) { 
		printf("connect failed. Error\n");
		return 1;
	}

	printf("Connected to the server ok\n");


	while(1) {
		printf("\nEnter message: ");
		scanf("%s", clientMessage);

		if( send(SID, clientMessage, strlen(clientMessage), 0) < 0)
		{
			printf("send failed\n");
			return 1;
		}
		
		if( recv(SID, serverMessage, 500, 0) < 0)
		{ 
			printf("IO error\n");
			break;
		}

		printf("\nserver sent: ");
		printf(serverMessage);

	}

	close(SID);
	return 0;
}



