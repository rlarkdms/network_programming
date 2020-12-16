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

int main(int argc, char *argv[]){

struct hostent *hp;
struct in_addr in;
int i;
char buf[20];

if (argc <2){
printf("Usage: %s hostname\n",argv[0]);
exit(1);
}
hp=gethostbyname(argv[1]);
if(hp==NULL){
printf("gethostbyname fail\n");
exit(0);
}
printf("호스트 이용:%s\n",hp->h_name);
printf("호스트 주소타입 번호 %d\n",hp->h_addrtype);
printf("호스트 주소의 길이 %d 바이트\n",hp->h_length);
for(i=0; hp->h_addr_list[i]; i++){
memcpy(&in.s_addr,hp->h_addr_list[i],sizeof(in.s_addr));
inet_ntop(AF_INET, &in,buf,sizeof(buf));
printf("IP주소(%d번째) :%s\n",i+1,buf);
}

for(i=0; hp->h_aliases[i]; i++)
printf("호스트 별명(%d 번째) :%s ",i+1,hp->h_aliases[i]);
puts("");
 

return 0;

}
