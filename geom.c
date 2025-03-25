// -----------------------------------------------------------------------------
// geom.h : REV_a 
// project : Telepantin VERSION_008 
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>    // pi
#include <string.h>  // strcat()
#include <stdlib.h>  // exit()
#include <gtk/gtk.h> //cairo drawing

#include "geom.h" 

// ... POINTS .................................................................. 

struct Point ORIGIN = {.x = 0,     .y = 0};

//  ... point rotate ...

struct Point point_rotate(struct Point * p, struct Point * center, float alpha)
{
    // First subtract the pivot point, then rotate, then add the point again.
    struct Point outpt; 
    // translate point back to origin:
    outpt.x = p->x - center->x;
    outpt.y = p->y - center->y;
    // rotate point
    float s = sin(alpha);
    float c = cos(alpha);
    float x = outpt.x * c - outpt.y * s;
    float y = outpt.x * s + outpt.y * c;
    // translate point back:
    outpt.x = x + center->x;
    outpt.y = y + center->y;
    return outpt;
}

// ... points distance ...


// ... QUADS ...................................................................

//  ... quad deep copy ...

struct Quad quad_deep_copy(struct Quad * q)
{ 
    return((struct Quad){
        .pt[0] = (struct Point) { q->pt[0].x, q->pt[0].y},
        .pt[1] = (struct Point) { q->pt[1].x, q->pt[1].y},
        .pt[2] = (struct Point) { q->pt[2].x, q->pt[2].y},
        .pt[3] = (struct Point) { q->pt[3].x, q->pt[3].y}});
}

//  ... quad copy/rotate ...

struct Quad quad_rotate(struct Quad * q, struct Point * center, float alpha)
{ 
    return((struct Quad){
        .pt[0] = point_rotate(& q->pt[0], center, alpha),
        .pt[1] = point_rotate(& q->pt[1], center, alpha),
        .pt[2] = point_rotate(& q->pt[2], center, alpha),
        .pt[3] = point_rotate(& q->pt[3], center, alpha)});
}


// ... COLORS .................................................................. 


struct Color WHITE =   {255/255.0, 255/255.0, 255/255.0, 1};
struct Color GREY =    {207/255.0, 197/255.0, 182/255.0, 1};
struct Color PAPER =   {254/255.0, 247/255.0, 225/255.0, 1};
struct Color DARK  =   { 34/255.0,  37/255.0,  41/255.0, 1};

struct Color KRED =    {255/255.0,  69/255.0,   0/255.0, 1};  // red telepantin
struct Color KBLUE =   {  0/255.0,   0/255.0, 140/255.0, 1};      // blue telepantin
struct Color KGREEN =  {  0/255.0,  99/255.0,   0/255.0, 1};      // green telepantin
struct Color KYELLOW = {255/255.0, 214/255.0,   0/255.0, 1};  // yellow telepantin

//  ... color deep copy ...

struct Color color_deep_copy(struct Color * c)
{ 
    return((struct Color){ c->r, c->g, c->b, c->a});
}


// ... CAIRO : drawing to canvas ...............................................

#define PSIZE 10 // point size, points are drawn as '+'
#define SCALE 1 // eps file scale;  20 would scale drawing to 1/20th
#define PT_STROKE_WIDTH 0.5 // stroke width 

//  ... draw DOT as a cross ...

void dot_draw(cairo_t * cr, struct Color * k, struct Point * p)
{
    cairo_set_source_rgba(cr, k->r, k->g, k->b, k->a);
    cairo_move_to(cr,  p->x - PSIZE, p->y);
    cairo_line_to(cr,  p->x + PSIZE, p->y);
    cairo_move_to(cr, p->x, p->y - PSIZE);
    cairo_line_to(cr, p->x, p->y + PSIZE);
    cairo_set_line_width(cr, PT_STROKE_WIDTH);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_stroke (cr);
}

//  ... draw DOT as a small disk ...

void spot_draw(cairo_t * cr, struct Color * k, struct Point * p)
{
    cairo_set_source_rgba(cr, k->r, k->g, k->b, k->a);
    cairo_arc(cr, p->x, p->y, PSIZE/2, 0, TWO_PI);
    cairo_fill (cr);
}

//  ... draw a simple rectangle ...

void rec_draw(cairo_t * cr, struct Color * k, struct Point * c, int w, int h)
{
    // rectangle(context, color, center position, width, height)
    cairo_set_source_rgba(cr,  k->r, k->g, k->b, k->a);
    // cairo_set_line_width(cr, 5);
    // cairo_rectangle(context, top, left, width, height)
    cairo_rectangle(cr, c->x - w * 0.5, c->y - h * 0.5, w, h);
    cairo_fill (cr);
}

//  ...draws a quadrangular shape ...

void quad_draw(cairo_t * cr, struct Color * k, struct Quad * q)
{
    // rectangle(context, color, center position, width, height)
    cairo_set_source_rgba(cr,  k->r, k->g, k->b, k->a);
    cairo_set_line_width(cr, 5);
    cairo_move_to(cr, q->pt[0].x, q->pt[0].y);
    cairo_line_to(cr, q->pt[1].x, q->pt[1].y);
    cairo_line_to(cr, q->pt[2].x, q->pt[2].y);
    cairo_line_to(cr, q->pt[3].x, q->pt[3].y);
    cairo_fill (cr);
}


// -----------------------------------------------------------------------------
// . HELPERS - useful math and 2D trigo functions
// -----------------------------------------------------------------------------

inline int cmp(float a, float b) { return (a > b) - (a < b); }
inline float sind(float x) { return sin(x * RAD2DEG); } // sin of x in degres .            TO BE CHECKED : * or / ?
inline float cosd(float x) { return cos(x * RAD2DEG); }
inline float tand(float x) { return tan(x * RAD2DEG); }
inline float arclength(float r, float a, float b) { return (fabs(r * RAD2DEG * (b - a)));}
inline float dist(struct Point a, struct Point b){ return (sqrt(pow(b.x-a.x,2)+pow(b.y-a.y,2)));}
inline float magn(struct Vec v){ return (sqrt(pow(v.x,2)+pow(v.y,2)));}
inline float cosv(struct Vec v) { return (v.x / magn(v));}
inline float sinv(struct Vec v) { return (v.y / magn(v));}
inline struct Vec vec(struct Point a, struct Point b) {return (struct Vec) {.x = b.x-a.x, .y = b.y-a.y};} // vec from 2 points

// NOTES
// - better funcs args : structs --> pointers to structs
