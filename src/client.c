#include <stdlib.h>
#include <X11/Xlib.h>
#include <client.h>
#include <halo.h>

struct halo_client *halo_client_add(struct halo *halo, Window window)
{

    struct halo_client *client = malloc(sizeof (struct halo_client));
    client->window = window;
    client->next = 0;

    if (!halo->clients)
        return halo->clients = halo->clientCurrent = client;

    struct halo_client *current = halo->clients;

    while (current->next)
        current = current->next;

    return current->next = halo->clientCurrent = client;

}

struct halo_client *halo_client_find(struct halo *halo, Window window)
{

    struct halo_client *current = halo->clients;

    while (current->window != window)
        current = current->next;

    return current;

}

void halo_client_remove(struct halo_client *client)
{

    // something like XCloseWindow(client->window);
    free(client);

}

void halo_client_destroy(struct halo *halo)
{

    struct halo_client *current = halo->clients;

    while (current != 0)
    {

        struct halo_client *next = current->next;
        halo_client_remove(current);
        current = next;

    }

    XSync(halo->display, 0);

}

void halo_client_init(struct halo *halo)
{

    halo->clients = 0;
    halo->clientCurrent = 0;

}

