#ifndef HALO_HALO_H
#define HALO_HALO_H

struct halo_client_list;
struct halo_menu;

struct halo
{

    Display *display;
    int connection;
    int screen;
    int screenWidth;
    int screenHeight;
    Window root;
    Window main;
    Picture mainPicture;
    Atom atom_net[4];
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
    char pathInit[128];
    char pathTitle[128];
    char pathDesc[128];
    char pathExec[128];

};

#endif

