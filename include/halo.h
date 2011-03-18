#ifndef HALO_HALO_H
#define HALO_HALO_H

struct halo
{

  Display *display;
  int screen;
  int screenWidth;
  int screenHeight;
  Window root;
  Window main;
  int running;

};

extern void halo_quit(struct halo *halo);
extern void halo_spawn(struct halo *halo);

#endif

