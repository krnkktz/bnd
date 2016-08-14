#include <sys/time.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

#include "notify.h"
#include "log.h"


int MARGIN=15;


XFontSet getFont(Display* dpy) {
        XFontSet font = NULL;
        int nmissing;
        char** missing;
        char* def_string;

        font = XCreateFontSet(dpy, "fixed", &missing, &nmissing, &def_string);
        XFreeStringList(missing);
        return font;
}

int notify(char* com) {
        /* looping stuff */
        unsigned i;
        int j, s1 = 0, s2 = 0;

        /* timing stuff */
        int x11_fd;
        struct timeval rtv;
        struct timeval* tv = &rtv;
        int timeout;
        char* stimeout;
        fd_set in_fds;

        /* window information */
        int width, height, screen;

        /* xlib stuff */
        Window win;
        XEvent ev;
        Display* dpy;
        GC pen;
        XSetWindowAttributes att;
        XGCValues val;
        XColor col, dummy;
        char* bg_col;

        /* text */
        int strlength;
        int text_width, text_height;
        int textx, texty;
        char* text;

        /* font */
        int nfonts;
        char** font_names;
        XFontSet font;
        XFontStruct** fonts;


        w_log("REQUEST: %s", com);

        /* locale */
        setlocale(LC_ALL, getenv("LANG"));

        /* com parser */
        for (i = 0; i < strlen(com); ++i) {
                if (com[i] == ' ') {
                        s1 = i;
                        com[i] = '\0';
                }
        }

        bg_col = com;

        w_log("bg_col: \"%s\"", bg_col);

        stimeout = &com[s1+1];

        w_log("stimeout before loop: %s", stimeout);

        for (i = 0; i < strlen(stimeout); ++i) {
                if (stimeout[i] == ' ') {
                        s2 = i;
                        stimeout[i] = '\0';
                }
        }

        timeout = strtol(stimeout, NULL, 10);

        text = &stimeout[s2+1];

        w_log("s1: %d, s2: %d, timeout: %d, stimeout: %s, text: %s",
                        s1, s2, timeout, stimeout, text);

        if (!s1 || !s2) { /* ignore bad request */
                w_log("ERROR: bad request!");
                return EXIT_FAILURE;
        }


        /* display setup */

        dpy = XOpenDisplay(NULL);
        if (!dpy) {
                w_log("ERROR: unable to connect to display.");

                return EXIT_FAILURE;
        }
        screen = DefaultScreen(dpy);

        /* pen setup */
        val.foreground = XWhitePixel(dpy, screen);

        /* text setup */
        strlength = strlen(text);
        font = getFont(dpy);
        text_width = Xutf8TextEscapement(font, text, strlength);

        text_height = 0;
        nfonts = XFontsOfFontSet(font, &fonts, &font_names);
        for(j = 0; j < nfonts; ++j) {
                if (text_height < fonts[j]->ascent)
                        text_height = fonts[j]->ascent;
        }


        /* window setup */

        width = text_width+MARGIN;
        height = text_height+MARGIN;

        att.override_redirect = True;

        XAllocNamedColor(dpy, DefaultColormap(dpy, screen),
                        bg_col, &col, &dummy);
        att.background_pixel = col.pixel;


        /* window creation */
        win = XCreateWindow(dpy, DefaultRootWindow(dpy), 10,10, width, height,
                        0, CopyFromParent, InputOutput, CopyFromParent,
                        CWBackPixel|CWOverrideRedirect, &att);

        XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask);

        XMapWindow(dpy, win);


        /* pen creation */
        pen = XCreateGC(dpy, win, GCForeground, &val);



        /* text writing */
        textx = (width - text_width)/2;
        texty = (height + text_height)/2;
        Xutf8DrawString(dpy, win, font, pen, textx, texty, text, strlength);

        w_log("NOTIFICATION: %s", text);


        /* timing */
        x11_fd = ConnectionNumber(dpy);

        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);

        if (timeout > 0) {
                tv->tv_usec = 0;
                tv->tv_sec = timeout;
        } else {
                tv = NULL;
        }

        XFlush(dpy);

        while(1) {
                w_log("loop");
                if (!select(x11_fd + 1, &in_fds, NULL, NULL, tv)) {
                        XCloseDisplay(dpy);
                        w_log("timeout!");
                        return EXIT_SUCCESS;
                }

                while(XPending(dpy)) {
                        XNextEvent(dpy, &ev);
                        w_log("Event! ev.type = %d", ev.type);
                        switch(ev.type){
                                case ButtonPress:
                                        XCloseDisplay(dpy);
                                return EXIT_SUCCESS;
                        }
                }
        }
}
