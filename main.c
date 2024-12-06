#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>
#define MAX_ORDER 100
void cutting_string(char* str,char*** result,int *count){
    int size=10;
    int i=0;
    char*token;
    *result=malloc(size*sizeof(char*));
    token=strtok(str," ");
    while(token){
        if(i==size){
            size*=2;
            *result=realloc(*result,size*sizeof(char*));
        }
        (*result)[i++]=strdup(token);
        token=strtok(NULL," ");
    }
    *result=realloc(*result,i*sizeof(char*));
    *count=i;
}
void free_result(char**result,int count){
    for(int i=0;i<count;i++){
        free(result[i]);
    }
    free(result);
}
int main(){
    int count=0;
    char str[MAX_ORDER];
    char** result;
    while(1){
        fgets(str,MAX_ORDER,stdin);
        while((opt=getopt_long()))
    }
    return 0;
}