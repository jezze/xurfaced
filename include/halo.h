#ifndef HALO_HALO_H
#define HALO_HALO_H

struct halo_client_list;
struct halo_menu;

struct halo
{

    struct halo_xlib_backend *backend;
    int running;
    int paused;
    pthread_mutex_t mutexMenu;
    pthread_mutex_t mutexRender;
    pthread_cond_t condRender;
    struct halo_menu *menu;
    struct halo_client_list *clients;
    int pipe[2];
    char *pathHome;
    char pathConfig[128];
    char pathHead[128];
    char pathPid[128];

};

#endif

