#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <animation.h>
#include <display.h>
#include <halo.h>
#include <menu.h>
#include <surface.h>

static cairo_t *halo_cairo;
static cairo_surface_t *halo_surface;
static cairo_pattern_t *halo_background_pattern;

static void halo_surface_blit_background(int width, int height)
{

    cairo_rectangle(halo_cairo, 0, 0, width, height);
    cairo_set_source(halo_cairo, halo_background_pattern);
    cairo_fill(halo_cairo);

}

static void halo_surface_blit_notification(int width, int height)
{

    cairo_font_extents_t fe;

    cairo_rectangle(halo_cairo, 0, 0, width, height / 16);
    cairo_set_source_rgba(halo_cairo, 0.0, 0.0, 0.0, 0.2);
    cairo_fill(halo_cairo);

    cairo_select_font_face(halo_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(halo_cairo, 18.0);
    cairo_font_extents(halo_cairo, &fe);
    cairo_move_to(halo_cairo, 20.0, height / 16 - fe.height - fe.descent);
    cairo_text_path(halo_cairo, "Halo");
    cairo_set_source_rgba(halo_cairo, 1.0, 1.0, 1.0, 1.0);
    cairo_fill(halo_cairo);

    cairo_set_line_width(halo_cairo, 2.0);
    cairo_set_source_rgba(halo_cairo, 0.0, 0.0, 0.0, 0.7);
    cairo_stroke(halo_cairo);

}

static void halo_surface_blit_menu(unsigned int height, struct halo_menu *menu)
{

    struct halo_menu_option *current = menu->opts->current;

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

    struct halo_menu_option *start = menu->opts->current;
    struct halo_menu_option *stop = menu->opts->current;

    unsigned int i;

    for (i = 0; i < 10; i++)
    {

        if (start != menu->opts->head)
            start = start->prev;

        if (stop != menu->opts->head->prev)
            stop = stop->next;

    }
    
    struct halo_menu_option *option = start;

    do
    {

        if (option == current)
            option->animationProperties.alpha += 1.0;
        else
            option->animationProperties.alpha -= 0.05;

        if (option->animationProperties.alpha >= 0.8)
            option->animationProperties.alpha = 0.8;

        if (option->animationProperties.alpha <= 0.3)
            option->animationProperties.alpha = 0.3;

        cairo_select_font_face(halo_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(halo_cairo, 32.0);
        cairo_move_to(halo_cairo, menu->animationProperties.translationX, menu->animationProperties.translationY + option->animationProperties.translationY);
        cairo_text_path(halo_cairo, option->name);
        cairo_set_source_rgba(halo_cairo, 1.0, 1.0, 1.0, option->animationProperties.alpha);
        cairo_fill(halo_cairo);

        cairo_set_line_width(halo_cairo, 4.0);
        cairo_set_source_rgba(halo_cairo, 0.0, 0.0, 0.0, option->animationProperties.alpha - 0.3);
        cairo_stroke(halo_cairo);

        if (option == current)
        {

            cairo_select_font_face(halo_cairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(halo_cairo, 18.0);
            cairo_move_to(halo_cairo, menu->animationProperties.translationX, menu->animationProperties.translationY + option->animationProperties.translationY + 24);
            cairo_text_path(halo_cairo, option->description);
            cairo_set_source_rgba(halo_cairo, 1.0, 1.0, 1.0, option->animationProperties.alpha);
            cairo_fill(halo_cairo);
            
            cairo_set_line_width(halo_cairo, 2.0);
            cairo_set_source_rgba(halo_cairo, 0.0, 0.0, 0.0, option->animationProperties.alpha - 0.3);
            cairo_stroke(halo_cairo);

        }

        option = option->next;

    }
    while (option != stop->next);

}

void halo_surface_prep(struct halo *halo)
{

    cairo_push_group(halo_cairo);

    halo_surface_blit_background(halo->backend->width, halo->backend->height);
    halo_surface_blit_menu(halo->backend->height, halo->menu);
    halo_surface_blit_notification(halo->backend->width, halo->backend->height);

    cairo_pop_group_to_source(halo_cairo);

}

void halo_surface_blit(struct halo *halo)
{

    XLockDisplay(halo->backend->display);

    cairo_paint(halo_cairo);

    XUnlockDisplay(halo->backend->display);

}

void halo_surface_init(struct halo *halo)
{

    halo_surface = cairo_xlib_surface_create(halo->backend->display, halo->backend->main, halo->backend->visual, halo->backend->width, halo->backend->height);

    halo_cairo = cairo_create(halo_surface);
    cairo_set_line_cap(halo_cairo, CAIRO_LINE_CAP_ROUND);

    halo_background_pattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, halo->backend->height);
    cairo_pattern_add_color_stop_rgba(halo_background_pattern, 0.0, 0.2, 0.0, 0.3, 1.0);
    cairo_pattern_add_color_stop_rgba(halo_background_pattern, 1.0, 0.4, 0.2, 0.4, 1.0);

}

void halo_surface_destroy(struct halo *halo)
{

    cairo_destroy(halo_cairo);
    cairo_surface_destroy(halo_surface);

}

