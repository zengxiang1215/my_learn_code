/*
*
*这是组播的接收端
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char** argv)
{
	if(argc < 0){
		fprintf(stderr," %s is too few argumnet < ip > < port >  \n",argv[0]);
		exit(EXIT_FAILURE);
	}//if
	
	int sockfd;
	struct sockaddr_in client_address,service_address;
	int client_addrlen = sizeof(client_address);
	int service_addrlen = sizeof(service_address);

	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(sockfd < 0 ){
		perror("socket");
		exit(EXIT_FAILURE);
	}//if
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}//if
	
	bzero(&client_address,sizeof(client_address));
	client_address.sin_family = PF_INET;
	client_address.sin_port = htons(atoi(argv[2]));
	client_address.sin_addr.s_addr = inet_addr(argv[1]);
	
	if(bind(sockfd,(struct sockaddr*)&client_address,(socklen_t)client_addrlen) < 0 ){
		perror("bind");
		exit(EXIT_FAILURE	);
	}//if
	
	char recv_buf[BUFSIZ];
	bzero(recv_buf,sizeof(recv_buf));
	while(1){
		if(recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&service_address,(socklen_t*)&service_addrlen) < 0 ){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}	
		printf(" [ %s :%d ]: %s \n",inet_ntoa(service_address.sin_addr),ntohs(service_address.sin_addr.s_addr),recv_buf);

		if(strncmp(recv_buf,"quit",4) == 0){
			break;
		}//if
	}//while
	close(sockfd);
	return 0;
}
