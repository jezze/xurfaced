#ifndef HALO_HALO_H
#define HALO_HALO_H

struct halo_client;
struct halo_menu;

struct halo
{

    Display *display;
    int connection;
    int screen;
    int screenWidth;
    int screenHeight;
    Window root;
    Atom atom_net[4];
    Window main;
    int running;
    struct halo_menu *menuCurrent;
    struct halo_client *clients;

};

extern void halo_none();
extern void halo_quit();
extern void halo_spawn();

#endif

