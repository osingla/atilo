/**
 * @file game.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

/**
 * TBD
 * 
 * @return TBD
 */
int max_level_depth( void ) {
    static int levels[MAX_LEVELS - 1][MAX_STRATEGIES - 1] = {
        {4, 4, 4, 4, 4, 4},     // Level #1
        {6, 6, 6, 6, 6, 6},     // Level #2
        {8, 8, 8, 8, 8, 7},     // Level #3
        {10, 10, 10, 10, 9, 9}, // Level #4
        {11, 11, 11, 11, 10, 10}    // Level #5
    };
    return levels[config.level - 1][config.strategy - 1];
}                               // max_level_depth

/**
 * TBD
 * 
 * @param _s1 TBD
 * @param _s2 TBD
 * @return TBD
 */
static inline int sort_scores_black( const void *_s1, const void *_s2 ) {
    const SCORE_TO_SORT *s1 = ( const SCORE_TO_SORT * ) _s1;
    const SCORE_TO_SORT *s2 = ( const SCORE_TO_SORT * ) _s2;
    int val;

    val = s1->score - s2->score;
    if ( val > 0 )
        return -1;
    else if ( val < 0 )
        return 1;
    return 0;

}                               // sort_scores_black

/**
 * TBD
 * 
 * @param _s1 TBD
 * @param _s2 TBD
 * @return TBD
 */
static inline int sort_scores_white( const void *_s1, const void *_s2 ) {
    const SCORE_TO_SORT *s1 = ( const SCORE_TO_SORT * ) _s1;
    const SCORE_TO_SORT *s2 = ( const SCORE_TO_SORT * ) _s2;
    int val;

    val = s1->score - s2->score;
    if ( val < 0 )
        return -1;
    else if ( val > 0 )
        return 1;
    return 0;

}                               // sort_scores_white

/*--- Used only by PLAY_CELL and UNPLAY_CELL : OPTIMIZATION ([ebp...] slow!] ---*/
static long unsigned int *play_p;
static int play_k;

#define PLAY_CELL( ptr0, the_player ) \
 \
	play_p = (long unsigned int *)ptr0; \
	*(unsigned char *)*play_p++ = (unsigned char )the_player; \
 \
	for ( play_k = (int)*play_p++; play_k ; play_k-- ) \
		*(unsigned char *)*play_p++ = (unsigned char )the_player;

#define UNPLAY_CELL( ptr0, the_enemy ) \
 \
	play_p = (long unsigned int *)ptr0; \
	*(unsigned char *)*play_p++ = CODE_EMPTY; \
 \
	for ( play_k = (int)*play_p++; play_k ; play_k-- ) \
		*(unsigned char *)*play_p++ = the_enemy;

/**
 * TBD
 * 
 * @param depth TBD
 * @param curr_ptr TBD
 * @param prev_pass TBD
 * @param black_best_coef TBD
 * @param white_best_coef TBD
 * @param the_player TBD
 * @param the_enemy TBD
 * @return TBD
 */
static inline int minimax( const int depth,
	PCELL curr_ptr, const int prev_pass, int black_best_coef, int white_best_coef, int the_player, int the_enemy ) {
    PCELL ptr1, ptr2, p, p0;
    int nb_poss, nb_took;
    int index;
    int skip;
    int curr_coef, best_coef;
    SCORE_TO_SORT scores_to_sort[33], *score_to_sort;
    PCELL dirs[33], *pdir;

    /*--- --------------------- ---*/

    /*--- Terminal Evaluation ? ---*/

    /*--- --------------------- ---*/

    if ( depth == max_depth ) {
        cnt++;
//              if ( f_refresh_time && (cnt%10000 == 1) )
//                      do_refresh_time();
        switch ( config.strategy ) {
            case STRATEGY1:
                return _ponderation1( the_player, the_enemy );
            case STRATEGY2:
                return _ponderation2( the_player, the_enemy );
            case STRATEGY3:
                return _ponderation3( the_player, the_enemy );
            case STRATEGY4:
                return _ponderation4( the_player, the_enemy );
        }                       // switch
    }                           // if


    /*--- ---------------------------------------------------------- ---*/

    /*--- Gener all the possibilities of game for the current player ---*/

    /*--- ---------------------------------------------------------- ---*/

    ptr1 = ptr2 = curr_ptr;
    nb_poss = do_build_possib( &ptr2, the_player, the_enemy );


    /*--- ---------------- ---*/

    /*--- No possibility ? ---*/

    /*--- ---------------- ---*/

    if ( !nb_poss ) {

        /*--- The two players passes, the game is over ---*/
        if ( prev_pass ) {
            if ( nb_pawns[CODE_BLACK] == nb_pawns[CODE_WHITE] )
                return 0;
            else if ( nb_pawns[CODE_BLACK] > nb_pawns[CODE_WHITE] )
                return INT_MAX;
            else
                return INT_MIN;
        }                       // if

        /*--- Game is not over, gnark gnark... ---*/
        return minimax( depth + 1, ptr2, TRUE, black_best_coef, white_best_coef, the_enemy, the_player );
    }                           // if


    /*--- ------------------------------------------------------ ---*/

    /*--- Evaluate each move, in order to sort the list of moves ---*/

    /*--- ------------------------------------------------------ ---*/

    if ( depth < ( max_depth - 1 ) )    // Dont't need to sort the last move before ponderation
        for ( score_to_sort = scores_to_sort, index = 0, p = ptr1, p++; p != ptr2; index++, score_to_sort++ ) {

            /*--- Play this position ---*/
            p0 = p++;
            PLAY_CELL( p0, the_player );
            nb_pawns[the_player] += ( ( nb_took = ( int ) *p++ ) + 1 );
            nb_pawns[the_enemy] -= nb_took;

            /*--- Give a 'note' to the move ---*/
            score_to_sort->index = index;
            switch ( config.strategy ) {
                case STRATEGY1:
                    score_to_sort->score = _ponderation1( the_player, the_enemy );
                    break;
                case STRATEGY2:
                    score_to_sort->score = _ponderation2( the_player, the_enemy );
                    break;
                case STRATEGY3:
                    score_to_sort->score = _ponderation3( the_player, the_enemy );
                    break;
                case STRATEGY4:
                    score_to_sort->score = _ponderation4( the_player, the_enemy );
                    break;
            }                   // switch

            /*--- Unplay this position ---*/
            UNPLAY_CELL( p0, the_enemy );
            nb_pawns[the_player] -= ( nb_took + 1 );
            nb_pawns[the_enemy] += nb_took;

            /*--- Next possibility for this player ---*/
            p += nb_took;

        }                       // for (p)


    /*--- ---------------------------- ---*/

    /*--- Then sort this list of moves ---*/

    /*--- ---------------------------- ---*/

    if ( depth < ( max_depth - 1 ) ) {

        /*--- 1a) Sort the list of possibles moves by their coef ---*/
        if ( the_player == CODE_BLACK )
            qsort( scores_to_sort, nb_poss, sizeof( SCORE_TO_SORT ), sort_scores_black );
        else
            qsort( scores_to_sort, nb_poss, sizeof( SCORE_TO_SORT ), sort_scores_white );

    }                           // if
    else {

        /*--- 1b) Create the list, not ordered ---*/
        for ( score_to_sort = scores_to_sort, index = 0, p = ptr1, p++; p != ptr2; index++, score_to_sort++ ) {
            p++;
            nb_took = ( int ) *p++;
            score_to_sort->index = index;
            p += nb_took;
        }                       // for (p)

    }                           // else


    /*--- 2) Create a 'directory' of each possible move ---*/
    for ( pdir = dirs, p = ptr1, p++; p != ptr2; ) {
        *pdir++ = p++;
        nb_took = ( int ) *p++;
        p += nb_took;
    }


    /*--- --------------------------------- ---*/

    /*--- Initialize alpha-beta coeficients ---*/

    /*--- --------------------------------- ---*/

    if ( the_player == CODE_BLACK )
        best_coef = INT_MIN;    // Black maximize coefs.
    else
        best_coef = INT_MAX;    // White minimize coefs.


    /*--- ------------------------------------- ---*/

    /*--- For each (sorted) possibility of play ---*/

    /*--- ------------------------------------- ---*/

    for ( skip = FALSE, score_to_sort = scores_to_sort, index = nb_poss; ( index ) && ( !skip ); index--, score_to_sort++ ) {

        /*--- Update the progress at level #0 ---*/
        if ( !depth )
            set_progressbar( nb_poss - index + 1, nb_poss );

        /*--- Play this position ---*/
        p = dirs[score_to_sort->index];
        p0 = p++;
        PLAY_CELL( p0, the_player );
        nb_pawns[the_player] += ( ( nb_took = ( int ) *p++ ) + 1 );
        nb_pawns[the_enemy] -= nb_took;

        curr_coef = minimax( depth + 1, ptr2, FALSE, black_best_coef, white_best_coef, the_enemy, the_player );

        /*--- Mini-Max Algorithm ---*/
        if ( the_player == CODE_BLACK ) {
            if ( curr_coef > best_coef ) {
                if ( !depth )
                    chosen_move = ( int ) *p0;
                if ( ( best_coef = curr_coef ) > black_best_coef ) {
                    if ( best_coef >= white_best_coef ) // Alpha-Beta Algorithm
                        skip = TRUE;
                    else
                        black_best_coef = best_coef;
                }
            }
        }
        else {
            if ( curr_coef < best_coef ) {
                if ( !depth )
                    chosen_move = ( int ) *p0;
                if ( ( best_coef = curr_coef ) < white_best_coef ) {
                    if ( best_coef <= black_best_coef ) // Alpha-Beta Algorithm
                        skip = TRUE;
                    else
                        white_best_coef = best_coef;
                }
            }
        }

        /*--- Unplay this position ---*/
        UNPLAY_CELL( p0, the_enemy );
        nb_pawns[the_player] -= ( nb_took + 1 );
        nb_pawns[the_enemy] += nb_took;

    }                           // for (p)

    /*--- Best coef. for this node is ... ---*/
    return best_coef;

}                               // minimax

#undef PLAY_CELL
#undef UNPLAY_CELL

/**
 * TBD
 * 
 * @param the_player TBD
 * @param the_enemy TBD
 * @param where_to_play
 */
void do_game( const unsigned char the_player, const unsigned char the_enemy, WHERE_TO_PLAY *where_to_play ) {
    time_t t1, t2;

    /*--- Select the max level ---*/
    max_depth = max_level_depth(  );
    if ( nb_pawns[CODE_BLACK] + nb_pawns[CODE_WHITE] >= 44 )
        max_depth++;

    /*--- Initial time ---*/
    time( &t1 );

    /*--- Refresh the time is player is the computer ---*/
//  f_refresh_time = (the_player == code_computer) ? TRUE : FALSE;

    /*
     * --- Minimax + Alpha/Beta --- 
     */
    cnt = 0;
    minimax( 0, &possib[0], FALSE, INT_MIN, INT_MAX, the_player, the_enemy );

    /*--- Fill the where-to-play structure ---*/
    time( &t2 );
    where_to_play->elapsed = ( time_t ) difftime( t2, t1 );
    where_to_play->x = ( ( chosen_move - ( long unsigned int ) board ) / 10 );
    where_to_play->y = ( ( chosen_move - ( long unsigned int ) board ) % 10 );
    where_to_play->cnt = cnt;

}                               //      do_game
