// Client code in C to sort the array
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// client
int main(int argc, char* argv[])
{


	int sock;
	struct sockaddr_in server;
	int server_reply;
	
	
	int i, temp, id ;
	
	srand(time(NULL));
	int number = rand() % (3 + 1);//number of tickets
	
for(int a =0 ; a<5;a++){
	
	
	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	printf("Socket created\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8880);

	// Connect to remote server
	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}

	printf("Connected\n");
	printf(" number of tickets is: %d \n", number );
	
// for all tickets they needs 	
for(int a=0; a < number ; a ++){


	
	id  = rand() % 10;//flight id
	
	if (send(sock, &id,sizeof(int), 0) < 0) {// send flight id 
		printf("Send failed");
		return 1;
	}
	if (recv(sock, &server_reply,sizeof(int), 0) < 0) {	//recieve a changed value 
		printf("recv failed");
		return 0;
	}

	if(server_reply == 1 ){
		printf("Success to book.\n");
	}else{
		printf("Can not book.\n");
	}
	
	
	id  = rand() % 10;
		
}
	

	// close the socket
	number = rand() % (3 + 1);//number of tickets
	close(sock);
	sleep(1);
	}
	return 0;
}




