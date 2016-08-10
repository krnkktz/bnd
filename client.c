#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

void print_help(_IO_FILE*);


void print_help(_IO_FILE* out) {
        fprintf(out, "help coming soon\n");
}

int main(int argc, char** argv) {
        /* args */
        int i;
        char* bg_col = "black";
        char* text;
        char* sarg;

        /* server connection */
        struct sockaddr_un addr;
        int sfd;

        /* parsing args */
        for (i = 1; i < argc; ++i) {
                if (strcmp(argv[i], "-h") == 0
                                || strcmp(argv[i], "--help") == 0) {
                        print_help(stdout);
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[i], "-b") == 0
                                || strcmp(argv[i], "--background") == 0) {
                        if (++i < argc) {
                                bg_col = argv[i];
                        } else {
                                fprintf(stderr, "usage:\n");
                                print_help(stderr);
                                return EXIT_FAILURE;
                        }
                } else {
                        text = argv[i];
                }
        }


        /* connecting to server */
        sfd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (sfd < 0) {
                fprintf(stderr, "failed to create socket.\n");
                return EXIT_FAILURE;
        }

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, "/home/marin/.bnd/SOCKET", sizeof(addr.sun_path) - 1);

        if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
                fprintf(stderr, "failed to connect.\n");
                return EXIT_FAILURE;
        }

        /* sending the command */
        sarg = malloc(strlen(bg_col) + strlen(text) + 2);

        if (!sarg) {
                fprintf(stderr, "failed to allocate memory.\n");
                return EXIT_FAILURE;
        }

        strcpy(sarg, bg_col);
        strcat(sarg, " ");
        strcat(sarg, text);

        write(sfd, sarg, strlen(sarg) + 1);

        close(sfd);

        return EXIT_SUCCESS;
}

