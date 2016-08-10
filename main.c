#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int MARGIN=15;

/*int utf8toXChar2b(XChar2b*, int, const char*, int);*/

XFontSet getFont(Display*);

void print_help(_IO_FILE*);

/*
int utf8toXChar2b(XChar2b* output_r, int outsize, const char* input, int inlen) {
        int j, k;
        for (j = 0; j < inlen && k < outsize; ++j) {
                unsigned char c = input[j];
                if (c < 128) {
                        output_r[k].byte1 = 0;
                        output_r[k].byte2 = c;
                        ++k;
                } else if (c < 0xC0) {
                        continue;
                } else switch (c&0xF0) {
                        case 0xC0: case 0xD0:
                                if (inlen < j + 1) {
                                        return k;
                                }
                                output_r[k].byte1 = (c&0x1c) >> 2;
                                ++j;
                                output_r[k].byte2 = ((c&0x3) << 6) + (input[k]&0x3F);
                                ++k;
                                break;
                        case 0xE0:
                                if (inlen < j+2) {
                                        return k;
                                }
                                ++j;
                                output_r[k].byte1 = ((c&0xF) << 4) + ((input[j]&0x3C) >> 2);
                                c = input[j];
                                ++j;
                                output_r[k].byte2 = ((c&0x3) << 6) + (input[j]&0x3F);
                                ++k;
                                break;
                        case 0xFF:
                                continue;
                }
        }
        return k;
}*/

XFontSet getFont(Display* dpy) {
        XFontSet font = NULL;
        int nmissing;
        char** missing;
        char* def_string;

        font = XCreateFontSet(dpy, "fixed", &missing, &nmissing, &def_string);
        XFreeStringList(missing);
        return font;
}


void print_help(_IO_FILE* out) {
        fprintf(out, "help coming soon\n");
}


int main(int argc, char** argv){
        /* looping stuff */
        int i, j;

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
        char* bg_col = "black";

        /* text */
        int strlength;
        int text_width, text_height;
        int textx, texty;
        char* text = "";

        /* font */
        int nfonts;
        char** font_names;
        XFontSet font;
        XFontStruct** fonts;



        /* locale */
        setlocale(LC_ALL, getenv("LANG"));

        /* argv parser */

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


        /* display setup */

        dpy = XOpenDisplay(NULL);
        if (!dpy) {
                fprintf(stderr, "unable to connect to display\n");\
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
