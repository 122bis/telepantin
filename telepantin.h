#ifndef TELEPANTIN
    #define TELEPANTIN
    
    #include <gtk/gtk.h>
    void telepantin_init(int w, int h);
    void telepantin_update(float);
    void telepantin_draw(cairo_t *, int w, int h); 
    void telepantin_export(cairo_t *, int w, int h);
#endif
