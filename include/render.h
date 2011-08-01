#ifndef XURFACED_RENDER_H
#define XURFACED_RENDER_H

#define XURFACED_RENDER_MENU_BEFORE 12
#define XURFACED_RENDER_MENU_AFTER 12

extern void xurfaced_render_prep(struct xurfaced_display_backend *backend, struct xurfaced_menu *menu);
extern void xurfaced_render_blit(struct xurfaced_display_backend *backend);
extern void xurfaced_render_destroy(struct xurfaced_display_backend *backend);
extern void xurfaced_render_init(struct xurfaced_display_backend *backend);

#endif

