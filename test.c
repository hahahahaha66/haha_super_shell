#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // 参数数组
    char *argv[] = {"env", NULL}; // 执行 env 命令查看环境变量

    // 自定义环境变量数组
    char *envp[] = {
        "MY_VAR=HelloWorld",
        "PATH=/usr/bin:/bin",
        "TERM=xterm-256color",
        NULL // 必须以 NULL 结束
    };

    printf("Before execve\n");

    // 使用 execve 显式设置环境变量
    if (execve("/usr/bin/env", argv, envp) == -1) {
        perror("execve failed");
        return 1;
    }

    printf("This will not be printed if execve succeeds\n");
    return 0;
}
