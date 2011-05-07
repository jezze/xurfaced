#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <halo.h>
#include <menu.h>

extern struct halo halo;

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

void halo_menu_add_option(struct halo_menu *menu, struct halo_menu_option *option)
{

    menu->options[menu->count] = option;
    menu->count++;

}

void halo_menu_remove_option(struct halo_menu *menu, struct halo_menu_option *option)
{

    free(option);

}

void halo_menu_clear(struct halo_menu *menu)
{

    unsigned int i;

    for (i = 0; i < menu->count; i++)
        halo_menu_remove_option(menu, menu->options[i]);

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

