#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <halo.h>
#include <menu.h>

extern struct halo halo;
struct halo_menu mainMenu;

static void halo_menu_option_control(struct halo_menu_option *option)
{

    if (!strcmp(option->command, "quit"))
        halo.running = 0;

}

static void halo_menu_option_execute(struct halo_menu_option *option)
{

    int pid = fork();

    if (pid == 0)
    {

        if (halo.display)
            close(halo.connection);

        setsid();

        execlp(option->command, option->command, 0);

        exit(0);

    }

}

struct halo_menu_option *halo_menu_option_create(unsigned int type, char *name, char *command)
{

    struct halo_menu_option *option = malloc(sizeof (struct halo_menu_option));
    option->type = type;
    option->name = name;
    option->command = command;

    return option;

}

void halo_menu_add(struct halo_menu *menu, struct halo_menu_option *option)
{

    menu->options[menu->count] = option;
    menu->count++;

}

void halo_menu_remove(struct halo_menu *menu, struct halo_menu_option *option)
{

    free(option);

}

void halo_menu_clear(struct halo_menu *menu)
{

    unsigned int i;

    for (i = 0; i < menu->count; i++)
        halo_menu_remove(menu, menu->options[i]);

    menu->count = 0;
    menu->current = 0;

}

void halo_menu_activate(struct halo_menu *menu)
{

    switch (menu->options[menu->current]->type)
    {

        case MENU_TYPE_CTRL:

            halo_menu_option_control(menu->options[menu->current]);

            break;

        case MENU_TYPE_EXEC:

            halo_menu_option_execute(menu->options[menu->current]);

            break;

    }

}

void halo_menu_next(struct halo_menu *menu)
{

    if (menu->current < menu->count - 1)
        menu->current++;

}

void halo_menu_previous(struct halo_menu *menu)
{

    if (menu->current > 0)
        menu->current--;

}

struct halo_menu *halo_menu_init()
{

    halo_menu_clear(&mainMenu);

    halo_menu_add(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Home", "show home"));
    halo_menu_add(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Media", "show media"));
    halo_menu_add(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Games", "show games"));
    halo_menu_add(&mainMenu, halo_menu_option_create(MENU_TYPE_EXEC, "Terminal", "xterm"));
    halo_menu_add(&mainMenu, halo_menu_option_create(MENU_TYPE_EXEC, "Eyes", "xeyes"));
    halo_menu_add(&mainMenu, halo_menu_option_create(MENU_TYPE_CTRL, "Quit", "quit"));

    return &mainMenu;

}

