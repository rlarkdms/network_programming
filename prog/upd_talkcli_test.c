#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 511

char *EXIT_STRING = "exit"; 

int recv_and_print(int sd, struct sockaddr_in* servaddr); 
int input_and_send(int sd, struct sockaddr_in* servaddr); 

int main(int argc, char *argv[]) {
        pid_t pid;
        static int s;
        static struct sockaddr_in servaddr;

        if(argc != 3) {
                printf("사용법: %s server_ip port\n", argv[0]);
                exit(0);
        }
        if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
                printf("Client: Can't open dgram socket.\n");
                exit(0);
        }
        bzero((char *)&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
        servaddr.sin_port = htons(atoi(argv[2]));
        if((pid = fork()) > 0) { 
                input_and_send(s, &servaddr);
        } else if(pid == 0) { 
                recv_and_print(s, &servaddr);
        }
        close(s);
        return 0;
}
int input_and_send(int sd, struct sockaddr_in *servaddr) {
        char buf[MAXLINE + 1];
        int nbyte, addrlen = sizeof(struct sockaddr_in);
        while(fgets(buf, sizeof(buf), stdin) != NULL) {
                nbyte = strlen(buf);
                if(sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)servaddr, addrlen) < 0)
                        perror("sendto fail\n");
                if(strstr(buf, EXIT_STRING) != NULL) {
                        puts("Good Bye!");
                        close(sd);
                        exit(0);
                }
        }
        return 0;
}
int recv_and_print(int sd, struct sockaddr_in *servaddr) {
        char buf[MAXLINE + 1];
        int nbyte, addrlen = sizeof(struct sockaddr_in);
        while(1) {
                if((nbyte = recvfrom(sd, buf, MAXLINE, 0, (struct sockaddr *)servaddr, &addrlen)) < 0) {
                        perror("read fail");
                        close(sd);
                        exit(0);
                }
                buf[nbyte] = 0;
                if(strstr(buf, EXIT_STRING) != NULL) {
                        break;
                }
                printf("%s", buf); 
        }
        return 0;
}
