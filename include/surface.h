#ifndef XURFACED_SURFACE_H
#define XURFACED_SURFACE_H

#define XURFACED_SURFACE_MENU_BEFORE 12
#define XURFACED_SURFACE_MENU_AFTER 12

extern void xurfaced_surface_prep(struct xurfaced_display_backend *backend, struct xurfaced_menu *menu);
extern void xurfaced_surface_blit(struct xurfaced_display_backend *backend);
extern void xurfaced_surface_destroy(struct xurfaced_display_backend *backend);
extern void xurfaced_surface_init(struct xurfaced_display_backend *backend);

#endif

