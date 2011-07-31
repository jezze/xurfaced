#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <animation.h>
#include <display.h>
#include <menu.h>
#include <surface.h>

static cairo_t *xurfaced_cairo;
static cairo_surface_t *xurfaced_surface;
static cairo_pattern_t *xurfaced_background_pattern;

static double xurfaced_convert_hex_to_decimal(unsigned int value)
{

    return (double)(value & 0xFF) / (double)0xFF;

}

static void xurfaced_surface_blit_background(int width, int height)
{

    cairo_rectangle(xurfaced_cairo, 0, 0, width, height);
    cairo_set_source(xurfaced_cairo, xurfaced_background_pattern);
    cairo_fill(xurfaced_cairo);

}

static void xurfaced_surface_blit_notification(int width, int height)
{

    cairo_font_extents_t fe;

    float boxheight = height / 24;

    cairo_rectangle(xurfaced_cairo, 0, 0, width, boxheight);
    cairo_set_source_rgba(xurfaced_cairo, 0.0, 0.0, 0.0, 0.4);
    cairo_fill(xurfaced_cairo);
    cairo_select_font_face(xurfaced_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(xurfaced_cairo, 18.0);
    cairo_font_extents(xurfaced_cairo, &fe);
    cairo_move_to(xurfaced_cairo, 24.0, boxheight / 2.0 + fe.height / 2.0 - fe.descent);
    cairo_text_path(xurfaced_cairo, "xurfaced");
    cairo_set_source_rgba(xurfaced_cairo, 1.0, 1.0, 1.0, 1.0);
    cairo_fill(xurfaced_cairo);
    cairo_set_line_width(xurfaced_cairo, 2.0);
    cairo_set_source_rgba(xurfaced_cairo, 0.0, 0.0, 0.0, 0.7);
    cairo_stroke(xurfaced_cairo);

}

static void xurfaced_surface_blit_menu(unsigned int height, struct xurfaced_menu *menu)
{

    struct xurfaced_menu_option *current = menu->opts->current;

    float middle = height / 4 + height / 8;
    float offset = menu->animationProperties.translationY + current->animationProperties.translationY;
    float distance = fabs(middle - offset);

    if (distance > 5.0)
    {

        if (middle - offset < 0)
            menu->animationProperties.translationY -= (distance) / 5.0;
        else
            menu->animationProperties.translationY += (distance) / 5.0;

    }

    struct xurfaced_menu_option *start = menu->opts->current;
    struct xurfaced_menu_option *stop = menu->opts->current;

    unsigned int i;

    for (i = 0; i < 10; i++)
    {

        if (start != menu->opts->head)
            start = start->prev;

        if (stop != menu->opts->head->prev)
            stop = stop->next;

    }
    
    struct xurfaced_menu_option *option = start;

    do
    {

        if (option == current)
            option->animationProperties.alpha += 1.0;
        else
            option->animationProperties.alpha -= 0.05;

        if (option->animationProperties.alpha >= 1.0)
            option->animationProperties.alpha = 1.0;

        if (option->animationProperties.alpha <= 0.3)
            option->animationProperties.alpha = 0.3;

        cairo_select_font_face(xurfaced_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(xurfaced_cairo, 32.0);
        cairo_move_to(xurfaced_cairo, menu->animationProperties.translationX, menu->animationProperties.translationY + option->animationProperties.translationY);
        cairo_text_path(xurfaced_cairo, option->name);
        cairo_set_source_rgba(xurfaced_cairo, 1.0, 1.0, 1.0, option->animationProperties.alpha);
        cairo_fill(xurfaced_cairo);
        cairo_set_line_width(xurfaced_cairo, 4.0);
        cairo_set_source_rgba(xurfaced_cairo, 0.0, 0.0, 0.0, option->animationProperties.alpha - 0.3);
        cairo_stroke(xurfaced_cairo);

        if (option == current)
        {

            cairo_select_font_face(xurfaced_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(xurfaced_cairo, 18.0);
            cairo_move_to(xurfaced_cairo, menu->animationProperties.translationX, menu->animationProperties.translationY + option->animationProperties.translationY + 24);
            cairo_text_path(xurfaced_cairo, option->description);
            cairo_set_source_rgba(xurfaced_cairo, 1.0, 1.0, 1.0, option->animationProperties.alpha);
            cairo_fill(xurfaced_cairo);
            cairo_set_line_width(xurfaced_cairo, 2.0);
            cairo_set_source_rgba(xurfaced_cairo, 0.0, 0.0, 0.0, option->animationProperties.alpha - 0.3);
            cairo_stroke(xurfaced_cairo);

        }

        option = option->next;

    }
    while (option != stop->next);

}

void xurfaced_surface_prep(struct xurfaced_display_backend *backend, struct xurfaced_menu *menu)
{

    cairo_push_group(xurfaced_cairo);
    xurfaced_surface_blit_background(backend->width, backend->height);
    xurfaced_surface_blit_menu(backend->height, menu);
    xurfaced_surface_blit_notification(backend->width, backend->height);
    cairo_pop_group_to_source(xurfaced_cairo);

}

void xurfaced_surface_blit(struct xurfaced_display_backend *backend)
{

    XLockDisplay(backend->display);
    cairo_paint(xurfaced_cairo);
    XUnlockDisplay(backend->display);

}

void xurfaced_surface_init(struct xurfaced_display_backend *backend)
{

    xurfaced_surface = cairo_xlib_surface_create(backend->display, backend->main, backend->visual, backend->width, backend->height);
    xurfaced_cairo = cairo_create(xurfaced_surface);

    cairo_set_line_cap(xurfaced_cairo, CAIRO_LINE_CAP_ROUND);

    xurfaced_background_pattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, backend->height);

    unsigned int top = 0x441155;
    unsigned int bottom = 0x112244;

    double r = xurfaced_convert_hex_to_decimal(top >> 16);
    double g = xurfaced_convert_hex_to_decimal(top >> 8);
    double b = xurfaced_convert_hex_to_decimal(top);

    double r2 = xurfaced_convert_hex_to_decimal(bottom >> 16);
    double g2 = xurfaced_convert_hex_to_decimal(bottom >> 8);
    double b2 = xurfaced_convert_hex_to_decimal(bottom);

    cairo_pattern_add_color_stop_rgb(xurfaced_background_pattern, 0.0, r, g, b);
    cairo_pattern_add_color_stop_rgb(xurfaced_background_pattern, 1.0, r2, g2, b2);

}

void xurfaced_surface_destroy(struct xurfaced_display_backend *backend)
{

    cairo_destroy(xurfaced_cairo);
    cairo_surface_destroy(xurfaced_surface);

}

