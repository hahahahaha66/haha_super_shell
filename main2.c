#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<signal.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/time.h>
#include<time.h>
#include<sys/ioctl.h>
#include<dirent.h>
#include<termios.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAX_ORDER 100
#define MAX_PATH 1024

int ha=0;
int over=0;
char** commands;
pid_t child_pid = -1;

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
                for (int j = 0; j < i; j++) {
                    free((*result)[j]);
                }
                free(*result);
                exit(1);
            }
            *result=temp;
        }
        (*result)[i]=strdup(token);
        if((*result)[i]==NULL) { 
            perror("strdup failed");
            for (int j = 0; j < i; j++) {
                free((*result)[j]);
            }
            free(*result);
            exit(1);
        }
        i++;
        token=strtok(NULL," ");
    }
    *result=realloc(*result,(i+1)*sizeof(char*));
    if (*result == NULL && i > 0) {  
        perror("final realloc failed");
        for (int j = 0; j < i; j++) {
            free((*result)[j]);
        }
        free(*result);
        exit(1);
    }
    (*result)[i]=NULL;
    *count=i;
}

void free_result(char**result,int count){
    for(int i=0;i<count;i++){
        free(result[i]);
        result[i]=NULL;
    }
    free(result);
    result=NULL;
}

int get_commands(char*path,char*commands[]){
    int count=0;
    DIR*dir=opendir(path);
    struct dirent* dirent;
    while((dirent=readdir(dir))!=NULL){
        if(!strcmp(dirent->d_name,".")||(!strcmp(dirent->d_name,".."))){
            continue;
        }
        commands[count]=strdup(dirent->d_name);
        if (commands[count] == NULL) {
            perror("strdup failed");
            closedir(dir);
            return 0;
        }
        count++;
    }
    commands[count]=NULL;
    closedir(dir);
    return count;
}

char* command_generator(const char*text,int state){
    static int index,len;
    char*cmd;
    if(!state){
        index=0;
        len=strlen(text);
    }
    while((cmd=commands[index++])){
        if(strncmp(cmd,text,len)==0){
            return strdup(cmd);
        }
    }
    return NULL;
}

char**command_completion(const char*text,int start,int len){
    if(start!=0){
        return NULL;
    }
    return rl_completion_matches(text, command_generator);
}

int cd(char*result[],int count){
    char str[MAX_PATH]={0};
    //printf("%d ",count);
    if(count==1){
        char*home=getenv("HOME");
        if(chdir(home)!=0){
            perror("chdir failed");
            exit(1);
        }
    }
    if(count>2){
        printf("参数错误\n");
        exit(1);
    }
    if(count==2){
        if(!strcmp(result[1],"-")){
            char*old_path=getenv("OLDPWD");
            if(chdir(old_path)!=0){
                perror("chdir failed");
                exit(1);
            }
        }
        else{
            if(chdir(result[1])!=0){
                perror("chdir failed");
                exit(1);
            }
        }
    }
    if(!getcwd(str,MAX_PATH)){
        perror("getcwd failed");
        exit(1);
    }
    printf("已切换到目录:%s\n",str);
    return 0;
}

void haha(int sig){
    ha=1;
    printf("捕捉到编号为%d的信号,已忽略,按回车恢复\n",sig);
}

// void over_time(int sig){
//     over=1;
//     if(child_pid!=-1){
//         kill(child_pid,SIGKILL);
//     }
//     printf("已超时,正在退出...\n");
// }

int main(){
    //setenv("PATH", "/home/hahaha/work/haha_super_shell", 1);
    commands=(char**)malloc(sizeof(char*)*10000);
    int num_commands=get_commands("/usr/bin",commands);
    if(num_commands==0){
        return 1;
    }
    for(int i=0;i<num_commands;i++){
        printf("%s ",commands[i]);
    }
    rl_attempted_completion_function = command_completion;
    time_t tim;
    time(&tim);
    char c[1024]={0};
    getcwd(c,MAX_PATH);
    setenv("PWD",c,1);
    setenv("OLDPWD",c,1);
    struct sigaction signa;
    signa.sa_handler=haha;
    signa.sa_flags=0;
    sigemptyset(&signa.sa_mask);
    char**result=NULL;
    int count;
    int a=0;
    char *str;
    while(1){
        char*username=getenv("USER");
        char*hostname=getenv("LOGNAME");
        char*path=getenv("PWD");
        if(!strcmp(path,"/home/hahaha")){
            path="~";
        }
        int uid=geteuid();
        struct winsize size;
        if(isatty(STDIN_FILENO)!=0){
            ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
        }
        time_t haha;
        struct tm* hahaha;
        char buf[1024];
        char tim[1024];
        time(&haha);
        hahaha=localtime(&haha);
        strftime(buf,100,"%H:%M:%S",hahaha);
        int j;
        for(j=0;j<size.ws_col-10;j++){
            tim[j]=' ';
        }
        tim[j]='\0';
        char fu;
        if(uid==0){
            fu='#';
        }
        else{
            fu='$';
        }
        sprintf(tim,"%s\033[30;47m %s \033[0m\r",tim,buf);
        //printf("%s\n",tim);
        char*begin=(char*)malloc(1024);
        sprintf(begin,"%s[%s@%s %s]%c ",tim,username,hostname,path,fu);
        if(a){
            sleep(100);
        }
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stdin = dup(STDIN_FILENO);
        sigaction(2,&signa,NULL);
        sigaction(3,&signa,NULL);
        str=readline(begin);
        
        free(begin);
        begin=NULL;
        if(str==NULL){
            perror("readline failed");
            return 1;
        }
        if (*str) {
            add_history(str);
        }
        //fgets(str,MAX_ORDER,stdin);
        
        if(ha){
            ha=0;
            continue;
        }
        cutting_string(str,&result,&count);
        if(count==0){
            continue;
        }
        
        free(str);
        str=NULL;
        
        if(strcmp(result[0],"exit")==0){
            free(str);
            free(begin);
            str=NULL;
            begin=NULL;
            clear_history();
            free_result(result,count);
            return 0;
        }

        if(count>=2&&!strcmp(result[count-1],"&")){
            a=1;
            pid_t q=fork();
            if(q<0){
                perror("fork failed");
                free_result(result,count);
                exit(1);
            }
            if(q>0){
                free_result(result,count);
                exit(0);
            }
            if (setsid() < 0) {
                perror("setsid failed");
                free_result(result,count);
                exit(1);
            }
            q=fork();
            if (q<0) {
                perror("fork failed");
                free_result(result,count);
                exit(1);
            }
            if (q>0) {
                free_result(result,count);
                exit(0);
            }
            printf("pid:%d\n",getpid());
            int dp=open("/dev/null",O_RDWR);
            dup2(dp,STDIN_FILENO);
            dup2(dp,STDOUT_FILENO);
            dup2(dp,STDERR_FILENO);
            close(dp);
        }

        if(count>=2&&!strcmp(result[0],"cd")){
            cd(result,count);
            char s1[MAX_PATH]={0};
            char s2[MAX_PATH]={0};
            char*old_pwd=getenv("PWD");
            snprintf(s2, sizeof(s2), "%s", old_pwd);
            setenv("OLDPWD",s2,1);
            getcwd(s1,MAX_PATH);
            setenv("PWD",s1,1);
            printf("当前目录: %s\n", s1);
            printf("上一目录: %s\n", s2);
            free_result(result,count);
            continue;
        }

        int pipe_count=0;
        for(int i=0;i<count;i++){
            if(strcmp(result[i],"|")==0)
            pipe_count++;
        }
        int pipefd[pipe_count][2];
        for(int i=0;i<pipe_count;i++){
            if(pipe(pipefd[i])==-1){
                perror("pipe failed");
                free_result(result,count);
                exit(1);
            }
        }
        int start=0;

        for(int i=0;i<=pipe_count;i++){
            int pipe_error[2];
            if(pipe(pipe_error)==-1){
                perror("pipe failed");
                free_result(result,count);
                exit(1);
            }
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
                free_result(result,count);
                exit(1);
            }
            else if(pid==0){
                close(pipe_error[0]);
                dup2(pipe_error[1],STDERR_FILENO);
                close(pipe_error[1]);
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
                    free_result(result,count);
                    exit(127);
                }
            }
            if(i>0){
                close(pipefd[i-1][0]);
            }
            if(i<pipe_count){
                close(pipefd[i][1]);
            }
            start=end+1;

            child_pid=pid;
            close(pipe_error[1]);
            //alarm(3);
            int status;
            waitpid(pid,&status,0);
            //signal(SIGALRM,over_time);
            if(over){
                free_result(result,count);
                return -1;
            }
            // if(WIFEXITED(status)){
            //     int exit_status=WEXITSTATUS(status);
            //     if(exit_status==127){
            //         printf("Commend Not Found\n");
            //         continue;
            //         free_result(result, count);
            //         exit(EXIT_FAILURE);
            //     }
            //     else if(exit_status==1&&strcmp(result[start],"grep")){
            //         continue;
            //     }
            //     else if(exit_status!=0){
            //         printf("Commend Failed\n");
            //         continue;
            //         exit(exit_status);
            //     }
            // }ls
            alarm(0);
        }
        free_result(result,count);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
    }
    return 0;
}