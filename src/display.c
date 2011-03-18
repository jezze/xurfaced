#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <display.h>
#include <halo.h>

void halo_display_init(struct halo *halo)
{

    if (!(halo->display = XOpenDisplay(0)))
    {

        fputs("halo: can not open display", stderr);
        exit(EXIT_FAILURE);

    }

    halo->screen = XDefaultScreen(halo->display);
    halo->screenWidth = XDisplayWidth(halo->display, halo->screen);
    halo->screenHeight = XDisplayHeight(halo->display, halo->screen);

}

void halo_display_destroy(struct halo *halo)
{

    XCloseDisplay(halo->display);

}

