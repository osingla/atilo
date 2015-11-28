/**
 * @file menu.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef MENU_H_
#  define MENU_H_

/* menu.c */
void on_New_game_activate( GtkWidget * w, gpointer data );
void on_Save_game_activate( GtkWidget * w, gpointer data );
void on_Load_game_activate( GtkWidget * w, gpointer data );
void on_Edit_game_activate( GtkWidget * w, gpointer data );
void on_Quit_activate( GtkWidget * w, gpointer data );
void on_Change_side_activate( GtkWidget * w, gpointer data );
void on_SuggestMove_activate( GtkWidget * w, gpointer data );
void on_Settings_activate( GtkWidget * w, gpointer data );
void on_Rules_of_Reversi_activate( GtkWidget * w, gpointer data );
void on_About_atilo_activate( GtkWidget * w, gpointer data );
void enable_NewGame( const int flag );
void enable_SaveGame( const int flag );
void enable_ChangeSide( const int flag );
void enable_SuggestMove( const int flag );
void create_main_menu( GtkWidget * vbox );

#endif                         /*MENU_H_ */
