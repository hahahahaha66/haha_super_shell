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

pid_t child_pid;

void sigint_handler(int signum) {
    if (child_pid > 0) {
        kill(child_pid, SIGCONT); // 恢复后台进程
        printf("已将后台进程 %d 恢复到前台\n", child_pid);
        // 将终端控制权转给子进程
        tcsetpgrp(STDIN_FILENO, child_pid);
    }
}

int main() {
    signal(SIGINT, sigint_handler); // 处理 Ctrl+C 信号

    char *result[2] = {"/bin/sleep", "60"}; // 这里假设你希望运行 sleep 60
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (child_pid > 0) {
        // 父进程
        printf("后台进程 PID: %d\n", child_pid);
        while (1) {
            pause(); // 等待信号
        }
    }
    
    // 子进程部分
    int dp = open("/dev/null", O_RDWR);
    dup2(dp, STDIN_FILENO);
    dup2(dp, STDOUT_FILENO);
    dup2(dp, STDERR_FILENO);
    close(dp);
    
    // 执行实际的命令
    execvp(result[0], result);
    perror("exec failed");
    exit(1); // execvp 失败时退出
}
