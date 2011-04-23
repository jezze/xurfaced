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

static void halo_event_destroywindow(struct halo *halo, XDestroyWindowEvent *event)
{

    struct halo_client *client = halo_client_find(halo, event->window);

    if (!client)
        return;

    halo_client_remove(halo, client);

    halo->clientCurrent = halo->clients;

}

static void halo_event_expose(struct halo *halo, XExposeEvent *event)
{

    if (event->count)
        return;

}

static void halo_event_unmap(struct halo *halo, XUnmapEvent *event)
{

}

static void halo_event_maprequest(struct halo *halo, XMapRequestEvent *event)
{

    struct halo_client *client = halo_client_add(halo, event->window);

    if (!client)
        return;

    XSelectInput(halo->display, client->window, StructureNotifyMask | PropertyChangeMask);
    XRaiseWindow(halo->display, client->window);
    XMoveResizeWindow(halo->display, client->window, 0, 0, halo->screenWidth, halo->screenHeight);
    XMapWindow(halo->display, client->window);
    XSetInputFocus(halo->display, client->window, RevertToParent, CurrentTime);

    halo->clientCurrent = client;

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

                if (halo->clientCurrent)
                {

                    halo->clientCurrent = (halo->clientCurrent->next) ? halo->clientCurrent->next : halo->clients;

                    XRaiseWindow(halo->display, halo->clientCurrent->window);
                    XSetInputFocus(halo->display, halo->clientCurrent->window, RevertToParent, CurrentTime);

                }

            }

            break;

        case XK_Escape:

            XRaiseWindow(halo->display, halo->main);
            XSetInputFocus(halo->display, halo->main, RevertToParent, CurrentTime);

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

        case UnmapNotify:

            halo_event_unmap(halo, &event.xunmap);

            break;

        case DestroyNotify:

            halo_event_destroywindow(halo, &event.xdestroywindow);

            break;

    }

}

