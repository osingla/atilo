/**
 * @file loadsave.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

/**
 * Modify the button label
 * 
 * @param widget TBD
 * @param child TBD
 * @param data TBD
 */
static void LabelModify( GtkWidget * child, gpointer data ) {
    if ( GTK_IS_LABEL( child ) )
        gtk_label_set( GTK_LABEL( child ), ( char * ) data );
}                               // LabelModify

/**
 * TBD
 * 
 * @param str TBD
 * @return TBD
 */
static char *strupr( char *str ) {
    for ( ; *str; str++ )
        *str = toupper( *str );
    return str;
}                               // strupr

/**
 * TBD
 * 
 * @param w TBD
 * @param wgt TBD
 */
static void file_load_ok_sel( GtkWidget * w, GtkWidget * wgt ) {
    char const *fname;
    GtkFileSelection *fs = GTK_FILE_SELECTION( wgt );
    FILE *fp;
    unsigned char player, enemy;
    char line[80];
    int len;
    int x, y;
    PCELL ptr1, ptr2, p;
    CELL possib[60];
    int nb_poss, nb_took;
    int valid_move;
    int k, cnt, hist_nb;
    char buff[200], *pitem[1];

    /*
     * --- Get the name --- 
     */
    fname = gtk_file_selection_get_filename( fs );
    fp = fopen( fname, "r" );
    if ( fp == NULL )
        return;

    /*--- Reset the board ---*/
    init_new_game( TRUE, TRUE );

    /*--- Read each line of the file ---*/
    player = CODE_BLACK;
    enemy = CODE_WHITE;
//      busyWindow( ABW_phothello, true );
//      PtHold();
    for ( cnt = 0;; ) {

        /*--- Read a new line ---*/
        if ( !fgets( line, sizeof( line ) - 1, fp ) )
            break;
        if ( ( ( len = strlen( line ) ) > 0 ) && ( line[len - 1] == '\n' ) )
            line[len - 1] = 0;

        /*--- Get the position ---*/
        if ( !strcmp( line, "" ) )
            continue;
        strupr( line );
        if ( ( line[0] >= 'A' ) && ( line[0] <= 'H' ) && ( line[1] >= '1' ) && ( line[1] <= '8' ) ) {
            x = ( line[1] - '0' );
            y = ( line[0] - '@' );
        }
        else if ( ( line[1] >= 'A' ) && ( line[1] <= 'H' ) && ( line[0] >= '1' ) && ( line[0] <= '8' ) ) {
            x = ( line[0] - '0' );
            y = ( line[1] - '@' );
        }
        else {
//                      message( "Unable to decode line: '%s'\n", line );
            fclose( fp );
//                      PtRelease();
//                      busyWindow( ABW_phothello, false );
            init_new_game( TRUE, TRUE );
            return;
        }

        /*--- Generate all the possible move ---*/
        ptr1 = ptr2 = &possib[0];
        if ( ( nb_poss = do_build_possib( &ptr2, player, enemy ) ) == 0 ) {
            player = ( CODE_BLACK + CODE_WHITE ) - player;
            enemy = ( CODE_BLACK + CODE_WHITE ) - enemy;
            ptr1 = ptr2 = &possib[0];
            if ( ( nb_poss = do_build_possib( &ptr2, player, enemy ) ) == 0 ) {
                //                      message( "Error, can't play at position %s\n", line );
                fclose( fp );
                //                      PtRelease();
                //                      busyWindow( ABW_phothello, false );
                init_new_game( TRUE, TRUE );
                return;
            }
        }                       // if

        /*--- Is-it a valid move ? ---*/
        for ( valid_move = FALSE, p = ptr1, p++; p != ptr2; ) {
            if ( *p++ == &board[XY( x, y )] ) {
                valid_move = TRUE;
                break;
            }
            nb_took = ( int ) *p++;
            p += nb_took;
        }                       // for (p)
        if ( !valid_move ) {
//                      message( "Error, position %s not valid\n", line );
            fclose( fp );
//                      PtRelease();
//                      busyWindow( ABW_phothello, false );
            init_new_game( TRUE, TRUE );
            return;
        }

        /*--- Play this new move ---*/
        board[XY( x, y )] = player;
        for ( nb_took = ( int ) *p++, k = 0; k < nb_took; k++ )
            *( CELL ) * p++ = player;

        /*--- Update the widget list 'History' ---*/
        sprintf( buff, "%2d: %s: %c%d", cnt + 1, lib_colors( player ), y + '@', x );
        pitem[0] = buff;
        gtk_clist_append( GTK_CLIST( hist_list ), pitem );

        /*--- Next player ---*/
        player = ( CODE_BLACK + CODE_WHITE ) - player;
        enemy = ( CODE_BLACK + CODE_WHITE ) - enemy;
        color_to_play = player;
        cnt++;

    }                           // for (cnt)

    /*--- Close the file ---*/
    fclose( fp );

    /*--- Selection of the last element of the history list ---*/
    hist_nb = GTK_CLIST( hist_list )->rows;
    if ( hist_nb > 0 )
        gtk_clist_select_row( GTK_CLIST( hist_list ), hist_nb - 1, 0 );

    /*--- Redraw the board ---*/
    redraw_board(  );
    compute_nb_pawns( TRUE );

    /*
     * --- Destroy the file selection --- 
     */
    gtk_widget_destroy( wgt );

}                               // file_load_ok_sel

/**
 * TBD
 * 
 * @param filename TBD
 * @return TBD
 */
static int is_oth_file( const char *filename ) {
    FILE *fp;
    long length;

    if ( !strcmp( filename, "" ) )
        return FALSE;

    if ( ( fp = fopen( filename, "r" ) ) == NULL )
        return FALSE;

    length = filelength( fp );
    if ( ( length < 2 ) || ( length > 60 * ( 2 + 2 ) ) )
        return fclose( fp ), FALSE;

    fclose( fp );
    return TRUE;

}                               // is_oth_file

/**
 * TBD
 * 
 * @param wgt TBD
 * @return TBD
 */
static void file_load_entry( GtkWidget * wgt ) {
    GtkFileSelection *fs = GTK_FILE_SELECTION( wgt );
    char const *sel;

    sel = gtk_file_selection_get_filename( fs );
    gtk_widget_set_sensitive( fs->ok_button, is_oth_file( sel ) );

}                               // file_load_entry

/**
 * TBD
 */
void file_load_game( void ) {
    GtkWidget *fw;

    fw = gtk_file_selection_new( "Load game" );
    gtk_file_selection_set_filename( GTK_FILE_SELECTION( fw ), "" );

    gtk_signal_connect_object( GTK_OBJECT( GTK_FILE_SELECTION( fw )->cancel_button ),
       "clicked", GTK_SIGNAL_FUNC( gtk_widget_destroy ), GTK_OBJECT( fw ) );

    gtk_widget_set_sensitive( GTK_FILE_SELECTION( fw )->ok_button, FALSE );
    gtk_signal_connect( GTK_OBJECT( GTK_FILE_SELECTION( fw )->ok_button ),
       "clicked", GTK_SIGNAL_FUNC( file_load_ok_sel ), GTK_OBJECT( fw ) );

    gtk_signal_connect_object( GTK_OBJECT( GTK_FILE_SELECTION( fw )->selection_entry ),
       "changed", GTK_SIGNAL_FUNC( file_load_entry ), GTK_OBJECT( GTK_FILE_SELECTION( fw ) ) );

    gtk_container_foreach( GTK_CONTAINER( GTK_FILE_SELECTION( fw )->ok_button ), ( GtkCallback ) LabelModify, "Load" );

    gtk_widget_show( fw );

}                               // file_load_game

/**
 * TBD
 * 
 * @param w TBD
 * @param wgt TBD
 */
static void file_save_ok_sel( GtkWidget * w, GtkWidget * wgt ) {
    char const *fname;
    GtkFileSelection *fs = GTK_FILE_SELECTION( wgt );
    int hist_nb, row;
    char *text;
    FILE *fp;

    /*
     * --- Get the name --- 
     */
    fname = gtk_file_selection_get_filename( fs );
    fp = fopen( fname, "w" );
    if ( !fp )
        return;

    hist_nb = GTK_CLIST( hist_list )->rows;
    for ( row = 0; row < hist_nb; row++ ) {
        gtk_clist_get_text( GTK_CLIST( hist_list ), row, 0, &text );
        fprintf( fp, "%c%c\n", text[11], text[12] );
    }                           // for (row)

    /*
     * --- Destroy the file selection --- 
     */
    fclose( fp );
    gtk_widget_destroy( wgt );

}                               // file_save_ok_sel

/**
 * TBD
 * 
 * @param wgt TBD
 */
static void file_save_entry( GtkWidget * wgt ) {
    GtkFileSelection *fs = GTK_FILE_SELECTION( wgt );
    char const *sel;

    sel = gtk_file_selection_get_filename( fs );
    gtk_widget_set_sensitive( fs->ok_button, strcmp( sel, "" ) );
}                               // file_save_entry

/**
 * TBD
 */
void file_save_game( void ) {
    GtkWidget *fw;

    fw = gtk_file_selection_new( "Save game" );
    gtk_file_selection_set_filename( GTK_FILE_SELECTION( fw ), "" );

    gtk_signal_connect_object( GTK_OBJECT( GTK_FILE_SELECTION( fw )->cancel_button ),
       "clicked", GTK_SIGNAL_FUNC( gtk_widget_destroy ), GTK_OBJECT( fw ) );

    gtk_widget_set_sensitive( GTK_FILE_SELECTION( fw )->ok_button, FALSE );
    gtk_signal_connect( GTK_OBJECT( GTK_FILE_SELECTION( fw )->ok_button ),
       "clicked", GTK_SIGNAL_FUNC( file_save_ok_sel ), GTK_OBJECT( fw ) );

    gtk_signal_connect_object( GTK_OBJECT( GTK_FILE_SELECTION( fw )->selection_entry ),
       "changed", GTK_SIGNAL_FUNC( file_save_entry ), GTK_OBJECT( GTK_FILE_SELECTION( fw ) ) );

    gtk_container_foreach( GTK_CONTAINER( GTK_FILE_SELECTION( fw )->ok_button ), ( GtkCallback ) LabelModify, "Save" );

    gtk_widget_show( fw );
}                               // file_save_game
