#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <limb/dlist.h>
#include <animation.h>
#include <display.h>
#include <xurfaced.h>
#include <menu.h>

struct xurfaced_menu_option *xurfaced_menu_option_create()
{

    struct xurfaced_menu_option *option = malloc(sizeof (struct xurfaced_menu_option));
    option->name = 0;
    option->description = 0;
    option->command = 0;
    option->animationProperties.alpha = 0;
    option->next = 0;
    option->prev = 0;

    return option;

}

void xurfaced_menu_option_destroy(struct xurfaced_menu_option *option)
{

    free(option->name);
    free(option->description);
    free(option->command);
    free(option);

}

struct xurfaced_menu_option_list *xurfaced_menu_option_list_create()
{

    struct xurfaced_menu_option_list *list = malloc(sizeof (struct xurfaced_menu_option_list));
    list->head = 0;
    list->current = 0;

    return list;

}

void xurfaced_menu_option_list_destroy(struct xurfaced_menu_option_list *list)
{

    struct xurfaced_menu_option *current = list->head;

    do
    {

        struct xurfaced_menu_option *next = current->next;

        xurfaced_menu_option_destroy(current);

        current = next;

    }
    while (current != list->head);

    list->current = 0;

}

void xurfaced_menu_option_list_add(struct xurfaced_menu_option_list *list, struct xurfaced_menu_option *option)
{

    if (!list->head)
    {

        option->next = option;
        option->prev = option;

        list->head = option;

    }

    else
    {

        option->next = list->head;
        option->prev = list->head->prev;

        list->head->prev->next = option;
        list->head->prev = option;

    }

}

void xurfaced_menu_option_list_remove(struct xurfaced_menu_option_list *list, struct xurfaced_menu_option *option)
{

    if (option == list->head)
        list->head = option->next;

    option->prev->next = option->next;
    option->next->prev = option->prev;

    if (option == option->next)
        list->head = 0;

}

struct xurfaced_menu *xurfaced_menu_create()
{

    struct xurfaced_menu *menu = malloc(sizeof (struct xurfaced_menu));
    menu->opts = xurfaced_menu_option_list_create();

    return menu;

}

void xurfaced_menu_destroy(struct xurfaced_menu *menu)
{

    xurfaced_menu_option_list_destroy(menu->opts);
    free(menu->opts);
    free(menu);

}

void xurfaced_menu_activate(struct xurfaced_menu *menu)
{

    if (strlen(menu->opts->current->command))
        xurfaced_execute(menu->opts->current->command, 0);

}

void xurfaced_menu_next(struct xurfaced_menu *menu, unsigned int num)
{

    struct xurfaced_menu_option *option = menu->opts->current;

    if (option == menu->opts->head->prev)
        return;

    while ((option = option->next))
    {

        if (!strlen(option->name))
            continue;

        if (!--num)
            break;

        if (option == menu->opts->head->prev)
            break;

    }

    menu->opts->current = option;

}

void xurfaced_menu_previous(struct xurfaced_menu *menu, unsigned int num)
{

    struct xurfaced_menu_option *option = menu->opts->current;

    if (option == menu->opts->head)
        return;

    while ((option = option->prev))
    {

        if (!strlen(option->name))
            continue;

        if (!--num)
            break;

        if (option == menu->opts->head)
            break;

    }

    menu->opts->current = option;

}

static FILE *xurfaced_open(char *path, int apipe[])
{

    struct stat info;

    stat(path, &info);

    if (info.st_mode & S_IXUSR)
    {

        pipe(apipe);
        xurfaced_execute(path, apipe);

        return fdopen(apipe[0], "r");

    }
    
    else
    {

        return fopen(path, "r");

    }

}

struct xurfaced_menu *xurfaced_menu_init(struct xurfaced *xurfaced, unsigned int width, unsigned int height)
{

    struct xurfaced_menu *menu = xurfaced_menu_create();
    menu->animationProperties.translationX = width / 4;
    menu->animationProperties.translationY = height / 4 + height / 8;

    char path[128];
    char current[128];
    char content[4096];

    FILE *head = fopen(xurfaced->config.head, "r");

    if (fgets(path, 128, head) == NULL)
    {

        fprintf(stderr, "Could not read head\n");
        exit(EXIT_FAILURE);

    }

    fclose(head);

    path[strlen(path) - 1] = '\0';

    sprintf(current, "%s/%s/title", xurfaced->config.base, path);

    FILE *fileTitle = xurfaced_open(current, xurfaced->pipe);

    if (!fileTitle)
        return 0;

    float y = 0;

    struct xurfaced_menu_option *option;

    while (fgets(content, 4096, fileTitle) != NULL)
    {

        content[strlen(content) - 1] = '\0';
        option = xurfaced_menu_option_create();
        option->name = (char *)malloc(strlen(content) + 1);

        strcpy(option->name, content);
        
        option->animationProperties.translationX = 0;
        option->animationProperties.translationY = y;

        xurfaced_menu_option_list_add(menu->opts, option);

        y += 80.0;

    }

    fclose(fileTitle);

    sprintf(current, "%s/%s/desc", xurfaced->config.base, path);

    FILE *fileDesc = xurfaced_open(current, xurfaced->pipe);

    if (!fileDesc)
        return 0;

    option = menu->opts->head;

    while (fgets(content, 4096, fileDesc) != NULL)
    {

        content[strlen(content) - 1] = '\0';
        option->description = (char *)malloc(strlen(content) + 1);

        strcpy(option->description, content);

        option = option->next;

    }

    fclose(fileDesc);

    sprintf(current, "%s/%s/exec", xurfaced->config.base, path);

    FILE *fileExec = xurfaced_open(current, xurfaced->pipe);

    if (!fileExec)
        return 0;

    option = menu->opts->head;

    while (fgets(content, 4096, fileExec) != NULL)
    {

        content[strlen(content) - 1] = '\0';
        option->command = (char *)malloc(strlen(content) + 1);

        strcpy(option->command, content);

        option = option->next;

    }

    fclose(fileExec);

    menu->opts->current = menu->opts->head;

    return menu;

}

