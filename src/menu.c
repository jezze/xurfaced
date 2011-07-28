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
#include <halo.h>
#include <menu.h>

extern struct halo halo;

static void halo_execute(char *command, int pipe[])
{

    char args[4096], *argv[32];

    memcpy(args, command, strlen(command) + 1);

    argv[0] = strtok(args, " ");

    unsigned int i = 0;

    while ((argv[++i] = strtok(0, " ")));

    int pid = fork();
    int pchild;

    if (pid == -1)
        return;

    if (!pid)
    {

        pchild = getpid();

        if (pipe)
        {

            close(1);
            dup(pipe[1]);
            close(pipe[0]);
            close(pipe[1]);

        }

        if (halo.backend->display)
            close(halo.backend->descriptor);

        setsid();
        execvp(argv[0], argv);
        exit(EXIT_FAILURE);

    }

    if (pipe)
    {

        close(pipe[1]);

        int status;

        wait(&status);

    }

}

struct halo_menu_option *halo_menu_option_create()
{

    struct halo_menu_option *option = malloc(sizeof (struct halo_menu_option));
    option->name = 0;
    option->description = 0;
    option->command = 0;
    option->animationProperties.alpha = 0;
    option->next = 0;
    option->prev = 0;

    return option;

}

void halo_menu_option_destroy(struct halo_menu_option *option)
{

    free(option->name);
    free(option->description);
    free(option->command);
    free(option);

}

struct halo_menu_option_list *halo_menu_option_list_create()
{

    struct halo_menu_option_list *list = malloc(sizeof (struct halo_menu_option_list));
    list->head = 0;
    list->current = 0;

    return list;

}

void halo_menu_option_list_destroy(struct halo_menu_option_list *list)
{

    struct halo_menu_option *current = list->head;

    do
    {

        struct halo_menu_option *next = current->next;

        halo_menu_option_destroy(current);

        current = next;

    }
    while (current != list->head);

    list->current = 0;

}

void halo_menu_option_list_add(struct halo_menu_option_list *list, struct halo_menu_option *option)
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

void halo_menu_option_list_remove(struct halo_menu_option_list *list, struct halo_menu_option *option)
{

    if (option == list->head)
        list->head = option->next;

    option->prev->next = option->next;
    option->next->prev = option->prev;

    if (option == option->next)
        list->head = 0;

}

struct halo_menu *halo_menu_create()
{

    struct halo_menu *menu = malloc(sizeof (struct halo_menu));
    menu->opts = halo_menu_option_list_create();

    return menu;

}

void halo_menu_destroy(struct halo_menu *menu)
{

    halo_menu_option_list_destroy(menu->opts);
    free(menu->opts);
    free(menu);

}

void halo_menu_activate(struct halo_menu *menu)
{

    if (strlen(menu->opts->current->command))
        halo_execute(menu->opts->current->command, 0);

}

void halo_menu_next(struct halo_menu *menu, unsigned int num)
{

    struct halo_menu_option *option = menu->opts->current;

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

void halo_menu_previous(struct halo_menu *menu, unsigned int num)
{

    struct halo_menu_option *option = menu->opts->current;

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

static FILE *halo_open(char *path)
{

    struct stat info;

    stat(path, &info);

    if (info.st_mode & S_IXUSR)
    {

        pipe(halo.pipe);
        halo_execute(path, halo.pipe);

        return fdopen(halo.pipe[0], "r");

    }
    
    else
    {

        return fopen(path, "r");

    }

}

struct halo_menu *halo_menu_init(unsigned int width, unsigned int height)
{

    struct halo_menu *menu = halo_menu_create();
    menu->animationProperties.translationX = width / 4;
    menu->animationProperties.translationY = height / 4 + height / 8;

    char path[128];
    char current[128];
    char content[4096];

    FILE *head = fopen(halo.pathHead, "r");

    if (fgets(path, 128, head) == NULL)
    {

        fprintf(stderr, "Could not read head\n");
        exit(EXIT_FAILURE);

    }

    fclose(head);

    path[strlen(path) - 1] = '\0';

    sprintf(current, "%s/%s/title", halo.pathConfig, path);

    FILE *fileTitle = halo_open(current);

    if (!fileTitle)
        return 0;

    float y = 0;

    struct halo_menu_option *option;

    while (fgets(content, 4096, fileTitle) != NULL)
    {

        content[strlen(content) - 1] = '\0';
        option = halo_menu_option_create();
        option->name = (char *)malloc(strlen(content) + 1);

        strcpy(option->name, content);
        
        option->animationProperties.translationX = 0;
        option->animationProperties.translationY = y;

        halo_menu_option_list_add(menu->opts, option);

        y += 80.0;

    }

    fclose(fileTitle);

    sprintf(current, "%s/%s/desc", halo.pathConfig, path);

    FILE *fileDesc = halo_open(current);

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

    sprintf(current, "%s/%s/exec", halo.pathConfig, path);

    FILE *fileExec = halo_open(current);

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

