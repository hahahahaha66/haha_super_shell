#include<stdlib.h>
#include<string.h>
#include<stdio.h>
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
int main() {
    char str[] = "  This is an improved example of cutting string";
    char **result;
    int count;

    cutting_string(str, &result, &count);

    for (int i = 0; i < count; i++) {
        printf("%s\n", result[i]);
    }

    free_result(result, count);
    return 0;
}