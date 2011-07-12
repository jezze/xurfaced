#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <animation.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>

cairo_t *halo_cairo;
cairo_surface_t *halo_surface;
cairo_pattern_t *halo_background_pattern;

void halo_surface_blit_background(struct halo *halo)
{

    cairo_rectangle(halo_cairo, 0, 0, halo->screenWidth, halo->screenHeight);
    cairo_set_source(halo_cairo, halo_background_pattern);
    cairo_fill(halo_cairo);

}

void halo_surface_blit_menu(unsigned int height, struct halo_menu *menu)
{

    int i;

    float middle = height / 4 + height / 8;
    float offset = menu->animationProperties.translationY + menu->options[menu->current]->animationProperties.translationY;
    float distance = fabs(middle - offset);

    if (distance > 5.0)
    {

        if (middle - offset < 0)
            menu->animationProperties.translationY -= (distance) / 5.0;
        else
            menu->animationProperties.translationY += (distance) / 5.0;

    }

    for (i = 0; i < menu->count; i++)
    {

        if (i == menu->current)
            menu->options[i]->animationProperties.alpha += 1.0;
        else
            menu->options[i]->animationProperties.alpha -= 0.05;

        if (menu->options[i]->animationProperties.alpha >= 0.8)
            menu->options[i]->animationProperties.alpha = 0.8;

        if (menu->options[i]->animationProperties.alpha <= 0.3)
            menu->options[i]->animationProperties.alpha = 0.3;

        cairo_select_font_face(halo_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_move_to(halo_cairo, menu->animationProperties.translationX, menu->animationProperties.translationY + menu->options[i]->animationProperties.translationY);
        cairo_set_font_size(halo_cairo, 32.0);
        cairo_set_line_width(halo_cairo, 4.0);
        cairo_text_path(halo_cairo, menu->options[i]->name);
        cairo_set_source_rgba(halo_cairo, 1.0, 1.0, 1.0, menu->options[i]->animationProperties.alpha);
        cairo_fill(halo_cairo);
        cairo_set_source_rgba(halo_cairo, 0.0, 0.0, 0.0, menu->options[i]->animationProperties.alpha - 0.3);
        cairo_stroke(halo_cairo);

        if (i != menu->current)
            continue;

        cairo_select_font_face(halo_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_move_to(halo_cairo, menu->animationProperties.translationX, menu->animationProperties.translationY + menu->options[i]->animationProperties.translationY + 24);
        cairo_set_font_size(halo_cairo, 18.0);
        cairo_set_line_width(halo_cairo, 2.0);
        cairo_text_path(halo_cairo, menu->options[i]->description);
        cairo_set_source_rgba(halo_cairo, 1.0, 1.0, 1.0, menu->options[i]->animationProperties.alpha);
        cairo_fill(halo_cairo);
        cairo_set_source_rgba(halo_cairo, 0.0, 0.0, 0.0, menu->options[i]->animationProperties.alpha - 0.3);
        cairo_stroke(halo_cairo);

    }

}

void halo_surface_prep(struct halo *halo)
{

    cairo_push_group(halo_cairo);

    halo_surface_blit_background(halo);
    halo_surface_blit_menu(halo->screenHeight, halo->menues->current);

    cairo_pop_group_to_source(halo_cairo);

}

void halo_surface_blit(struct halo *halo)
{

    XLockDisplay(halo->display);

    cairo_paint(halo_cairo);

    XUnlockDisplay(halo->display);

}

void halo_surface_init(struct halo *halo)
{

    halo_surface = cairo_xlib_surface_create(halo->display, halo->main, XDefaultVisual(halo->display, 0), halo->screenWidth, halo->screenHeight);

    halo_cairo = cairo_create(halo_surface);
    cairo_set_line_cap(halo_cairo, CAIRO_LINE_CAP_ROUND);

    halo_background_pattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, halo->screenHeight);
    cairo_pattern_add_color_stop_rgba(halo_background_pattern, 0.0, 0.2, 0.0, 0.3, 1.0);
    cairo_pattern_add_color_stop_rgba(halo_background_pattern, 1.0, 0.4, 0.2, 0.4, 1.0);

}

void halo_surface_destroy(struct halo *halo)
{

    cairo_destroy(halo_cairo);
    cairo_surface_destroy(halo_surface);

}

