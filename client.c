#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<poll.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<signal.h>

struct sigaction old_action;
int sockfd;

void sigint_handler(int sig_no){
	printf("exiting\n");
	send(sockfd, "/exit", 255, 0);
	close(sockfd);
	sigaction(SIGINT, &old_action, NULL);
	kill(0, SIGINT);
}

int main(){
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = &sigint_handler;
	sigaction(SIGINT, &action, &old_action);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address = {
		AF_INET,
		htons(8888),
		0
	};
	
	if (connect(sockfd, &address, sizeof(address)) != 0) { 
		perror("call to connect failed"); 
		close(sockfd);
		return 1;
	}

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
	
	char channel[15] = "#main";

	while (0 == 0){
		char buffer[256] = { 0 };
		char nick[30];
		poll(fds, 2, 50000);

		if (fds[0].revents & POLLIN) {
			read(0, buffer, 255);
			if (strcmp(buffer, "/exit\n") == 0){ 
				send(sockfd, "/exit", 255, 0);
				break;
			}
			else if (strcmp(buffer, "/join\n") == 0){
				printf("channel to join: ");
				scanf("%s", channel);
				strcpy(channel, strtok(channel, "\n"));
			}
			else if (strcmp(buffer, "/nick\n") == 0){
				printf("new nick: ");
				scanf("%s", nick);
			}else {
				char tmp[256];
				snprintf(tmp, 256, "%s:%s: %s", channel, strtok(nick, "\n"), strtok(buffer, "\n"));
				send(sockfd, tmp, 255, 0);
			}
		} if (fds[1].revents & POLLIN){
			if (recv(sockfd, buffer, 255, 0) == 0){ break; }
			char tmp[15];
			strcpy(tmp, buffer);
			if (strcmp(channel, strtok(tmp, ":")) == 0){
				printf("%s\n", buffer);
			}
		}
	}
	close(sockfd);
	return 0;
}
