#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
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

static void halo_destroy(struct halo *halo)
{

    halo_menu_destroy(halo->menu);
    halo_client_list_destroy(halo->clients);
    halo_surface_destroy(halo);
    halo_window_destroy(halo->backend);
    halo_display_destroy(halo->backend);

}

static void halo_signal_term(int sig)
{

    halo_destroy(&halo);

}

static void halo_signal_usr1(int sig)
{

    struct halo_menu *new = halo_menu_init(halo.backend->width, halo.backend->height);

    if (new)
    {

        pthread_mutex_lock(&halo.mutexMenu);

        if (halo.menu)
            halo_menu_destroy(halo.menu);

        halo.menu = new;

        pthread_mutex_unlock(&halo.mutexMenu);

    }

}

static void halo_init(struct halo *halo)
{

    int status;
    struct stat info;

    halo->pathHome = getenv("HOME");

    sprintf(halo->pathConfig, "%s/.halo", halo->pathHome);

    if (stat(halo->pathConfig, &info) == -1)
    {

        char copyCmd[128];

        sprintf(copyCmd, "/bin/cp -r /usr/share/halo %s", halo->pathConfig);
        
        system(copyCmd);

    }

    sprintf(halo->pathHead, "%s/head", halo->pathConfig);
    sprintf(halo->pathPid, "%s/pid", halo->pathConfig);
    sprintf(halo->pathNotify, "%s/notify", halo->pathConfig);
    signal(SIGTERM, halo_signal_term);
    signal(SIGUSR1, halo_signal_usr1);

    FILE *file = fopen(halo->pathPid, "w");
    
    fprintf(file, "%d", getpid());
    fclose(file);
    sync();

    halo->backend = halo_display_create();

    halo_window_init(halo->backend);
    halo_surface_init(halo);
    
    halo->clients = halo_client_list_create();

    char pathInit[128];

    sprintf(pathInit, "%s/init", halo->pathConfig);
    halo_execute(pathInit, 0);
    wait(&status);

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
    pthread_create(&halo->threadRender, 0, halo_thread_render, halo);
    pthread_create(&halo->threadEvents, 0, halo_thread_events, halo);
    pthread_join(halo->threadEvents, 0);
    pthread_join(halo->threadRender, 0);
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

