#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <animation.h>
#include <client.h>
#include <display.h>
#include <event.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>
#include <window.h>

struct halo halo;

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void halo_init(struct halo *halo)
{

    halo_display_init(halo);
    halo_window_init(halo);
    halo_surface_init(halo);
    halo->clients = halo_client_list_create();
    halo->menues = halo_menu_list_create(halo->screenWidth, halo->screenHeight);

}

static void halo_destroy(struct halo *halo)
{

    halo_menu_list_destroy(halo->menues);
    halo_client_list_destroy(halo->clients);
    halo_surface_destroy(halo);
    halo_display_destroy(halo);

}

static void *halo_thread_gui(void *p)
{

    struct halo *halo = (struct halo *)p;

    struct timespec ts;
    struct timeval tv;

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);

    while (halo->running)
    {

        halo_surface_prep(halo);
        halo_surface_blit(halo);

        gettimeofday(&tv, 0);

        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
        ts.tv_nsec += 10 * 1000 * 1000;

        pthread_cond_timedwait(&cond, &mutex, &ts);

    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;

}

static void *halo_thread_events(void *p)
{

    struct halo *halo = (struct halo *)p;

    while (halo->running)
        halo_event_handler(halo);

    return 0;

}

static void halo_run(struct halo *halo)
{

    halo->running = 1;
    halo->paused = 0;

    pthread_t threadGui;
    pthread_t threadEvents;

    pthread_create(&threadGui, 0, halo_thread_gui, halo);
    pthread_create(&threadEvents, 0, halo_thread_events, halo);

    pthread_join(threadGui, 0);
    pthread_join(threadEvents, 0);

}

int main(int argc, char *argv[])
{

    halo_init(&halo);
    halo_run(&halo);
    halo_destroy(&halo);

    return 0;

}

