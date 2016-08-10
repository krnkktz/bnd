#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>

int server(void);


int server(void) {
        struct sockaddr_un addr;
        int sfd, cfd, rfd;
        char buffer[256];

        sfd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (sfd < 0) {
                fprintf(stderr, "failed to create socket.\n");
                return EXIT_FAILURE;
        }

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, "/home/marin/.bnd/SOCKET", sizeof(addr.sun_path) - 1);

        if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
                fprintf(stderr, "failed to bind socket (errno %d).\n", errno);
                return EXIT_FAILURE;
        }

        if (listen(sfd, 5) < 0) {
                fprintf(stderr, "failed to listen to socket.\n");
                return EXIT_FAILURE;
        }

        while (1) {
                cfd = accept(sfd, NULL, NULL);
                if (cfd < 0) {
                        fprintf(stderr, "failed to accept connection.\n");
                        return EXIT_FAILURE;
                }
                rfd = read(cfd, buffer, 255);
                if (rfd < 0) {
                        fprintf(stderr, "failed to read from buffer.\n");
                        return EXIT_FAILURE;
                }
                fprintf(stdout, "read from buffer: %s\n", buffer);
                close(cfd);
        }
        return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
        /*
        pid_t process_id = 0;
        pid_t sid = 0;

        process_id = fork();

        if (process_id < 0) {
                fprintf(stderr, "failed to create child process.\n");
                return EXIT_FAILURE;
        }

        if (process_id > 0) {
                fprintf(stdout, "created child process with pid %d.\n", process_id);
                return EXIT_SUCCESS;
        }

        sid = setsid();

        if (sid < 0) {
                fprintf(stderr, "failed to set sid.\n");
                return EXIT_FAILURE;
        }

        chdir("/");

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        */

        return server();
}
