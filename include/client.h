#ifndef HALO_CLIENT_H
#define HALO_CLIENT_H

struct halo_client
{

    Window window;
    struct halo_client *next;
    struct halo_client *prev;

};

struct halo_client_list
{

    struct halo_client *head;
    struct halo_client *current;

};

extern struct halo_client *halo_client_add(Window window);
extern void halo_client_list_add(struct halo_client_list *list, struct halo_client *client);
extern void halo_client_list_remove(struct halo_client_list *list, struct halo_client *client);
extern struct halo_client *halo_client_list_find(struct halo_client_list *list, Window window);
extern void halo_client_destroy(struct halo_client_list *list);
extern struct halo_client_list *halo_client_init();

#endif

