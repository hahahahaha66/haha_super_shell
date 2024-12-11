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

void haha(int sig){
    printf("捕捉到编号为%d信号,已忽略\n",sig);
}

int main(){
    struct sigaction signal;
    signal.sa_handler=haha;
    signal.sa_flags=0;
    sigemptyset(&signal.sa_mask);
    sigaction(1,&signal,NULL);
    return 0;
}