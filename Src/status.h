/**
 * @file status.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef STATUS_H_
#  define STATUS_H_

/* status.c */
void create_status( void );
void set_progressbar( const int val, const int max_val );
void show_nb_pawns( void );
void set_owners( void );
void show_horo( void );

#endif                         /*STATUS_H_ */
