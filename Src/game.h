/**
 * @file game.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef GAME_H_
#  define GAME_H_

/* game.c */
int max_level_depth( void );
void do_game( const unsigned char the_player, const unsigned char the_enemy, WHERE_TO_PLAY * where_to_play );

#endif                         /*GAME_H_ */
