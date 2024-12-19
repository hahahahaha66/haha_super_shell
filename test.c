
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
    char str[]={' ',' ',' ',' ',' ',' ',' ','a','\r','b'};
    printf("%s\n",str);
    return 0;
}