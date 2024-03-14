#include<sys/socket.h>
#include<fcntl.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<poll.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

#define MAXCLIENTS 10

int main(){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address = {
		AF_INET,
		htons(8888),
		0
	};

	if (bind(sockfd, &address, sizeof(address)) != 0){
		perror("failed to bind port");
		printf("is a server already running on this port\n");
		exit(1);
	}

	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	printf("%d\n",errno);
	perror(0);

	listen(sockfd, 10);


	int clients[MAXCLIENTS];
	int connectedClients[MAXCLIENTS] = { 0 };
	int clientCount = 0;
		
	struct pollfd fds[MAXCLIENTS];
	while (0 == 0){
		usleep(50000);
		for (int i = 0; i < clientCount+1; i++){
			if (connectedClients[i] == 0){
				int tmp = accept(sockfd, 0, 0);
				if (tmp != -1){
					clients[i] = tmp;
					printf("client connected\n");
					fds[i].fd = clients[i];
					fds[i].events = POLLIN;
					fds[i].revents = 0;
					connectedClients[i] = 1;
					clientCount++;
				}
			}
		}

		poll(fds, MAXCLIENTS, 50000);

		char buffer[256];
		int tmp = clientCount;
		for (int i = 0; i < tmp; i++){
			if (connectedClients[i] == 1){
				if (fds[i].revents & POLLIN){
					if (recv(clients[i], buffer, 255, 0) == 0){ break; }
					else if (strcmp(buffer, "/exit") == 0){
						connectedClients[i] = 0;
						clientCount--;
						fds[i].revents = 0;
						fds[i].fd = 0;
						fds[i].events = 0;
						close(clients[i]);
						printf("client %d disconnected\n", i);
					} else {
						for (int j = 0; j < clientCount; j++){
							if (j != i){
								send(clients[j], buffer, 255, 0);
							}
						}
					}
				}
			}
		}
	}
	close(sockfd);
	return 0;
}
