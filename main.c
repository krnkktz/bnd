#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MARGIN=15;

int main(int argc, char ** argv){
        /* basic window information */
        int width, height;

        /* xlib stuff */
        Window win;
        XEvent ev;
        Display *dpy;
        GC pen;
        XSetWindowAttributes att;
        XGCValues values;
        int screen;

        /* font */
        XFontStruct *font;
        char * text = "marin@thinkpad:";
        int text_width, text_height;
        int textx, texty;


        /* display setup */

        dpy = XOpenDisplay(NULL);
        if (!dpy) {
                fprintf(stderr, "unable to connect to display\n");\
                        return 7;
        }
        screen = DefaultScreen(dpy);

        /* pen setup */
        values.foreground = XWhitePixel(dpy, screen);

        /* font setup */
        font = XLoadQueryFont(dpy, "fixed");
        values.font = font->fid;
        text_width = XTextWidth(font, text, strlen(text));
        text_height = font->ascent;

        /* window setup */

        width = text_width+MARGIN;
        height = text_height+MARGIN;

        att.override_redirect = True;
        att.background_pixel = XBlackPixel(dpy,screen);


        /* window creation */
        win = XCreateWindow(dpy, DefaultRootWindow(dpy), 10,10, width, height,
                        0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel|CWOverrideRedirect, &att);

        XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask);

        XMapWindow(dpy, win);


        /* pen creation */
        pen = XCreateGC(dpy, win, GCForeground|GCFont, &values);

        printf("main loop...\n");

        textx = (width - text_width)/2;
        texty = (height + font->ascent)/2;
        XDrawString(dpy, win, pen, textx, texty, text, strlen(text));

        while(1) {
                XNextEvent(dpy, &ev);
                switch(ev.type){
                        case ButtonPress:
                                XCloseDisplay(dpy);
                                return 0;
                }
        }
}
