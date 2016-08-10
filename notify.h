#ifndef NOTIFY_H
#define NOTIFY_H

#include <X11/Xlib.h>

XFontSet getFont(Display*);

void print_help(_IO_FILE*);

int notify(char*);

#endif

