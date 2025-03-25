#ifndef GEOM
    #define GEOM
    
    #define PI 3.1415927
    #define TWO_PI 6.2831854
    #define RAD2DEG 360 / TWO_PI 
    #define DEG2RAD TWO_PI / 360


    // ... POINT ...............................................................

    struct Point {
        float x;
        float y;
    };
    
    #define Vec Point
    #define Rec Point

    struct Point ORIGIN;
    
    // ... FUNCTIONS ...

    struct Point point_rotate(struct Point * p, struct Point * center, float alpha);
  
    // ... QUAD ................................................................

    struct Quad {
        struct Point pt[4];
    };

    // .... FUNCTIONS ...
    
    struct Quad quad_deep_copy(struct Quad * q);
    struct Quad quad_rotate(struct Quad * q, struct Point * center, float alpha );
    

    // ... COLORS  .............................................................

    struct Color {
        float r; //   0 <=  red    <= 1 
        float g; //   0 <=  green  <= 1 
        float b; //   0 <=  blue   <= 1 
        float a; //   0 <=  alpha  <= 1     
    };

    struct Color WHITE;
    struct Color GREY;
    struct Color PAPER;
    struct Color DARK;

    struct Color KRED;      // red telepantin
    struct Color KBLUE;     // blue telepantin
    struct Color KGREEN;    // green telepantin
    struct Color KYELLOW;   // yellow telepantin

    // ... FUNCTIONS ...

    struct Color color_deep_copy(struct Color * q);


    // ... CAIRO ...............................................................
    
    // ... DRAWING FUNCTIONS ...

    #include <gtk/gtk.h>
    void  dot_draw(cairo_t * cr, struct Color * k, struct Point * p);
    void spot_draw(cairo_t * cr, struct Color * k, struct Point * p);
    void  rec_draw(cairo_t * cr, struct Color * k, struct Point * c, int w, int h);
    void quad_draw(cairo_t * cr, struct Color * k, struct  Quad * q);

#endif  

// NOTES:
// - all functions now use pointers
