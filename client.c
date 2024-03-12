#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<poll.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

int main(){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address = {
		AF_INET,
		htons(8888),
		0
	};
	
	connect(sockfd, &address, sizeof(address));

	printf("%d\n", errno);

	struct pollfd fds[2] = {
		{
			0,
			POLLIN,
			0
		},
		{
			sockfd,
			POLLIN,
			0
		}
	};

	while (0 == 0){
		char buffer[256] = { 0 };
		char nick[30];
		poll(fds, 2, 50000);

		if (fds[0].revents & POLLIN) {
			read(0, buffer, 255);
			if (strcmp(buffer, "/exit\n") == 0){ break; }
			else if (strcmp(buffer, "/nick\n") == 0){
				printf("new nick: ");
				scanf("%s", nick);
			}else {
				char tmp[256];
				snprintf(tmp, 256, "%s: %s", strtok(nick, "\n"), buffer);
				send(sockfd, tmp, 255, 0);
			}
		} if (fds[1].revents & POLLIN){
			if (recv(sockfd, buffer, 255, 0) == 0){ break; }
			printf("%s\n", strtok(buffer, "\n"));
		}
	}

	close(sockfd);
	return 0;
}
