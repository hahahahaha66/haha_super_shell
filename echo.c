#include<unistd.h>
#include<getopt.h>
#include<string.h>
#include<stdio.h>
int main(int argc,char*argv[]){
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"|")==0){
            return i;
        }
        printf("%s ",argv[i]);
    }
    printf("\n");
    return 0;
}