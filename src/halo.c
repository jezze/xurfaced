#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <animation.h>
#include <client.h>
#include <display.h>
#include <event.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>
#include <window.h>

struct halo halo;

static void halo_init(struct halo *halo)
{

    halo->pathHome = getenv("HOME");
    strcpy(halo->pathConfig, halo->pathHome);
    strcat(halo->pathConfig, "/.halo");
    strcpy(halo->pathInit, halo->pathConfig);
    strcat(halo->pathInit, "/init");
    strcpy(halo->pathTitle, halo->pathInit);
    strcat(halo->pathTitle, "/title");
    strcpy(halo->pathDesc, halo->pathInit);
    strcat(halo->pathDesc, "/desc");
    strcpy(halo->pathExec, halo->pathInit);
    strcat(halo->pathExec, "/exec");

    halo_display_init(halo);
    halo_window_init(halo);
    halo_surface_init(halo);
    halo->clients = halo_client_list_create();
    halo->menu = halo_menu_init(halo->screenWidth, halo->screenHeight);

}

static void halo_destroy(struct halo *halo)
{

    halo_menu_destroy(halo->menu);
    halo_client_list_destroy(halo->clients);
    halo_surface_destroy(halo);
    halo_window_destroy(halo);
    halo_display_destroy(halo);

}

static void *halo_thread_render(void *arg)
{

    struct halo *halo = (struct halo *)arg;

    struct timespec ts;
    struct timeval tv;

    while (halo->running)
    {

        pthread_mutex_lock(&halo->mutexMenu);
        halo_surface_prep(halo);
        halo_surface_blit(halo);
        pthread_mutex_unlock(&halo->mutexMenu);

        gettimeofday(&tv, 0);

        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
        ts.tv_nsec += 10 * 1000 * 1000;

        pthread_cond_timedwait(&halo->condRender, &halo->mutexRender, &ts);

    }

    return 0;

}

static void *halo_thread_events(void *arg)
{

    struct halo *halo = (struct halo *)arg;

    while (halo->running)
        halo_event_handler(halo);

    return 0;

}

static void halo_start(struct halo *halo)
{

    halo->running = 1;
    halo->paused = 0;

    pthread_mutex_init(&halo->mutexRender, 0);
    pthread_cond_init(&halo->condRender, 0);
    pthread_mutex_init(&halo->mutexMenu, 0);

    pthread_t threadRender;
    pthread_t threadEvents;

    pthread_create(&threadRender, 0, halo_thread_render, halo);
    pthread_create(&threadEvents, 0, halo_thread_events, halo);

    pthread_join(threadEvents, 0);
    pthread_join(threadRender, 0);

    pthread_mutex_destroy(&halo->mutexMenu);
    pthread_cond_destroy(&halo->condRender);
    pthread_mutex_destroy(&halo->mutexRender);

}

int main(int argc, char *argv[])
{

    halo_init(&halo);
    halo_start(&halo);
    halo_destroy(&halo);

    return 0;

}

