#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void){

struct servent *servent;
servent =getservbyname("echo","udp");
if(servent==NULL){
printf("서비스 정보를 얻을 수 없음. \n\n");
exit(0);
}

printf("UDP 에코 포트번호(네트워크 순서): %d \n", servent->s_port);
printf("UDP 에코 포트번호(호스트 순서): %d \n", ntohs(servent->s_port));

return 0;

}
