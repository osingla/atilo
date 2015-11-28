/**
 * @file layout.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef LAYER_H_
#define LAYER_H_

void install_key_press_handler( const int enable );
void install_motion_handler( const int enable );
void expose_init( void );
void compute_size_elements_board( void );
void draw_pawn( GdkDrawable * drawable, const int r, const int c, const int pawn );
void redraw_board( void );
void draw_board( GdkDrawable * drawable );
void create_layout( void );
void computer_infos_msg( const char *fmt, ... );
void player_infos_msg( const char *fmt, ... );

#endif                         /*LAYER_H_ */
