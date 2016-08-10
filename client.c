#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
        struct sockaddr_un addr;
        int sfd;

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

        write(sfd, "mensch!", strlen("mensch!"));

        close(sfd);

        return EXIT_SUCCESS;
}

