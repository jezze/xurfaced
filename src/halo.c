#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <display.h>
#include <event.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>
#include <window.h>

struct halo halo;
struct halo_menu menu;

void halo_none()
{

}

void halo_quit()
{

    halo.running = 0;

}

void halo_spawn()
{

    if (fork() == 0)
    {

        if (halo.display)
            close(XConnectionNumber(halo.display));

        setsid();

        execl("/usr/bin/xterm", "/usr/bin/xterm", (char *)0);

        exit(0);

    }

}

static void halo_init()
{

    halo_display_init(&halo);
    halo_window_init(&halo);
    halo_surface_init(&halo);

    halo_menu_clear_options(&menu);
    halo_menu_add_option(&menu, "Home", halo_none);
    halo_menu_add_option(&menu, "Media", halo_none);
    halo_menu_add_option(&menu, "Games", halo_none);
    halo_menu_add_option(&menu, "Terminal", halo_spawn);
    halo_menu_add_option(&menu, "Quit", halo_quit);

}

static void halo_destroy()
{

    halo_surface_destroy(&halo);
    halo_display_destroy(&halo);

}

static void halo_run()
{

    halo.running = 1;

    while (halo.running)
    {

        halo_event_handler(&halo);
        halo_surface_blit(&halo, &menu);

    }

}

int main(int argc, char *argv[])
{

    halo_init();
    halo_run();
    halo_destroy();

    return 0;

}

