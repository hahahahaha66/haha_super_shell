#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX_READ 4096
int main(int argc,char*argv[]){
    char str[MAX_READ]={0};
    int fd=open(argv[1],O_RDONLY);
    if(fd==-1){
        perror("open failed");
        exit(1);
    }
    int len=read(fd,str,MAX_READ);
    if(len==-1){
        perror("read failed");
        close(fd);
        exit(1);
    }
    str[len]='\0';
    printf("%s\n",str);
    close(fd);
    return 0;
}