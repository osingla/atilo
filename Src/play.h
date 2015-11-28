/**
 * @file play.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef PLAY_H_
#  define PLAY_H_

/* play.c */
void compute_nb_pawns( const int show );
int show_poss_play_cursor( const unsigned char cplayer, const unsigned char cenemy );
gint main_wnd_motion( GtkWidget * signal_widget, GdkEventMotion * event, gpointer data );
int main_wnd_press( GtkWidget * signal_widget, GdkEventButton * event, gpointer data );
int main_wnd_release( GtkWidget * signal_widget, GdkEvent * event, gpointer data );
void computer_turn( void );
void computer_play( WHERE_TO_PLAY * where_to_play, const unsigned char the_player, const unsigned char the_enemy,
   const int f_suggest );
void suggest_move( void );
void change_side( void );

#endif                         /*PLAY_H_ */
