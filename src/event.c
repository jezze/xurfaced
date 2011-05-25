#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <animation.h>
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

    struct halo_client *client = halo_client_list_find(halo->clients, event->window);

    if (!client)
    {

        XSync(halo->display, 0);

        return;

    }

    halo_client_list_remove(halo->clients, client);
    halo_client_destroy(client);

    halo->clients->current = halo->clients->head;

    halo->paused = 0;

    XRaiseWindow(halo->display, halo->main);
    XSetInputFocus(halo->display, halo->main, RevertToParent, CurrentTime);
    XSync(halo->display, 0);

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

    XSelectInput(halo->display, client->window, StructureNotifyMask | PropertyChangeMask);
    XRaiseWindow(halo->display, client->window);
    XMoveResizeWindow(halo->display, client->window, 0, 0, halo->screenWidth, halo->screenHeight);
    XMapWindow(halo->display, client->window);
    XSetInputFocus(halo->display, client->window, RevertToParent, CurrentTime);
    XSync(halo->display, 0);

    XWindowAttributes wa;
    XGetWindowAttributes(halo->display, client->window, &wa);

    XRenderPictFormat *format = XRenderFindVisualFormat(halo->display, wa.visual);

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    client->picture = XRenderCreatePicture(halo->display, client->window, format, CPSubwindowMode, &pa);

    halo->paused = 1;

}

static void halo_event_unmap(struct halo *halo, XUnmapEvent *event)
{

}

static void halo_event_buttonpress(struct halo *halo, XButtonEvent *event)
{

    halo_menu_activate(halo->menues->current);

    XSync(halo->display, 0);

}

static void halo_event_motionnotify(struct halo *halo, XMotionEvent *event)
{

    unsigned int cx = halo->screenWidth / 2;
    unsigned int cy = halo->screenHeight / 2;

    if (event->y < cy)
        halo_menu_previous(halo->menues->current);
    else if (event->y > cy)
        halo_menu_next(halo->menues->current);

    XWarpPointer(halo->display, 0, event->window, 0, 0, 0, 0, cx, cy);

    XSync(halo->display, 0);

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

    		XKillClient(halo->display, halo->clients->current->window);

            break;

        case XK_q:

            halo->running = 0;

            break;

        case XK_Up:

            halo_menu_previous(halo->menues->current);

            break;

        case XK_Down:

            halo_menu_next(halo->menues->current);

            break;

        case XK_Tab:

            if (!(event->state & Mod1Mask))
                break;

            if (!halo->clients->current)
                break;

            halo->clients->current = halo->clients->current->next;

            XRaiseWindow(halo->display, halo->clients->current->window);
            XSetInputFocus(halo->display, halo->clients->current->window, RevertToParent, CurrentTime);

            break;

        case XK_Escape:

            if (!(event->state & Mod1Mask))
                break;

            halo->paused = 0;

            XRaiseWindow(halo->display, halo->main);
            XSetInputFocus(halo->display, halo->main, RevertToParent, CurrentTime);

            break;

        case XK_Return:

            halo_menu_activate(halo->menues->current);

            break;

    }

    XSync(halo->display, 0);

}

void halo_event_handler(struct halo *halo)
{

    XEvent event;
    XNextEvent(halo->display, &event);

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

        case MotionNotify:

            halo_event_motionnotify(halo, &event.xmotion);

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

