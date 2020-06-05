#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <dirent.h>
#include <time.h>
using namespace std;


int main(int argc, char* argv[])
{
		
	
	if (argc == 1)
	{
		printf("No PORT given to listen on and also no path given to fetch files from!\n");
		exit(1);
	}
	
	else if (argc > 1 && argc != 3)
	{
		printf("Arguments required are 2! PORT and PATH!\n");
		exit(1);
	}
	
	int size = strlen(argv[1]);
	
	int i;
	int count = 0;
	for (i = 0; i < size; i++)
	{
		if (isdigit(argv[1][i]))
		{
			count++;
		}
	}
	
	if (count != size)
	{
		printf("Invalid port number");
		exit(1);
	}

	
	int port = atoi(argv[1]);
	char* filePath = argv[2];
	
	
	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	
	printf("%d\n",port);
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", port);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				buffer[0] = '\0';
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					string line = buffer;
					if (buffer[0] == 'I' && buffer[1] == 'N' && buffer[2] == 'F' && buffer[3] == 'O')
					{
						time_t t;
						time(&t);
						string message = ctime(&t);
						const char * msg = message.c_str();
						send(newSocket, msg, strlen(msg), 0);
						bzero(buffer, sizeof(buffer));
						exit(1);
					}
					
					else if (buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T')
					{
						
						if (buffer[4] == '/' && line.size() > 6)
						{
							int length = strlen(filePath) + strlen(buffer) -5;
							char * dir = (char *)malloc(length);
							int num;
							int index = 4;
							for (num = 0; num < length; num++)
							{
								if (num < strlen(filePath))
								{
									dir[num] = filePath[num];
								}
								else
								{
									dir[num] = buffer[index];
									index++;
								}
							} 
							
							printf("%s\n",dir);
							
							if (access(dir,F_OK) == -1)
							{
								const string error = "File does not exist\n";
								const char * sendMsg = error.c_str();
								send(newSocket, sendMsg, strlen(sendMsg), 0);
								break;
							}
							
							
							else
							{
								FILE * file;
								file = fopen(dir,"rb");
								fseek(file,0,SEEK_END);
								long fsize = ftell(file);
								fseek(file,0,SEEK_SET);
								char * string = (char *)malloc(fsize + 1);
								fread(string,1,fsize,file);
								fclose(file);
								string[fsize] = 0;
								
								send(newSocket, string, strlen(string), 0);
								bzero(buffer, sizeof(buffer));
								exit(1);
							}
						}
						
						else if (buffer[4] == '/' && line.size() == 6)
						{
							DIR *dir;
							dirent *pdir;
							dir=opendir(".");
							string list = "";
							while(readdir(dir))
							{
								pdir=readdir(dir);
								list = list + pdir->d_name + "  ";
							}
							closedir(dir);
							
							const char * msg = list.c_str();
							printf("%s\n",msg);
							send(newSocket, msg, strlen(msg), 0);
							bzero(buffer, sizeof(buffer));
							exit(1);
						}
						
						else
						{
							printf("Invalid format! It should be: GET /filename");
							exit(1);
						}
					}
					
					else
					{
						printf("Invalid format! It should be: GET /filename");
						exit(1);
					}
					
					
					
				}
			}
		}

	}

	close(newSocket);

	return 0;
}
