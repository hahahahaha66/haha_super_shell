以下是 exec 系列函数的具体介绍和用法：

1. execl
功能：执行指定路径的程序，接受可变数量的参数。
函数原型：
```c
复制代码
int execl(const char *path, const char *arg, ..., (char *) NULL);
```
参数：
path：程序的完整路径。
arg：传递给程序的参数列表，以 NULL 结尾。
用法示例：
```c
复制代码
execl("/bin/ls", "ls", "-l", NULL);
```
等效于： 在终端运行 /bin/ls -l。
2. execv
功能：执行指定路径的程序，参数以数组形式传递。
函数原型：
```c
复制代码
int execv(const char *path, char *const argv[]);
```
参数：
path：程序的完整路径。
argv：参数数组，argv[0] 通常是程序名，最后一个元素必须是 NULL。
用法示例：
```c
复制代码
char *args[] = {"ls", "-l", NULL};
execv("/bin/ls", args);
```
3. execle
功能：与 execl 类似，但允许指定环境变量。
函数原型：
```c
复制代码
int execle(const char *path, const char *arg, ..., (char *) NULL, char *const envp[]);
```
参数：
path：程序的完整路径。
arg：传递给程序的参数列表，以 NULL 结尾。
envp：环境变量数组。
用法示例：
```c
复制代码
char *env[] = {"USER=example", "PATH=/bin", NULL};
execle("/bin/ls", "ls", "-l", NULL, env);
```
4. execve
功能：exec 系列中最底层的实现函数，直接接受路径、参数数组和环境变量数组。
函数原型：
```c
复制代码
int execve(const char *pathname, char *const argv[], char *const envp[]);
```
参数：
pathname：程序的完整路径。
argv：参数数组。
envp：环境变量数组。
用法示例：
```c
复制代码
char *args[] = {"ls", "-l", NULL};
char *env[] = {"USER=example", "PATH=/bin", NULL};
execve("/bin/ls", args, env);
```
5. execlp
功能：与 execl 类似，但会在 PATH 环境变量中搜索程序。
函数原型：
```c
复制代码
int execlp(const char *file, const char *arg, ..., (char *) NULL);
```
参数：
file：程序名或相对路径，会在 PATH 中查找。
arg：传递给程序的参数列表，以 NULL 结尾。
用法示例：
```c
复制代码
execlp("ls", "ls", "-l", NULL);
```
等效于： 在终端运行 ls -l。
6. execvp
功能：与 execv 类似，但会在 PATH 环境变量中搜索程序。
函数原型：
```c
复制代码
int execvp(const char *file, char *const argv[]);
```
参数：
file：程序名或相对路径，会在 PATH 中查找。
argv：参数数组。
用法示例：
```c
复制代码
char *args[] = {"ls", "-l", NULL};
execvp("ls", args);
```
总结对比
函数	参数传递方式	环境变量	搜索 PATH	使用场景
execl	可变参数	系统默认	否	简单路径和固定参数。
execv	数组	系统默认	否	参数动态构造时更方便。
execle	可变参数	手动指定	否	需要自定义环境变量。
execve	数组	手动指定	否	最底层、灵活性最高。
execlp	可变参数	系统默认	是	简单路径参数，自动查找路径。
execvp	数组	系统默认	是	动态参数构造并需自动查找路径。
