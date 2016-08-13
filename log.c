#include <stdio.h>
#include <time.h>

#include "log.h"

void wlog(char* message) {
        time_t t = time(NULL);
        struct tm* lt = localtime(&t);
        FILE* logfile = fopen("/home/marin/.bnd/bnd.log", "a");

        if (!logfile) {
                fprintf(stderr, "ERROR: unable to open log file.\n");
                return;
        }

        fprintf(logfile, "%d/%d/%d %d:%d:%d: ", lt->tm_year + 1900,
                        lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, lt->tm_sec);
        fprintf(logfile, message);
        fprintf(logfile, "\n");
        fclose(logfile);
}

