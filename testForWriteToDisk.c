#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    // int fd = open("test", O_WRONLY | O_APPEND);
    // write(fd, "abc", 3);
    // int fd = open("test", O_RDONLY);
    int dfd = open("/dev/disk2s1", O_WRONLY | O_APPEND);
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 1024; ++j)
            for (int k = 0; k < 1024; ++k)
                write(dfd, "a", 1);
    close(dfd);
    return 0;
}