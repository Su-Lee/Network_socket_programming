#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define MAX 80
#define SERVER_PORT 9090

#define MAXLINE 4097
#define SA struct sockaddr

void err_n_die(const char * fmt, ...);
char *bin2hex(const unsigned char * input, size_t len);
void func(int connfd, char *input);

int main(int argc, char ** argv){
	int listenfd, connfd, n;
	struct sockaddr_in servaddr;
	uint8_t buf[MAXLINE+1];
	uint8_t recvline[MAXLINE+1];
	char compare[MAX];

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //create internet stream TCP socket 
		err_n_die("Error while creating the socket!");
	else
        printf("Socket successfully created..\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //"127.0.0.1"
	servaddr.sin_port = htons(SERVER_PORT); //host ot network 

	if((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
		err_n_die("bind error");
	else
        printf("Socket successfully binded..\n");

	if((listen(listenfd, 10)) < 0)
		err_n_die("listen error");
	else
        printf("Server listening..\n");

	for( ; ; ){
		struct sockaddr_in addr;
		socklen_t addr_len;

		printf("waiting for a connection on port %d\n", SERVER_PORT);
		fflush(stdout);

		connfd = accept(listenfd, (SA *)NULL, NULL);
		if (connfd < 0) 
	        err_n_die("accept error");
	    else
	        printf("server accept the client...\n");

		memset(recvline, 0 ,MAXLINE);

		while((n = read(connfd, recvline, MAXLINE-1)) > 0){
			strncpy(compare, recvline, 10);
	        //printf("%s\n", compare);
	        if(strcmp(compare, "GET / HTTP") != 0){
	        	//printf("request from clients\n");
	        	func(connfd, recvline);
	        	break;
	        }

			fprintf(stdout, "/n%s/n/n%s", bin2hex(recvline, n), recvline);

			if(recvline[n-1] == '\n'){
				break; 
			}
			memset(recvline, 0, MAXLINE);
		}
		if(n < 0)
			err_n_die("read error");


		snprintf((char*)buf, sizeof(buf), "HTTP/1.0 200 ok\r\n\r\nName: Su lee\nStudent ID: b09902082\nemail: b09902082@ntu.edu.tw");

		if(write(connfd, (char*)buf, strlen((char*)buf)) < 0)
			err_n_die("write error");

		if(close(connfd) < 0)
			err_n_die("close error");

	}
	exit(0); // terminate successfully
}

// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/va-arg-va-copy-va-end-va-start?view=msvc-170
void err_n_die(const char * fmt, ...){
	int errno_save;
	va_list ap;

	errno_save = errno; //intialize errno 

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap); //stream 
	fprintf(stdout, "\n");
	fflush(stdout);

	if(errno_save != 0){
		fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
		fprintf(stdout, "\n");
		fflush(stdout);
	}
	va_end(ap);

	exit(1); // terminate due to errror
}

char *bin2hex(const unsigned char * input, size_t len){
	char * result; 
	char * hexits = "0123456789ABCDEF" ;

	if(input == NULL || len <= 0)
		return NULL;

	int resultlength = (len*3)+1;

	result = malloc(resultlength);
	bzero(result, resultlength);

	for(int i=0; i<len; i++){
		result[1*3] = hexits[input[i] >> 4];
		result[(i*3) + 1] = hexits[input[i] & 0x0F];
		result[(i*3) + 2] = ' ';
	}

	return result; 
}

void func(int connfd, char *input){
	char buff[MAX];
    int n;
    int flag = 1;
    // infinite loop for chat
    for ( ; ; ) {
    	if(flag != 1){
        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
 		}else if(flag == 1){
 			strcpy(buff, input);
 			flag = 0;
 		}
    
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;
   
        // and send that buffer to client
        write(connfd, buff, sizeof(buff));
   
        // if msg contains "exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}



