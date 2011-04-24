#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <client.h>
#include <halo.h>

struct halo_client_list clients;

struct halo_client *halo_client_add(struct halo_client_list *list, Window window)
{

    struct halo_client *client = malloc(sizeof (struct halo_client));
    client->window = window;
    client->next = client;
    client->prev = client;

    if (!list->head)
        return list->head = client;

    client->prev = list->head->prev;
    client->next = list->head;

    client->prev->next = client;
    list->head->prev = client;
    
    return client;

}

struct halo_client *halo_client_find(struct halo_client_list *list, Window window)
{

    struct halo_client *current = list->head;

    if (current->next == list->head)
        return (current->window == window) ? current : 0;

    while (current->next != list->head)
    {

        if (current->window == window)
            return current;

        current = current->next;

    }

    return 0;

}

void halo_client_remove(struct halo_client_list *list, struct halo_client *client)
{

    client->prev->next = client->next;
    client->next->prev = client->prev;

    if (client == list->head)
        list->head = client->next;

    free(client);

}

void halo_client_destroy(struct halo *halo)
{

/*
    struct halo_client *current = halo->clients;

    while (current != 0)
    {

        struct halo_client *next = current->next;
        halo_client_remove(halo, current);
        current = next;

    }
*/

}

void halo_client_init(struct halo *halo)
{

    clients.head = 0;
    clients.count = 0;
    clients.current = 0;

    halo->clients = &clients;

}

