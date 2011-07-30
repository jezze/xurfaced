#ifndef HALO_HALO_H
#define HALO_HALO_H

struct halo_config
{

    char base[128];
    char head[128];
    char oninit[128];
    char onexit[128];
    char pid[128];
    char notify[128];
    char notification[256];

};

struct halo
{

    struct halo_config config;
    struct halo_display_backend *backend;
    int running;
    int paused;
    pthread_t threadRender;
    pthread_t threadEvents;
    pthread_mutex_t mutexMenu;
    pthread_mutex_t mutexRender;
    pthread_cond_t condRender;
    struct halo_menu *menu;
    struct halo_client_list *clients;
    int pipe[2];

};

#endif

