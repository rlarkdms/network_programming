vi udp_talkcli1.c
vi udp_talkserv1.c
cc udp_talkserv1.c -o udp_talkserv1
vi udp_talkcli1.c
ls
./udp_talkserv1 3015
rm udp_talkserv1.c
vi udp_talkserv1.c
cc udp_talkserv1.c -o udp_talkserv1
./udp_talkserv1 3015
rm udp_talkserv1.c
vi udp_talkserv1.c
cc udp_talkserv1.c -o udp_talkserv1
./udp_talkserv1 3015
./udp_talkserv 3015
vi upd_talkserv.c
vi udp_talkserv.c
cc udp_talkserv.c -o udp_talkserv
./udp_talkserv 3015
vi udp_talkserv.c
cc udp_talkserv.c -o udp_talkserv
./udp_talkserv 3015
cd ..
ls
script quiz1
ls
mail netpa00 < quiz1
prog/udp_talkcli 117.17.142.162 8015
ls
cd prog
ls
./udp_talkserv 3015
vi udp_talkserv2.c
vi udp_talkserv.c
cc udp_talkserv.c -o udp_talkserv
./udp_talkserv 3015
vi udp_talkserv.c
cc udp_talkserv.c -o udp_talkserv
./udp_talkserv 3015
ls
cd prog
ls
vi udp_talkserv.c
vi udp_talkvcli.c
vi udp_talkcli.c
cat prog/udp_talkserv.c
cat prog/udp_talkcli.c
prog/udp_talkcli 117.17.142.162 8015
ls -l prog/udp_talk*
ls
cd prog
ls
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
vi udp_talkserv2.c
ls
rm udp_talkserv2.c
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
rm udp_talkserv2.c
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
rm udp_talkserv2.c
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
rm udp_talkserv2.c
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
rm udp_talkserv2.c
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
./udp_talkserv2 3015
rm udp_talkserv2.c
vi udp_talkserv2.c

./udp_talkserv2 3015
rm udp_talkserv2.c
vi udp_talkserv2.c
cc udp_talkserv2.c -o udp_talkserv2
./udp_talkserv2 3015
ls
rm udp_talkserv.c
rm udp_talkserv
mv udp_talkserv2.c udp_talkserv.c
mv udp_talkserv2 udp_talkserv
ls
cd ..
prog/udp_talkserv 7015
cd prog
ls
./udp_talkserv 8015
logout
cd prog
ls
./udp_talkcli 127.0.0.1 3015
prog/udp_talkcli 117.17.142.162 7015
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
char *EXIT_STRING = "exit";
int recv_and_print(int sd, struct sockaddr_in *servaddr); 
int input_and_send(int sd, struct sockaddr_in *servaddr);
#define MAXLINE 511
int main(int argc, char *argv[]) {
       struct sockaddr_in cliaddr, servaddr;
       char buf[MAXLINE+1];
       int s,nbyte,addrlen=sizeof(struct sockaddr);
       pid_t pid;
       if(argc != 2) {
               printf("사용법: %s port\n", argv[0]);
               exit(0);
       }
       if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
               perror("socket fail\n");
               exit(0);
       }
       bzero((char *)&servaddr, sizeof(servaddr));
       bzero((char *)&cliaddr, sizeof(cliaddr));
       servaddr.sin_family = AF_INET;
       servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
       servaddr.sin_port = htons(atoi(argv[1]));
       if(bind(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
               perror("bind fail\n");
               exit(0);
       }
       puts("서버가 클라이언트를 기다리고 있습니다.");
       if((nbyte = recvfrom(s, buf, MAXLINE, 0, (struct sockaddr *)&servaddr, &addrlen)) < 0) {
              perror("read fail");
              exit(0);
       }
       puts("클라이언트가 연결되었습니다.");
       buf[nbyte]=0;
       printf("%s", buf);
 
      if((pid = fork()) > 0) {
               input_and_send(s, &servaddr);
       } else if(pid == 0) {
               recv_and_print(s, &servaddr); 
       }
       close(s);
       return 0;
}
int input_and_send(int sd, struct sockaddr_in* servaddr) {
       char buf[MAXLINE + 1];
       int nbyte, addrlen = sizeof(struct sockaddr_in);
       while(fgets(buf, sizeof(buf), stdin) != NULL) {
               nbyte = strlen(buf);
               if(sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)servaddr, addrlen) <0)
                       perror("sendto fail");
               if(strstr(buf, EXIT_STRING) != NULL) {
                       close(sd);
                       exit(0);
               }
       }
       return 0;
}
int recv_and_print(int sd, struct sockaddr_in* servaddr) {
       char buf[MAXLINE + 1];
       int nbyte, addrlen = sizeof(struct sockaddr_in);
       while(1) {
               if((nbyte = recvfrom(sd, buf, MAXLINE, 0, (struct sockaddr *)servaddr, &addrlen)) < 0) {
                       perror("read fail");
                       exit(0);
               }
               buf[nbyte] = 0;
               if(strstr(buf, EXIT_STRING) != NULL) {
                       break;
               }
               printf("%s", buf);
 
}
return 0;
prog/udp_talkcli 117.17.142.162 7015
cd ..
prog/udp_talkcli 117.17.142.162 7015
ls
script quiz1b
mail netpa00 < quiz1b
logout
cd prog
ls
./tcp_chatcli 127.0.0.1 3015 kimgaeun
./tcp_talkcli 127.0.0.1 3015 
./tcp_chatcli 127.0.0.1 3015 kimgaeun
./tcp_talkcli_test 127.0.0.1 3015 gayoung
logout
ls
cd prog
ls
./tcp_chatcli 127.0.0.1 3015 dkdk
./tcp_talkcli_test 127.0.0.1 3015 nabi
logout
ls
cd prog
ls
./tcp_chatcli 127.0.0.1 3015 kimgayoung
./tcp_talkcli 127.0.0.1 3015
ls
vi tcp_talkcli_test.c 
cc tcp_talkcli_test.c -o tcp_talkcli_test
vi tcp_talkcli_test.c 
cc tcp_talkcli_test.c -o tcp_talkcli_test
vi tcp_talkcli_test.c 
./tcp_chatcli 127.0.0.1 3015 kimgayoung
vi tcp_talkcli_test.c 
cc tcp_talkcli_test.c -o tcp_talkcli_test
./tcp_talkcli 127.0.0.1 3015 gaeun
./t
./tcp_talkcli_test 127.0.0.1 3015 gaeun
ls
cd prog
history
ls
vi tcp_chatserv.c
cc tcp_chatserv.c -o tcp_chatserv
vi tcp_chatcli.c
cc tcp_chatcli.c -o tcp_chatcli
./tcp_chatserv 3015
ls
cp tcp_talkcli.c tcp_talkcli_test.c
ls
cp tcp_chatserv.c tcp_chatserv_test.c
ls
./tcp_chatserv 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
vi tcp_chatserv_test.c
cc tcp_chatserv_test.c -o tcp_chatserv_test
./tcp_chatserv_test 3015
cat prog/tcp_talkcli1.c
cat prog/tcp_chatserv1.c
prog/tcp_chatcli 127.0.0.1 5015 c15
cat prog/tcp_talkcli1.c
cat prog/tcp_chatserv1.c
prog/tcp_chatcli 127.0.0.1 5015 c15
ls -l prog/tcp_chat*
cat prog/tcp_talkcli1.c
cat prog/tcp_chatserv1.c
prog/tcp_chatcli 127.0.0.1 5015 c15
ls -l prog/tcp_chat*
prog/tcp_talkcli1 127.0.0.1 5015 b15
prog/tcp_talkcli1 127.0.0.1 6015 b15
prog/tcp_talkcli1 127.0.0.1 5015 b15
prog/tcp_chatcli1 127.0.0.1 5015 c15
prog/tcp_chatcli 127.0.0.1 5015 c15
script quiz2
ls
rm quiz2
script quiz2
mail netpa00 <quiz2
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
 while(fgets(bufmsg,sizeof(bufmsg),stdin) !=NULL) {
  nbyte=namelen+strlen(bufmsg);
  write(sd,bufall,nbyte);
 if(strstr(bufmsg,EXIT_STRING) !=NULL){
 puts("Good Bye");
 close(sd);
 exit(0);
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
return 0;
}
ls
prog/tcp_chatcli 127.0.0.1 5015 c15
[b15] : netpa 15
prog/tcp_chatcli 127.0.0.1 5015 c15
prog/tcp_chatcli 127.0.0.1 6015 c15
ls
rm quiz2
script quiz2
mail netpa00 < quiz2
logout
prog/tcp_chatcli 127.0.0.1 5015 c15
prog/tcp_chatcli 127.0.0.1 6015 c15
prog/tcp_chatcli 127.0.0.1 5015 c15
cd prog
ls
vi tcp_talkcli1.c
cc tcp_talkcli1.c -o tcp_talkcli1
./tcp_talkcli1 127.0.0.1 5015 b15
vi tcp_talkcli1.c
cc tcp_talkcli1.c -o tcp_talkcli1
./tcp_talkcli1 127.0.0.1 5015 b15
vi tcp_talkcli1.c
cc tcp_talkcli1.c -o tcp_talkcli1
./tcp_talkcli1 127.0.0.1 5015 b15
vi tcp_talkcli1.c
cc tcp_talkcli1.c -o tcp_talkcli1
./tcp_talkcli1 127.0.0.1 5015 b15
vi tcp_talkcli1.c
./tcp_talkcli1 127.0.0.1 5015 b15
./tcp_talkcli1 127.0.0.1 6015 b15
./tcp_talkcli1 127.0.0.1 5015 b15
logout
cd prog
ls
./tcp_chatcli 127.0.0.1 3000 netpa15
./tcp_talkcli 127.0.0.1 3000
ps
ls
mv tcp_chatserv_test.c tcp_chatserv1.c
mv tcp_chatserv_test tcp_chatserv
mv tcp_talkcli_test.c tcp_talkcli1.c
mv tcp_talkcli_test tcp_talkcli1
ls
vi tcp_chatserv1.c
vi tcp_talkcli1.c
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
cd ..
prog/tcp_chatserv1 5015
ps
prog/tcp_chatserv1 5015
ps
ps eaf| grep netpa15
ps -eaf | grep netpa15 
prog/tcp_chatserv1 5015
prog/tcp_chatserv1 6015
vi tcp_chatserv1.c
cd prog
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
vi tcp_chatserv1.c
cc tcp_chatserv1.c -o tcp_chatserv1
./tcp_chatserv1 5015
ps -eaf | grep netpa15
./tcp_chatserv1 015
./tcp_chatserv1 5015
./tcp_chatserv1 6015
./tcp_chatserv1 5015
logout
ls
cd prog
ls
cat tcp_talkcli1.c
logout
gedit aa
ls
cd prog
ls
mkdir proj
ls
cd proj
ls
vi aa.c
cc aa.c -o aa
vi aa.c
logout
cat prog/tcp_chatserv_nonb.c
prog/tcp_chatserv_nonb 8015
ls -l prog/tcp_chatserv_non*
prog/tcp_chatcli 117.17.142.162 8015 b15
prog/tcp_talkcli1 117.17.142.162 8015 c15
prog/tcp_talkcli1 117.17.142.162 8015 c15
script check6c
logout
prog/tcp_chatcli 117.17.142.162 8015 b15
script check6b
logout
ls
cd prog
ls
vi tcp_chat_nonb.c
cc tcp_chat_nonb.c -o tcp_chat_nonb
rm tcp_chat_nonb.c 
vi tcp_chat_nonb.c
cc tcp_chat_nonb.c -o tcp_chat_nonb
rm tcp_chat_nonb.c 
vi tcp_chat_nonb.c
cc tcp_chat_nonb.c -o tcp_chat_nonb
rm tcp_chat_nonb.c 
vi tcp_chat_nonb.c
cc tcp_chat_nonb.c -o tcp_chat_nonb
rm tcp_chat_nonb.c 
vi tcp_chat_nonb.c
cc tcp_chat_nonb.c -o tcp_chat_nonb
cd ..
prog/tcp_chatserv_nonb 8015
cd prog
ls
mv tcp_chat_nonb tcp_chatserv_nonb
mv tcp_chat_nonb.c  tcp_chatserv_nonb.c
cd ..
prog/tcp_chatserv_nonb 8015
cd prog
ls
vi tcp_chatserv_nonb.c
cc tcp_chatserv_nonb.c -o tcp_chatserv_nonb
cd ..
prog/tcp_chatserv_nonb 8015
ps -eaf | grep netpa15
kill -9 13285
ls
kill -9 13285
ps -eaf | grep netpa15
ls
script check6a
ls
mail netpa00 < check6a
mail netpa00 < check6b
mail netpa00 < check6c
logout
ls
cd prog
vi multicast.c
cc multicast.c -o multicast
rm multicast.c 
vi multicast.c
cc multicast.c -o multicast
rm multicast.c 
vi multicast.c
cc multicast.c -o multicast
multicast 239.0.3.3 3000 b15
./multicast 239.0.3.3 3000 b15
$TERM
TERM
echo TERM
echo $TERM
echo $LENG
ls
ls -l
ls -l prog
ls -ld prog
alias
alias -l
kill -l
jobs
ss
ls
ls -l
ld
ld -l
ls -ld
ls
ls -l
ls -ld /prog
ld /prog
ld -l /prog
 /prog
ld prog
ls -ld prog
ls -l prog
ps
ps aux
jobs
ls
cd prog
ls
git remote -v
pip install git
ls
cd ..
ls
git init
git remote add origin "https://github.com/rlarkdms/network_programming.git"
git add-A
git add -A
git commit -m "1st commit"
git push origin
git push --set-upstream origin master
git push origin
git push --set-upstream origin master
ls
cd prog
ls
./udp_talkserv 8015
ls
cd prog
./udp_talkcli 127.0.0.1 8015
cd
ls
cd prog
ls
./tcp_talkserv 3015
./tcp_talkcli 127.0.0.1 3015
cd prog
./tcp_talkcli 127.0.0.1 3015
ls
cd proj
cd ..
ls
cd netpa01
cd ..
cd
ls
cd proj
ls
rm aa.c
vi tcp_chat.c
cc tcp_chat.c -o tcp_chat
cd ..
cd prog
ls
cat tcp_chatserv_nonb.c
cd ..
ls
cd proj
ls
cat tcp_chatserv_nonb.c
cc tcp_chat.c -o tcp_chat
rm tcp_chat.c 
vi tcp_chat.c
cc tcp_chat.c -o tcp_chat
rm tcp_chat.c 
vi tcp_chat.c
cc tcp_chat.c -o tcp_chat
vi projcli.c
cc projcli.c -o projcli
ls
rm projcli.c
vi projcli.c
cc projcli.c -o projcli
ls
rm projcli.c
vi projcli.c
ls
cc projcli.c -o projcli
./tcp_chat 3015
ls
rm tcp_chat.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
vi projserv.c
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
ps -auf |grep netpa15
kill -9 25742
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
ps -auf |grep netpa15
ps -aux |grep netpa15
./projserv 4015
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
ls
cd proj
ls
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
cd proj
ls
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
cd proh
cd proj
ls
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
cd proj
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
cd proj
ls
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
./projcli 127.0.0.1 4015
./projcli 127.0.0.1 3015
ls
cd proj
ls
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
vi projserv.c
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 4015
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
./projserv 4015
clear
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
./projserv 3015
rm projserv.c
vi projserv.c
cc projserv.c -o projserv
cat projserv.c
./projserv 127.0.0.1 3015
cat projcli.c
./projcli 127.0.0.1 3015 gaeun
./projcli 127.0.0.1 3015 gayoung
cat projserv.c
./projserv 127.0.0.1 4015
cat projcli.c
./projcli 127.0.0.1 4015 gaeun
./projcli 127.0.0.1 4015 gayoung
cat projserv.c
./projserv 127.0.0.1 3015
cat projcli.c
./projcli 127.0.0.1 3015 gaeun
./projcli 127.0.0.1 3015 gayoung
cat projserv.c
./projserv 127.0.0.1 3015
./projserv 127.0.0.1 4015
./projcli 127.0.0.1 4015 gayoung
cat projcli.c
./projcli 127.0.0.1 4015 gaeun
cat projserv.c
./projserv 127.0.0.1 4015
