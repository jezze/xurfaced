#ifndef HALO_MENU_H
#define HALO_MENU_H

#define MENU_TYPE_NULL 0
#define MENU_TYPE_CTRL 1
#define MENU_TYPE_EXEC 2

struct animation_properties;

struct halo_menu_option
{

    unsigned int type;
    char *name;
    char *description;
    char *command;
    struct animation_properties animationProperties;

};

struct halo_menu
{

    int count;
    int current;
    struct halo_menu_option *options[32];
    struct halo_menu *next;
    struct halo_menu *prev;
    struct animation_properties animationProperties;

};

extern struct halo_menu_option *halo_menu_option_create();
extern void halo_menu_option_destroy(struct halo_menu_option *option);

extern struct halo_menu *halo_menu_create();
extern void halo_menu_destroy(struct halo_menu *menu);
extern void halo_menu_activate(struct halo_menu *menu);
extern void halo_menu_next(struct halo_menu *menu);
extern void halo_menu_previous(struct halo_menu *menu);
extern struct halo_menu *halo_menu_init(unsigned int width, unsigned int height);

#endif

