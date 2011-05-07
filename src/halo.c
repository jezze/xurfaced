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

struct halo halo;
struct halo_menu mainMenu;

void halo_none()
{

}

void halo_quit()
{

    halo.running = 0;

}

void halo_spawn()
{

    int pid = fork();

    if (pid == 0)
    {

        if (halo.display)
            close(XConnectionNumber(halo.display));

        setsid();

        execlp("xterm", "xterm", 0);

        exit(0);

    }

}

static void halo_init_menu()
{

    halo_menu_clear_options(&mainMenu);
    halo_menu_add_option(&mainMenu, "Home", halo_none);
    halo_menu_add_option(&mainMenu, "Media", halo_none);
    halo_menu_add_option(&mainMenu, "Games", halo_none);
    halo_menu_add_option(&mainMenu, "Terminal", halo_spawn);
    halo_menu_add_option(&mainMenu, "Quit", halo_quit);

    halo.menuCurrent = &mainMenu;

}

static void halo_init()
{

    halo_display_init(&halo);
    halo_window_init(&halo);
    halo_surface_init(&halo);
    halo_client_init(&halo);

    halo_init_menu();

}

static void halo_destroy()
{

    halo_client_destroy(&halo);
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

