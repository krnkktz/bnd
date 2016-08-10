#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MARGIN=15;

void print_help(void) {
        printf("help coming soon\n");
}

int main(int argc, char ** argv){
        /* argv parser stuff */
        int i;

        /* window information */
        int width, height, screen;

        /* xlib stuff */
        Window win;
        XEvent ev;
        Display *dpy;
        GC pen;
        XSetWindowAttributes att;
        XGCValues val;
        XColor col, dummy;
        char * bg_col = "black";

        /* text */
        XFontStruct *font;
        char * text = "";
        int text_width, text_height;
        int textx, texty;


        /* argv parser */

        for (i = 1; i < argc; ++i) {
                if (strcmp(argv[i], "-h") == 0
                                || strcmp(argv[i], "--help") == 0) {
                        print_help();
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[i], "-b") == 0
                                || strcmp(argv[i], "--background") == 0) {
                        if (++i < argc) {
                                bg_col = argv[i];
                        } else {
                                printf("usage:\n");
                                print_help();
                                return EXIT_FAILURE;
                        }
                } else {
                        text = argv[i];
                }
        }


        fprintf("color: %s\n", bg_col);

        /* display setup */

        dpy = XOpenDisplay(NULL);
        if (!dpy) {
                fprintf(stderr, "unable to connect to display\n");\
                        return EXIT_FAILURE;
        }
        screen = DefaultScreen(dpy);

        /* pen setup */
        val.foreground = XWhitePixel(dpy, screen);

        /* font setup */
        font = XLoadQueryFont(dpy, "fixed");
        val.font = font->fid;
        text_width = XTextWidth(font, text, strlen(text));
        text_height = font->ascent;

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
        pen = XCreateGC(dpy, win, GCForeground|GCFont, &val);


        /* text writing */
        textx = (width - text_width)/2;
        texty = (height + font->ascent)/2;
        XDrawString(dpy, win, pen, textx, texty, text, strlen(text));


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
