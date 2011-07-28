#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <display.h>
#include <halo.h>

struct halo_display_backend *halo_display_create()
{

    XInitThreads();

    struct halo_display_backend *backend = malloc(sizeof (struct halo_display_backend));
    backend->display = XOpenDisplay(0);

    if (!backend->display)
    {

        fprintf(stderr, "halo: can not open display\n");
        exit(EXIT_FAILURE);

    }

    backend->descriptor = XConnectionNumber(backend->display);
    backend->screen = XDefaultScreen(backend->display);
    backend->screenWidth = XDisplayWidth(backend->display, backend->screen);
    backend->screenHeight = XDisplayHeight(backend->display, backend->screen);
    backend->visual = XDefaultVisual(backend->display, 0);

    return backend;

}

void halo_display_destroy(struct halo_display_backend *backend)
{

    XCloseDisplay(backend->display);
    free(backend);

}

