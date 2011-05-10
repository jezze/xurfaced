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
#include <client.h>
#include <halo.h>
#include <menu.h>

extern struct halo halo;
struct halo_menu menuHome;
struct halo_menu menuMedia;
struct halo_menu menuGames;
struct halo_menu menuSessions;

static void halo_menu_option_control(struct halo_menu_option *option)
{

    if (!strcmp(option->command, "quit"))
        halo.running = 0;

    if (!strncmp(option->command, "show ", 5))
    {

        if (!strcmp(option->command + 5, "home"))
            halo.menu = &menuHome;

        if (!strcmp(option->command + 5, "media"))
            halo.menu = &menuMedia;

        if (!strcmp(option->command + 5, "games"))
            halo.menu = &menuGames;

        if (!strcmp(option->command + 5, "sessions"))
        {

            halo_menu_clear(&menuSessions);
            halo_menu_add(&menuSessions, halo_menu_option_create(MENU_TYPE_CTRL, "< Return", "show home"));

            halo.menu = &menuSessions;

            if (!halo.clients->head)
                return;

            struct halo_client *client = halo.clients->head;

            while (client->next != halo.clients->head)
            {

                halo_menu_add(&menuSessions, halo_menu_option_create(MENU_TYPE_CTRL, "Program X", "window 1"));

                client = client->next;

            }

            halo_menu_add(&menuSessions, halo_menu_option_create(MENU_TYPE_CTRL, "Program X", "window 1"));

        }

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

        if (halo.display)
            close(halo.connection);

        setsid();

        execvp(argv[0], argv);

        exit(0);

    }

}

struct halo_menu_option *halo_menu_option_create(unsigned int type, char *name, char *command)
{

    struct halo_menu_option *option = malloc(sizeof (struct halo_menu_option));
    option->type = type;
    option->name = name;
    option->command = command;

    return option;

}

void halo_menu_add(struct halo_menu *menu, struct halo_menu_option *option)
{

    menu->options[menu->count] = option;
    menu->count++;

}

void halo_menu_remove(struct halo_menu *menu, struct halo_menu_option *option)
{

    free(option);

}

void halo_menu_clear(struct halo_menu *menu)
{

    unsigned int i;

    for (i = 0; i < menu->count; i++)
        halo_menu_remove(menu, menu->options[i]);

    menu->count = 0;
    menu->current = 0;

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

struct halo_menu *halo_menu_init()
{

    menuHome.name = "home";

    halo_menu_clear(&menuHome);
    halo_menu_add(&menuHome, halo_menu_option_create(MENU_TYPE_CTRL, "Media", "show media"));
    halo_menu_add(&menuHome, halo_menu_option_create(MENU_TYPE_CTRL, "Games", "show games"));
    halo_menu_add(&menuHome, halo_menu_option_create(MENU_TYPE_CTRL, "Sessions", "show sessions"));
    halo_menu_add(&menuHome, halo_menu_option_create(MENU_TYPE_CTRL, "Quit", "quit"));

    menuMedia.name = "media";

    halo_menu_clear(&menuMedia);
    halo_menu_add(&menuMedia, halo_menu_option_create(MENU_TYPE_CTRL, "< Return", "show home"));
    halo_menu_add(&menuMedia, halo_menu_option_create(MENU_TYPE_EXEC, "The Matrix", "xterm"));

    menuGames.name = "games";

    halo_menu_clear(&menuGames);
    halo_menu_add(&menuGames, halo_menu_option_create(MENU_TYPE_CTRL, "< Return", "show home"));
    halo_menu_add(&menuGames, halo_menu_option_create(MENU_TYPE_EXEC, "SNES: Secret of Mana", "snes9x-gtk /home/jfu/SecretOfMana.smc"));

    menuSessions.name = "sessions";

    halo_menu_clear(&menuSessions);

    return &menuHome;

}

