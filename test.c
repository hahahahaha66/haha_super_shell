
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include<sys/ioctl.h>

#define READ_END 0
#define WRITE_END 1
//gcc main2.c -l readline -fsanitize=address -o shell 



// 定义可补全的命令列表
int main(){
    time_t haha;
    struct tm* hahaha;
    char buf[100];
    time(&haha);
    hahaha=localtime(&haha);
    strftime(buf,100,"%H:%M:%S",hahaha);
    for(int i=0;i<90;i++){
        printf(" ");
    }
    char*begin;
    char*username=getenv("USER");
    char*hostname=getenv("LOGNAME");
    char*path=getenv("PWD");
    int uid=geteuid();
    sprintf(begin,"\033[30;47m %s \033[0m\r",buf);
        printf("[%s@%s %s]",username,hostname,path);
        if(uid==0){
            printf("# ");
        }
        else{
            printf("$ ");
        }
    return 0;
}