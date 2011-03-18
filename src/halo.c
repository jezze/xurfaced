#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <client.h>
#include <display.h>
#include <event.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>
#include <window.h>

static void test()
{

}

void halo_quit(struct halo *halo)
{

    halo->running = 0;

}

void halo_spawn(struct halo *halo)
{

    if (fork() == 0)
    {

        if (halo->display)
            close(XConnectionNumber(halo->display));

        setsid();

        execl("/usr/bin/xclock", "/usr/bin/xclock", (char *)0);

        exit(0);

    }

}

static void halo_init(struct halo *halo)
{

    halo_display_init(halo);
    halo_window_init(halo);
    halo_surface_init(halo);

    halo_client_init();

    halo_menu_clear_options();
    halo_menu_add_option("Home", test);
    halo_menu_add_option("Media", test);
    halo_menu_add_option("Games", test);
    halo_menu_add_option("Quit", test);

}

static void halo_destroy(struct halo *halo)
{

    halo_client_destroy(halo);
    halo_surface_destroy(halo);
    halo_display_destroy(halo);

}

static void halo_run(struct halo *halo)
{

    halo->running = 1;

    while (halo->running)
    {

        halo_event_handler(halo);

    }

}

int main(int argc, char *argv[])
{

    struct halo halo;

    halo_init(&halo);
    halo_run(&halo);
    halo_destroy(&halo);

    return 0;

}

