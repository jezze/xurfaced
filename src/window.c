#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <halo.h>
#include <window.h>

static void halo_window_init_root(struct halo *halo)
{

    halo->backend->root = XRootWindow(halo->backend->display, halo->backend->screen);

    XSelectInput(halo->backend->display, halo->backend->root, SubstructureRedirectMask | SubstructureNotifyMask | StructureNotifyMask | PropertyChangeMask);

    halo->backend->atom_net[0] = XInternAtom(halo->backend->display, "_NET_SUPPORTED", 0);
    halo->backend->atom_net[1] = XInternAtom(halo->backend->display, "_NET_WM_NAME", 0);
    halo->backend->atom_net[2] = XInternAtom(halo->backend->display, "_NET_WM_STATE", 0);
    halo->backend->atom_net[3] = XInternAtom(halo->backend->display, "_NET_WM_STATE_FULLSCREEN", 0);

    XChangeProperty(halo->backend->display, halo->backend->root, halo->backend->atom_net[0], XA_ATOM, 32, PropModeReplace, (unsigned char *)halo->backend->atom_net, 4);

    XGrabKey(halo->backend->display, XKeysymToKeycode(halo->backend->display, XK_c), Mod1Mask | ShiftMask, halo->backend->root, 1, GrabModeAsync, GrabModeAsync);
    XGrabKey(halo->backend->display, XKeysymToKeycode(halo->backend->display, XK_q), Mod1Mask | ShiftMask, halo->backend->root, 1, GrabModeAsync, GrabModeAsync);
    XGrabKey(halo->backend->display, XKeysymToKeycode(halo->backend->display, XK_Escape), Mod1Mask, halo->backend->root, 1, GrabModeAsync, GrabModeAsync);
    XGrabKey(halo->backend->display, XKeysymToKeycode(halo->backend->display, XK_Tab), Mod1Mask, halo->backend->root, 1, GrabModeAsync, GrabModeAsync);

    XCompositeRedirectSubwindows(halo->backend->display, halo->backend->root, CompositeRedirectAutomatic);

    XSync(halo->backend->display, 0);

}

static void halo_window_init_base(struct halo *halo)
{

    halo->backend->main = XCreateSimpleWindow(halo->backend->display, halo->backend->root, 0, 0, halo->backend->screenWidth, halo->backend->screenHeight, 0, XBlackPixel(halo->backend->display, halo->backend->screen), XBlackPixel(halo->backend->display, halo->backend->screen));

    XWindowAttributes wa;
    XGetWindowAttributes(halo->backend->display, halo->backend->main, &wa);

    XRenderPictFormat *format = XRenderFindVisualFormat(halo->backend->display, wa.visual);

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    halo->backend->mainPicture = XRenderCreatePicture(halo->backend->display, halo->backend->main, format, CPSubwindowMode, &pa); 

//    XShapeSelectInput(halo->backend->display, halo->backend->main, ShapeNotifyMask);

    XSelectInput(halo->backend->display, halo->backend->main, ButtonPressMask | KeyPressMask);
    XMapWindow(halo->backend->display, halo->backend->main);

    XSync(halo->backend->display, 0);

}

void halo_window_init(struct halo *halo)
{

    halo_window_init_root(halo);
    halo_window_init_base(halo);

}

void halo_window_destroy(struct halo *halo)
{

    XUnmapWindow(halo->backend->display, halo->backend->main);

    XUngrabKeyboard(halo->backend->display, CurrentTime);

    XSync(halo->backend->display, 0);

}

