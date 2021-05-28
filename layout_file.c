#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

int str_upr(char *cstr)
{
    char *str=cstr;
    for (;*str;str++) {
        if (isalpha(*str))
            *str += 'A' - 'a';
    }
    return 0;
}


int main(int argc, char **argv)
{
    XEvent e;
    Display *d;
    FILE *fptr;

    if (!(d = XOpenDisplay(NULL))) {
            fprintf(stderr, "cannot open display\n");
            return 1;
    }

    int xkbEventType;
    XKeysymToKeycode(d, XK_F1);
    XkbQueryExtension(d, 0, &xkbEventType, 0, 0, 0);
    XkbSelectEvents(d, XkbUseCoreKbd, XkbAllEventsMask, XkbAllEventsMask);
    XSync(d, False);

    XkbRF_VarDefsRec vd;
    XkbRF_GetNamesProp(d, NULL, &vd);

    char *layouts_str = (char*) calloc(strlen(vd.layout)+1, sizeof(char));
    strncpy(layouts_str, vd.layout, strlen(vd.layout));
    str_upr(layouts_str);

    int delimiters;
    for (delimiters=0; vd.layout[delimiters]; vd.layout[delimiters]==',' ? delimiters++ : *vd.layout++);

    char **langs = (char**) calloc(delimiters+1, sizeof(char*));
    for (int i = 0; i < delimiters+1; i++ )
        langs[i] = (char*) calloc(3, sizeof(char));

    for (int i = 0; i < delimiters+1; i++) {
        strncpy(langs[i], layouts_str, 2);
        layouts_str = strstr(layouts_str, ",");
        if (!layouts_str)
            break;
        layouts_str++;
    }
    free(layouts_str);

    fptr = fopen("/tmp/current_layout","w");
    XkbEvent* xkbEvent = (XkbEvent*) &e;
    int lang = xkbEvent->state.group;
    fprintf(fptr, "%s\n", langs[lang]);
    fflush(fptr);

    int prevlang = 0;
    while (1) {
        XNextEvent(d, &e);
        if (e.type != xkbEventType) continue;
        if (xkbEvent->any.xkb_type != XkbStateNotify) continue;
        lang = xkbEvent->state.group;
        if (lang == prevlang) continue;
        prevlang = lang;
        fseek(fptr, 0, SEEK_SET);
        fprintf(fptr, "%.2s\n", langs[lang]);
        fflush(fptr);
    }

    return(0);
}


//gcc layout_file.c -lX11 -lxkbfile -o layout_file
