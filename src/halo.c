#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <client.h>
#include <display.h>
#include <event.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>
#include <window.h>

struct halo halo;

static void halo_init()
{

    halo_display_init(&halo);
    halo_window_init(&halo);
    halo_surface_init(&halo);
    halo.clients = halo_client_init();
    halo.menu = halo_menu_init();

}

static void halo_destroy()
{

    halo_client_destroy(halo.clients);
    halo_surface_destroy(&halo);
    halo_display_destroy(&halo);

}

static void halo_run()
{

    halo.running = 1;

    while (halo.running)
    {

        halo_event_handler(&halo);
        halo_surface_blit(&halo);

    }

}

int main(int argc, char *argv[])
{

    halo_init();
    halo_run();
    halo_destroy();

    return 0;

}

