#ifndef HALO_WINDOW_H
#define HALO_WINDOW_H

struct halo_xlib_backend;

extern void halo_window_init(struct halo_xlib_backend *backend);
extern void halo_window_destroy(struct halo_xlib_backend *backend);

#endif

