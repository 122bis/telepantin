// -----------------------------------------------------------------------------
// telepantin.c
// project : telepantin VERSION_008
// -----------------------------------------------------------------------------

#include <math.h>
#include "svg.h"
#include "wheel.h"
#include "telepantin.h"
#include "geom.h"

//#define BACKGROUND_COLOR PAPER
#define BACKGROUND_COLOR DARK

static int FR = 0;                             // frame number
static int PNG_WIDTH = 2560;
static int PNG_HEIGHT = 1600;
static struct Color BG;

static struct Point wheel_center = {.x = -350, .y = 0};
static struct Point wheel_scale  = {.x = 0.125, .y = 0.125};

static struct Point sideview_center = {.x = 600, .y = 0};
static struct Point sideview_scale  = {.x = 0.125,  .y = 0.125};

static struct Point inbody_center = {.x = 170,        .y = 0};
static struct Point inbody_scale  = {.x = 0.125* 1.7,  .y = 0.125};

static struct Point svg_center = {.x =   100, .y = -400};
static struct Point svg_scale  = {.x = 1.5,  .y = 1.5};


// FIGURES FUNCTIONS -----------------------------------------------------------
void telepantin_init(int width, int height)
{
    system("mkdir png");
    system("rm ./png/*.png");
    BG = color_deep_copy(& BACKGROUND_COLOR);
    wheel_init(width, height); 
    sideview_init();
    // TODO svg_init(width, height);  // set the size of silouhette box 
}

void telepantin_update(float elapsed_seconds)
{
    wheel_update(elapsed_seconds);
    sideview_update(elapsed_seconds);
}

void telepantin_draw(cairo_t * cr, int width, int height) 
{
    //cairo_paint(cr);
    cairo_set_source_rgba(cr, BG.r, BG.g, BG.b, BG.a);
    cairo_paint(cr);
    cairo_translate(cr, width/2, height/2);  // move to center

    cairo_save(cr);
    cairo_translate(cr, wheel_center.x, wheel_center.y); // position of the figure
    cairo_scale(cr, wheel_scale.x, wheel_scale.y); 
    wheel_draw(cr);
    cairo_restore(cr);

    cairo_save(cr);
    cairo_translate(cr, sideview_center.x, sideview_center.y);
    cairo_scale(cr, sideview_scale.x, sideview_scale.y);
    sideview_draw(cr);
    cairo_restore(cr);

    cairo_save(cr);
    cairo_translate(cr, inbody_center.x, inbody_center.y);
    cairo_scale(cr, inbody_scale.x, inbody_scale.y);
    inbody_draw(cr);
    cairo_restore(cr);
   
    cairo_save(cr);
    cairo_translate(cr, svg_center.x, svg_center.y);
    cairo_scale(cr, svg_scale.x, svg_scale.y);
    cairo_set_source_rgba(cr, BG.r, BG.g, BG.b, BG.a);
    svg_draw(cr);
    cairo_fill(cr);
    cairo_restore(cr);
}

void telepantin_export(cairo_t * cr, int width, int height)
{
    FR ++;
    char fr_filename[20];
    sprintf(fr_filename, "./png/fr_%04d.png", FR);

    cairo_surface_t * T;
    cairo_t * crxp;

    T = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, PNG_WIDTH, PNG_HEIGHT);
    crxp = cairo_create(T);
    cairo_translate(crxp, PNG_WIDTH/2, PNG_HEIGHT/2); // move origin to center of the image
    // PNG export
    telepantin_draw(crxp, width, height);                            // ... DRAW ALL ...
    cairo_surface_write_to_png(T, fr_filename);

    cairo_destroy(crxp);
    cairo_surface_destroy(T);
}

