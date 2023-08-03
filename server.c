// Server code in C to sort the array
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
struct flight{    
	
	int flight_id;    
	char from [15];
	char to [15];
	int available_seats;
	float price;
	int success,unsuccess;     
 };

// server
int main(int argc, char* argv[])
{
	int socket_desc, client_sock, c, read_size,rd;
	struct sockaddr_in server, client;
	int message, i, flights;
	struct flight flight[10];
	int booking,sbooking;
	float earnings;
	char *namesF[]={"Hrakleio","Athens","Thessaloniki","Lyon","Madrit","London","Kerkura","Bansko","Tripoli","Mykonos"};
	char *namesT[]={"Milano","Tokyo","Wien","Athens","Thessaloniki","Paris","Munchen","Torino","Rhoades","Roma"};
	float prices[10]={230.4,120.9,34.50,339.99,120.0,321.99,400.80,99.99,104.99,111};
 	int count[10],success[10],unsuccess[10];	
 	
 	
 	for(int k=0; k<10; k++){				
		flight[k].flight_id = k;
		strcpy(flight[k].from,namesF[k]);
		strcpy(flight[k].to,namesT[k]);
		srand(time(0));
		flight[k].available_seats = rand() % (5 + 1);
		flight[k].price = prices[k];
		count[k]=0;	
		success[k]=0;	
		unsuccess[k]=0;	
	}
	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}
	printf("Socket created\n");

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8880);

	// Bind the socket
	if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {

		// print the error message
		perror("bind failed. Error");
		return 1;
	}
	printf("bind done\n");

	// lsiten to the socket
	listen(socket_desc, 3);

	printf("Waiting for incoming connections...\n");
	c = sizeof(struct sockaddr_in);
	booking = 0;
	sbooking=0;
	earnings = 0.0;
  for(int a = 0 ;a < 5; a++){
	// accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c);

	if (client_sock < 0) {
		perror("accept failed");
		return 1;
	}
	flights=0;
	printf("Connection accepted\n");

	// Receive a id from client
	while ((read_size = recv(client_sock, &message,  sizeof(int), 0)) > 0) {
	flights++;
	booking++; //requests for booking 
	printf("received : %d \n" , message );//reads id of flight
	
	count[message]=count[message] + 1 ;
		if(flight[message].available_seats  > 0){
		//reserve tickets by 1 every time
		 	flight[message].available_seats  = flight[message].available_seats - 1;
		 	success[message] = success [message] + 1;
		 	sbooking ++;
		 	earnings= earnings + flight[message].price ;
		 	message = 1;
			write(client_sock, &message,  sizeof(int));
			
		}else{
			unsuccess[message] = unsuccess[message] + 1; 
			message =0;
			write(client_sock, &message,  sizeof(int));
		}
	
		
		
	}
	
	
	if (read_size == 0) {
	
		printf("Client disconnected\n");
	}
	else if (read_size == -1) {
		perror("recv failed");
	}
	

	for(int index = 0;index < 10;index++){
		if(count[index] > 0){
		printf("From : %s to : %s requests for booking : %d \n",flight[index].from ,flight[index].to,count[index]);
		}
			
	}
		
	for(int j=0 ; j < 10 ; j++){
		printf("Flight id  : %d  Success : %d , Unsuccess : %d\n",j,success[j],unsuccess[j]);
	
	}
	for(int j=0 ; j < 10 ; j++){
		count[j]=0;
		success[j]=0;
		unsuccess[j]=0;
	
	}
	sleep(1);
 }
  
  printf(" Total requests : %d \n Total successed requests : %d \n Total earnings : %f EUR\n" , booking, sbooking ,earnings );

	return 0;
}



