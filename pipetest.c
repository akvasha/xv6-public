//
// Created by akvasha on 14.06.19.
//

#include "types.h"
#include "stat.h"
#include "user.h"

#define TO_WRITE 256
#define FORKS 30

int main() {
    int fd[2];
    pipe(fd);
    printf(1, "test started!\n");
    for (int i = 0; i < FORKS; ++i) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "test failed! fork was not created!\n");
            exit();
        }
        if (!pid) {
            close(fd[0]);
            char buf[TO_WRITE];
            for (int j = 0; j < TO_WRITE; ++j) {
                buf[j] = 'a' + i;
            }
            int x = write(fd[1], buf, TO_WRITE);
            if (x == -1) {
                printf(1, "test failed! write didn't succeed\n");
            }
            exit();
        }
    }
    int pid_main = fork();
    if (pid_main < 0) {
        printf(1, "test failed! main fork was not created!\n");
        exit();
    }
    if (!pid_main) {
        for (int i = 0; i < FORKS; ++i) {
            char buf[TO_WRITE + 1];
            int done = 0;
            while (done != TO_WRITE) {
                int pref = read(fd[0], buf + done, TO_WRITE - done);
                if (pref <= 0) {
                    printf(1, "test failed! read didn't succeed!\n");
                    exit();
                }
                done += pref;
            }
            buf[TO_WRITE] = '\0';
            for (int i = 1; i < TO_WRITE; ++i) {
                if (buf[0] != buf[i]) {
                    printf(1, "test failed! pipe is not working atomically!\n");
                    exit();
                }
            }
        }
        exit();
    }
    close(fd[0]);
    close(fd[1]);
    for (int i = 0; i < FORKS + 1; ++i) {
        wait();
    }
    printf(1, "test finished!\n");
    exit();
}