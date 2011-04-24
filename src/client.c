#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <client.h>
#include <halo.h>

struct halo_client *halo_client_add(struct halo *halo, Window window)
{

    struct halo_client *client = malloc(sizeof (struct halo_client));
    client->window = window;
    client->next = client;
    client->prev = client;

    if (!halo->clients)
        return halo->clients = client;

    client->prev = halo->clients->prev;
    client->next = halo->clients;

    client->prev->next = client;
    halo->clients->prev = client;
    
    return client;

}

struct halo_client *halo_client_find(struct halo *halo, Window window)
{

    struct halo_client *current = halo->clients;

    if (current->next == halo->clients)
        return (current->window == window) ? current : 0;

    while (current->next != halo->clients)
    {

        if (current->window == window)
            return current;

        current = current->next;

    }

    return 0;

}

void halo_client_remove(struct halo *halo, struct halo_client *client)
{

    client->prev->next = client->next;
    client->next->prev = client->prev;

    if (client == halo->clients)
        halo->clients = client->next;

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

    halo->clients = 0;
    halo->clientCurrent = 0;

}

