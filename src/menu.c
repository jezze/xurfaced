#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
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

static void halo_execute(char *command)
{

    char args[256], *argv[32];

    memcpy(args, command, strlen(command) + 1);

    argv[0] = strtok(args, " ");

    unsigned int i = 0;

    while ((argv[++i] = strtok(0, " ")));

    int pid = fork();

    if (pid == -1)
        return;

    if (pid)
    {

    }

    else
    {

        close(1);
        dup(halo.fd[1]);

        if (halo.display)
            close(halo.connection);

        setsid();

        execvp(argv[0], argv);

    }

}

struct halo_menu_option *halo_menu_option_create()
{

    struct halo_menu_option *option = malloc(sizeof (struct halo_menu_option));
    option->name = 0;
    option->description = 0;
    option->command = 0;
    option->animationProperties.alpha = 0;

    return option;

}

void halo_menu_option_destroy(struct halo_menu_option *option)
{

    free(option->name);
    free(option->description);
    free(option->command);
    free(option);

}

static void halo_menu_add_option(struct halo_menu *menu, struct halo_menu_option *option)
{

    menu->options[menu->count] = option;
    menu->count++;

}

static void halo_menu_remove_option(struct halo_menu *menu, struct halo_menu_option *option)
{

}

struct halo_menu *halo_menu_create()
{

    struct halo_menu *menu = malloc(sizeof (struct halo_menu));
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

void halo_menu_activate(struct halo_menu *menu)
{

    halo_execute(menu->options[menu->current]->command);

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

struct halo_menu *halo_menu_init(unsigned int width, unsigned int height)
{

    struct halo_menu *menu = halo_menu_create();
    menu->animationProperties.translationX = width / 4;
    menu->animationProperties.translationY = height / 4 + height / 8;

    struct halo_menu_option *option;
    FILE *file;
    char line[512];
    int i;

    file = fopen("/home/jfu/.halo/init/title", "r");

    if (!file)
        return 0;

    float y = 0;

    while (fgets(line, 512, file) != NULL)
    {

        line[strlen(line) - 1] = '\0';
        option = halo_menu_option_create();
        option->name = (char *)malloc(strlen(line) + 1);
        strcpy(option->name, line);
        option->animationProperties.translationX = 0;
        option->animationProperties.translationY = y;
        halo_menu_add_option(menu, option);

        y += 80.0;

    }

    fclose(file);

    file = fopen("/home/jfu/.halo/init/desc", "r");

    if (!file)
        return 0;

    i = 0;

    while (fgets(line, 512, file) != NULL)
    {

        line[strlen(line) - 1] = '\0';
        option = menu->options[i];
        option->description = (char *)malloc(strlen(line) + 1);
        strcpy(option->description, line);

        i++;

    }

    fclose(file);

    file = fopen("/home/jfu/.halo/init/exec", "r");

    if (!file)
        return 0;

    i = 0;

    while (fgets(line, 512, file) != NULL)
    {

        line[strlen(line) - 1] = '\0';
        option = menu->options[i];
        option->command = (char *)malloc(strlen(line) + 1);
        strcpy(option->command, line);

        i++;

    }

    fclose(file);

    return menu;

}

