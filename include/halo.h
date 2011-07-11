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
    struct halo_menu_list *menues;
    struct halo_client_list *clients;
    int fd[2];

};

#endif

