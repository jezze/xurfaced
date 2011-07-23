#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <display.h>
#include <halo.h>

void halo_display_init(struct halo *halo)
{

    XInitThreads();

    halo->display = XOpenDisplay(0);

    if (!halo->display)
    {

        fprintf(stderr, "halo: can not open display\n");
        exit(EXIT_FAILURE);

    }

    halo->connection = XConnectionNumber(halo->display);
    halo->screen = XDefaultScreen(halo->display);
    halo->screenWidth = XDisplayWidth(halo->display, halo->screen);
    halo->screenHeight = XDisplayHeight(halo->display, halo->screen);

}

void halo_display_destroy(struct halo *halo)
{

    XCloseDisplay(halo->display);

}

