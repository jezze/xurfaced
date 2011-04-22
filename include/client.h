#ifndef HALO_CLIENT_H
#define HALO_CLIENT_H

struct halo;

struct halo_client
{

    Window window;
    struct halo_client *next;
    struct halo_client *prev;

};

extern struct halo_client *halo_client_add(struct halo *halo, Window window);
extern struct halo_client *halo_client_find(struct halo *halo, Window window);
extern void halo_client_remove(struct halo_client *client);
extern void halo_client_destroy(struct halo *halo);
extern void halo_client_init(struct halo *halo);

#endif

