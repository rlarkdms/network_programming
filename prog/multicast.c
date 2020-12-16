#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1023

int main(int argc, char *argv[]){
int send_s,recv_s,n,len;
pid_t pid;
unsigned int yes=1;
struct sockaddr_in mcast_group;
struct ip_mreq mreq;
char name[10];

if (argc!=4){
printf("시용법: %s multicast_address port My_name \n",argv[0]);
exit(0);

}

sprintf(name, "[%s]",argv[3]);
memset(&mcast_group,0 ,sizeof(mcast_group));
mcast_group.sin_family=AF_INET;
mcast_group.sin_port=htons(atoi(argv[2]));
inet_pton(AF_INET,argv[1],&mcast_group.sin_addr);

if ((recv_s=socket(AF_INET,SOCK_DGRAM,0))<0){
printf("error:Can't create receive socket \n");
exit(0);
}

mreq.imr_multiaddr=mcast_group.sin_addr;
mreq.imr_interface.s_addr=htonl(INADDR_ANY);
if(setsockopt(recv_s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))<0){
printf("error:add membership \n");
exit(0);
}

if(setsockopt(recv_s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))<0){
printf("error: reuse setsockopt \n");
exit(0);
}

if(bind(recv_s, (struct sockaddr*)&mcast_group, sizeof(mcast_group))<0){
printf("error: reuse setsockopt \n");
exit(0);
}

if((send_s=socket(AF_INET, SOCK_DGRAM,0))<0){
printf("error: cant create send socket \n");
exit(0);
}


if ((pid=fork())<0){
printf("error:fork\n");
exit(0);
}else if (pid==0){
struct sockaddr_in from;
char message[MAXLINE+1];
for(;;){
printf("receiving message....\n");
len=sizeof(from);
if ((n=recvfrom(recv_s,message,MAXLINE,0, (struct sockaddr*)&from, &len))<0){
printf("error: recvfrom\n");
exit(0);
}
message[n]=0;
printf("Receiced Message: %s\n",message);
}
}else{
char message[MAXLINE+1],line[MAXLINE+1];
printf("Send message:");
while(fgets(message,MAXLINE,stdin)!=NULL){
sprintf(line, "%s %s", name, message);
len=strlen(line);
if(sendto(send_s,line,strlen(line),0,(struct sockaddr*)&mcast_group,sizeof(mcast_group))<len){
printf("error: sendto\n");
exit(0);
}
}
}
return 0;
}



