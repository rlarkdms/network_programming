#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int global_var=0;

int main(void){

pid_t pid;
int local_var=0;
if ((pid=fork())<0){
printf("fork error\n");
exit(0);

}else if(pid==0){
global_var++;
local_var++;
printf("CHILD -my pid is %d and parent's pid is %d\n", getpid(), getppid());
}
else{

sleep(2);
global_var+=5;
local_var+=5;
printf("Parent-my pid is %d,child's pid is %d\n",getpid(),pid);
}
printf("\t global var: %d\n", global_var);
printf("\t local var: %d\n", local_var);


}



