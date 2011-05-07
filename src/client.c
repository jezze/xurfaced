#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <client.h>

struct halo_client_list clients;

struct halo_client *halo_client_create(Window window)
{

    struct halo_client *client = malloc(sizeof (struct halo_client));
    client->window = window;
    client->picture = 0;
    client->next = 0;
    client->prev = 0;

    return client;

}

void halo_client_list_add(struct halo_client_list *list, struct halo_client *client)
{

    if (!list->head)
    {

        client->next = client;
        client->prev = client;

        list->head = client;

    }

    else
    {

        client->next = list->head;
        client->prev = list->head->prev;

        list->head->prev->next = client;
        list->head->prev = client;

    }

}

void halo_client_list_remove(struct halo_client_list *list, struct halo_client *client)
{

    if (client == list->head)
        list->head = client->next;

    client->prev->next = client->next;
    client->next->prev = client->prev;

    if (client == client->next)
        list->head = 0;

    free(client);

}

struct halo_client *halo_client_list_find(struct halo_client_list *list, Window window)
{

    struct halo_client *current = list->head;

    if (!current)
        return 0;

    while (current->next != list->head)
    {

        if (current->window == window)
            return current;

        current = current->next;

    }

    return (current->window == window) ? current : 0;

}

void halo_client_destroy(struct halo_client_list *list)
{

    while (list->head)
        halo_client_list_remove(list, list->head);

    list->current = 0;

}

struct halo_client_list *halo_client_init()
{

    clients.head = 0;
    clients.current = 0;

    return &clients;

}

