#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrender.h>
#include <animation.h>
#include <client.h>
#include <menu.h>
#include <halo.h>
#include <event.h>

static void halo_event_configurerequest(struct halo *halo, XConfigureRequestEvent *event)
{

    XWindowChanges wc;
    wc.x = 0;
    wc.y = 0;
    wc.width = halo->backend->screenWidth;
    wc.height = halo->backend->screenHeight;
    wc.border_width = 0;
    wc.sibling = event->above;
    wc.stack_mode = event->detail;

    XConfigureWindow(halo->backend->display, event->window, event->value_mask, &wc);

    XConfigureEvent ce;
    ce.type = ConfigureNotify;
    ce.display = halo->backend->display;
    ce.event = event->window;
    ce.window = event->window;
    ce.x = 0;
    ce.y = 0;
    ce.width = halo->backend->screenWidth;
    ce.height = halo->backend->screenHeight;
    ce.border_width = 0;
    ce.above = 0;
    ce.override_redirect = 0;

    XSendEvent(halo->backend->display, event->window, 0, StructureNotifyMask, (XEvent *)&ce);

}

static void halo_event_destroywindow(struct halo *halo, XDestroyWindowEvent *event)
{

    struct halo_client *client = halo_client_list_find(halo->clients, event->window);

    if (!client)
    {

        XSync(halo->backend->display, 0);

        return;

    }

    halo_client_list_remove(halo->clients, client);
    halo_client_destroy(client);

    halo->clients->current = halo->clients->head;

    halo->paused = 0;

    XRaiseWindow(halo->backend->display, halo->backend->main);
    XSetInputFocus(halo->backend->display, halo->backend->main, RevertToParent, CurrentTime);
    XSync(halo->backend->display, 0);

}

static void halo_event_expose(struct halo *halo, XExposeEvent *event)
{

    if (event->count)
        return;

}

static void halo_event_maprequest(struct halo *halo, XMapRequestEvent *event)
{

    struct halo_client *client = halo_client_create(event->window);

    if (!client)
        return;

    halo_client_list_add(halo->clients, client);
    halo->clients->current = client;

    XSelectInput(halo->backend->display, client->window, StructureNotifyMask | PropertyChangeMask);
    XRaiseWindow(halo->backend->display, client->window);
    XMoveResizeWindow(halo->backend->display, client->window, 0, 0, halo->backend->screenWidth, halo->backend->screenHeight);
    XMapWindow(halo->backend->display, client->window);
    XSetInputFocus(halo->backend->display, client->window, RevertToParent, CurrentTime);
    XSync(halo->backend->display, 0);

    XWindowAttributes wa;
    XGetWindowAttributes(halo->backend->display, client->window, &wa);

    XRenderPictFormat *format = XRenderFindVisualFormat(halo->backend->display, wa.visual);

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    client->picture = XRenderCreatePicture(halo->backend->display, client->window, format, CPSubwindowMode, &pa);

    halo->paused = 1;

}

static void halo_event_unmap(struct halo *halo, XUnmapEvent *event)
{

}

static void halo_event_buttonpress(struct halo *halo, XButtonEvent *event)
{

    switch (event->button)
    {

        case 1:

            halo_menu_activate(halo->menu);

            break;

        case 4:

            halo_menu_previous(halo->menu, 1);

            break;

        case 5:

            halo_menu_next(halo->menu, 1);

            break;

    }

    XSync(halo->backend->display, 0);

}

static void halo_event_keypress(struct halo *halo, XKeyPressedEvent *event)
{

    KeySym key = XLookupKeysym(event, 0);

    switch (key)
    {

        case XK_c:

            if (!(event->state & Mod1Mask))
                break;

            if (!halo->clients->current)
                break;

    		XKillClient(halo->backend->display, halo->clients->current->window);

            break;

        case XK_q:

            halo->running = 0;

            break;

        case XK_Up:

            halo_menu_previous(halo->menu, 1);

            break;

        case XK_Page_Up:

            halo_menu_previous(halo->menu, 8);

            break;

        case XK_Down:

            halo_menu_next(halo->menu, 1);

            break;

        case XK_Page_Down:

            halo_menu_next(halo->menu, 8);

            break;

        case XK_Tab:

            if (!(event->state & Mod1Mask))
                break;

            if (!halo->clients->current)
                break;

            halo->clients->current = halo->clients->current->next;

            XRaiseWindow(halo->backend->display, halo->clients->current->window);
            XSetInputFocus(halo->backend->display, halo->clients->current->window, RevertToParent, CurrentTime);

            break;

        case XK_Escape:

            if (!(event->state & Mod1Mask))
                break;

            halo->paused = 0;

            XRaiseWindow(halo->backend->display, halo->backend->main);
            XSetInputFocus(halo->backend->display, halo->backend->main, RevertToParent, CurrentTime);

            break;

        case XK_Return:

            halo_menu_activate(halo->menu);

            break;

    }

    XSync(halo->backend->display, 0);

}

void halo_event_handler(struct halo *halo)
{

    XEvent event;
    XNextEvent(halo->backend->display, &event);

    switch (event.type)
    {

        case ButtonPress:

            halo_event_buttonpress(halo, &event.xbutton);

            break;

        case Expose:

            halo_event_expose(halo, &event.xexpose);

            break;

        case KeyPress:

            halo_event_keypress(halo, &event.xkey);

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

