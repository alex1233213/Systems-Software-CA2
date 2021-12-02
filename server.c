#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


#define LENGTH 512

void *connection_handler(void *);

int main() { 
	int socket_desc;
	int client_sock;
	int conSize;
	int READSIZE;


	struct sockaddr_in server, client;
	//char message[500];

	//create socket	
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if( socket_desc == -1) { 
		printf("Could not create socket\n");
	} else { 
		printf("Socket successfully created!!\n");
	}

	//set sockaddr_in variables
	server.sin_port = htons(8801);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;



	//bind
	if(bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
		perror("Bind issue\n");	
		return 1;
	} else { 
		printf("bind complete\n");
	}
	
	//listen for a connection
	listen(socket_desc, 3);

	//accept any incoming connection
	printf("Waiting for incoming connection from client\n");
	conSize = sizeof(struct sockaddr_in);

	pthread_t tid;
	while( (client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &conSize )))
       	{
		puts("Connection accepted\n");
		if( pthread_create( &tid, NULL, connection_handler, (void*) &client_sock) < 0) {
			perror("could not create thread");
			return 1;
		}

		puts("handler assigned for client connection\n");
	}

	if(client_sock < 0) {
		perror("accept failed");
		return 1;
	}

	return 0;


}


void *connection_handler(void *socket_desc) { 
	//get the socket descriptor
	int sock = *(int *) socket_desc;
	char msg[500];
	int READSIZE;
	char *message, client_message[2000];

	memset(msg, 0, 500);
	
	//read message from the client
	READSIZE = recv(sock, msg, 500, 0);

	if(strcmp(msg, "initTransfer") == 0) {
		printf("Init Transfer\n");
		write(sock, "filename", strlen("filename") );
	//	memset(msg, 0, 500);
	}


	memset(msg, 0, 500);
	READSIZE = recv(sock, msg, 500, 0);

	if( strcmp(msg, "initTransfer") != 0 && strlen(msg) > 0) { 
		printf("File\n");
		write(sock, "begin", strlen("begin") );
		printf("Filename: %s\n", msg);
		

		//receive file from client
		char *fr_path = "/home/alex/Desktop/ca2/server_upload_dir/";
		char revbuf[LENGTH];
		char *fr_name = (char *) malloc( 1 + strlen(fr_path) + strlen(msg) );
		strcpy( fr_name, fr_path );
		strcat( fr_name, msg );

		FILE *fr = fopen(fr_name, "w");
		if(fr == NULL) {
			printf("File %s cannot be opened in the server\n", fr_name);
		} else { 
			bzero(revbuf, LENGTH);
			int fr_block_sz = 0;
			int i = 0;
			
			while( (fr_block_sz = recv(sock, revbuf, LENGTH, 0) ) > 0 ) {
				printf("Data received %d = %d\n", i , fr_block_sz);
				
				int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
				if(write_sz < fr_block_sz) { 
					perror("File write failed on the server\n");
				}

				bzero(revbuf, LENGTH);
				i++;

			}

			if(fr_block_sz < 0) { 
				if(errno == EAGAIN) { 
					printf("recv() timed out\n");
				}
			} else {
				fprintf(stderr, "recv() failed due to error = %d\n", errno);
				exit(1);
			}
		}

		printf("Ok received from the client\n");
		fclose(fr);
	}


	memset(msg, 0, 500);


	if(READSIZE == 0) { 
		puts("Client disconnected");
		fflush(stdout);

	} else if(READSIZE == -1) { 
		perror("recv() failed");
	}

	return 0;

}

		
			




//old code
	//accept connection from incoming client
//	cs = accept(s, (struct sockaddr * ) &client, (socklen_t *) &conSize);
//
//	if(cs < 0) { 
//		perror("cant establish connection\n");
//		return 1 ;
//	} else  { 
//		printf("connection from client accepted\n");
//	}
//
//
//	while(1) { 
//		memset( message, 0, 500);
//
//		READSIZE = recv(cs, message, 2000, 0);
//
//		if(READSIZE == 0) {
//			puts("client disconnected\n");
//			fflush(stdout);
//		} else  if (READSIZE == -1) { 
//			perror("read error\n");
//		}
//
//
//
//		printf("Client said: %s\n", message);
//
//		write(cs, "what??\n", strlen("what??\n"));
//	}
//	
//
//	return 0;

