#ifndef HALO_MENU_H
#define HALO_MENU_H

struct animation_properties;

struct halo_menu_option
{

    unsigned int type;
    char *name;
    char *description;
    char *command;
    struct animation_properties animationProperties;
    struct halo_menu_option *next;
    struct halo_menu_option *prev;

};

struct halo_menu_option_list
{

    struct halo_menu_option *head;
    struct halo_menu_option *current;

};

struct halo_menu
{

    struct halo_menu_option_list *opts;
    struct animation_properties animationProperties;

};

extern void halo_execute(char *command, int pipe[]);

extern struct halo_menu_option *halo_menu_option_create();
extern void halo_menu_option_destroy(struct halo_menu_option *option);

extern struct halo_menu_option_list *halo_menu_option_list_create();
extern void halo_menu_option_list_destroy(struct halo_menu_option_list *list);
extern void halo_menu_option_list_add(struct halo_menu_option_list *list, struct halo_menu_option *option);
extern void halo_menu_option_list_remove(struct halo_menu_option_list *list, struct halo_menu_option *option);

extern struct halo_menu *halo_menu_create();
extern void halo_menu_destroy(struct halo_menu *menu);
extern void halo_menu_activate(struct halo_menu *menu);
extern void halo_menu_next(struct halo_menu *menu, unsigned int num);
extern void halo_menu_previous(struct halo_menu *menu, unsigned int num);
extern struct halo_menu *halo_menu_init(unsigned int width, unsigned int height);

#endif

