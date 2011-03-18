#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <halo.h>
#include <menu.h>
#include <timer.h>
#include <surface.h>

struct halo *halo_current;
extern struct halo_menu halo_menu;
struct itimerval t;

void halo_timer_add_handler(void *callback())
{

}

void halo_timer_callback(int signum)
{

    halo_surface_blit(halo_current, &halo_menu);

}

void halo_timer_start()
{


}

void halo_timer_init(struct halo *halo)
{

    halo_current = halo;

    signal(SIGALRM, halo_timer_callback);

    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 100000;
    t.it_value.tv_sec = 0;
    t.it_value.tv_usec = 100000;
    setitimer(ITIMER_REAL, &t, 0);

    halo_timer_callback(0);

}
