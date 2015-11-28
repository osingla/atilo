/**
 * @file status.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

static GtkWidget *progressbar;

static GtkWidget *black_pawnsnb, *white_pawnsnb;
static GtkWidget *black_owner, *white_owner;
static GtkWidget *black_horo, *white_horo;

/**
 * TBD		
 * 
 * @param widget TBD
 * @param event TBD
 * @return TBD
 */
static gint black_pawn_expose( GtkWidget * widget, GdkEventExpose * event ) {
    if ( penBlackCell )
        gdk_draw_arc( widget->window,
           penBlackCell, TRUE, 0, 0, widget->allocation.width - 1, widget->allocation.height - 1, 0, 360 * 64 );
    return FALSE;
}                               // black_pawn_expose

/**
 * TBD		
 * 
 * @param widget TBD
 * @param event TBD
 * @return TBD
 */
static gint white_pawn_expose( GtkWidget * widget, GdkEventExpose * event ) {
    if ( penWhiteCell )
        gdk_draw_arc( widget->window,
           penWhiteCell, TRUE, 0, 0, widget->allocation.width - 1, widget->allocation.height - 1, 0, 360 * 64 );
    return FALSE;
}                               // white_pawn_expose

/**
 * TBD		
 * 
 * @param is_black TBD
 * @param msg TBD
 * @param status TBD
 * @param nb TBD
 * @param owner TBD
 * @param horo TBD
 */
static void create_user_status( const int is_black,
   const char *msg, GtkWidget * status, GtkWidget ** nb, GtkWidget ** owner, GtkWidget ** horo ) {
    GtkWidget *hbox;
    GtkWidget *drawing_area;

    /*
     * --- Create a horizontal box for the status area -- 
     */
    hbox = gtk_hbox_new( FALSE, 1 );
    gtk_container_add( GTK_CONTAINER( status ), hbox );
    gtk_widget_show( hbox );

    *owner = gtk_label_new( msg );
    gtk_box_pack_start( GTK_BOX( hbox ), *owner, FALSE, FALSE, 1 );
    gtk_widget_show( *owner );

    *nb = gtk_label_new( " 45 " );
    gtk_box_pack_start( GTK_BOX( hbox ), *nb, FALSE, FALSE, 1 );
    gtk_widget_show( *nb );

    drawing_area = gtk_drawing_area_new(  );
    gtk_drawing_area_size( GTK_DRAWING_AREA( drawing_area ), 16, 16 );
    gtk_box_pack_start( GTK_BOX( hbox ), drawing_area, FALSE, FALSE, 1 );
    gtk_widget_show( drawing_area );
    gtk_signal_connect( GTK_OBJECT( drawing_area ), "expose_event",
       ( GtkSignalFunc ) ( ( is_black ) ? black_pawn_expose : white_pawn_expose ), NULL );

    *horo = gtk_label_new( "XX:XX" );
    gtk_box_pack_start( GTK_BOX( hbox ), *horo, FALSE, FALSE, 1 );
    gtk_widget_show( *horo );

}                               // create_user_status

GtkAdjustment *progressadj;

/**
 * TBD		
 */
void create_status( void ) {
    GtkStyle *style;
    GdkFont *font;
    int max_sz, height;
    GtkWidget *status_black, *status_white;

    status_black = gtk_frame_new( NULL );
    style = gtk_widget_get_style( status_black );
    font = gtk_style_get_font( style );
    max_sz = gdk_string_width( font, "XXXXXXXX:X99XX99:99" ) + 5;
    height = gdk_string_height( font, "X" );
    gtk_frame_set_shadow_type( GTK_FRAME( status_black ), GTK_SHADOW_IN );
    gtk_widget_set_usize( status_black, max_sz, 23 );
    gtk_box_pack_start( GTK_BOX( status_hbox ), status_black, FALSE, TRUE, 0 );
    gtk_widget_show( status_black );
    create_user_status( 1, " Player:", status_black, &black_pawnsnb, &black_owner, &black_horo );

    status_white = gtk_frame_new( NULL );
    max_sz = gdk_string_width( font, "XXXXXXXX:X99XX99:99" ) + 5;
    gtk_frame_set_shadow_type( GTK_FRAME( status_white ), GTK_SHADOW_IN );
    gtk_widget_set_usize( status_white, max_sz, 23 );
    gtk_box_pack_start( GTK_BOX( status_hbox ), status_white, FALSE, TRUE, 0 );
    gtk_widget_show( status_white );
    create_user_status( 0, " Computer:", status_white, &white_pawnsnb, &white_owner, &white_horo );

    progressadj = ( GtkAdjustment * ) gtk_adjustment_new( 0, 0, 1000, 0, 0, 0 );
    progressbar = gtk_progress_bar_new_with_adjustment( progressadj );
    gtk_progress_bar_set_discrete_blocks( GTK_PROGRESS_BAR( progressbar ), 16 );
    gtk_progress_bar_set_bar_style( GTK_PROGRESS_BAR( progressbar ), GTK_PROGRESS_DISCRETE );
    gtk_box_pack_start( GTK_BOX( status_hbox ), progressbar, TRUE, TRUE, 0 );
    gtk_widget_show( progressbar );

}                               // create_status

/**
 * TBD		
 * 
 * @param val TBD
 * @param max_val TBD
 * @return TBD
 */
void set_progressbar( const int val, const int max_val ) {
    gtk_progress_set_value( GTK_PROGRESS( progressbar ), ( double ) ( 1000 / max_val ) * val );
    gdk_flush(  );
    while ( gtk_events_pending(  ) )
        gtk_main_iteration(  );
}                               // set_progressbar

/**
 * TBD		
 */
void show_nb_pawns( void ) {
    char tmp[80];

    sprintf( tmp, "%d", nb_pawns[CODE_BLACK] );
    gtk_label_set_text( GTK_LABEL( black_pawnsnb ), tmp );

    sprintf( tmp, "%d", nb_pawns[CODE_WHITE] );
    gtk_label_set_text( GTK_LABEL( white_pawnsnb ), tmp );
}                               // show_nb_pawns

/**
 * TBD		
 */
void set_owners( void ) {
    gtk_label_set_text( GTK_LABEL( black_owner ), ( code_computer == CODE_BLACK ) ? "Computer:" : "Player:" );
    gtk_label_set_text( GTK_LABEL( white_owner ), ( code_computer == CODE_WHITE ) ? "Computer:" : "Player:" );
}                               // set_owners

/**
 * TBD		
 */
void show_horo( void ) {
    char buff[80];

    sprintf( buff, "%02d:%02d", ctime_black / 60, ctime_black % 60 );
    gtk_label_set_text( GTK_LABEL( black_horo ), buff );

    sprintf( buff, "%02d:%02d", ctime_white / 60, ctime_white % 60 );
    gtk_label_set_text( GTK_LABEL( white_horo ), buff );
}                               // show_horo
