#ifndef HALO_MENU_H
#define HALO_MENU_H

struct halo_menu_option
{

  char *name;
  void (*command)(int current);

};

struct halo_menu
{

  struct halo_menu_option options[32];
  int count;
  int current;
  int select;

};

extern void halo_menu_add_option(char *name, void (*command)(int current));
extern void halo_menu_clear_options();
extern void halo_menu_next();
extern void halo_menu_previous();

#endif

