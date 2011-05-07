#include <stdlib.h>
#include <stdio.h>
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

static void halo_init_menu()
{

    halo_menu_clear(&mainMenu);

    halo_menu_add_option(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Home", 0));
    halo_menu_add_option(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Media", 0));
    halo_menu_add_option(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Games", 0));
    halo_menu_add_option(&mainMenu, halo_menu_option_create(MENU_TYPE_EXEC, "Terminal", "xterm"));
    halo_menu_add_option(&mainMenu, halo_menu_option_create(MENU_TYPE_EXEC, "Eyes", "xeyes"));
    halo_menu_add_option(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Quit", "quit"));

    halo.menuCurrent = &mainMenu;

}

static void halo_init()
{

    halo_display_init(&halo);
    halo_window_init(&halo);
    halo_surface_init(&halo);
    halo.clients = halo_client_init();

    halo_init_menu();

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

