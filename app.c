// -----------------------------------------------------------------------------
// app.c  
// project : Telepantin VERSION_008 
// -----------------------------------------------------------------------------


#include <gtk/gtk.h>
#include <unistd.h> // usleep() in idle loop

#include "telepantin.h"

#define FPS 24 
#define PNGEXPORT 0       // 0 or 1
#define FULLSCREEN 1      // 0 or 1
#define MICROSEC 1000000  // 1 million

static const int TICK = (int)(MICROSEC / FPS); // wait duration (microseconds) in idle loop usleep()
 
static int SCREEN_WIDTH = 1680;
static int SCREEN_HEIGHT = 1050;  // not mac resolution WHY ??????
 
static unsigned long TT=0;                     // total elapsed time (microseconds)
static float tt=0;                             // total elapsed time (seconds)
static GtkWidget * C;                          // the drawing widget
static GtkWidget * W;                          // the window
int is_paused = 1;

// ... INTERACTION .............................................................

static gboolean on_key_pressed(GtkWidget * drawing_area, guint keyval, guint keycode, GdkModifierType state, GtkEventControllerKey * event_controller)
{
    if (keyval == ' ') is_paused *= -1 ;
    else if (keyval == 'q') gtk_window_destroy (GTK_WINDOW (W));
    return TRUE;
}


// ... DRAW  ...................................................................

static void on_draw (GtkDrawingArea * area, cairo_t * cr, int width, int height, gpointer user_data)
{
    telepantin_draw(cr, width,height); // .................. TELEPANTIN DRAW ...
    if (PNGEXPORT) telepantin_export(cr, width, height);
}


// ... UPDATE ..................................................................

gboolean on_idle_loop(GtkWidget * widget)
{
    usleep(TICK); //wait (time in microseconds)
    if (is_paused > 0) TT += TICK;
    tt = (float)TT / MICROSEC;  // .......... pass time converted to seconds ...
    telepantin_update(tt);      // ........................TELEPANTIN UPDATE ... 
    gtk_widget_queue_draw(C); // Request to redraw
    printf("... \t t(sec) : %4f", tt);
    return TRUE;
}


static void on_activate (GApplication * app)
{
    gtk_window_present (GTK_WINDOW (W));
}


static void on_shutdown (GApplication * application)
{
    g_application_quit(G_APPLICATION(application));
}


static void on_startup (GApplication * app, gpointer user_data)
{
    W = gtk_application_window_new (GTK_APPLICATION (app));
    gtk_window_set_default_size(GTK_WINDOW(W), SCREEN_WIDTH, SCREEN_HEIGHT);
    if (FULLSCREEN){
        gtk_window_fullscreen (GTK_WINDOW(W));
    }
    gtk_window_set_resizable (GTK_WINDOW(W), FALSE);
    gtk_window_set_title (GTK_WINDOW (W), "mimi's telepantin dance");
    
    C = gtk_drawing_area_new ();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (C), on_draw, NULL, NULL);
    gtk_window_set_child (GTK_WINDOW (W), C);
    
    GtkEventController * event_controller = gtk_event_controller_key_new ();
    gtk_widget_add_controller (GTK_WIDGET (W), event_controller);
    g_signal_connect_object (event_controller, "key-pressed", G_CALLBACK (on_key_pressed), C, G_CONNECT_SWAPPED);
    g_idle_add((GSourceFunc) on_idle_loop, (gpointer) C);
}


#define APPLICATION_ID "cairo.app"

int
main (int argc, char ** argv) {

// ... TELEPANTIN INIT .........................................................

    telepantin_init(SCREEN_WIDTH, SCREEN_HEIGHT); // ....... TELEPANTIN INIT ...

// ... GTK INIT ................................................................

    GtkApplication * app;
    if (! FULLSCREEN){
        SCREEN_WIDTH  = 960;
        SCREEN_HEIGHT = 600;
    }
    app = gtk_application_new (APPLICATION_ID, G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "startup", G_CALLBACK (on_startup), NULL);
    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    g_signal_connect (app, "shutdown", G_CALLBACK (on_shutdown), NULL);

    int stat;
    stat = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return stat;
}
