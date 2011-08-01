#ifndef XURFACED_GRAPHIC_H
#define XURFACED_GRAPHIC_H

#define XURFACED_GRAPHIC_MENU_BEFORE 12
#define XURFACED_GRAPHIC_MENU_AFTER 12

extern void xurfaced_graphic_prep(struct xurfaced_display_backend *backend, struct xurfaced_menu *menu);
extern void xurfaced_graphic_blit(struct xurfaced_display_backend *backend);
extern void xurfaced_graphic_destroy(struct xurfaced_display_backend *backend);
extern void xurfaced_graphic_init(struct xurfaced_display_backend *backend);

#endif

