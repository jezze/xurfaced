#include <stdlib.h>
#include <X11/Xlib.h>
#include <client.h>
#include <halo.h>

struct halo_client *halo_client_list_head;

struct halo_client *halo_client_add(Window *window)
{

    struct halo_client *current = halo_client_list_head;

    while (current != 0)
    {

        current = current->next;

    }

    current = malloc(sizeof (struct halo_client));
    current->window = window;
    current->next = 0;

    return current;

}

void halo_client_remove(Window *window)
{

}

void halo_client_destroy(struct halo *halo)
{

    struct halo_client *current = halo_client_list_head;

    while (current != 0)
    {

        struct halo_client *next = current->next;
        free(current);
        current = next;

    }

    XSync(halo->display, 0);

}

void halo_client_init()
{

    halo_client_list_head = 0;

}

