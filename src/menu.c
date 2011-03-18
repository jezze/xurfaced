#include <menu.h>

struct halo_menu halo_menu;

void halo_menu_add_option(char *name, void (*command)())
{

  halo_menu.options[halo_menu.count].name = name;
  halo_menu.options[halo_menu.count].command = command;
  halo_menu.count++;

}

void halo_menu_clear_options()
{

  halo_menu.count = 0;
  halo_menu.current = 0;
  halo_menu.select = -1;

}

void halo_menu_next()
{

  if (halo_menu.current < halo_menu.count - 1)
    halo_menu.current++;

}

void halo_menu_previous()
{

  if (halo_menu.current > 0)
    halo_menu.current--;

}

