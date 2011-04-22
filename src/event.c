#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <client.h>
#include <menu.h>
#include <surface.h>
#include <halo.h>
#include <event.h>

static void halo_event_configurerequest(struct halo *halo, XConfigureRequestEvent *event)
{

    XWindowChanges wc;
    wc.x = 0;
    wc.y = 0;
    wc.width = halo->screenWidth;
    wc.height = halo->screenHeight;
    wc.border_width = 0;
    wc.sibling = event->above;
    wc.stack_mode = event->detail;

    XConfigureWindow(halo->display, event->window, event->value_mask, &wc);

    XConfigureEvent ce;
    ce.type = ConfigureNotify;
    ce.display = halo->display;
    ce.event = event->window;
    ce.window = event->window;
    ce.x = 0;
    ce.y = 0;
    ce.width = halo->screenWidth;
    ce.height = halo->screenHeight;
    ce.border_width = 0;
    ce.above = 0;
    ce.override_redirect = 0;

    XSendEvent(halo->display, event->window, 0, StructureNotifyMask, (XEvent *)&ce);

}

static void halo_event_expose(struct halo *halo, XExposeEvent *event)
{

}

static void halo_event_maprequest(struct halo *halo, XMapRequestEvent *event)
{

    XSelectInput(halo->display, event->window, StructureNotifyMask);
    XRaiseWindow(halo->display, event->window);
    XMoveResizeWindow(halo->display, event->window, 0, 0, halo->screenWidth, halo->screenHeight);
    XMapWindow(halo->display, event->window);

    halo_client_add(halo, &event->window);

}

static void halo_event_keypress(struct halo *halo, XKeyPressedEvent *event)
{

    KeySym key = XLookupKeysym(event, 0);

    switch (key)
    {

        case XK_q:

            halo_quit(halo);

            break;

        case XK_Up:

            halo_menu_previous(halo->menuCurrent);

            break;

        case XK_Down:

            halo_menu_next(halo->menuCurrent);

            break;

        case XK_Tab:

            if (event->state & Mod1Mask)
            {

                XRaiseWindow(halo->display, event->window);
                XSetInputFocus(halo->display, event->window, RevertToParent, CurrentTime);

            }

            break;

        case XK_Escape:

            XRaiseWindow(halo->display, halo->main);

            break;

        case XK_Return:

            halo_menu_activate(halo->menuCurrent);

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

