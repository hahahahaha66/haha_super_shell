
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


int main() {
    struct winsize size;
    if(isatty(STDIN_FILENO)!=0){
        ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
    }
    time_t haha;
    struct tm* hahaha;
    char buf[100];
    time(&haha);
    hahaha=localtime(&haha);
    strftime(buf,100,"%H:%M:%S",hahaha);
    for(int i=0;i<size.ws_col-10;i++){
        printf(" ");
    }
    printf("\033[30;47m %s \033[0m\r",buf);
    char*p=getenv("PWD");
    printf("%s\n",p);
    return 0;
}
