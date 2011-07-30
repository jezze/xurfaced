#ifndef XURFACED_SURFACE_H
#define XURFACED_SURFACE_H

#define XURFACED_SURFACE_MENU_BEFORE 12
#define XURFACED_SURFACE_MENU_AFTER 12

struct xurfaced;

extern void xurfaced_surface_prep(struct xurfaced *xurfaced);
extern void xurfaced_surface_blit(struct xurfaced *xurfaced);
extern void xurfaced_surface_destroy(struct xurfaced *xurfaced);
extern void xurfaced_surface_init(struct xurfaced *xurfaced);

#endif

