#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>

void notify(char* arg) {
        fprintf(stdout, "%s\n", arg);
}

int main(int argc, char* argv[]) {
        /* forking */
        pid_t process_id = 0;
        pid_t sid = 0;

        /* client communication */
        struct sockaddr_un addr;
        int sfd, cfd, rfd;
        char buffer[1024];


        /* forking daemon */
        process_id = fork();

        if (process_id < 0) {
                fprintf(stderr, "failed to create child process.\n");
                return EXIT_FAILURE;
        }

        if (process_id > 0) {
                fprintf(stdout, "created child process with pid %d.\n",
                                process_id);
                return EXIT_SUCCESS;
        }

        sid = setsid();

        if (sid < 0) {
                fprintf(stderr, "failed to set sid.\n");
                return EXIT_FAILURE;
        }

        chdir("/");

        /*        close(STDIN_FILENO);
                  close(STDOUT_FILENO);
                  close(STDERR_FILENO); */

        /* client communication */
        sfd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (sfd < 0) {
                fprintf(stderr, "failed to create socket.\n");
                return EXIT_FAILURE;
        }

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, "/home/marin/.bnd/SOCKET",
                        sizeof(addr.sun_path) - 1);

        remove("/home/marin/.bnd/SOCKET");

        if (bind(sfd, (struct sockaddr *) &addr,
                                sizeof(struct sockaddr_un)) < 0) {
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
                rfd = read(cfd, buffer, 1023);
                if (rfd < 0) {
                        fprintf(stderr, "failed to read from buffer.\n");
                        return EXIT_FAILURE;
                }
                process_id = fork();
                if (process_id < 0) {
                        fprintf(stderr, "failed to fork for notification.\n");
                }

                if (process_id == 0) {
                        notify(buffer);
                        return EXIT_SUCCESS;
                }

                fprintf(stdout, "created notification process with pid %d\n",
                                process_id);
                close(cfd);
        }
        return EXIT_SUCCESS;
}
