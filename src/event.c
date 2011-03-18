#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
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

    XConfigureEvent ce;
    ce.type = ConfigureNotify;
    ce.display = halo->display;
    ce.event = e->window;
    ce.window = e->window;
    ce.x = 0;
    ce.y = 0;
    ce.width = halo->screenWidth;
    ce.height = halo->screenHeight;
    ce.border_width = 0;
    ce.above = 0;
    ce.override_redirect = 0;

    XSendEvent(halo->display, e->window, False, StructureNotifyMask, (XEvent *)&ce);

}

static void halo_event_expose(struct halo *halo, XExposeEvent *e)
{

}

static void halo_event_maprequest(struct halo *halo, XMapRequestEvent *e)
{

    XSelectInput(halo->display, e->window, StructureNotifyMask);
    XRaiseWindow(halo->display, e->window);
    XMoveResizeWindow(halo->display, e->window, 0, 0, halo->screenWidth, halo->screenHeight);
    XMapWindow(halo->display, e->window);

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

        case XK_Return:

            halo_menu.options[halo_menu.current].command();

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

