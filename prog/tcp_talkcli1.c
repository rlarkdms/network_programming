#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 511
#define NAME_LEN 20

char *EXIT_STRING ="exit";
char bufall[MAXLINE+NAME_LEN],*bufmsg;
int maxfdp1,socket_id,namelen;


int main(int argc, char *argv[]){
pid_t pid;
static int s;
static struct sockaddr_in servaddr;

if (argc !=4){
printf("사용법: %s serv_ip port \n",argv[0]);
exit(0);
}

sprintf(bufall,"[%s] : ", argv[3]);
namelen=strlen(bufall);
bufmsg=bufall+namelen;


if((s=socket(PF_INET,SOCK_STREAM,0))<0){
printf("client:can'topen stream socket.\n");
exit(0);
}

bzero((char*)&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
inet_pton(AF_INET, argv[1],&servaddr.sin_addr);
servaddr.sin_port = htons(atoi(argv[2]));

if(connect(s,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
 printf("Client:can't connetc to server.\n");
 exit(0);
}

if( (pid=fork())>0)
 input_and_send(s);
else if(pid==0)
 recv_and_print(s);
 close(s);
 return 0;
}

int input_and_send(int sd){
 char buf[MAXLINE+1];
 int nbyte;
 while(fgets(bufmsg,MAXLINE,stdin) !=NULL) {
  nbyte=namelen+strlen(bufall);
  write(sd,bufall,nbyte);
 if(strstr(bufmsg,EXIT_STRING) !=NULL){
 puts("Good Bye");
 close(sd);
 exit(0);
}
}
return 0;
}

int recv_and_print(int sd) {
 char buf[MAXLINE+1];
 int nbyte;
 while(1){
 if( (nbyte=read(sd,bufmsg,MAXLINE))<0){
 perror("read fail");
 close(sd);
 exit(0);
}
bufmsg[nbyte]=0;
printf("%s",bufmsg);
if(strstr(bufmsg, EXIT_STRING) !=NULL ) {
puts("Good bye");
exit(0);
}

}
return 0;
}
