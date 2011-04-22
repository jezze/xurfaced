#ifndef HALO_MENU_H
#define HALO_MENU_H

struct halo_menu_option
{

    char *name;
    void (*command)();

};

struct halo_menu
{

    struct halo_menu_option options[32];
    int count;
    int current;
    int select;

};

extern void halo_menu_add_option(struct halo_menu *menu, char *name, void (*command)());
extern void halo_menu_clear_options(struct halo_menu *menu);
extern void halo_menu_next(struct halo_menu *menu);
extern void halo_menu_previous(struct halo_menu *menu);

#endif

