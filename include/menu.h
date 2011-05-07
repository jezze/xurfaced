#ifndef HALO_MENU_H
#define HALO_MENU_H

#define MENU_TYPE_NULL 0
#define MENU_TYPE_CTRL 1
#define MENU_TYPE_EXEC 2

struct halo_menu_option
{

    unsigned int type;
    char *name;
    char *command;

};

struct halo_menu
{

    struct halo_menu_option *options[32];
    int count;
    int current;

};

extern struct halo_menu_option *halo_menu_option_create(unsigned int type, char *name, char *command);
extern void halo_menu_add(struct halo_menu *menu, struct halo_menu_option *option);
extern void halo_menu_remove(struct halo_menu *menu, struct halo_menu_option *option);
extern void halo_menu_clear(struct halo_menu *menu);
extern void halo_menu_activate(struct halo_menu *menu);
extern void halo_menu_next(struct halo_menu *menu);
extern void halo_menu_previous(struct halo_menu *menu);
extern struct halo_menu *halo_menu_init();

#endif

