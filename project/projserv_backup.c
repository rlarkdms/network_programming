#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>


#define MAXLINE 10
#define MAX_SOCK 1024

char *EXIT_STRING = "exit";
char *START_STRING = "조건이 충족하여 게임을 시작합니다. \n";
char *NOT_START_STRING= "조건이 충족하지 않아 게임을 시작할 수 없습니다. \n";
int maxfdp1; 
int num_chat = 0;
int clisock_list[MAX_SOCK]; 
int listen_sock; 

void addClient(int s, struct sockaddr_in *newcliaddr);
void removeClient(int s); 
int tcp_listen(int host, int port, int backlog);
void errquit(char *msg) { perror(msg); exit(1); }
int set_nonblock(int sockfd);
int is_nonblock(int sockfd);

int main(int argc, char *argv[]) {
        struct sockaddr_in cliaddr;
        int i, j, nbyte, count;
        int addrlen;
        int accp_sock, clilen;
        char buf[MAXLINE]={0};
        int randNum1,randNum2;
        int temp=0;
        char *chip_num="칩의 개수: ";
        char chip_numberA[100];
        char chip_numberB[100];
        char *you_num="상대방의 카드 숫자: ";
        char *betting_num="배팅할 칩의 개수를 입력해주세요";
        char you_numberA[100];
        char you_numberB[100];
        char *round_num="라운드 ";
        char *you_betting_num="상대방이 배팅한 칩의 개수: ";
        char *Fight="승부를 진행하시겠습니까?(y/n)";
        char you_betting_numberA[100];
        char you_betting_numberB[100];
        char round_number[10];
        char *last="):";
        char *jump="\n";
        int round=0;
        int gs=0;
        int arrayA[10]={1,2,3,4,5,6,7,8,9,10};
        int arrayB[10]={1,2,3,4,5,6,7,8,9,10};
        int chipA=15;
        int chipB=15;
        char betting_chipA[MAXLINE]={0};
        char betting_chipB[MAXLINE]={0};
        char trueA[MAXLINE+1];
        char trueB[MAXLINE+1];
        char *win="당신이 이겼습니다.\n";
        char *lose="당신이 졌습니다.\n";
        char *draw="비겼습니다.\n";
        int penalty=0;



        if(argc != 2) {
                printf("Usage : %s port\n", argv[0]);
                exit(0);
        }

        listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
if(listen_sock==1)
errquit("tcp_listen fail");
if(set_nonblock(listen_sock)==-1)
errquit("set_nonblock fail");

srand(time(NULL));    
for(i=0; i<30; i++) {   

randNum1 = rand() % 10;  

randNum2 = rand() % 10;

temp = arrayA[randNum1];

arrayA[randNum1] = arrayA[randNum2];

arrayA[randNum2] = temp;

}
for(i=0; i<30; i++) {   

randNum1 = rand() % 10;  

randNum2 = rand() % 10;

temp = arrayB[randNum1];

arrayB[randNum1] = arrayB[randNum2];

arrayB[randNum2] = temp;

}

while(1){

addrlen=sizeof(cliaddr);
accp_sock=accept(listen_sock, (struct sockaddr*)&cliaddr, &clilen);

if(accp_sock==-1 && errno!=EWOULDBLOCK)
errquit("accept fail");
else if (accp_sock>0){
clisock_list[num_chat]=accp_sock;
if(is_nonblock(accp_sock)!=0 &&set_nonblock(accp_sock)<0)
errquit("set_nonblock fail");
addClient(accp_sock,&cliaddr);
printf("%d번째 사용자 추가.\n",num_chat);

if (num_chat==2){
    gs=1;

    for(i=0; i<num_chat; i++)
     send(clisock_list[i],START_STRING,strlen(START_STRING),0);
     printf("사용자 A와 B가 게임을 시작합니다\n");
    
    

}
else{
    gs=0;
    for(i=0; i<num_chat; i++)
     send(clisock_list[i],NOT_START_STRING,strlen(NOT_START_STRING),0);

}

}




for(i=0; i<num_chat; i++){
    errno=0;
    if (gs==1){
        gs++;
        sprintf(chip_numberA, "%s%d%s",chip_num,chipA,jump);
        sprintf(chip_numberB, "%s%d%s",chip_num,chipB,jump);
        sprintf(round_number,"%s%d%s",round_num,round+1,jump);

        sprintf(you_numberA, "%s%d%s",you_num,arrayA[round],jump);
        sprintf(you_numberB, "%s%d%s",you_num,arrayB[round],jump);


    send(clisock_list[0],round_number,strlen(round_number),0);    
    send(clisock_list[0],chip_numberA,strlen(chip_numberA),0);
    send(clisock_list[0],you_numberB,strlen(you_numberB),0);
    send(clisock_list[0],betting_num,strlen(betting_num),0);
    

    send(clisock_list[1],round_number,strlen(round_number),0);    
    send(clisock_list[1],chip_numberB,strlen(chip_numberB),0);
    send(clisock_list[1],you_numberA,strlen(you_numberA),0);
    send(clisock_list[1],betting_num,strlen(betting_num),0);
    printf("A의 %s",chip_numberA);
    printf("B의 %s",chip_numberB);
    printf("\n betting_chipA는 %s",betting_chipA);
    printf("\n betting_chipB는 %s",betting_chipB);
    printf("\n buf는 %s",buf);
    



    }







    if (gs==2){

    nbyte=recv(clisock_list[i],buf,MAXLINE,0);

    if(nbyte==0){
        removeClient(i);
        continue;
    }else if (nbyte==-1&&errno==EWOULDBLOCK)
    continue;
    if(strstr(buf,EXIT_STRING)!=NULL){
        removeClient(i);
        continue;
    }
    buf[nbyte]=0;
    if(i==0){
        memcpy(betting_chipA,buf,sizeof(buf));
    }
    if(i==1){
        memcpy(betting_chipB,buf,sizeof(buf));
    }
    if (betting_chipA[0]!=0&&betting_chipB[0]!=0){

        gs=gs+1; 
        printf("둘다 입력했음");
        printf("A%s ,B%s", betting_chipA,betting_chipB);
        sprintf(you_betting_numberA,"%s%s%s",you_betting_num,betting_chipB,jump);
        sprintf(you_betting_numberB,"%s%s%s",you_betting_num,betting_chipA,jump);
        send(clisock_list[0],you_betting_numberA,strlen(you_betting_numberA),0);        
        send(clisock_list[1],you_betting_numberB,strlen(you_betting_numberB),0);
        send(clisock_list[0],Fight,strlen(Fight),0);
        send(clisock_list[1],Fight,strlen(Fight),0);

    }
    printf("%d", gs);
    }







    if (gs==3){
    
    nbyte=recv(clisock_list[i],buf,MAXLINE,0);
    if(i==0){
        memcpy(trueA,buf,sizeof(buf));
       
    }
    if(i==1){
        memcpy(trueB,buf,sizeof(buf));
    
    }
   if(nbyte==0){
        removeClient(i);
        continue;
    }
    else if (nbyte==-1&&errno==EWOULDBLOCK)
    continue;
    if(strstr(buf,EXIT_STRING)!=NULL){
        removeClient(i);
        continue;
    }
    printf("A%c , B%c",trueA[0],trueB[0]);
    if (trueA[0]=='y'&&trueB[0]=='y'){ 
        printf("\nbetting_chipA : %d \n betting_chipB : %d\n",atoi(betting_chipA),atoi(betting_chipB));
        gs=1;

        if (arrayA[round]>arrayB[round]){
            chipA=chipA+atoi(betting_chipB);
            chipB=chipB-atoi(betting_chipB);
         send(clisock_list[0],win,strlen(win),0);
        send(clisock_list[1],lose,strlen(lose),0);


        }else if (arrayA[round]<arrayB[round]){
            chipA=chipA-atoi(betting_chipA);
            chipB=chipB+atoi(betting_chipA);
        send(clisock_list[1],win,strlen(win),0);
        send(clisock_list[0],lose,strlen(lose),0);


        }else{
        send(clisock_list[0],draw,strlen(draw),0);
        send(clisock_list[1],draw,strlen(draw),0);
        }
        round++;
        betting_chipA[0]='\0';
        betting_chipB[0]='\0';

    }
    else if(trueA[0]=='y'&&trueB[0]=='n'){
        gs=1;
        if (arrayB[round]==10){
            chipB=chipB-5;
        }
        else{
               chipB=chipB-2;
        }
        
        send(clisock_list[0],win,strlen(win),0);
        send(clisock_list[1],lose,strlen(lose),0);
        round++;        
        betting_chipA[0]='\0';
        betting_chipB[0]='\0';

    }
    else if(trueA[0]=='n'&&trueB[0]=='y'){
        gs=1;
        if (arrayA[round]==10){
            chipA=chipA-5;
        }
        else{
               chipA=chipA-2;
        }
        send(clisock_list[1],win,strlen(win),0);
        send(clisock_list[0],lose,strlen(lose),0);
        round++;
        
        betting_chipA[0]='\0';
        betting_chipB[0]='\0';

    }
    else if(trueA[0]=='n'&&trueB[0]=='n'){
        gs=1;
        send(clisock_list[0],draw,strlen(draw),0);
        send(clisock_list[1],draw,strlen(draw),0);
        round++;
        
        betting_chipA[0]='\0';
        betting_chipB[0]='\0';

    }


}







}
}
}

void addClient(int s, struct sockaddr_in *newcliaddr) {
        char buf[20];
        inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
        printf("new client : %s\n", buf);
        clisock_list[num_chat] = s;
        num_chat++;
}

void removeClient(int s) {
        close(clisock_list[s]);
        if(s != num_chat-1)
                clisock_list[s] = clisock_list[num_chat-1];
        num_chat--;
        printf("참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_chat);
}

int is_nonblock(int sockfd){
    int val;
    val=fcntl(sockfd,F_GETFL,0);
    if(val&O_NONBLOCK)
    return 0;
    return -1;

}

int set_nonblock(int sockfd){
    int val;
    val=fcntl(sockfd,F_GETFL,0);
    if(fcntl(sockfd,F_SETFL,val | O_NONBLOCK)==-1)
    return -1;
    return 0;

}

int tcp_listen(int host, int port, int backlog) {
        int sd;
        struct sockaddr_in servaddr;

        sd = socket(AF_INET, SOCK_STREAM, 0);
        if(sd == 1) {
                perror("socket fail");
                exit(1);
        }

        bzero((char *)&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(host);
        servaddr.sin_port = htons(port);

        if(bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
                perror("bind fail");
                exit(1);
        }

        listen(sd, backlog);
        return sd;
}
