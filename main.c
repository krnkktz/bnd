#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        /* color */
        Colormap cmap;
        XColor xc, xc2;

        /* font */
        XFontStruct *font;
        char * text = "marin@thinkpad:";
        int text_width;
        int textx, texty;


        /* display setup */

        dpy = XOpenDisplay(NULL);
        if (!dpy) {
                fprintf(stderr, "unable to connect to display\n");\
                        return 7;
        }

        /* pen setup */
        cmap = DefaultColormap(dpy, DefaultScreen(dpy));
        XAllocNamedColor(dpy, cmap, "Red", &xc, &xc2);
        values.foreground = xc.pixel;
        values.line_width = 1;
        values.line_style = LineSolid;

        /* font setup */
        font = XLoadQueryFont(dpy, "fixed");
        values.font = font->fid;
        text_width = XTextWidth(font, text, strlen(text));

        /* window setup */

        width = 400;
        height = 400;

        att.override_redirect = True;


        /* window creation */
        win = XCreateWindow(dpy, DefaultRootWindow(dpy), 10,10, width, height,
                        0, CopyFromParent, CopyFromParent, CopyFromParent, CWOverrideRedirect, &att);

        XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask);

        XMapWindow(dpy, win);


        /* pen creation */
        pen = XCreateGC(dpy, win, GCForeground|GCLineWidth|GCLineStyle|GCFont, &values);

        printf("main loop...\n");

        while (1) {
                XNextEvent(dpy, &ev);
                switch(ev.type){
                        case Expose:
                                printf("Expose!\n");
                                XDrawLine(dpy, win, pen, 0, 0, width, height);
                                XDrawLine(dpy, win, pen, width, 0, 0, height);
                                break;
                        case ConfigureNotify:
                                if (width != ev.xconfigure.width
                                                || height != ev.xconfigure.height) {
                                        width = ev.xconfigure.width;
                                        height = ev.xconfigure.height;
                                        XClearWindow(dpy, ev.xany.window);
                                        printf("Size changed to: %d by %d\n", width, height);
                                }
                                XDrawLine(dpy, win, pen, 0, 0, width/2-text_width/2, height/2);
                                XDrawLine(dpy, win, pen, width, 0, width/2+text_width/2, height/2);
                                XDrawLine(dpy, win, pen, 0, height, width/2-text_width/2, height/2);
                                XDrawLine(dpy, win, pen, width, height, width/2+text_width/2, height/2);
                                textx = (width - text_width)/2;
                                texty = (height + font->ascent)/2;
                                XDrawString(dpy, ev.xany.window, pen, textx, texty, text, strlen(text));
                                break;
                        case ButtonPress:
                                XCloseDisplay(dpy);
                                return 0;
                }
        }
}
