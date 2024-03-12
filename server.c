#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<poll.h>
#include<unistd.h>
#include<errno.h>

#define MAXCLIENTS 10

int main(){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address = {
		AF_INET,
		htons(8888),
		0
	};

	bind(sockfd, &address, sizeof(address));

	printf("%d\n",errno);
	perror(0);

	listen(sockfd, 10);


	int numOfClients;
	printf("number of clients: ");
	scanf("%d", &numOfClients);

	int clients[MAXCLIENTS];
		
	for (int i = 0; i < numOfClients; i++){
		clients[i] = accept(sockfd, 0, 0);
		printf("client %d connected\n", i);
	}
	
	struct pollfd fds[MAXCLIENTS];
	for (int i = 0; i < numOfClients; i++){
		fds[i].fd = clients[i];
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

	while (0 == 0){
		poll(fds, 3, 50000);

		char buffer[256];
		for (int i = 0; i < numOfClients; i++){
			if (fds[i].revents & POLLIN){
				if (recv(clients[i], buffer, 255, 0) == 0){ break; }
				for (int j = 0; j < numOfClients; j++){
					if (j != i){
						send(clients[j], buffer, 255, 0);
					}
				}
			}
		}
	}
	close(sockfd);
	return 0;
}
