#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <limb/dlist.h>
#include <animation.h>
#include <client.h>
#include <halo.h>
#include <menu.h>

extern struct halo halo;

struct halo_menu_list menues;

static void halo_menu_option_control(struct halo_menu_option *option)
{

    if (!strcmp(option->command, "quit"))
        halo.running = 0;

    if (!strncmp(option->command, "show ", 5))
    {

        struct halo_menu *menu = halo_menu_list_find(halo.menues, option->command + 5);

        if (!menu)
            return;

        halo.menues->current = menu;
        halo.menues->current->animationProperties.translationY = 0;
        menu->current = 0;

    }

}

static void halo_menu_option_execute(struct halo_menu_option *option)
{

    char args[256], *argv[32];

    memcpy(args, option->command, strlen(option->command) + 1);

    argv[0] = strtok(args, " ");

    unsigned int i = 0;

    while ((argv[++i] = strtok(0, " ")));

    int pid = fork();

    if (pid == 0)
    {

        close(1);
        dup(halo.fd[1]);

        if (halo.display)
            close(halo.connection);

        setsid();

        execvp(argv[0], argv);

        exit(0);

    }

}

struct halo_menu_option *halo_menu_option_create(unsigned int type, char *name, char *description, char *command)
{

    struct halo_menu_option *option = malloc(sizeof (struct halo_menu_option));
    option->type = type;
    option->name = name;
    option->description = description;
    option->command = command;
    option->animationProperties.alpha = 0;

    return option;

}

void halo_menu_option_destroy(struct halo_menu_option *option)
{

    free(option);

}

struct halo_menu *halo_menu_create(char *name)
{

    struct halo_menu *menu = malloc(sizeof (struct halo_menu));
    menu->name = name;
    menu->current = 0;
    menu->count = 0;
    menu->next = 0;
    menu->prev = 0;

    return menu;

}

void halo_menu_destroy(struct halo_menu *menu)
{

    unsigned int i;

    for (i = 0; i < menu->count; i++)
    {

        halo_menu_remove_option(menu, menu->options[i]);
        halo_menu_option_destroy(menu->options[i]);

    }

    free(menu);

}

void halo_menu_add_option(struct halo_menu *menu, struct halo_menu_option *option)
{

    menu->options[menu->count] = option;
    menu->count++;

}

void halo_menu_remove_option(struct halo_menu *menu, struct halo_menu_option *option)
{

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

struct halo_menu_list *halo_menu_list_create(unsigned int width, unsigned int height)
{

    unsigned int center = width / 4;
    unsigned int middle = height / 4 + height / 8;

    struct halo_menu *menu;
    struct halo_menu_option *option;

    menu = halo_menu_create("home");
    menu->animationProperties.translationX = center;
    menu->animationProperties.translationY = middle;
    halo_menu_list_add(&menues, menu);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Games", "Play games installed on your system", "show games");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 0;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Videos", "Watch videos on your system or from an online streaming service", "show videos");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 80;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Music", "Connect to your online music provider and start listening", "show music");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 160;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Sessions", "Browse applications running on your system", "show sessions");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 240;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Quit", "Turn off your system", "quit");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 320;
    halo_menu_add_option(menu, option);

    menu = halo_menu_create("games");
    menu->animationProperties.translationX = center;
    menu->animationProperties.translationY = middle;
    halo_menu_list_add(&menues, menu);

    option = halo_menu_option_create(MENU_TYPE_EXEC, "SNES: Super Mario World", "Emulator: snes9x", "snes9x-gtk /home/jfu/roms/SuperMarioWorld.smc");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 0;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_EXEC, "SNES: Secret of Mana", "Emulator: snes9x", "snes9x-gtk /home/jfu/roms/SecretOfMana.smc");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 80;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_EXEC, "NDS: Children of Mana", "Emulator: desmume", "desmume /home/jfu/roms/ChildrenOfMana.nds");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 160;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Return", "Return back to the home screen", "show home");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 320;
    halo_menu_add_option(menu, option);

    menu = halo_menu_create("videos");
    menu->animationProperties.translationX = center;
    menu->animationProperties.translationY = middle;
    halo_menu_list_add(&menues, menu);

    option = halo_menu_option_create(MENU_TYPE_EXEC, "Big buck bunny", "Playtime: 12 minutes 49 seconds, Format: MPEG", "mplayer /home/jfu/movies/big_buck_bunny_720p_surround.avi");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 0;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Return", "Return back to the home screen", "show home");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 160;
    halo_menu_add_option(menu, option);

    menu = halo_menu_create("music");
    menu->animationProperties.translationX = center;
    menu->animationProperties.translationY = middle;
    halo_menu_list_add(&menues, menu);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Return", "Return back to the home screen", "show home");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 0;
    halo_menu_add_option(menu, option);

    menu = halo_menu_create("sessions");
    menu->animationProperties.translationX = center;
    menu->animationProperties.translationY = middle;
    halo_menu_list_add(&menues, menu);

    option = halo_menu_option_create(MENU_TYPE_EXEC, "Echo", "Testing echo", "cat ./share/main");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 0;
    halo_menu_add_option(menu, option);

    option = halo_menu_option_create(MENU_TYPE_CTRL, "Return", "Return back to the home screen", "show home");
    option->animationProperties.translationX = 0;
    option->animationProperties.translationY = 160;
    halo_menu_add_option(menu, option);

    menues.current = menues.head;

    return &menues;

}

void halo_menu_list_destroy(struct halo_menu_list *list)
{
/*
    while (list->head)
    {

        halo_menu_list_remove(list, list->head);
        halo_menu_destroy(list->head);

    }
*/
    list->current = 0;

}

void halo_menu_list_add(struct halo_menu_list *list, struct halo_menu *menu)
{

    if (!list->head)
    {

        menu->next = menu;
        menu->prev = menu;

        list->head = menu;

    }

    else
    {

        menu->next = list->head;
        menu->prev = list->head->prev;

        list->head->prev->next = menu;
        list->head->prev = menu;

    }

}

void halo_menu_list_remove(struct halo_menu_list *list, struct halo_menu *menu)
{

    if (menu == list->head)
        list->head = menu->next;

    menu->prev->next = menu->next;
    menu->next->prev = menu->prev;

    if (menu == menu->next)
        list->head = 0;

}

struct halo_menu *halo_menu_list_find(struct halo_menu_list *list, char *name)
{

    struct halo_menu *current = list->head;

    if (!current)
        return 0;

    while (current->next != list->head)
    {

        if (!strcmp(current->name, name))
            return current;

        current = current->next;

    }

    return (!strcmp(current->name, name)) ? current : 0;

}

