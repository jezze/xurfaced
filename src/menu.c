#include <menu.h>

void halo_menu_add_option(struct halo_menu *menu, char *name, void (*command)())
{

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

    menu->options[menu->current].command();

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

