#ifndef HALO_DISPLAY_H
#define HALO_DISPLAY_H

struct halo_xlib_backend
{

    Display *display;
    int descriptor;
    int screen;
    int screenWidth;
    int screenHeight;
    Window root;
    Window main;
    Picture mainPicture;
    Atom atom_net[4];

};

extern struct halo_xlib_backend *halo_display_create();
extern void halo_display_destroy(struct halo_xlib_backend *backend);

#endif

