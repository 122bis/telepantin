#ifndef wheel
    #define wheel

    // prototypes
    void wheel_init(int, int);
    void wheel_draw(cairo_t *);
    void wheel_update(float);

    void sideview_init(void);
    void sideview_update(float);
    void sideview_draw(cairo_t * );

    void inbody_draw(cairo_t * );

#endif
