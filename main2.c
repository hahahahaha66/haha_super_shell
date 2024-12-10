#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<sys/wait.h>
#include<fcntl.h>
#define MAX_ORDER 100
pid_t q;

void cutting_string(char*str,char***result,int *count){
    int size=10;
    int i=0;
    char*token;
    *result=malloc(sizeof(char*)*size);
    if(*result==NULL){
        perror("malloc failed");
        exit(1);
    }
    str[strcspn(str,"\n")]='\0';
    token=strtok(str," ");
    while(token){
        if(i==size){
            size*=2;
            char**temp=realloc(*result,size*sizeof(char*));
            if(!temp){
                perror("realloc failed");
                free(*result);
                exit(1);
            }
            *result=temp;
        }
        (*result)[i++]=strdup(token);
        token=strtok(NULL," ");
    }
    *result=realloc(*result,(i+1)*sizeof(char*));
    (*result)[i]=NULL;
    *count=i;
}

void free_result(char**result,int count){
    for(int i=0;i<count;i++){
        free(result[i]);
    }
    free(result);
}



int main(){
    setenv("PATH", "/home/hahaha/work/haha_super_shell", 1);
    char**result=NULL;
    int count;
    char str[MAX_ORDER];
    while(1){
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stdin = dup(STDIN_FILENO);
        fgets(str,MAX_ORDER,stdin);
        cutting_string(str,&result,&count);
        int pipe_count=0;
        if(strcmp(result[0],"exit")==0){
            break;
        }
        for(int i=0;i<count;i++){
            if(strcmp(result[i],"|")==0)
            pipe_count++;
        }
        int pipefd[pipe_count][2];
        for(int i=0;i<pipe_count;i++){
            if(pipe(pipefd[i])==-1){
                perror("pipe failed");
                exit(1);
            }
        }
        int start=0;
        for(int i=0;i<=pipe_count;i++){
            int end=(i==pipe_count)?count:start;
            while(end<count&&strcmp(result[end],"|")!=0){
                end++;
            }
            result[end]=NULL;
            for(int k=start;k<end;k++){
                if(!strcmp(result[k],"<")){
                    int fd=open(result[k+1],O_RDWR|O_CREAT,0644);
                    dup2(fd,STDIN_FILENO);
                    result[k]=NULL;
                    close(fd);
                    break;
                }
                if(!strcmp(result[k],">")){
                    int fd=open(result[k+1],O_RDWR | O_CREAT,0644);
                    dup2(fd,STDOUT_FILENO);
                    result[k]=NULL;
                    close(fd);
                    break;
                }
                if(!strcmp(result[k],">>")){
                    int fd=open(result[k+1],O_WRONLY | O_CREAT | O_APPEND, 0644);
                    dup2(fd,STDOUT_FILENO);
                    result[k]=NULL;
                    close(fd);
                    break;
                }
            }
            pid_t pid=fork();
            if(pid<0){
                perror("fork failed");
                exit(1);
            }
            else if(pid==0){
                if(i>0){
                    dup2(pipefd[i-1][0],STDIN_FILENO);
                }
                if(i<pipe_count){
                    dup2(pipefd[i][1],STDOUT_FILENO);
                }
                for(int j=0;j<pipe_count;j++){
                    close(pipefd[j][0]);
                    close(pipefd[j][1]);
                }
                if(execvp(result[start],&result[start])==-1){
                    perror("execvp failed");
                    exit(1);
                }
            }
            if(i>0){
                close(pipefd[i-1][0]);
            }
            if(i<pipe_count){
                close(pipefd[i][1]);
            }
            start=end+1;
        }
        for(int i=0;i<=pipe_count;i++){
            wait(NULL);
        }
        free_result(result,count);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);

    }
}