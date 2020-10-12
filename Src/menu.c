/**
 * @file menu.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

static GtkWidget *New_game;
static GtkWidget *Save_game;
static GtkWidget *Change_side;
static GtkWidget *Suggest_move;

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_New_game_activate( GtkWidget * w, gpointer data ) {
    init_new_game( TRUE, TRUE );
}                               // on_New_game_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Save_game_activate( GtkWidget * w, gpointer data ) {
    file_save_game(  );
}                               // on_Save_game_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Load_game_activate( GtkWidget * w, gpointer data ) {
    file_load_game(  );
}                               // on_Load_game_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Edit_game_activate( GtkWidget * w, gpointer data ) {
    warning_msg( "Feature not yet implemented." );
}                               // on_Edit_game_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Quit_activate( GtkWidget * w, gpointer data ) {
    gtk_widget_destroy( main_wnd );
}                               // on_Quit_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Change_side_activate( GtkWidget * w, gpointer data ) {
    change_side(  );
}                               // on_Change_side_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_SuggestMove_activate( GtkWidget * w, gpointer data ) {
    suggest_move(  );
}                               // on_SuggestMove_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Settings_activate( GtkWidget * w, gpointer data ) {
    dialog_settings(  );
}                               // on_Settings_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_Rules_of_Reversi_activate( GtkWidget * w, gpointer data ) {
    GtkWidget *dlg;

    dlg = gtk_message_dialog_new( GTK_WINDOW( main_wnd ),
       GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Please take a look at\n" "https://rawgit.com/osingla/atilo/gh-pages/index.html" );
    gtk_dialog_run( GTK_DIALOG( dlg ) );
    gtk_widget_destroy( dlg );
}                               // on_Rules_of_Reversi_activate

/**
 * TBD
 * 
 * @param w TBD
 * @param data TBD
 */
void on_About_atilo_activate( GtkWidget * w, gpointer data ) {
    GtkWidget *dlg;

    dlg = gtk_message_dialog_new( GTK_WINDOW( main_wnd ),
       GTK_DIALOG_DESTROY_WITH_PARENT,
       GTK_MESSAGE_INFO,
       GTK_BUTTONS_OK,
       "Atilo v%d.%d.%d\n"
       "https://rawgit.com/osingla/atilo/gh-pages/index.html\n\n"
       "Olivier Singla, Raleigh, NC, USA\n" "olivier.singla@gmail.com", VERSION >> 16, ( VERSION >> 8 ) & 0xFF, VERSION & 0xFF );
    gtk_dialog_run( GTK_DIALOG( dlg ) );
    gtk_widget_destroy( dlg );
}                               // on_About_atilo_activate

/**
 * TBD
 * 
 * @param flag TBD
 */
void enable_NewGame( const int flag ) {
    gtk_widget_set_sensitive( New_game, flag );
}                               // enable_NewGame

/**
 * TBD
 * 
 * @param flag TBD
 */
void enable_SaveGame( const int flag ) {
    gtk_widget_set_sensitive( Save_game, flag );
}                               // enable_SaveGame

/**
 * TBD
 * 
 * @param flag TBD
 */
void enable_ChangeSide( const int flag ) {
    gtk_widget_set_sensitive( Change_side, flag );
}                               // enable_ChangeSide

/**
 * TBD
 * 
 * @param flag TBD
 */
void enable_SuggestMove( const int flag ) {
    gtk_widget_set_sensitive( Suggest_move, flag );
}                               // enable_SuggestMove

/**
 * TBD
 * 
 * @param vbox TBD
 */
void create_main_menu( GtkWidget *vbox ) {
    GtkWidget *menubar1;
    GtkWidget *File;
    GtkWidget *File_menu;
    GtkWidget *Load_game;
    GtkWidget *separator1;
    GtkWidget *Edit_game;
    GtkWidget *separator2;
    GtkWidget *Quit;
    GtkWidget *item3;
    GtkWidget *item3_menu;
    GtkWidget *separator3;
    GtkWidget *item18;
    GtkWidget *item4;
    GtkWidget *item4_menu;
    GtkWidget *Rules_of_Reversi;
    GtkWidget *separator4;
    GtkWidget *About_atilo;
    GtkAccelGroup *accel_group;

    menubar1 = gtk_menu_bar_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "menubar1", menubar1 );
    gtk_widget_show( menubar1 );
    gtk_box_pack_start( GTK_BOX( vbox ), menubar1, FALSE, TRUE, 0 );

    File = gtk_menu_item_new_with_label( "File" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "File", File );
    gtk_widget_show( File );
    gtk_container_add( GTK_CONTAINER( menubar1 ), File );

    File_menu = gtk_menu_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "File_menu", File_menu );
    gtk_menu_item_set_submenu( GTK_MENU_ITEM( File ), File_menu );

    New_game = gtk_menu_item_new_with_label( "New game" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "New_game", New_game );
    gtk_widget_show( New_game );
    gtk_container_add( GTK_CONTAINER( File_menu ), New_game );
    gtk_signal_connect( GTK_OBJECT( New_game ), "activate", GTK_SIGNAL_FUNC( on_New_game_activate ), NULL );
    accel_group = gtk_accel_group_new(  );
    gtk_window_add_accel_group( GTK_WINDOW( main_wnd ), accel_group );
    gtk_widget_add_accelerator( New_game, "activate", accel_group, GDK_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    Save_game = gtk_menu_item_new_with_label( "Save game" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Save_game", Save_game );
    gtk_widget_show( Save_game );
    gtk_container_add( GTK_CONTAINER( File_menu ), Save_game );
    gtk_signal_connect( GTK_OBJECT( Save_game ), "activate", GTK_SIGNAL_FUNC( on_Save_game_activate ), NULL );
    gtk_widget_add_accelerator( Save_game, "activate", accel_group, GDK_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    Load_game = gtk_menu_item_new_with_label( "Load game" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Load_game", Load_game );
    gtk_widget_show( Load_game );
    gtk_container_add( GTK_CONTAINER( File_menu ), Load_game );
    gtk_signal_connect( GTK_OBJECT( Load_game ), "activate", GTK_SIGNAL_FUNC( on_Load_game_activate ), NULL );
    gtk_widget_add_accelerator( Load_game, "activate", accel_group, GDK_L, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    separator1 = gtk_menu_item_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "separator1", separator1 );
    gtk_widget_show( separator1 );
    gtk_container_add( GTK_CONTAINER( File_menu ), separator1 );

    Edit_game = gtk_menu_item_new_with_label( "Edit game" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Edit_game", Edit_game );
    gtk_widget_show( Edit_game );
    gtk_container_add( GTK_CONTAINER( File_menu ), Edit_game );
    gtk_signal_connect( GTK_OBJECT( Edit_game ), "activate", GTK_SIGNAL_FUNC( on_Edit_game_activate ), NULL );
    gtk_widget_add_accelerator( Edit_game, "activate", accel_group, GDK_E, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    separator2 = gtk_menu_item_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "separator2", separator2 );
    gtk_widget_show( separator2 );
    gtk_container_add( GTK_CONTAINER( File_menu ), separator2 );

    Quit = gtk_menu_item_new_with_label( "Quit" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Quit", Quit );
    gtk_widget_show( Quit );
    gtk_container_add( GTK_CONTAINER( File_menu ), Quit );
    gtk_signal_connect( GTK_OBJECT( Quit ), "activate", GTK_SIGNAL_FUNC( on_Quit_activate ), NULL );
    gtk_widget_add_accelerator( Quit, "activate", accel_group, GDK_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    item3 = gtk_menu_item_new_with_label( "Options" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "item3", item3 );
    gtk_widget_show( item3 );
    gtk_container_add( GTK_CONTAINER( menubar1 ), item3 );

    item3_menu = gtk_menu_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "item3_menu", item3_menu );
    gtk_menu_item_set_submenu( GTK_MENU_ITEM( item3 ), item3_menu );

    Change_side = gtk_menu_item_new_with_label( "Change side" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Change_side", Change_side );
    gtk_widget_show( Change_side );
    gtk_container_add( GTK_CONTAINER( item3_menu ), Change_side );
    gtk_signal_connect( GTK_OBJECT( Change_side ), "activate", GTK_SIGNAL_FUNC( on_Change_side_activate ), NULL );
    gtk_widget_add_accelerator( Change_side, "activate", accel_group, GDK_G, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    Suggest_move = gtk_menu_item_new_with_label( "Suggest move" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Suggest_move", Suggest_move );
    gtk_widget_show( Suggest_move );
    gtk_container_add( GTK_CONTAINER( item3_menu ), Suggest_move );
    gtk_signal_connect( GTK_OBJECT( Suggest_move ), "activate", GTK_SIGNAL_FUNC( on_SuggestMove_activate ), NULL );
    gtk_widget_add_accelerator( Suggest_move, "activate", accel_group,
       GDK_question, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE );

    separator3 = gtk_menu_item_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "separator3", separator3 );
    gtk_widget_show( separator3 );
    gtk_container_add( GTK_CONTAINER( item3_menu ), separator3 );

    item18 = gtk_menu_item_new_with_label( "Settings" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "item18", item18 );
    gtk_widget_show( item18 );
    gtk_container_add( GTK_CONTAINER( item3_menu ), item18 );
    gtk_signal_connect( GTK_OBJECT( item18 ), "activate", GTK_SIGNAL_FUNC( on_Settings_activate ), NULL );
    gtk_widget_add_accelerator( item18, "activate", accel_group, GDK_T, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE );

    item4 = gtk_menu_item_new_with_label( "Help" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "item4", item4 );
    gtk_widget_show( item4 );
    gtk_container_add( GTK_CONTAINER( menubar1 ), item4 );
    gtk_menu_item_right_justify( GTK_MENU_ITEM( item4 ) );

    item4_menu = gtk_menu_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "item4_menu", item4_menu );
    gtk_menu_item_set_submenu( GTK_MENU_ITEM( item4 ), item4_menu );

    Rules_of_Reversi = gtk_menu_item_new_with_label( "Rules of Reversi" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "Rules_of_Reversi", Rules_of_Reversi );
    gtk_widget_show( Rules_of_Reversi );
    gtk_container_add( GTK_CONTAINER( item4_menu ), Rules_of_Reversi );
    gtk_signal_connect( GTK_OBJECT( Rules_of_Reversi ), "activate", GTK_SIGNAL_FUNC( on_Rules_of_Reversi_activate ), NULL );
    gtk_widget_add_accelerator( Rules_of_Reversi, "activate", accel_group, GDK_F1, 0, GTK_ACCEL_VISIBLE );

    separator4 = gtk_menu_item_new(  );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "separator4", separator4 );
    gtk_widget_show( separator4 );
    gtk_container_add( GTK_CONTAINER( item4_menu ), separator4 );

    About_atilo = gtk_menu_item_new_with_label( "About atilo" );
    gtk_object_set_data( GTK_OBJECT( main_wnd ), "About_atilo", About_atilo );
    gtk_widget_show( About_atilo );
    gtk_container_add( GTK_CONTAINER( item4_menu ), About_atilo );
    gtk_signal_connect( GTK_OBJECT( About_atilo ), "activate", GTK_SIGNAL_FUNC( on_About_atilo_activate ), NULL );

}                               // create_main_menu
