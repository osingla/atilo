/**
 * @file atilo.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n \n
 */

/**
 * @mainpage ATILO: Othello/revseri Game
 * 
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 *
 * The game of Reversi is a two players game. The game is played on an 8x8 board. 
 * The pawns are black on one side (i.e. belongs to one player), and white on the opposite side 
 * (i.e. belongs to the other player). 
 * 
 * The goal of the game is to have more pawns belonging to his color, at the end of the game
 * 
 **/

#define __MAIN__
#include "all.h"

/**
 * If we come here, then the user has selected a row in the list.
 * 
 * @param clist TBD
 * @param row TBD
 * @param column TBD
 * @param event TBD
 * @param data TBD
 */
static void hist_list_selected( GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data ) {
    gchar *text;
    unsigned char player, enemy;
    GList *list;
    int cnt;
    int x, y;
    PCELL ptr1, ptr2, p;
    CELL possib[256];
    int nb_poss, nb_took;
    int k;
    int valid_move;
    int pass;

    /*
     * Get the text that is stored in the selected row and column * which was 
     * clicked in. We will receive it as a pointer in the * argument text. 
     */
    gtk_clist_get_text( GTK_CLIST( clist ), row, column, &text );

    /*--- Reset the game ---*/
    init_new_game( FALSE, FALSE );

    /*--- Bring the game to this point ---*/
    player = CODE_BLACK;
    enemy = CODE_WHITE;
    list = GTK_CLIST( hist_list )->selection;
    for ( cnt = 0; cnt <= row; cnt++ ) {

        /*--- Get the position ---*/
        gtk_clist_get_text( GTK_CLIST( hist_list ), cnt, 0, &text );
        y = text[11] - '@';
        x = text[12] - '0';

        /*--- Generate all the possible move ---*/
        ptr1 = ptr2 = &possib[0];
        if ( ( nb_poss = do_build_possib( &ptr2, player, enemy ) ) == 0 ) {
            player = ( CODE_BLACK + CODE_WHITE ) - player;
            enemy = ( CODE_BLACK + CODE_WHITE ) - enemy;
            ptr1 = ptr2 = &possib[0];
            if ( ( nb_poss = do_build_possib( &ptr2, player, enemy ) ) == 0 )
                return;
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
        if ( !valid_move )
            return;

        /*--- Play this new move ---*/
        board[XY( x, y )] = player;
        nb_took = ( int ) *p++;
        for ( k = 0; k < nb_took; k++ )
            *( unsigned char * ) *p++ = player;

        /*--- Next player ---*/
        player = ( CODE_BLACK + CODE_WHITE ) - player;
        enemy = ( CODE_BLACK + CODE_WHITE ) - enemy;

    }                           // for (cnt)

    /*--- Refresh the display ---*/
    redraw_board(  );
    compute_nb_pawns( TRUE );

    /*--- Which player next, if any ? ---*/
    pass = FALSE;
    ptr1 = ptr2 = &possib[0];
    if ( ( nb_poss = do_build_possib( &ptr2, player, enemy ) ) == 0 ) {
        ptr2 = &possib[0];
        if ( ( nb_poss = do_build_possib( &ptr2, enemy, player ) ) == 0 )
            return;
        pass = TRUE;
        if ( player == code_human )
            player_infos_msg( "Sorry, no possibility left, you pass next turn!" );
        else
            computer_infos_msg( "No possibility left, computer pass next turn!" );
        player = ( CODE_BLACK + CODE_WHITE ) - player;
    }                           // if

    /*--- Next player ---*/
    if ( player == code_computer ) {
        nb_poss = show_poss_play_cursor( code_computer, code_human );
        computer_infos_msg( "%d possibilit%s. Press <SPACE> to play Computer", nb_poss, ( nb_poss == 1 ) ? "y" : "ies" );
        install_key_press_handler( TRUE );
        if ( !pass )
            player_infos_msg( " " );
    }
    else {
        nb_poss = show_poss_play_cursor( code_human, code_computer );
        player_infos_msg( "%d possibilit%s. Choose a position and click to play", nb_poss, ( nb_poss == 1 ) ? "y" : "ies" );
        install_key_press_handler( FALSE );
        if ( !pass )
            computer_infos_msg( " " );
    }
    color_to_play = player;

    return;
}                               // hist_list_selected

/**
 * TBD
 * 
 * @param widget TBD
 * @param event TBD
 * @return BD
 */
gint main_wnd_resized( GtkWidget *widget, GdkEventConfigure *event ) {
#if 0
    printf( "new size = %d , %d\n", main_wnd->allocation.width, main_wnd->allocation.height );
#endif
    return TRUE;
}                               // main_wnd_resized

/**
 * TBD
 */
void clear_hist_list( void ) {
    /*
     * Clear the list using gtk_clist_clear. This is much faster than *
     * calling gtk_clist_remove once for each row. 
     */
    gtk_clist_clear( GTK_CLIST( hist_list ) );
}                               // clear_hist_list

/**
 * TBD
 * 
 * @param widget TBD
 * @param event TBD
 * @return TBD
 */
gint main_wnd_expose( GtkWidget * widget, GdkEventExpose * event ) {
    static int first = TRUE;

    if ( first ) {
        expose_init(  );
        first = FALSE;
    }

    compute_size_elements_board(  );
    draw_board( widget->window );

    return FALSE;
}                               // main_wnd_expose

/**
 * TBD
 */
void create_hist_list( void ) {
    static char *title[1] = { "History" };
    GtkStyle *style, *new_style;
    gint sz, max_sz;
    int n;
    char tmp[80];
    GtkWidget *scrolled_win;
    GdkFont *private_font;

    hist_list = gtk_clist_new_with_titles( 1, title );

    /*
     * When a selection is made, we want to know about it. The callback *
     * used is selection_made, and its code can be found further down 
     */
    select_hist_handler_id =
       gtk_signal_connect( GTK_OBJECT( hist_list ), "select_row", GTK_SIGNAL_FUNC( hist_list_selected ), NULL );

    /*
     * It isn't necessary to shadow the border, but it looks nice :) 
     */
    gtk_clist_set_shadow_type( GTK_CLIST( hist_list ), GTK_SHADOW_OUT );

    /*
     * Make sure titles are being shown
     */
    gtk_clist_column_titles_show( GTK_CLIST( hist_list ) );

    /*
     * What however is important, is that we set the column widths as * they
     * will never be right otherwise. Note that the columns are * numbered
     * from 0 and up. 
     */
    style = gtk_widget_get_style( hist_list );
    new_style = gtk_style_copy( style );
    if ( ( private_font = gdk_font_load( "*courier-*-r*140*" ) ) == NULL )
        if ( ( private_font = gdk_font_load( "*courier-*-r*120*" ) ) == NULL )
            private_font = gtk_style_get_font( style );
    gtk_style_set_font( style, private_font );
    gtk_widget_set_style( hist_list, new_style );
    for ( max_sz = 0, n = 0; n < 10; n++ ) {
        sprintf( tmp, "%d: %s: %c%d",
           RANDOM( 1, 60 ), ( rand(  ) % 2 ) ? "Black" : "White", RANDOM( 0, 7 ) + 'A', RANDOM( 1, 8 ) );
        sz = gdk_string_width( private_font, tmp );
        if ( sz > max_sz )
            max_sz = sz;
    }
    gtk_clist_set_column_width( GTK_CLIST( hist_list ), 0, max_sz );

    /*
     * --- Set the column justifications --- 
     */
    gtk_clist_set_column_justification( GTK_CLIST( hist_list ), 0, GTK_JUSTIFY_LEFT );
    gtk_clist_column_titles_passive( GTK_CLIST( hist_list ) );

    /*
     * --- Selection mode --- 
     */
    gtk_clist_set_selection_mode( GTK_CLIST( hist_list ), GTK_SELECTION_BROWSE );

    /*
     * Add the GtkCList widget to the vertical box and show it. 
     */
    gtk_widget_show( hist_list );
    scrolled_win = gtk_scrolled_window_new( NULL, NULL );
    gtk_container_add( GTK_CONTAINER( scrolled_win ), hist_list );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolled_win ), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS );
    gtk_box_pack_start( GTK_BOX( main_hbox ), scrolled_win, FALSE, FALSE, 0 );
    gtk_widget_show( scrolled_win );

}                               // create_hist_list

/**
 * TBD
 * 
 * @param f_draw TBD
 * @param f_reset_history TBD
 */
void init_new_game( const int f_draw, const int f_reset_history ) {
    int k, x, y;

    /*--- By default, computer has black, play has white ---*/
    code_computer = CODE_WHITE;
    code_human = CODE_BLACK;
    color_to_play = CODE_BLACK; // Next color to play

    /*--- Initialize the board ---*/
    for ( k = 0; k < 9; k++ )
        board[XY( 0, k )] = board[XY( k, 9 )] = board[XY( 9, 9 - k )] = board[XY( 9 - k, 0 )] = CODE_AROUND;
    for ( x = 1; x <= 8; x++ )
        for ( y = 1; y <= 8; y++ )
            board[XY( x, y )] = CODE_EMPTY;
    board[XY( 4, 4 )] = CODE_WHITE;
    board[XY( 4, 5 )] = CODE_BLACK;
    board[XY( 5, 4 )] = CODE_BLACK;
    board[XY( 5, 5 )] = CODE_WHITE;

    /*--- Cumul time played by each color ---*/
    if ( f_reset_history ) {
        time( &t_start );       // Time when player starts thinking
        ctime_black = ctime_white = 0;
        show_horo(  );
    }

    /*--- Draw the complete board ---*/
    if ( f_draw )
        if ( board_drawing_area->window )
            redraw_board(  );
    compute_nb_pawns( f_draw );

    /*--- Show the possibilities of the player ---*/
    if ( f_draw )
        show_poss_play_cursor( code_human, code_computer );

    /*--- Reset history of the game ---*/
    if ( f_reset_history )
        clear_hist_list(  );

    /*--- Reset msg ---*/
    computer_infos_msg( " " );
    player_infos_msg( "Choose a position and click to play." );

    /*--- Disable the menu option 'Save the game' ---*/
    enable_SaveGame( FALSE );

}                               // init_new_game

/**
 * TBD
 * 
 * @param argc TBD
 * @param argv TBD
 * @return TBD
 */
int main( int argc, char *argv[] ) {

	// Initialize the GTK library
    gtk_init( &argc, &argv );

    if ( !load_config(  ) )
        scan_fonts(  );
    create_layout(  );
    init_new_game( TRUE, TRUE );
    gtk_main(  );

    return ( 0 );
}                               // main
