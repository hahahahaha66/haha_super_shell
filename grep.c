#include<stdio.h>
#include<string.h>
#define MAX_BUFFER 4096
int main(int argc,char*argv[]){
    char str[MAX_BUFFER];
    FILE*p=stdin;
    if(argc==3){
        p=fopen(argv[2],"r");
    }
    while(fgets(str,MAX_BUFFER,p)!=NULL){
            int len=strlen(str);
            str[len]='\0';
            if(strstr(str,argv[1])!=NULL){
                printf("%s",str);
            }
    }
    fclose(p);
    return 0;
}