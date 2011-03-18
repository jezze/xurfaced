#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/shape.h>
#include <halo.h>
#include <window.h>

static void halo_window_init_root(struct halo *halo)
{

    halo->root = XRootWindow(halo->display, halo->screen);

    halo->atom_net[0] = XInternAtom(halo->display, "_NET_SUPPORTED", 0);
    halo->atom_net[1] = XInternAtom(halo->display, "_NET_WM_NAME", 0);
    halo->atom_net[2] = XInternAtom(halo->display, "_NET_WM_STATE", 0);
    halo->atom_net[3] = XInternAtom(halo->display, "_NET_WM_STATE_FULLSCREEN", 0);

    XChangeProperty(halo->display, halo->root, halo->atom_net[0], XA_ATOM, 32, PropModeReplace, (unsigned char *)halo->atom_net, 4);

    XSetWindowAttributes attributes;
    attributes.event_mask = SubstructureRedirectMask | SubstructureNotifyMask | StructureNotifyMask | PropertyChangeMask;

    XChangeWindowAttributes(halo->display, halo->root, CWEventMask, &attributes);
    XSelectInput(halo->display, halo->root, attributes.event_mask);

//    XCompositeRedirectSubwindows(halo->display, halo->root, CompositeRedirectAutomatic);

    XGrabKey(halo->display, XKeysymToKeycode(halo->display, XK_Escape), Mod1Mask, halo->root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(halo->display, XKeysymToKeycode(halo->display, XK_Tab), Mod1Mask, halo->root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(halo->display, XKeysymToKeycode(halo->display, XK_q), Mod1Mask | ShiftMask, halo->root, True, GrabModeAsync, GrabModeAsync);

    XSync(halo->display, 0);

}

static void halo_window_init_base(struct halo *halo)
{

    halo->main = XCreateSimpleWindow(halo->display, halo->root, 1, 1, halo->screenWidth, halo->screenHeight, 0, XBlackPixel(halo->display, halo->screen), XBlackPixel(halo->display, halo->screen));

//    XWindowAttributes wa;
//    XGetWindowAttributes(halo->display, halo->main, &wa);

//    XRenderPictFormat *format = XRenderFindVisualFormat(halo->display, wa.visual);

//    XRenderPictureAttributes pa;
//    pa.subwindow_mode = IncludeInferiors;

//    Picture picture = XRenderCreatePicture(halo->display, halo->main, format, CPSubwindowMode, &pa); 

//    XShapeSelectInput(halo->display, halo->main, ShapeNotifyMask);

    XSelectInput(halo->display, halo->main, ButtonPressMask | KeyPressMask);
    XMapWindow(halo->display, halo->main);

}

void halo_window_init(struct halo *halo)
{

    halo_window_init_root(halo);
    halo_window_init_base(halo);

}

