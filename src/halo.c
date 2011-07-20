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

static pthread_mutex_t mutexRender;
static pthread_cond_t condRender;

static void halo_init(struct halo *halo)
{

    halo_display_init(halo);
    halo_window_init(halo);
    halo_surface_init(halo);
    halo->clients = halo_client_list_create();
    halo->menu = halo_menu_init(halo->screenWidth, halo->screenHeight);
    pipe(halo->fd);

}

static void halo_destroy(struct halo *halo)
{

    halo_menu_destroy(halo->menu);
    halo_client_list_destroy(halo->clients);
    halo_surface_destroy(halo);
    halo_window_destroy(halo);
    halo_display_destroy(halo);

}

static void *halo_thread_read(void *arg)
{

    struct halo *halo = (struct halo *)arg;

    FILE *input = fdopen(halo->fd[0], "r");

    int ch = getc(input);

    while (halo->running && ch != EOF)
    {

        fputc(ch, stdout);
        ch = getc(input);

    }

    return 0;

}

static void *halo_thread_render(void *arg)
{

    struct halo *halo = (struct halo *)arg;

    struct timespec ts;
    struct timeval tv;

    while (halo->running)
    {

        halo_surface_prep(halo);
        halo_surface_blit(halo);

        gettimeofday(&tv, 0);

        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
        ts.tv_nsec += 10 * 1000 * 1000;

        pthread_cond_timedwait(&condRender, &mutexRender, &ts);

    }

    return 0;

}

static void *halo_thread_events(void *arg)
{

    struct halo *halo = (struct halo *)arg;

    while (halo->running)
        halo_event_handler(halo);

    FILE *output = fdopen(halo->fd[0], "w");
    fputc(EOF, output);

    return 0;

}

static void halo_start(struct halo *halo)
{

    halo->running = 1;
    halo->paused = 0;

    pthread_t threadRead;
    pthread_t threadRender;
    pthread_t threadEvents;

    pthread_mutex_init(&mutexRender, 0);
    pthread_cond_init(&condRender, 0);

    pthread_create(&threadRead, 0, halo_thread_read, halo);
    pthread_create(&threadRender, 0, halo_thread_render, halo);
    pthread_create(&threadEvents, 0, halo_thread_events, halo);

    pthread_join(threadEvents, 0);
    pthread_join(threadRender, 0);
    pthread_join(threadRead, 0);

    pthread_cond_destroy(&condRender);
    pthread_mutex_destroy(&mutexRender);


}

int main(int argc, char *argv[])
{

    halo_init(&halo);
    halo_start(&halo);
    halo_destroy(&halo);

    return 0;

}

