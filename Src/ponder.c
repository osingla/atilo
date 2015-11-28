/**
 * @file ponder.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

static int note;

/**
 * TBD
 */
static void test_corners( void ) {

    /*--- Bords occupes ? ---*/
    if ( board[11] == CODE_BLACK )
        note += 1000000;
    else if ( board[11] == CODE_WHITE )
        note -= 1000000;
    if ( board[18] == CODE_BLACK )
        note += 1000000;
    else if ( board[18] == CODE_WHITE )
        note -= 1000000;
    if ( board[81] == CODE_BLACK )
        note += 1000000;
    else if ( board[81] == CODE_WHITE )
        note -= 1000000;
    if ( board[88] == CODE_BLACK )
        note += 1000000;
    else if ( board[88] == CODE_WHITE )
        note -= 1000000;
}

/**
 * TBD
 */
static void
test_around_corners( void ) {

    /*--- Sous-bords occupes ? ---*/
    if ( !board[11] ) {
        if ( board[22] == CODE_BLACK )
            note -= 100000;
        else if ( board[22] == CODE_WHITE )
            note += 100000;
        if ( board[12] == CODE_BLACK )
            note -= 100;
        else if ( board[12] == CODE_WHITE )
            note += 100;
        if ( board[21] == CODE_BLACK )
            note -= 100;
        else if ( board[21] == CODE_WHITE )
            note += 100;
    }
    if ( !board[18] ) {
        if ( board[27] == CODE_BLACK )
            note -= 100000;
        else if ( board[27] == CODE_WHITE )
            note += 100000;
        if ( board[17] == CODE_BLACK )
            note -= 100;
        else if ( board[17] == CODE_WHITE )
            note += 100;
        if ( board[28] == CODE_BLACK )
            note -= 100;
        else if ( board[28] == CODE_WHITE )
            note += 100;
    }
    if ( !board[81] ) {
        if ( board[72] == CODE_BLACK )
            note -= 100000;
        else if ( board[72] == CODE_WHITE )
            note += 100000;
        if ( board[71] == CODE_BLACK )
            note -= 100;
        else if ( board[71] == CODE_WHITE )
            note += 100;
        if ( board[82] == CODE_BLACK )
            note -= 100;
        else if ( board[82] == CODE_WHITE )
            note += 100;
    }
    if ( !board[88] ) {
        if ( board[77] == CODE_BLACK )
            note -= 100000;
        else if ( board[77] == CODE_WHITE )
            note += 100000;
        if ( board[78] == CODE_BLACK )
            note -= 100;
        else if ( board[78] == CODE_WHITE )
            note += 100;
        if ( board[87] == CODE_BLACK )
            note -= 100;
        else if ( board[87] == CODE_WHITE )
            note += 100;
    }

}                               // test_around_corners

/**
 * Strategy #1 : Juste take care of corners and sub-corners
 * 
 * @param the_player TBD
 * @param the_enemy TBD
 * @return TBD
 */
int _ponderation1( const int the_player, const int the_enemy ) {

    note = 0;
    test_corners(  );
    test_around_corners(  );

    return note;
}                               // _ponderation1

/**
 * Strategy #2 : Take care of corners and sub-corners AND 
 * Computer tries to maximize his number of pawns		
 * 
 * @param the_player TBD
 * @param the_enemy TBD
 * @return TBD
 */
int _ponderation2( const int the_player, const int the_enemy ) {

    note = nb_pawns[CODE_BLACK] - nb_pawns[CODE_WHITE];
    test_corners(  );
    test_around_corners(  );

    return note;
}                               // _ponderation2

/**
 * Strategy #3 : Take care of corners and sub-corners AND 
 * Computer tries to minimize his number of pawns		
 * 
 * @param the_player TBD
 * @param the_enemy TBD
 * @return TBD
 */
int _ponderation3( const int the_player, const int the_enemy ) {

    note = nb_pawns[CODE_WHITE] - nb_pawns[CODE_BLACK];
    test_corners(  );
    test_around_corners(  );

    return note;

}                               // _ponderation3

/**
 * TBD		
 * 
 * @param the_player TBD
 * @param the_enemy TBD
 * @return TBD
 */
int _ponderation4( const int the_player, const int the_enemy ) {
    int nb_poss;

    /*--- Gener all the possibilities of game for the next player ---*/
    nb_poss = do_nb_possib( the_enemy, the_player );
    if ( the_player == CODE_BLACK )
        note = -nb_poss;
    else
        note = nb_poss;

    test_corners(  );
    test_around_corners(  );

    return note;

}                               // _ponderation4
