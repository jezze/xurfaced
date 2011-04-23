#include <X11/Xlib.h>
#include <stdlib.h>
#include <client.h>
#include <halo.h>

struct halo_client *halo_client_add(struct halo *halo, Window window)
{

    struct halo_client *client = malloc(sizeof (struct halo_client));
    client->window = window;
    client->next = 0;

    if (!halo->clients)
        return halo->clients = client;

    struct halo_client *current = halo->clients;

    while (current->next)
        current = current->next;

    client->prev = current;

    return current->next = client;

}

struct halo_client *halo_client_find(struct halo *halo, Window window)
{

    struct halo_client *current = halo->clients;

    while (current && current->window != window)
        current = current->next;

    return current;

}

void halo_client_remove(struct halo *halo, struct halo_client *client)
{

    client->prev->next = client->next;
    client->next->prev = client->prev;

    free(client);

}

void halo_client_destroy(struct halo *halo)
{

    struct halo_client *current = halo->clients;

    while (current != 0)
    {

        struct halo_client *next = current->next;
        halo_client_remove(halo, current);
        current = next;

    }

}

void halo_client_init(struct halo *halo)
{

    halo->clients = 0;
    halo->clientCurrent = 0;

}

