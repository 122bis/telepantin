// -----------------------------------------------------------------------------
// wheel.c
// project : telepantin VERSION_008
// -----------------------------------------------------------------------------

#include "geom.h"
#include "wheel.h"

// constants
#define RPM 15    // rotation speed
#define ANTIALIAS 100 // multiplier of kapla mm size for better antialias

// FIGURE wheel PRIVATE DATA ---------------------------------------------------
// Always use static in .c files unless you need to reference the object from a 
// different .c module.
//
// Never use static in .h files, because you will create a different object every
// time it is included.
static float alpha;    // angle of wheel
static float omega;    // angular velocity in radian
static int   len;      // dimension of kapla blocks
static int   wth;
static int   thk;
static struct Quad q4[4];           // 4 recs rotate for display 
static struct Quad q4_ref[4];       // 4 keep initial recs to avoid cumulated angle errors
static struct Color kol[4];         // 4 colors for the recs


static struct Quad  in_body[4];   // live 4 quads for in-bodyy colors areas
static struct Quad  corners[4]; // live local copy of wheel's quads used for their 16 vertices 

struct Draw_order  {
    float xpos; //   quad's max x coordinate 
    int    qid; //   quad identified by its number in the quad array 
};

static struct Draw_order inbdo[4]; //use x coordinates to sort in_body draw_order;


// FIGS FUNCTIONS --------------------------------------------------------------
void wheel_init(int width, int height)
{
    len = 33 * ANTIALIAS;
    wth = 11 * ANTIALIAS;  
    thk =  6 * ANTIALIAS;
    alpha = 0;
    omega = TWO_PI * RPM / 60;  // angular velocity in radians per sec: 1 RPM = 1/60 RPS
    // positions of four static + 1 sliding horizontal rects
    q4_ref[0] = (struct Quad) { 
                    (struct Point) {-thk/2,   +thk/2 },
                    (struct Point) {-thk/2,   +thk/2 + len },
                    (struct Point) { thk/2,   +thk/2 + len },
                    (struct Point) { thk/2,   +thk/2 }
    };
    q4_ref[1] = quad_rotate( & q4_ref[0], & ORIGIN,     TWO_PI / 4);
    q4_ref[2] = quad_rotate( & q4_ref[0], & ORIGIN,     TWO_PI / 2);
    q4_ref[3] = quad_rotate( & q4_ref[0], & ORIGIN, 3 * TWO_PI / 4);
    for (int i=0; i<4; i++){
        q4[i] = quad_deep_copy(& q4_ref[i]);
    }
    kol[0]= KBLUE;
    kol[1]= KRED;
    kol[2]= KYELLOW;
    kol[3]= KGREEN;
}

void wheel_draw(cairo_t * cr)
{
     for (int i=0; i<4; i++){
        quad_draw(cr, & kol[i], & q4[i]);
    }   
    dot_draw(cr, & KRED, & ORIGIN);
}


void wheel_update(float elapsed_seconds)
{
    // wheel is rotating, angle stays within [0,360°]
    // % operator returns an int from the remaining of a division
    // fmod is real modulo for floats
    alpha = fmod(elapsed_seconds * omega, TWO_PI);
    printf("\t alpha : %f\n", alpha * RAD2DEG);
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            q4[i].pt[j] = point_rotate( & q4_ref[i].pt[j], & ORIGIN, alpha);
        }
    }
    // sync corners with updated quads
    for (int i=0; i<4; i++){
        corners[i] = quad_deep_copy(& q4[i]);
    }       

}


// HELPER FUNCTIONS ------------------------------------------------------------
// Comparison function for qsort() sorting qcads 
int compare_xpos(const void* a, const void* b)
{
    return (((struct Draw_order * )b)->xpos
         - ((struct Draw_order * )a)->xpos);
}


// SIDEVIEW FUNCTIONS ----------------------------------------------------------

void sideview_init(void)
{
    for (int i=0; i<4; i++){
        corners[i] = quad_deep_copy(& q4[i]);
    }       
}


void sideview_update(float elapsed_seconds)
{
    for (int i=0; i<4; i++){
        // first find the vertical (y axis) exrents of each quad
        int y_max, y_min;
        y_min = corners[i].pt[0].y;
        y_max = corners[i].pt[0].y;
        for (int p = 1; p < 4; p++) {
            if (corners[i].pt[p].y < y_min) y_min = corners[i].pt[p].y;
            if (corners[i].pt[p].y > y_max) y_max = corners[i].pt[p].y;
        }
        // same for the horizontal (x axis) extents
        int x_max, x_min;
        x_min = corners[i].pt[0].x;
        x_max = corners[i].pt[0].x;
        for (int p = 1; p < 4; p++) {
            if (corners[i].pt[p].x < x_min) x_min = corners[i].pt[p].x;
            if (corners[i].pt[p].x > x_max) x_max = corners[i].pt[p].x;
        }
        // data for draw order  
        inbdo[i].xpos = x_max;
        inbdo[i].qid = i;
        // updated body quad
        in_body[i].pt[0].x = 0; 
        in_body[i].pt[1].x = 0;
        in_body[i].pt[2].x = wth; 
        in_body[i].pt[3].x = wth;
        in_body[i].pt[0].y = y_min;
        in_body[i].pt[1].y = y_max;
        in_body[i].pt[2].y = y_max;
        in_body[i].pt[3].y = y_min;
    }
    // then sort in_body draw order leftmost/last drawn (= on top) to rightmost/first drawn 
    qsort(inbdo, 4, sizeof(struct Draw_order), compare_xpos);
}


/*
void corners_draw(cairo_t * cr)  // dot all corners for information
{
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            spot_draw(cr, & kol[i], & corners[i].pt[j]);
        }
    }
*/


void sideview_draw(cairo_t * cr)
{
    for (int i=0; i<4; i++){
        quad_draw(cr, & kol[inbdo[i].qid], & in_body[inbdo[i].qid]);
    }
}


void inbody_draw(cairo_t * cr)   // in body rectangles
{
    for (int i=0; i<4; i++){
        //cairo_set_operator (cr, CAIRO_OPERATOR_OVERLAY);
        //cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
        cairo_set_operator (cr, CAIRO_OPERATOR_ADD);
        quad_draw(cr, & kol[inbdo[i].qid], & in_body[inbdo[i].qid]);
    }
}

// NOTES
// - using qsort (for in_body quads x max coordinate) for display order
// NOTES :
// - changed funcs arguments : struct --> struct pointers
// - changed Quads to struct of array of points
