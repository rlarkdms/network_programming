#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define MAXLINE 127

int main(int argc, char *argv[]){
int s,nbyte;
struct sockaddr_in servaddr;
int i;
char buf[MAXLINE+1];
struct hostent *hp;
struct in_addr in;
char buf1[20];

if (argc !=3){
printf("Usage: %s hostname\n",argv[0]);
exit(0);
}

if((s=socket(PF_INET,SOCK_STREAM,0))<0){
perror("socket fail");
exit(1);
}
hp=gethostbyname(argv[1]);

bzero((char*)&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
for(i=0; hp->h_addr_list[i]; i++){
memcpy(&in.s_addr,hp->h_addr_list[i],sizeof(in.s_addr));
inet_ntop(AF_INET, &in,buf1,sizeof(buf));
printf("Server IP:%s\n",buf1);
}


inet_pton(AF_INET,buf1,&servaddr.sin_addr);
servaddr.sin_port=htons(atoi(argv[2]));

if(connect(s,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
perror("connect fail");
exit(0);
}

printf("입력: ");
if(fgets(buf,sizeof(buf),stdin)==NULL)
exit(0);

nbyte=strlen(buf);

if(write(s,buf,nbyte)<0){
printf("write error\n");
exit(0);
}

printf("수신: ");
if((nbyte=read(s,buf,MAXLINE))<0){
perror("read fail");
exit(0);
}

buf[nbyte]=0;
printf("%s",buf);
close(s);
return 0;

}
