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
    char *command;
    struct animation_properties animationProperties;

};

struct halo_menu
{

    char *name;
    int count;
    int current;
    struct halo_menu_option *options[32];
    struct animation_properties animationProperties;
    struct halo_menu *next;
    struct halo_menu *prev;

};

struct halo_menu_list
{

    struct halo_menu *head;
    struct halo_menu *current;

};

extern struct halo_menu_option *halo_menu_option_create(unsigned int type, char *name, char *command);
extern void halo_menu_option_destroy(struct halo_menu_option *option);

extern struct halo_menu *halo_menu_create(char *name);
extern void halo_menu_destroy(struct halo_menu *menu);
extern void halo_menu_add_option(struct halo_menu *menu, struct halo_menu_option *option);
extern void halo_menu_remove_option(struct halo_menu *menu, struct halo_menu_option *option);
extern void halo_menu_activate(struct halo_menu *menu);
extern void halo_menu_next(struct halo_menu *menu);
extern void halo_menu_previous(struct halo_menu *menu);

extern struct halo_menu_list *halo_menu_list_create();
extern void halo_menu_list_destroy(struct halo_menu_list *list);
extern void halo_menu_list_add(struct halo_menu_list *list, struct halo_menu *menu);
extern void halo_menu_list_remove(struct halo_menu_list *list, struct halo_menu *menu);
extern struct halo_menu *halo_menu_list_find(struct halo_menu_list *list, char *name);

#endif

