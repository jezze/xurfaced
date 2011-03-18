#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <client.h>
#include <menu.h>
#include <surface.h>
#include <halo.h>
#include <event.h>

extern struct halo_menu halo_menu;

static void halo_event_configurerequest(struct halo *halo, XConfigureRequestEvent *e)
{

    XWindowChanges wc;

    wc.x = 0;
    wc.y = 0;
    wc.width = halo->screenWidth;
    wc.height = halo->screenHeight;
    wc.border_width = 0;
    wc.sibling = e->above;
    wc.stack_mode = e->detail;

    XConfigureWindow(halo->display, e->window, e->value_mask, &wc);

}

static void halo_event_expose(struct halo *halo, XExposeEvent *e)
{

}

static void halo_event_maprequest(struct halo *halo, XMapRequestEvent *e)
{

    halo_client_add(&e->window);

    XSelectInput(halo->display, e->window, StructureNotifyMask | PropertyChangeMask);

    XRaiseWindow(halo->display, e->window);
    XMoveResizeWindow(halo->display, e->window, 0, 0, halo->screenWidth, halo->screenHeight);
    XMapWindow(halo->display, e->window);

    Atom halo_atom_wm[3];
    halo_atom_wm[0] = XInternAtom(halo->display, "WM_PROTOCOLS", 0);
    halo_atom_wm[1] = XInternAtom(halo->display, "WM_DELETE_WINDOW", 0);
    halo_atom_wm[2] = XInternAtom(halo->display, "WM_STATE", 0);

    long data[] = { NormalState, None };

    XChangeProperty(halo->display, e->window, halo_atom_wm[2], halo_atom_wm[2], 32, PropModeReplace, (unsigned char *)data, 2);

}

static void halo_event_keypress(struct halo *halo, XKeyPressedEvent *e)
{

    KeySym key = XLookupKeysym(e, 0);

    switch (key)
    {

        case XK_q:
            halo_quit(halo);
            break;

        case XK_Up:
            halo_menu_previous();
            break;

        case XK_Down:
            halo_menu_next();
            break;

        case XK_Tab:

            if (e->state & Mod1Mask)
            {

                XRaiseWindow(halo->display, e->window);
                XSetInputFocus(halo->display, e->window, RevertToParent, CurrentTime);

            }

            break;

        case XK_Escape:
            XRaiseWindow(halo->display, halo->main);
            break;

        case XK_x:
            halo_spawn(halo);
            break;

    }

}

void halo_event_handler(struct halo *halo)
{

    XEvent event;
    XNextEvent(halo->display, &event);

    switch (event.type)
    {

        case Expose:
            if (event.xexpose.count < 1)
                halo_event_expose(halo, &event.xexpose);
            break;

        case KeyPress:
            halo_event_keypress(halo, &event.xkey);
            halo_surface_blit(halo, &halo_menu);
            break;

        case ButtonPress:
            break;

        case MapRequest:
            halo_event_maprequest(halo, &event.xmaprequest);
            break;

        case ConfigureRequest:
            halo_event_configurerequest(halo, &event.xconfigurerequest);
            break;

    }

}

