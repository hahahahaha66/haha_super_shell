#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_READ 2048

int main(int argc, char *argv[]) {
    if (argc<1) {
        fprintf(stderr,"Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }
    char buffer[MAX_READ];
    int len;
    while ((len = read(fd, buffer, MAX_READ)) > 0) {
        if (write(STDOUT_FILENO, buffer, len) != len) {
            perror("write failed");
            close(fd);
            exit(1);
        }
    }
    if (len == -1) {
        perror("read failed");
        close(fd);
        exit(1);
    }
    close(fd);
    return 0;
}
