/**
 * @file layout.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

static GtkWidget *main_vbox;
static GtkWidget *computer_infos_label;
static GtkWidget *player_infos_label;

static GdkGC *penBlack = NULL;
static GdkGC *penForegroundBoard = NULL;

/**
 * TBD
 * 
 * @param widget TBD
 * @param data TBD
 * @return TBD
 */
static gint close_app( GtkWidget * widget, gpointer data ) {
    save_config(  );
    gtk_main_quit(  );
    return ( TRUE );
}                               // close_app

/**
 * Handle the button "key_press" event.
 * 
 * Function looks for the keystroke in the calculator 
 * data structure and (if a match is found) presses the 
 * button that matches the keystroke for the user.  It
 * keeps our code small since we only have to handle the
 * button_clicked events.
 * 
 * @param widget TBD
 * @param event TBD
 * @param data
 */
static void main_wnd_key_press( GtkWidget * widget, GdkEventKey * event, gpointer data ) {
    GList *list;
    int hist_sel, hist_nb;

    if ( event->keyval != ' ' )
        return;

    install_key_press_handler( FALSE );

    /*--- Update the list of move ---*/
    list = GTK_CLIST( hist_list )->selection;
    hist_sel = ( list ) ? ( int ) list->data : -1;
    hist_nb = GTK_CLIST( hist_list )->rows;
    if ( hist_sel + 1 != hist_nb ) {
        int cnt;

        // Remove all elements after the one selected in the list
        for ( cnt = hist_sel + 1; cnt < hist_nb; cnt++ )
            gtk_clist_remove( GTK_CLIST( hist_list ), hist_sel + 1 );
    }

    computer_turn(  );

}                               // main_wnd_key_press

/**
 * TBD
 * 
 * @param enable TBD
 */
void install_key_press_handler( const int enable ) {
    static guint id = 0;

    /*
     * We care if a key is pressed 
     */
    if ( enable ) {
        if ( id == 0 )
            id = gtk_signal_connect( GTK_OBJECT( main_wnd ), "key_press_event", GTK_SIGNAL_FUNC( main_wnd_key_press ), NULL );
    }
    else {
        if ( id > 0 ) {
            gtk_signal_disconnect( GTK_OBJECT( main_wnd ), id );
            id = 0;
        }
    }

}                               // install_key_press_handler

/**
 * TBD
 * 
 * @param enable TBD
 */
void install_motion_handler( const int enable ) {
    static guint id = 0;

    /*
     * We care if a key is pressed 
     */
    if ( enable ) {
        if ( id == 0 )
            id =
               gtk_signal_connect( GTK_OBJECT( board_drawing_area ),
               "motion_notify_event", ( GtkSignalFunc ) main_wnd_motion, NULL );
    }
    else {
        if ( id > 0 ) {
            gtk_signal_disconnect( GTK_OBJECT( board_drawing_area ), id );
            id = 0;
        }
    }
}                               // install_motion_handler

/**
 * Create and allocate a GdkColor with the color specified in the parameter list
 * 
 * @param red TBD
 * @param green TBD
 * @param blue TBD
 * @return TBD
 */
static GdkColor *NewColor( const long red, const long green, const long blue ) {
    /*
     * Get the color 
     */
    GdkColor *c = ( GdkColor * ) g_malloc( sizeof( GdkColor ) );

    /*
     * Fill it in. 
     */
    c->red = red;
    c->green = green;
    c->blue = blue;

    gdk_color_alloc( gdk_colormap_get_system(  ), c );

    return ( c );
}                               // NewColor


/**
 * Get a pen using the GdkColor passed in.  
 * The "pen"(just a GdkGC) is created and returned ready for use.
 * 
 * @param c TBD
 * @return TBD
 */
static GdkGC *GetPen( GdkColor * c ) {
    GdkGC *gc;

    /*
     * Create a gc 
     */
    gc = gdk_gc_new( main_wnd->window );

    /*
     * Set the forground to the color 
     */
    gdk_gc_set_foreground( gc, c );

    /*
     * Return it. 
     */
    return ( gc );
}                               // GetPen

/**
 * TBD
 */
void expose_init( void ) {

    penBlack = GetPen( NewColor( 0, 0, 0 ) );
    penForegroundBoard = GetPen( NewColor( 0, 0xE000, 0 ) );
    penWhiteCell = GetPen( NewColor( 0xFF00, 0xFF00, 0xFF00 ) );
    penBlackCell = GetPen( NewColor( 0xFF, 0xFF, 0xFF ) );

}                               // expose_init


static GtkStyle *new_style = NULL;

/**
 * TBD
 */
void compute_size_elements_board( void ) {
    int histlist_width;
    int vbox_width, vbox_height;
    int area_width, area_height;
    GtkStyle *style;
    char *font_name;
    GdkFont *font;

    /*
     * Size of the complete area 
     */
    histlist_width = hist_list->allocation.width;
    vbox_width = main_hbox->allocation.width;
    vbox_height = main_hbox->allocation.height;
    area_width = vbox_width - histlist_width;
    area_height = vbox_height;
    if ( area_width < area_height )
        board_width = board_height = area_width;
    else
        board_width = board_height = area_height;

    board_height -= 2 * BOARD_HEIGHT_FENCE;
    board_width -= 2 * BOARD_WIDTH_FENCE;

    board_height = board_height * 8 / 9;
    board_width = board_width * 8 / 9;

    board_height -= ( board_height % 8 );
    board_width -= ( board_width % 8 );

    cell_width = board_width / 8;
    cell_height = board_height / 8;

    board_xstart = ( area_width - board_width ) / 2;
    board_ystart = ( area_height - board_height ) / 2;

    font_name = find_max_font( cell_width * 3 / 5, cell_height * 3 / 5 );
    style = gtk_widget_get_style( board_drawing_area );
    new_style = gtk_style_copy( style );
    font = gdk_font_load( font_name );
    if ( font != NULL )
        gtk_style_set_font( new_style, font );
    else
        gtk_style_set_font( new_style, gtk_style_get_font( style ) );

}                               // compute_size_elements_board

/**
 * TBD
 * 
 * @param drawable TBD
 * @param r TBD
 * @param c TBD
 * @param pawn TBD
 * @return TBD
 */
void draw_pawn( GdkDrawable * drawable, const int r, const int c, const int pawn ) {
    int cx, cy, x, y;

    cx = cell_width - 2 * CELL_WIDTH_FENCE;
    cy = cell_height - 2 * CELL_HEIGHT_FENCE;
    x = board_xstart + ( c - 1 ) * cell_width + CELL_WIDTH_FENCE;
    y = board_ystart + ( r - 1 ) * cell_height + CELL_HEIGHT_FENCE;

    gdk_draw_arc( drawable,
       ( pawn == CODE_EMPTY ) ? penForegroundBoard :
       ( ( pawn == CODE_BLACK ) ? penBlackCell : penWhiteCell ), TRUE, x, y, cx, cy, 0, 360 * 64 );

}                               // draw_pawn

/**
 * TBD
 */
void redraw_board( void ) {
    int x, y;
    GdkDrawable *drawable = board_drawing_area->window;
    static int first = TRUE;
    char str[2];
    int px, py;
    int w, h;
    char *font_name;
    GdkFont *font, *private_font;

    if ( first ) {
        first = FALSE;
        return;
    }

    /*--- Draw each pawn ---*/
    for ( x = 1; x <= 8; x++ )
        for ( y = 1; y <= 8; y++ )
            draw_pawn( drawable, x, y, board[XY( x, y )] );

    /*--- Draw horizontal A-H ---*/
    font_name = find_max_font( cell_width * 3 / 5, cell_height * 3 / 5 );
    font = gdk_font_load( font_name );
    private_font = gtk_style_get_font( new_style );
    for ( x = 1; x <= 8; x++ ) {
        str[0] = '@' + x;
        str[1] = 0;
        w = gdk_string_width( font, str );
        h = gdk_string_height( font, str );
        px = board_xstart + ( x - 1 ) * cell_width;
        py = board_ystart + ( 0 - 1 ) * cell_height;
        px += ( cell_width - w ) / 2;
        py += ( cell_height - h - font->descent ) / 2;
        py += private_font->ascent;
        gdk_draw_text( drawable, private_font, penBlackCell, px, py, str, 1 );
    }

    /*--- Draw vertical 1-8 ---*/
    for ( y = 1; y <= 8; y++ ) {
        str[0] = '0' + y;
        str[1] = 0;
        w = gdk_string_width( font, str );
        h = gdk_string_height( font, str );
        px = board_xstart + ( 0 - 1 ) * cell_width;
        py = board_ystart + ( y - 1 ) * cell_height;
        px += ( cell_width - w ) / 2;
        py += ( cell_height - h - font->descent ) / 2;
        py += private_font->ascent;
        gdk_draw_text( drawable, private_font, penBlackCell, px, py, str, 1 );
    }

    gdk_flush(  );

}                               // redraw_board

/**
 * TBD
 * 
 * @param drawable TBD
 */
void draw_board( GdkDrawable * drawable ) {
    int n, x, y;

    /*
     * Draw green board 
     */
    gdk_draw_rectangle( drawable, penForegroundBoard, TRUE, board_xstart, board_ystart, board_width, board_height );

    /*
     * Draw lines to form cells 
     */
    for ( x = board_xstart, y = board_ystart, n = 0; n <= 8; n++, x += cell_width, y += cell_height ) {
        gdk_draw_line( drawable, penBlack, x, board_ystart, x, board_ystart + board_height );
        gdk_draw_line( drawable, penBlack, board_xstart, y, board_xstart + board_width, y );
    }


    redraw_board(  );

}                               // draw_board


static void
install_board( void ) {
    int histlist_width;
    int vbox_width, vbox_height;
    GtkWidget *infos_hbox1, *infos_hbox2;

    /*
     * Create the history?  
     */
    create_hist_list(  );

    /*
     * Create a horizontal box for the status area
     */
    status_hbox = gtk_hbox_new( FALSE, 1 );
    gtk_box_pack_start( GTK_BOX( main_vbox ), status_hbox, FALSE, TRUE, 0 );
    gtk_widget_show( status_hbox );

    /*
     * Create status boxes at the bottom 
     */
    create_status(  );

    /*
     * Create a horizontal box for the infos area (Computer)
     */
    infos_hbox1 = gtk_hbox_new( FALSE, 1 );
    gtk_box_pack_start( GTK_BOX( main_vbox ), infos_hbox1, FALSE, TRUE, 0 );
    gtk_widget_show( infos_hbox1 );

    /*
     * Create a label inside the infos area (Player) 
     */
    computer_infos_label = gtk_label_new( "Hello" );
    gtk_box_pack_start( GTK_BOX( infos_hbox1 ), computer_infos_label, FALSE, FALSE, 1 );
    gtk_widget_show( computer_infos_label );

    /*
     * Create a horizontal box for the infos area (Computer)
     */
    infos_hbox2 = gtk_hbox_new( FALSE, 1 );
    gtk_box_pack_start( GTK_BOX( main_vbox ), infos_hbox2, FALSE, TRUE, 0 );
    gtk_widget_show( infos_hbox2 );

    /*
     * Create a label inside the infos area (Player) 
     */
    player_infos_label = gtk_label_new( "Hello" );
    gtk_box_pack_start( GTK_BOX( infos_hbox2 ), player_infos_label, FALSE, FALSE, 1 );
    gtk_widget_show( player_infos_label );

    /*
     * Create the drawing area  
     */
    board_drawing_area = gtk_drawing_area_new(  );
    histlist_width = hist_list->allocation.width;
    vbox_width = main_hbox->allocation.width;
    vbox_height = main_hbox->allocation.height;
    gtk_drawing_area_size( GTK_DRAWING_AREA( board_drawing_area ), vbox_width - histlist_width, vbox_height );
    gtk_widget_show( board_drawing_area );

    gtk_signal_connect( GTK_OBJECT( board_drawing_area ), "expose_event", ( GtkSignalFunc ) main_wnd_expose, NULL );

    gtk_signal_connect( GTK_OBJECT( board_drawing_area ), "configure_event", ( GtkSignalFunc ) main_wnd_resized, NULL );

    gtk_signal_connect( GTK_OBJECT( board_drawing_area ), "button_press_event", ( GtkSignalFunc ) main_wnd_press, NULL );

    gtk_signal_connect( GTK_OBJECT( board_drawing_area ), "button_release_event", ( GtkSignalFunc ) main_wnd_release, NULL );

    install_motion_handler( TRUE );

    /*
     * Events to listen for 
     */
    gtk_widget_set_events( board_drawing_area,
       gtk_widget_get_events( board_drawing_area ) |
       GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK );

    /*
     * Add drawing area to packing box 
     */
    gtk_box_pack_start( GTK_BOX( main_hbox ), board_drawing_area, TRUE, TRUE, 0 );

    gtk_widget_show_all( main_wnd );

}                               // install_board

/**
 * TBD
 */
void create_layout( void ) {

    /*--- Create the base window ---*/
    main_wnd = gtk_widget_new( gtk_window_get_type(  ),
       "GtkWidget::can_focus", FALSE,
       "GtkWidget::events", GDK_EXPOSURE_MASK | GDK_POINTER_MOTION_MASK |
       GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_PRESS_MASK |
       GDK_BUTTON_RELEASE_MASK,
       "GtkContainer::resize_mode", GTK_RESIZE_QUEUE,
       "GtkWindow::title", "Atilo: Reversi Game",
       "GtkWindow::resizable", TRUE,
       "GtkWindow::allow_shrink", TRUE,
       "GtkWindow::allow_grow", TRUE, "GtkWindow::default_width", 460, "GtkWindow::default_height", 400, NULL );

    /*--- Connect signals for destruction ---*/
    gtk_signal_connect( GTK_OBJECT( main_wnd ), "destroy", GTK_SIGNAL_FUNC( close_app ), &main_wnd );
    gtk_signal_connect( GTK_OBJECT( main_wnd ), "delete-event", GTK_SIGNAL_FUNC( close_app ), NULL );

    gtk_window_set_title( GTK_WINDOW( main_wnd ), "Atilo: Reversi Game" );
    gtk_widget_set_usize( GTK_WIDGET( main_wnd ), 460, 400 );   // Minimal size
    gtk_window_set_default_size( GTK_WINDOW( main_wnd ), config.mainwnd_w, config.mainwnd_h );

    /*--- VBOX to handle: menu / Hist list & Board / Status bar ---*/
    main_vbox = gtk_widget_new( gtk_vbox_get_type(  ),
       "GtkWidget::parent", main_wnd, "GtkWidget::can_focus", FALSE, "GtkBox::spacing", 4, NULL );
    gtk_container_border_width( GTK_CONTAINER( main_vbox ), 0 );
    gtk_widget_show( main_vbox );

    /*--- Create the main menu ---*/
    create_main_menu( main_vbox );

    /*--- HBOX to handle: ---*/
    main_hbox = gtk_hbox_new( FALSE, 1 );
    gtk_container_add( GTK_CONTAINER( main_vbox ), main_hbox );
    gtk_widget_show( main_hbox );

    install_board(  );

}                               // create_layout

/**
 * TBD
 * 
 * @param fmt TBD
 * @param ... TBD
 */
void computer_infos_msg( const char *fmt, ... ) {
    va_list ap;
    char msg[200];

    va_start( ap, fmt );
    vsprintf( msg, fmt, ap );
    va_end( ap );

    gtk_label_set_text( GTK_LABEL( computer_infos_label ), msg );

}                               // computer_infos_msg

/**
 * TBD
 * 
 * @param fmt TBD
 * @param ... TBD
 * @return TBD
 */
void player_infos_msg( const char *fmt, ... ) {
    va_list ap;
    char msg[200];

    va_start( ap, fmt );
    vsprintf( msg, fmt, ap );
    va_end( ap );

    gtk_label_set_text( GTK_LABEL( player_infos_label ), msg );

}                               // player_infos_msg
