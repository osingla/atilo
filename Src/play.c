/**
 * @file play.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

/**
 * TBD
 * 
 * @param n TBD
 * @return TBD
 */
char *lib_colors( const int n ) {
    switch ( n ) {
        case 1:
            return "black";
        case 2:
            return "white";
        default:
            return "empty";
    }
}                               // lib_colors

/**
 * TBD
 * 
 * @param show TBD
 */
void compute_nb_pawns( const int show ) {
    int x, y;

    /*--- Compute the nb of pawns ---*/
    nb_pawns[CODE_WHITE] = nb_pawns[CODE_BLACK] = 0;
    for ( x = 1; x <= 8; x++ )
        for ( y = 1; y <= 8; y++ )
            nb_pawns[board[XY( x, y )]]++;

    /*
     * --- Update on screen? --- 
     */
    if ( show )
        show_nb_pawns(  );

}                               // compute_nb_pawns     

#define CHECK_DIR( dir_offs ) \
\
	/* How many contigues enemies ? */ \
	for ( pscan = pcell, enemy_cnt = 0; ; enemy_cnt++ ) \
		if ( (c = *(pscan += dir_offs)) != cenemy ) \
			break; \
\
	/* At least one contigue enemy and last pawn is the player */ \
	if ( (enemy_cnt) && (c == cplayer) ) \
	{ \
\
		/* Cumulate the nb of pawns took */ \
		nb_took += enemy_cnt; \
\
		/* Record all the enclosed enemy pawns */ \
		for ( n = enemy_cnt; n; n-- ) \
		{ \
			pscan -= dir_offs; \
			*p++ = pscan; \
		} \
\
	}                           /* if */

/**
 * TBD
 * 
 * @param ptr TBD
 * @param cplayer TBD
 * @param cenemy TBD
 * @return TBD
 */
int do_build_possib( PCELL * ptr, const int cplayer, const int cenemy ) {
    int nb_poss;
    unsigned char *pcell;
    int nb_took, n;
    PCELL p, p0;
    unsigned char *pscan;
    int enemy_cnt;              // Enemy enclosed counter
    unsigned char c;

    // Total nb of possibilities
    nb_poss = 0;

    // Pointer into the possibilities table
    p = *ptr;
    p++;                        // Cell for total nb of possibilities

    // Check each non empty cell
    for ( pcell = &board[11]; pcell != &board[89]; pcell++ ) {

        // Non empty ?
        if ( *pcell )
            continue;

        // Count the number of opponents pawns which could be took
        p0 = p;
        p += 2;                 // 2 cells for: [pointer] [nb took]
        nb_took = 0;
        CHECK_DIR( -11 );
        CHECK_DIR( -10 );
        CHECK_DIR( -9 );
        CHECK_DIR( -1 );
        CHECK_DIR( 1 );
        CHECK_DIR( 9 );
        CHECK_DIR( 10 );
        CHECK_DIR( 11 );

#undef CHECK_DIR

        if ( !nb_took ) {
            p -= 2;             // 2 cells for: [pointer] [nb took]
        }
        else {
            *p0++ = pcell;
            *p0 = ( CELL ) nb_took;
            nb_poss++;          // Total nb of possibilities
        }

    }                           // for

    // Total nb of possibilities
    **ptr = ( CELL ) nb_poss;
    *ptr = p;
    return ( nb_poss );

}                               // do_build_possib

#define CHECK_DIR( dir_offs ) \
\
	/* How many contigues enemies ? */ \
	for ( pscan = pcell, enemy_cnt = 0; ; enemy_cnt++ ) \
		if ( (c = *(pscan += dir_offs)) != cenemy ) \
			break; \
\
	/* At least one contigue enemy and last pawn is the player */ \
	if ( (enemy_cnt != 0) && (c == cplayer) ) \
		{ \
\
		/* Cumulate the nb of pawns took */ \
		nb_took += enemy_cnt; \
\
		}                       /* if */

/**
 * TBD
 * 
 * @param cplayer TBD
 * @param cenemy TBD
 * @return TBD
 */
int do_nb_possib( const int cplayer, const int cenemy ) {
    int nb_poss;
    unsigned char *pcell;
    int nb_took;
    unsigned char *pscan;
    int enemy_cnt;              // Enemy enclosed counter
    unsigned char c;

    // Check each non empty cell
    for ( nb_poss = 0, pcell = &board[11]; pcell != &board[89]; pcell++ ) {

        // Empty ?
        if ( !*pcell ) {

            // Count the number of opponents pawns which could be took
            nb_took = 0;
            CHECK_DIR( -11 );
            CHECK_DIR( -10 );
            CHECK_DIR( -9 );
            CHECK_DIR( -1 );
            CHECK_DIR( 1 );
            CHECK_DIR( 9 );
            CHECK_DIR( 10 );
            CHECK_DIR( 11 );

            // Total nb of possibilities
            if ( nb_took )
                nb_poss++;

        }

    }                           // for (cell, cpt_cell)

    // Total nb of possibilities
    return ( nb_poss );

}                               // do_nb_possib

#undef CHECK_DIR

/**
 * TBD
 */
static void diag_end_game( void ) {

    if ( nb_pawns[code_computer] > nb_pawns[code_human] )
        warning_msg( "Sorry!\nComputer won!" );
    else if ( nb_pawns[code_computer] < nb_pawns[code_human] )
        warning_msg( "Congratulations!\nYou won this game!" );
    else
        warning_msg( "Tie game!" );
}                               // diag_end_game


/**
 * Check each cell.
 * If player can play, then change the cursor shape (ok).
 * Else change also the cursor shape (nok).
 * A local possibilities stack is used.
 * 
 * @param cplayer TBD
 * @param cenemy TBD
 * @return TBD
 */
int show_poss_play_cursor( const unsigned char cplayer, const unsigned char cenemy ) {
    int x, y;
    PCELL ptr1, ptr2;
    PCELL p;
    CELL possib[256];
    int nb_poss, nb_took;
    int state;

    ptr1 = ptr2 = &possib[0];
    nb_poss = do_build_possib( &ptr2, cplayer, cenemy );

    for ( x = 1; x <= 8; x++ ) {
        for ( y = 1; y <= 8; y++ ) {

            state = FALSE;
            if ( nb_poss ) {
                for ( state = FALSE, p = ptr1, p++; p != ptr2; ) {
                    if ( *p++ == &board[XY( x, y )] ) {
                        state = TRUE;
                        break;
                    }
                    nb_took = ( int ) *p++;
                    p += nb_took;
                }               // for (p)
            }                   // if

            state_selectable[x - 1][y - 1] = state;

        }                       // for (y)
    }                           // for (x)

    /*--- Show the possibilities of the player ---*/
    which_show_poss = cplayer;

    return nb_poss;
}                               // show_poss_play_cursor

/**
 * TBD
 * 
 * @param signal_widget TBD
 * @param event TBD
 * @param data TBD
 * @return TBD
 */
gint main_wnd_motion( GtkWidget * signal_widget, GdkEventMotion * event, gpointer data ) {
    int x, y, r, c;
    GdkCursor *cursor;

    /*
     * --- Convert mouse position to board position [1..8] --- 
     */
    x = ( int ) event->x;
    y = ( int ) event->y;
    c = ( x - board_xstart + cell_width ) / cell_width;
    r = ( y - board_ystart + cell_height ) / cell_height;

    /*
     * --- Set the cursor depending if current player can play or not here --- 
     */
    if ( ( c < 1 ) || ( c > 8 ) || ( r < 1 ) || ( r > 8 ) )
        cursor = gdk_cursor_new( GDK_TOP_LEFT_ARROW );
    else
        cursor = gdk_cursor_new( ( state_selectable[r - 1][c - 1] ) ? GDK_CENTER_PTR : GDK_X_CURSOR );
    gdk_window_set_cursor( signal_widget->window, cursor );
    gdk_cursor_destroy( cursor );

    return TRUE;

}                               // main_wnd_motion

static int transient_sz = 0;
static int transient_pos[60];

/**
 * TBD
 * 
 * @param signal_widget TBD
 * @param event TBD
 * @param data TBD
 * @return TBD
 */
int main_wnd_press( GtkWidget *signal_widget, GdkEventButton *event, gpointer data ) {
    int x, y, r, c;
    PCELL ptr1, ptr2;
    CELL possib[256];
    int nb_poss, nb_took;
    int valid_move;
    int k, xy;
    PCELL p;
    char hist_buff[80], *pitem[1];
    GList *list;
    int hist_nb, hist_sel;
    time_t t_end;

    /*
     * --- Make sure it's a button event --- 
     */
    if ( event->type != GDK_BUTTON_PRESS )
        return FALSE;

    /*
     * --- Convert mouse position to board position [1..8] --- 
     */
    x = ( int ) event->x;
    y = ( int ) event->y;
    c = ( x - board_xstart + cell_width ) / cell_width;
    r = ( y - board_ystart + cell_height ) / cell_height;
    if ( ( c < 1 ) || ( c > 8 ) || ( r < 1 ) || ( r > 8 ) )
        return FALSE;

    /*
     * --- Take not allowed ? --- 
     */
    if ( which_show_poss != code_human )
        return FALSE;

    /*
     * --- Show the enclosed enemy pawns --- 
     */
    ptr1 = ptr2 = &possib[0];
    if ( !( nb_poss = do_build_possib( &ptr2, code_human, code_computer ) ) )
        return FALSE;
    transient_sz = 0;
    for ( valid_move = FALSE, p = ptr1, p++; p != ptr2; ) {
        if ( *p++ == &board[XY( r, c )] ) {
            draw_pawn( board_drawing_area->window, r, c, code_human );
            if ( event->button != 1 )
                transient_pos[transient_sz++] = XY( r, c );
            else
                board[XY( r, c )] = code_human;
            nb_took = ( int ) *p++;
            memmove( cancel_last_human_move, &p[-2], ( nb_took + 2 ) * sizeof( void * ) );
            for ( k = 0; k < nb_took; k++ ) {
                xy = ( long unsigned int ) *p - ( long unsigned int ) board;
                draw_pawn( board_drawing_area->window, xy / 10, xy % 10, code_human );
                if ( event->button != 1 )
                    transient_pos[transient_sz++] = xy;
                else
                    *( unsigned char * ) *p++ = code_human;
            }
            valid_move = TRUE;
            break;
        }
        nb_took = ( int ) *p++;
        p += nb_took;
    }                           // for (p)
    if ( !valid_move )
        return FALSE;

    /*
     * --- Recompute the number of pawns --- 
     */
    compute_nb_pawns( TRUE );

    /*--- Update the list of move ---*/
    if ( event->button == 1 ) {
        list = GTK_CLIST( hist_list )->selection;
        hist_sel = ( list ) ? ( int ) list->data : -1;
        hist_nb = GTK_CLIST( hist_list )->rows;
        if ( hist_sel + 1 != hist_nb ) {
            int cnt;

            // Remove all elements after the one selected in the list
            for ( cnt = hist_sel + 1; cnt < hist_nb; cnt++ )
                gtk_clist_remove( GTK_CLIST( hist_list ), hist_sel + 1 );
        }
        sprintf( hist_buff, "%2d: %s: %c%d", hist_sel + 2, lib_colors( code_human ), c + '@', r );
        pitem[0] = hist_buff;
        gtk_signal_handler_block( GTK_OBJECT( hist_list ), select_hist_handler_id );
        gtk_clist_append( GTK_CLIST( hist_list ), pitem );
        gtk_clist_select_row( GTK_CLIST( hist_list ), GTK_CLIST( hist_list )->rows - 1, 0 );
        gtk_signal_handler_unblock( GTK_OBJECT( hist_list ), select_hist_handler_id );
    }

    /*--- Update the time took by the player ---*/
    if ( event->button == 1 ) {
        time( &t_end );
        if ( code_human == CODE_BLACK )
            ctime_black += ( ( unsigned long ) t_end - ( unsigned long ) t_start );
        else
            ctime_white += ( ( unsigned long ) t_end - ( unsigned long ) t_start );
        show_horo(  );
    }

    /*--- Show the possibilities for the computer ---*/
    if ( event->button == 1 ) {
        if ( ( nb_poss = show_poss_play_cursor( code_computer, code_human ) ) == 0 ) {
            ptr2 = &possib[0];
            if ( do_build_possib( &ptr2, code_human, code_computer ) == 0 ) {
                diag_end_game(  );
                return valid_move;
            }
        }
    }

    /*--- Message: where human played ---*/

    /*--- How many possibilities for the computer ? ---*/
    if ( event->button == 1 ) {
        if ( nb_poss == 0 ) {
            color_to_play = code_human;
            computer_infos_msg( "No possibility left for computer. Must pass..." );
            nb_poss = show_poss_play_cursor( code_human, code_computer );
            player_infos_msg
               ( "%d possibilit%s. Choose a position and click to play", nb_poss, ( nb_poss == 1 ) ? "y" : "ies" );
        }
        else {
            color_to_play = code_computer;
            player_infos_msg( "You have played in %c%d", c + '@', r );
        }
    }

    /*--- Enable the menu option 'Save the game' ---*/
    if ( event->button == 1 )
        enable_SaveGame( TRUE );

    /*--- Let's play computer now (1st button only) ---*/
    if ( event->button == 1 )
        computer_turn(  );

    /*--- Done ---*/
    return valid_move;

}                               // main_wnd_press

/**
 * TBD
 * 
 * @param signal_widget TBD
 * @param event TBD
 * @param data TBD
 * @return TBD
 */
int main_wnd_release( GtkWidget *signal_widget, GdkEvent *event, gpointer data ) {
    int n, xy;

    for ( n = 0; n < transient_sz; n++ ) {
        xy = transient_pos[n];
        draw_pawn( board_drawing_area->window, xy / 10, xy % 10, ( n == 0 ) ? CODE_EMPTY : code_computer );
    }

    transient_sz = 0;
    return FALSE;
}                               // main_wnd_release

/**
 * TBD
 */
void computer_turn( void ) {
    WHERE_TO_PLAY where_to_play;
    PCELL ptr1, ptr2;
    PCELL p;
    CELL possib[256];
    int nb_poss, nb_took;
    int k;
    int valid_move;
    char *pitem[1], hist_buff[80];
    GList *list;
    int hist_nb, hist_sel;
    time_t t_end;

    /*--- Verify this is the turn of computer ---*/
    if ( color_to_play == code_human )
        return;

    /*--- The computer plays... ---*/
    time( &t_start );
    computer_play( &where_to_play, code_computer, code_human, FALSE );
    set_progressbar( 0, 1 );

    time( &t_end );
    if ( code_computer == CODE_BLACK )
        ctime_black += ( ( unsigned long ) t_end - ( unsigned long ) t_start );
    else
        ctime_white += ( ( unsigned long ) t_end - ( unsigned long ) t_start );
    show_horo(  );
    time( &t_start );           // Time when player starts thinking

    /*--- Show the enclosed enemy pawns ---*/
    ptr1 = ptr2 = &possib[0];
    if ( ( nb_poss = do_build_possib( &ptr2, code_computer, code_human ) ) == 0 )
        return;
    for ( valid_move = FALSE, p = ptr1, p++; p != ptr2; ) {
        if ( *p++ == &board[XY( where_to_play.x, where_to_play.y )] ) {
            board[XY( where_to_play.x, where_to_play.y )] = code_computer;
            for ( nb_took = ( int ) *p++, k = 0; k < nb_took; k++ )
                *( unsigned char * ) *p++ = code_computer;
            valid_move = TRUE;
            break;
        }
        for ( nb_took = ( int ) *p++, k = 0; k < nb_took; k++ )
            p++;
    }                           // for (p)

    /*--- Update the board ---*/
    if ( !valid_move )
        return;

    /*--- Recount number of pawns ---*/
    compute_nb_pawns( TRUE );
    redraw_board(  );

    /*
     * --- Update the history --- 
     */
    list = GTK_CLIST( hist_list )->selection;
    hist_sel = ( list ) ? ( int ) list->data : -1;
    hist_nb = GTK_CLIST( hist_list )->rows;
    if ( hist_sel + 1 != hist_nb ) {
        int cnt;

        // Remove all elements after the one selected in the list
        for ( cnt = hist_sel + 1; cnt < hist_nb; cnt++ )
            gtk_clist_remove( GTK_CLIST( hist_list ), hist_sel + 1 );
    }
    sprintf( hist_buff, "%2d: %s: %c%d", hist_sel + 2, lib_colors( code_computer ), where_to_play.y + '@', where_to_play.x );
    pitem[0] = hist_buff;
    gtk_clist_undo_selection( GTK_CLIST( hist_list ) );
    gtk_signal_handler_block( GTK_OBJECT( hist_list ), select_hist_handler_id );
    gtk_clist_append( GTK_CLIST( hist_list ), pitem );
    gtk_clist_select_row( GTK_CLIST( hist_list ), GTK_CLIST( hist_list )->rows - 1, 0 );
    gtk_signal_handler_unblock( GTK_OBJECT( hist_list ), select_hist_handler_id );

    /*--- End-of-Game ? ---*/
    if ( ( nb_poss = show_poss_play_cursor( code_human, code_computer ) ) == 0 ) {
        player_infos_msg( "Sorry, no possibility left for you. You pass your turn..." );
        ptr2 = &possib[0];
        if ( do_build_possib( &ptr2, code_computer, code_human ) == 0 ) {
            diag_end_game(  );
            return;
        }
        warning_msg( "You pass your turn!" );
        color_to_play = code_computer;
        computer_turn(  );
    }
    else {
        player_infos_msg( "%d possibilit%s. Choose a position and click to play", nb_poss, ( nb_poss == 1 ) ? "y" : "ies" );
        color_to_play = code_human;
    }

}                               // computer_turn

/**
 * TBD
 * 
 * @param nb TBD
 * @return TBD
 */
static char *str_nb( int nb ) {
    static char str[80];
    int l, k, w;
    char *p, *q, tmp[80];

    if ( nb == 0 )
        nb = 1;
    sprintf( tmp, "%d", nb );
    if ( ( l = strlen( tmp ) ) <= 3 )
        return strcpy( str, tmp );

    w = l % 3;
    for ( p = str, q = tmp, k = 0; k < l; k++ ) {
        if ( k && ( k % 3 == w ) )
            *p++ = ',';
        *p++ = *q++;
    }
    *p = 0;
    return str;
}                               // str_nb

/**
 * TBD
 * 
 * @param where_to_play TBD
 * @param the_player TBD
 * @param the_enemy TBD
 * @param f_suggest TBD
 */
void computer_play( WHERE_TO_PLAY * where_to_play,
   const unsigned char the_player, const unsigned char the_enemy, const int f_suggest ) {
    GdkCursor *cursor;
    int total_nb_pawns;
    int nb_eog;
    char msg[80];

    /*
     * --- Change cursor while thinking --- 
     */
    computer_infos_msg( "Computer is thinking..." );
    cursor = gdk_cursor_new( GDK_WATCH );
    gdk_window_set_cursor( main_wnd->window, cursor );
    gdk_window_set_cursor( board_drawing_area->window, cursor );
    gdk_cursor_destroy( cursor );
    install_motion_handler( FALSE );
    gdk_flush(  );

    /*--- How many pawns remainding ? ---*/
    total_nb_pawns = nb_pawns[CODE_BLACK] + nb_pawns[CODE_WHITE];

    /*--- End of game ---*/
    switch ( config.level ) {
        case LEVEL1:
            nb_eog = 64 - 5;
            break;
        case LEVEL2:
            nb_eog = 64 - 7;
            break;
        case LEVEL3:
            nb_eog = 64 - 9;
            break;
        case LEVEL4:
        default:
            nb_eog = 64 - 12;
            break;
        case LEVEL5:
            nb_eog = 64 - 13;
            break;
    }                           // switch

    /*--- Call the routine to compute where to play ---*/
    if ( total_nb_pawns >= nb_eog )
        do_end_game( the_player, the_enemy, where_to_play );
    else
        do_game( the_player, the_enemy, where_to_play );

    /*
     * --- Restore normal cursor --- 
     */
    cursor = gdk_cursor_new( GDK_TOP_LEFT_ARROW );
    gdk_window_set_cursor( main_wnd->window, cursor );
    gdk_window_set_cursor( board_drawing_area->window, cursor );
    gdk_cursor_destroy( cursor );
    install_motion_handler( TRUE );

    /*--- Show where to play the 'best' position ---*/
    sprintf( msg, "%s %sconfigurations examined in %d sec",
       str_nb( where_to_play->cnt ),
       ( total_nb_pawns >= nb_eog ) ? "exhaustive " : "", MAX( ( int ) where_to_play->elapsed, 1 ) );
    if ( where_to_play->x == -1 ) {
        computer_infos_msg( "%s. Computer must pass!", msg );
    }
    else {
        if ( !f_suggest ) {
            computer_infos_msg( "%s. Computer plays %c%d", msg, where_to_play->y + '@', where_to_play->x );
        }
        else {
            if ( the_player == code_computer ) {
                computer_infos_msg( "%s. Computer would play in %c%d", msg, where_to_play->y + '@', where_to_play->x );
            }
            else {
                player_infos_msg( "%s. Your 'best' move would be %c%d", msg, where_to_play->y + '@', where_to_play->x );
            }
        }
    }

}                               // computer_play

/**
 * TBD
 */
void suggest_move( void ) {
    WHERE_TO_PLAY where_to_play;

    /*--- The computer plays... ---*/
    computer_play( &where_to_play, color_to_play, ( CODE_BLACK + CODE_WHITE ) - color_to_play, TRUE );

    /*--- Clear progress bar ---*/
    set_progressbar( 0, 1 );
    computer_infos_msg( " " );

}                               // suggest_move

/**
 * TBD
 */
void change_side( void ) {
    int tmp, nb_poss;

    /*--- Stop the current clock ---*/
    computer_infos_msg( " " );
    player_infos_msg( " " );
    //      stop_count_time();

    /*--- By default, computer has black, play has white ---*/
    tmp = code_computer;
    code_computer = code_human;
    code_human = tmp;

    /*--- Update status ---*/
    set_owners(  );
    show_nb_pawns(  );

    /*--- Which turn? ---*/
    if ( color_to_play == code_computer ) {
        computer_turn(  );
    }
    else {
        nb_poss = show_poss_play_cursor( code_human, code_computer );
        player_infos_msg( "%d possibilit%s. Choose a position and click to play", nb_poss, ( nb_poss == 1 ) ? "y" : "ies" );
    }

}                               // change_side
