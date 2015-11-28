/**
 * @file endgame.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

/*--- Used only by PLAY_CELL and UNPLAY_CELL : OPTIMIZATION ([ebp...] slow!] ---*/
static long unsigned int *play_p;
static int play_k;
static int play_pos_xy;

#define PLAY_CELL( ptr0, the_player ) \
 \
	play_p = (long unsigned int *)ptr0; \
	*(unsigned char *)*play_p++ = (unsigned char)the_player; \
 \
	for ( play_k = (int)*play_p++; play_k; play_k-- ) \
		*(unsigned char *)*play_p++ = (unsigned char )the_player;


#define UNPLAY_CELL( ptr0, the_enemy ) \
 \
	play_p = (long unsigned int *)ptr0; \
	*(unsigned char *)*play_p++ = (unsigned char)CODE_EMPTY; \
 \
	for ( play_k = (int)*play_p++; play_k; play_k-- ) \
		*(unsigned char *)*play_p++ = (unsigned char)the_enemy;


/**
 * TBD
 * 
 * @param depth TBD
 * @param curr_ptr TBD
 * @param the_player
 * @param the_enemy
 * @return TBD
 */
static inline int do_scan_end_game( const int depth, PCELL curr_ptr, const int the_player, const int the_enemy ) {
    PCELL ptr1, ptr2;
    PCELL p, p0;
    int nb_poss, nb_took;
    int coef, coef_best;

    /*--- Gener all the possibilities of game for the current player ---*/
    ptr1 = ptr2 = curr_ptr;
    nb_poss = do_build_possib( &ptr2, the_player, the_enemy );

    /*--- Count the number of cells analysed ---*/
    cnt += nb_poss;

    /*--- End of analyse if : (1) none player can play ---*/

    /*---                     (2) board is full        ---*/
    if ( !nb_poss ) {

        ptr2 = curr_ptr;
        if ( !do_build_possib( &ptr2, the_enemy, the_player ) )
            return 0;

        ptr2 = curr_ptr;
        return -do_scan_end_game( depth + 1, ptr2, the_enemy, the_player );

    }                           // if

    /*--- Initialize coeficient ---*/
    coef_best = INT_MIN;

    /*--- For each possibility of play ---*/
    for ( p = ptr1, p++; p != ptr2; ) {

        /*--- Play this position ---*/
        p0 = p++;
        PLAY_CELL( p0, the_player );
        nb_pawns[the_player] += ( ( nb_took = ( int ) *p++ ) + 1 );
        nb_pawns[the_enemy] -= nb_took;

        /*--- Check all possibilities one level depther ---*/
        if ( nb_pawns[CODE_WHITE] + nb_pawns[CODE_BLACK] == 64 ) {
            coef = nb_took;
//                      if ( f_refresh_time && (cnt%10000 == 1) )
//                              do_refresh_time();
        }
        else
            coef = nb_took - do_scan_end_game( depth + 1, ptr2, the_enemy, the_player );

        /*--- Update coeficient ---*/
        if ( coef > coef_best ) {
            coef_best = coef;
            if ( !depth )
                play_pos_xy = ( int ) *p0;
        }                       // if

        /*--- Unplay this position ---*/
        UNPLAY_CELL( p0, the_enemy );
        nb_pawns[the_player] -= ( nb_took + 1 );
        nb_pawns[the_enemy] += nb_took;

        /*--- Next possibility for this player ---*/
        p += nb_took;

    }                           // for (p)

    return coef_best;

}                               // do_scan_end_game

#undef PLAY_CELL
#undef UNPLAY_CELL

/**
 * TBD
 * 
 * @param the_player TBD
 * @param the_enemy TBD
 * @param where_to_play TBD
 */
void do_end_game( const unsigned char the_player, const unsigned char the_enemy, WHERE_TO_PLAY * where_to_play ) {
    time_t t1, t2;
    int coef_best;

    /*--- Initial time ---*/
    time( &t1 );

    /*--- Refresh the time is player is the computer ---*/
//      f_refresh_time = (the_player == code_computer) ? true : false;

    cnt = 0;
    coef_best = do_scan_end_game( 0, &possib[0], the_player, the_enemy );

    /*--- Fill the where-to-play structure ---*/
    time( &t2 );
    where_to_play->elapsed = ( time_t ) difftime( t2, t1 );
    if ( ( where_to_play->coef_best = coef_best ) == INT_MIN ) {
        where_to_play->x = where_to_play->y = -1;
    }
    else {
        where_to_play->x = ( ( play_pos_xy - ( unsigned int ) board ) / 10 );
        where_to_play->y = ( ( play_pos_xy - ( unsigned int ) board ) % 10 );
    }
    where_to_play->cnt = cnt;
}                               // do_end_game_...
