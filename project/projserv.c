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
char *START_STRING = "조건이 충족하여 게임을 시작합니다.\n-인디언 포커 규칙-\n1대 1 방식의 게임이며 각각의 플레이어는 10개의 카드(1~10)와 칩(15개)을 지급받습니다.\n각 라운드마다 상대방의 카드를 볼수있으며 카드를 보고 배팅할 칩의 개수를(1개 이상) 결정합니다.\n그 후 상대방이 배팅한 칩의 개수와 상대방의 카드의 수를 보고 승부를 진행할지 결정합니다.\n만약 당신이 승부에 임하고 상대방도 승부에 임했을시에는 카드패가 높은 사람이 이기며 패배한 사람이 배팅한 칩의 수 만큼 이기사람이 가져갑니다.\n";
char *START_STRING2="또한 한명은 승부에 임하고 힌명은 승부에 임하지 않았다면 승부에 임하지 않은 사람이 패널티로 2개의 칩을 지불합니다.\n그런데 여기서 자신이 가지고 있는 카드의 수가 10이였는데 승부를 포기했다면 패널티로 5개의 칩을 지불합니다.\n";
char *START_STRING3="마지막으로 둘다 승부에 임하지 않을 경우는 아무일도 일어나지 않습니다.\n라운드는 총 10번이며 10번안에 승부가 나지 않는다면 10번째 라운드가 끝나고 각자 가지고 있는 칩의 개수로 승부를 내고 exit합니다.\n그리고 라운드 진행동안 칩의 개수가 0개가 된다면 칩의 개수 0개인 사람이 지게 되며 클라이언트들은 exit됩니다.";
char *NOT_START_STRING= "조건이 충족하지 않아 게임을 시작할 수 없습니다. \n";
int maxfdp1; 
int num_chat = 0;
int clisock_list[MAX_SOCK]; 
int listen_sock; 
struct sockaddr_in cliaddr;
        int i, j,s, nbyte, count;
        int addrlen;
        int accp_sock, clilen;
        char buf[MAXLINE]={0};
        int randNum1,randNum2;
        int temp=0;
        int gs=0;
        int arrayA[10]={1,2,3,4,5,6,7,8,9,10};
        int arrayB[10]={1,2,3,4,5,6,7,8,9,10};
        int chipA=15;
        int chipB=15;
        char *chip_num="당신의 칩의 개수: ";
        char chip_numberA[100];
        char chip_numberB[100];
        char *round_line="----------------------------------------------\n";
        char *you_num="상대방의 카드 숫자: ";
        char *betting_num1="배팅할 칩의 개수를 입력해주세요(1~";
        char *betting_num2=")";
        char betting_numberA[100];
        char betting_numberB[100];
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
        char betting_chipA[MAXLINE]={0};
        char betting_chipB[MAXLINE]={0};
        char user_nameA[MAXLINE]={0};
        char user_nameB[MAXLINE]={0};
        char trueA[MAXLINE+1];
        char trueB[MAXLINE+1];
        char *win="당신이 이겼습니다.\n";
        char *lose="당신이 졌습니다.\n";
        char *draw="비겼습니다.\n";
        char *win_set="\n상대방의 칩이 0개가 되어 진행을 종료합니다.\n 당신은 승리하셨습니다!\n";
        char *lose_set="\n당신의 칩이 0가 되어 진행을 종료합니다.\n 당신은 패배하였습니다!\n";
        int penalty=0;
        char *win_round_set="\n10 라운드가 모두 종료되어 승자를 발표합니다!\n 당신은 승리하셨습니다!\n";
        char *lose_round_set="\n10 라우드가 모두 종료되어 승자를 발표합니다!\n 당신은 패배하셨습니다!\n";
        char *draw_round_set="\n10 라우드가 모두 종료되어 승자를 발표합니다!\n 칩의 개수가 똑같아 무승부입니다!\n";
        char *chip_result="\n당신의 카드의 숫자는 ";
        char *chip_final=" 였습니다.\n";
        char chip_allA[100];
        char chip_allB[100];

void addClient(int s, struct sockaddr_in *newcliaddr);
void removeClient(int s); 
int tcp_listen(int host, int port, int backlog);
void errquit(char *msg) { perror(msg); exit(1); }
int set_nonblock(int sockfd);
int is_nonblock(int sockfd);
void bat();

int main(int argc, char *argv[]) {
    int round=0;


        

        if(argc != 3) {
                printf("Usage : %s ip port\n", argv[0]);
                exit(0);
        }

        listen_sock = tcp_listen(INADDR_ANY, atoi(argv[2]), 2);
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
    for (s=0; s<3000; s++){


    }

    for(i=0; i<num_chat; i++){
    for (s=0; s<3000; s++){

        
    }
    nbyte=recv(clisock_list[i],buf,MAXLINE,0);
    for (s=0; s<3000; s++){

        
    }

    buf[nbyte]=0;
    if(i==0){
        memcpy(user_nameA,buf,sizeof(buf));
    }
    if(i==1){
        memcpy(user_nameB,buf,sizeof(buf));
    }

    for (s=0; s<3000; s++){

        
    }


     send(clisock_list[i],START_STRING,strlen(START_STRING),0);

    for (s=0; s<3000; s++){

        
    }


    }



     printf("사용자 %s와 %s가 게임을 시작합니다\n",user_nameA,user_nameB);

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


        sprintf(betting_numberA,"%s%d%s",betting_num1,chipA,betting_num2);
        sprintf(betting_numberB,"%s%d%s",betting_num1,chipB,betting_num2);

    send(clisock_list[0],round_line,strlen(round_line),0);    
    send(clisock_list[0],round_number,strlen(round_number),0);    
    send(clisock_list[0],chip_numberA,strlen(chip_numberA),0);
    send(clisock_list[0],you_numberB,strlen(you_numberB),0);
    send(clisock_list[0],betting_numberA,strlen(betting_numberA),0);
    
    send(clisock_list[1],round_line,strlen(round_line),0);    
    send(clisock_list[1],round_number,strlen(round_number),0);    
    send(clisock_list[1],chip_numberB,strlen(chip_numberB),0);
    send(clisock_list[1],you_numberA,strlen(you_numberA),0);
    send(clisock_list[1],betting_numberB,strlen(betting_numberB),0);

    }







    if (gs==2){

    nbyte=recv(clisock_list[i],buf,MAXLINE,0);

    if(nbyte==0){
        removeClient(i);
        gs=0;
        continue;
    }else if (nbyte==-1&&errno==EWOULDBLOCK)
    continue;
    if(strstr(buf,EXIT_STRING)!=NULL){
        removeClient(i);
        gs=0;
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

        sprintf(you_betting_numberA,"%s%s%s",you_betting_num,betting_chipB,jump);
        sprintf(you_betting_numberB,"%s%s%s",you_betting_num,betting_chipA,jump);
        send(clisock_list[0],you_betting_numberA,strlen(you_betting_numberA),0);        
        send(clisock_list[1],you_betting_numberB,strlen(you_betting_numberB),0);
        send(clisock_list[0],Fight,strlen(Fight),0);
        send(clisock_list[1],Fight,strlen(Fight),0);

    }
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
        gs=0;
        continue;
    }
    else if (nbyte==-1&&errno==EWOULDBLOCK)
    continue;
    if(strstr(buf,EXIT_STRING)!=NULL){
        gs=0;
        removeClient(i);
        continue;
    }
    if (trueA[0]=='y'&&trueB[0]=='y'){ 
        gs=1;

        if (arrayA[round]>arrayB[round]){
            chipA=chipA+atoi(betting_chipB);
            chipB=chipB-atoi(betting_chipB);
         send(clisock_list[0],win,strlen(win),0);
        send(clisock_list[1],lose,strlen(lose),0);

        printf("\n라운드%d는 %s사용자가 이겼습니다.",round+1,user_nameA);

        }else if (arrayA[round]<arrayB[round]){
            chipA=chipA-atoi(betting_chipA);
            chipB=chipB+atoi(betting_chipA);
        send(clisock_list[1],win,strlen(win),0);
        send(clisock_list[0],lose,strlen(lose),0);

        printf("\n라운드%d는 %s사용자가 이겼습니다.",round+1,user_nameB);

        }else{
        send(clisock_list[0],draw,strlen(draw),0);
        send(clisock_list[1],draw,strlen(draw),0);
        printf("\n라운드%d는 비겼습니다.",round+1);
        }
        sprintf(chip_allA, "%s%d%s",chip_result,arrayA[round],chip_final);
        sprintf(chip_allB, "%s%d%s",chip_result,arrayB[round],chip_final);
        send(clisock_list[0],chip_allA,strlen(chip_allA),0);
        send(clisock_list[1],chip_allB,strlen(chip_allB),0);
        bat(round);

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

        printf("\n라운드%d는 %s사용자가 이겼습니다.",round+1,user_nameA);

        sprintf(chip_allA, "%s%d%s",chip_result,arrayA[round],chip_final);
        sprintf(chip_allB, "%s%d%s",chip_result,arrayB[round],chip_final);
        send(clisock_list[0],chip_allA,strlen(chip_allA),0);
        send(clisock_list[1],chip_allB,strlen(chip_allB),0);
        

        bat(round);

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
        printf("\n라운드%d는 %s사용자가 이겼습니다.",round+1,user_nameB);

        sprintf(chip_allA, "%s%d%s",chip_result,arrayA[round],chip_final);
        sprintf(chip_allB, "%s%d%s",chip_result,arrayB[round],chip_final);
        send(clisock_list[0],chip_allA,strlen(chip_allA),0);
        send(clisock_list[1],chip_allB,strlen(chip_allB),0);

        bat(round);


        round++;

        betting_chipA[0]='\0';
        betting_chipB[0]='\0';

    }
    else if(trueA[0]=='n'&&trueB[0]=='n'){
        gs=1;
        send(clisock_list[0],draw,strlen(draw),0);
        send(clisock_list[1],draw,strlen(draw),0);
        printf("\n라운드%d는 비겼습니다.",round+1);

        sprintf(chip_allA, "%s%d%s",chip_result,arrayA[round],chip_final);
        sprintf(chip_allB, "%s%d%s",chip_result,arrayB[round],chip_final);
        send(clisock_list[0],chip_allA,strlen(chip_allA),0);
        send(clisock_list[1],chip_allB,strlen(chip_allB),0);

        bat(round);

        round++;

        betting_chipA[0]='\0';
        betting_chipB[0]='\0';

    }


}
}
}
}

void bat(int round_order){
        int k=0;
        if (chipA<=0){
            send(clisock_list[0],lose_set,strlen(lose_set),0);
            send(clisock_list[1],win_set,strlen(win_set),0);

printf("\n");
            for(k=0; k<20; k++){
                printf("*");
            }
            printf("\n최종 승자는 %s사용자입니다.",user_nameB);

        send(clisock_list[0],EXIT_STRING,strlen(EXIT_STRING),0);        
        send(clisock_list[1],EXIT_STRING,strlen(EXIT_STRING),0);
        exit(0);
        }
        if (chipB<=0){
            send(clisock_list[1],lose_set,strlen(lose_set),0);
            send(clisock_list[0],win_set,strlen(win_set),0);

printf("\n");
            for(k=0; k<20; k++){
                printf("*");
            }
            printf("\n최종 승자는 %s사용자입니다.",user_nameA);

        send(clisock_list[0],EXIT_STRING,strlen(EXIT_STRING),0);        
        send(clisock_list[1],EXIT_STRING,strlen(EXIT_STRING),0);
        exit(0);
        }
 
        if (round_order==9){

            if(chipA>chipB){ 
            send(clisock_list[1],lose_round_set,strlen(lose_round_set),0);
            send(clisock_list[0],win_round_set,strlen(win_round_set),0);
            
            printf("\n");
            for(k=0; k<20; k++){
                printf("*");
            }
            printf("\n최종 승자는 %s사용자입니다.",user_nameA);

            }
            else if(chipA<chipB){
            send(clisock_list[0],lose_round_set,strlen(lose_round_set),0);
            send(clisock_list[1],win_round_set,strlen(win_round_set),0);

printf("\n");
            for(k=0; k<20; k++){
                printf("*");
            }
            printf("\n최종 승자는 %s사용자입니다.",user_nameB);

            }else{
            send(clisock_list[0],draw_round_set,strlen(draw_round_set),0);
            send(clisock_list[1],draw_round_set,strlen(draw_round_set),0);
printf("\n");            
            for(k=0; k<20; k++){
                printf("*");
            }
            printf("\n서로 비겼습니다.");

            }
        send(clisock_list[0],EXIT_STRING,strlen(EXIT_STRING),0);        
        send(clisock_list[1],EXIT_STRING,strlen(EXIT_STRING),0);
        exit(0);


        }



}

void addClient(int s, struct sockaddr_in *newcliaddr) {
        char buf[20];
        inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
        printf("new client!\n");
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
