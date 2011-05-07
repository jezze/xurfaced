#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <halo.h>
#include <menu.h>

extern struct halo halo;

static void halo_menu_ctrl(struct halo_menu_option *option)
{

}

static void halo_menu_exec(struct halo_menu_option *option)
{

    int pid = fork();

    if (pid == 0)
    {

        if (halo.display)
            close(XConnectionNumber(halo.display));

        setsid();

        execlp(option->command, option->command, 0);

        exit(0);

    }

}

void halo_menu_add_option(struct halo_menu *menu, unsigned int type, char *name, char *command)
{

    menu->options[menu->count].type = type;
    menu->options[menu->count].name = name;
    menu->options[menu->count].command = command;
    menu->count++;

}

void halo_menu_clear_options(struct halo_menu *menu)
{

    menu->count = 0;
    menu->current = 0;
    menu->select = -1;

}

void halo_menu_activate(struct halo_menu *menu)
{

    switch (menu->options[menu->current].type)
    {

        case MENU_TYPE_CTRL:

            halo_menu_ctrl(&menu->options[menu->current]);

            break;

        case MENU_TYPE_EXEC:

            halo_menu_exec(&menu->options[menu->current]);

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

