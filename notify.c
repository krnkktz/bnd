#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        int j, s = 0;

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



        /* locale */
        setlocale(LC_ALL, getenv("LANG"));

        /* com parser */
        for (i = 1; i < strlen(com); ++i) {
                if (com[i] == ' ') {
                        s = i;
                        com[i] = '\0';
                }
        }

        if (!s) { /* ignore bad request */
                fprintf(stderr, "bad request!\n");
                wlog("ERROR: bad request!");
                return EXIT_FAILURE;
        }

        bg_col = com;
        text = &com[s+1];

        /* display setup */

        dpy = XOpenDisplay(NULL);
        if (!dpy) {
                fprintf(stderr, "unable to connect to display.\n");
                wlog("ERROR: unable to connect to display.");

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
                if (text_height < fonts[j]->ascent) text_height = fonts[j]->ascent;
        }


        /* window setup */

        width = text_width+MARGIN;
        height = text_height+MARGIN;

        att.override_redirect = True;

        XAllocNamedColor(dpy, DefaultColormap(dpy, screen), bg_col, &col, &dummy);
        att.background_pixel = col.pixel;


        /* window creation */
        win = XCreateWindow(dpy, DefaultRootWindow(dpy), 10,10, width, height,
                        0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel|CWOverrideRedirect, &att);

        XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask);

        XMapWindow(dpy, win);


        /* pen creation */
        pen = XCreateGC(dpy, win, GCForeground, &val);



        /* text writing */
        textx = (width - text_width)/2;
        texty = (height + text_height)/2;
        Xutf8DrawString(dpy, win, font, pen, textx, texty, text, strlength);

        wlog("wrote some text!");


        /* waiting for the end */
        while(1) {
                XNextEvent(dpy, &ev);
                switch(ev.type){
                        case ButtonPress:
                                XCloseDisplay(dpy);
                                return EXIT_SUCCESS;
                }
        }
}
