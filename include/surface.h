#ifndef HALO_SURFACE_H
#define HALO_SURFACE_H

struct halo;

extern void halo_surface_blit(struct halo *halo);
extern void halo_surface_destroy(struct halo *halo);
extern void halo_surface_init(struct halo *halo);

#endif

