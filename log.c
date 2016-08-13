#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "log.h"

void w_log(const char* message, ...) {
        va_list argp;
        time_t t = time(NULL);
        struct tm* lt = localtime(&t);
        char ts[20];

        FILE* logfile = fopen("/home/marin/.bnd/log", "a");

        if (!logfile) {
                fprintf(stderr, "ERROR: unable to open log file.\n");
                return;
        }

        strftime(ts, 20, "%Y/%m/%d %H:%M:%S", lt);
        fprintf(logfile, "%s: ", ts);
        va_start(argp, message);
        vfprintf(logfile, message, argp);
        va_end(argp);
        fprintf(logfile, "\n");
        fclose(logfile);
}

