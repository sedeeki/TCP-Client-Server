#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
using namespace std;


int main(int argc, char* argv[]){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[99999];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	
	int port = atoi(argv[2]);
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		//scanf("%s", &buffer[0]);
		
		string * arr;
		arr = new string[argc];
		int j;
		
		for (j = 0; j < argc; j++)
		{
			arr[j] = argv[j];
		}
		string fileName = "";
		string msg = "";
		int i;
		for (i = 3; i < argc; i++)
		{
			msg = msg + arr[i] + " ";
		}
		string temp = arr[arr->length()-1];
		for (i = 1; i < temp.size(); i++)
		{
			fileName = fileName + temp[i];
		}
		
		const char* message = msg.c_str();
		send(clientSocket, message, strlen(message), 0);

		if(strcmp(buffer, "exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 99999, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			ofstream file;
			file.open(fileName);
			cout << buffer << endl;
			file << buffer;
			file.close();
			exit(1);
		}
	}

	return 0;
}
