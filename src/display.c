#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <display.h>
#include <halo.h>

void halo_display_init(struct halo *halo)
{

    XInitThreads();

    halo->backend.display = XOpenDisplay(0);

    if (!halo->backend.display)
    {

        fprintf(stderr, "halo: can not open display\n");
        exit(EXIT_FAILURE);

    }

    halo->backend.descriptor = XConnectionNumber(halo->backend.display);
    halo->backend.screen = XDefaultScreen(halo->backend.display);
    halo->backend.screenWidth = XDisplayWidth(halo->backend.display, halo->backend.screen);
    halo->backend.screenHeight = XDisplayHeight(halo->backend.display, halo->backend.screen);

}

void halo_display_destroy(struct halo *halo)
{

    XCloseDisplay(halo->backend.display);

}

