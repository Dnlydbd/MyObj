#include <pthread.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <errno.h>  
#include <unistd.h>  
#include <fcntl.h> 
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  



int 
socket_set_noneblock(int isockfd)
{
	int iflags;
	if ((iflags = fcntl(isockfd, F_GETFL, NULL)) < 0) {
		printf("fcntl(%d, F_GETFL)", isockfd);
		return -1;
	}
	if (!(iflags & O_NONBLOCK)) {
		if (fcntl(isockfd, F_SETFL, iflags | O_NONBLOCK) == -1) {
			printf("fcntl(%d, F_SETFL)", isockfd);
			return -1;
		}
	}
	return 0;
}
