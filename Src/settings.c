/**
 * @file settings.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

static GtkWidget *dlg_settings = NULL;

static GtkWidget *radiobtn_level_1;
static GtkWidget *radiobtn_level_2;
static GtkWidget *radiobtn_level_3;
static GtkWidget *radiobtn_level_4;
static GtkWidget *radiobtn_level_5;

static GtkWidget *radiobtn_strategy_1;
static GtkWidget *radiobtn_strategy_2;
static GtkWidget *radiobtn_strategy_3;
static GtkWidget *radiobtn_strategy_4;
static GtkWidget *radiobtn_strategy_5;


/**
 * This is an internally used function to set notebook tab widgets.
 * 
 * @param notebook TBD
 * @param page_num TBD
 * @param widget TBD
 */
static void set_notebook_tab( GtkWidget * notebook, gint page_num, GtkWidget * widget ) {
    gtk_notebook_set_current_page( GTK_NOTEBOOK( notebook ), page_num );
}                               // set_notebook_tab

/**
 * TBD		
 */
static void set_level( void ) {
    GtkWidget *btn = NULL;

    switch ( config.level ) {
        case LEVEL1:
            btn = radiobtn_level_1;
            break;
        case LEVEL2:
            btn = radiobtn_level_2;
            break;
        case LEVEL3:
            btn = radiobtn_level_3;
            break;
        case LEVEL4:
            btn = radiobtn_level_4;
            break;
        case LEVEL5:
            btn = radiobtn_level_5;
            break;
    }
    gtk_toggle_button_set_state( GTK_TOGGLE_BUTTON( btn ), TRUE );
}                               // set_level

/**
 * TBD		
 * 
 * @return TBD
 */
static int get_level( void ) {
    if ( GTK_WIDGET_STATE( radiobtn_level_1 ) )
        return LEVEL1;
    else if ( GTK_WIDGET_STATE( radiobtn_level_2 ) )
        return LEVEL2;
    else if ( GTK_WIDGET_STATE( radiobtn_level_3 ) )
        return LEVEL3;
    else if ( GTK_WIDGET_STATE( radiobtn_level_4 ) )
        return LEVEL4;
    else if ( GTK_WIDGET_STATE( radiobtn_level_5 ) )
        return LEVEL5;
    else
        return -1;
}                               // get_level

/**
 * TBD		
 */
static void set_strategy( void ) {
    GtkWidget *btn = NULL;

    switch ( config.strategy ) {
        case STRATEGY1:
            btn = radiobtn_strategy_1;
            break;
        case STRATEGY2:
            btn = radiobtn_strategy_2;
            break;
        case STRATEGY3:
            btn = radiobtn_strategy_3;
            break;
        case STRATEGY4:
            btn = radiobtn_strategy_4;
            break;
        case STRATEGY5:
            btn = radiobtn_strategy_5;
            break;
    }
    gtk_toggle_button_set_state( GTK_TOGGLE_BUTTON( btn ), TRUE );
}                               // set_strategy

/**
 * TBD		
 * 
 * @return TBD
 */
static int get_strategy( void ) {
    if ( GTK_WIDGET_STATE( radiobtn_strategy_1 ) )
        return STRATEGY1;
    else if ( GTK_WIDGET_STATE( radiobtn_strategy_2 ) )
        return STRATEGY2;
    else if ( GTK_WIDGET_STATE( radiobtn_strategy_3 ) )
        return STRATEGY3;
    else if ( GTK_WIDGET_STATE( radiobtn_strategy_4 ) )
        return STRATEGY4;
    else if ( GTK_WIDGET_STATE( radiobtn_strategy_5 ) )
        return STRATEGY5;
    else
        return -1;
}                               // get_strategy

/**
 * TBD		
 * 
 * @param w TBD
 * @param data TBD
 */
static void on_cancel_clicked( GtkWidget * w, gpointer data ) {
    gtk_widget_destroy( dlg_settings );
}                               // on_cancel_clicked

/**
 * TBD		
 * 
 * @param w TBD
 * @param data TBD
 */
static void on_ok_clicked( GtkWidget * w, gpointer data ) {
    config.level = get_level(  );
    config.strategy = get_strategy(  );
    gtk_widget_destroy( dlg_settings );
}                               // on_ok_clicked

/**
 * TBD		
 * 
 * @param obj TBD
 * @param dlg TBD
 * @return TBD
 */
static void dlg_settings_destroy_cb( GtkObject *obj, GtkWidget **dlg ) {
    *dlg = NULL;
}                               // dlg_settings_destroy_cb

/**
 * TBD		
 */
void dialog_settings( void ) {
    GtkWidget *dialog_vbox1;
    GtkWidget *notebook1;
    GtkWidget *vbox2;
    GSList *vbox2_group = NULL;
    GtkWidget *vbox3;
    GSList *vbox3_group = NULL;
    GtkWidget *Label1;
    GtkWidget *label2;
    GtkWidget *dialog_action_area1;
    GtkWidget *hbuttonbox1;
    GtkWidget *btn_ok;
    GtkWidget *btn_cancel;

    if ( dlg_settings ) {
        gdk_window_raise( GTK_WIDGET( dlg_settings )->window );
        return;
    }

    dlg_settings = gtk_dialog_new(  );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "dlg_settings", dlg_settings );
    gtk_window_set_title( GTK_WINDOW( dlg_settings ), "Settings" );
    gtk_window_set_policy( GTK_WINDOW( dlg_settings ), TRUE, TRUE, FALSE );
    gtk_signal_connect( GTK_OBJECT( dlg_settings ), "destroy", GTK_SIGNAL_FUNC( dlg_settings_destroy_cb ), &dlg_settings );

    dialog_vbox1 = GTK_DIALOG( dlg_settings )->vbox;
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "dialog_vbox1", dialog_vbox1 );
    gtk_widget_show( dialog_vbox1 );

    notebook1 = gtk_notebook_new(  );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "notebook1", notebook1 );
    gtk_widget_show( notebook1 );
    gtk_box_pack_start( GTK_BOX( dialog_vbox1 ), notebook1, TRUE, TRUE, 0 );

    vbox2 = gtk_vbox_new( FALSE, 0 );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "vbox2", vbox2 );
    gtk_widget_show( vbox2 );
    gtk_container_add( GTK_CONTAINER( notebook1 ), vbox2 );

    radiobtn_level_1 = gtk_radio_button_new_with_label( vbox2_group, "Beginners" );
    vbox2_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_level_1 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_level_1", radiobtn_level_1 );
    gtk_widget_show( radiobtn_level_1 );
    gtk_box_pack_start( GTK_BOX( vbox2 ), radiobtn_level_1, TRUE, TRUE, 0 );

    radiobtn_level_2 = gtk_radio_button_new_with_label( vbox2_group, "Intermediate" );
    vbox2_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_level_2 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_level_2", radiobtn_level_2 );
    gtk_widget_show( radiobtn_level_2 );
    gtk_box_pack_start( GTK_BOX( vbox2 ), radiobtn_level_2, TRUE, TRUE, 0 );

    radiobtn_level_3 = gtk_radio_button_new_with_label( vbox2_group, "Good player" );
    vbox2_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_level_3 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_level_3", radiobtn_level_3 );
    gtk_widget_show( radiobtn_level_3 );
    gtk_box_pack_start( GTK_BOX( vbox2 ), radiobtn_level_3, TRUE, TRUE, 0 );

    radiobtn_level_4 = gtk_radio_button_new_with_label( vbox2_group, "Very good player" );
    vbox2_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_level_4 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_level_4", radiobtn_level_4 );
    gtk_widget_show( radiobtn_level_4 );
    gtk_box_pack_start( GTK_BOX( vbox2 ), radiobtn_level_4, TRUE, TRUE, 0 );

    radiobtn_level_5 = gtk_radio_button_new_with_label( vbox2_group, "Excellent player (cpu >= 2 Ghz)" );
    vbox2_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_level_5 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_level_5", radiobtn_level_5 );
    gtk_widget_show( radiobtn_level_5 );
    gtk_box_pack_start( GTK_BOX( vbox2 ), radiobtn_level_5, TRUE, TRUE, 0 );

    vbox3 = gtk_vbox_new( FALSE, 0 );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "vbox3", vbox3 );
    gtk_widget_show( vbox3 );
    gtk_container_add( GTK_CONTAINER( notebook1 ), vbox3 );

    radiobtn_strategy_1 = gtk_radio_button_new_with_label( vbox3_group, "Just take care of the corners and sub-corners" );
    vbox3_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_strategy_1 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_strategy_1", radiobtn_strategy_1 );
    gtk_widget_show( radiobtn_strategy_1 );
    gtk_box_pack_start( GTK_BOX( vbox3 ), radiobtn_strategy_1, TRUE, TRUE, 0 );

    radiobtn_strategy_2 =
       gtk_radio_button_new_with_label( vbox3_group, "Corners / Sub-corners + Computer try to maximise his number of pawns" );
    vbox3_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_strategy_2 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_strategy_2", radiobtn_strategy_2 );
    gtk_widget_show( radiobtn_strategy_2 );
    gtk_box_pack_start( GTK_BOX( vbox3 ), radiobtn_strategy_2, TRUE, TRUE, 0 );

    radiobtn_strategy_3 =
       gtk_radio_button_new_with_label( vbox3_group, "Corners / Sub-corners + Computer try to minimise his number of pawns" );
    vbox3_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_strategy_3 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_strategy_3", radiobtn_strategy_3 );
    gtk_widget_show( radiobtn_strategy_3 );
    gtk_box_pack_start( GTK_BOX( vbox3 ), radiobtn_strategy_3, TRUE, TRUE, 0 );

    radiobtn_strategy_4 =
       gtk_radio_button_new_with_label( vbox3_group, "Corners / Sub-corners + Computer try to block opponent" );
    vbox3_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_strategy_4 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_strategy_4", radiobtn_strategy_4 );
    gtk_widget_show( radiobtn_strategy_4 );
    gtk_box_pack_start( GTK_BOX( vbox3 ), radiobtn_strategy_4, TRUE, TRUE, 0 );

    radiobtn_strategy_5 =
       gtk_radio_button_new_with_label( vbox3_group,
       "Corners / Sub-corners + Computer try to block opponent + \nAnalyse some situations" );
    vbox3_group = gtk_radio_button_group( GTK_RADIO_BUTTON( radiobtn_strategy_5 ) );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "radiobtn_strategy_5", radiobtn_strategy_5 );
    gtk_widget_show( radiobtn_strategy_5 );
    gtk_box_pack_start( GTK_BOX( vbox3 ), radiobtn_strategy_5, TRUE, FALSE, 0 );

    Label1 = gtk_label_new( "Level" );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "Label1", Label1 );
    gtk_widget_show( Label1 );
    set_notebook_tab( notebook1, 0, Label1 );

    label2 = gtk_label_new( "Strategy" );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "label2", label2 );
    gtk_widget_show( label2 );
    set_notebook_tab( notebook1, 1, label2 );

    dialog_action_area1 = GTK_DIALOG( dlg_settings )->action_area;
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "dialog_action_area1", dialog_action_area1 );
    gtk_widget_show( dialog_action_area1 );
    gtk_container_border_width( GTK_CONTAINER( dialog_action_area1 ), 10 );

    hbuttonbox1 = gtk_hbutton_box_new(  );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "hbuttonbox1", hbuttonbox1 );
    gtk_widget_show( hbuttonbox1 );
    gtk_box_pack_start( GTK_BOX( dialog_action_area1 ), hbuttonbox1, TRUE, TRUE, 0 );

    btn_ok = gtk_button_new_with_label( "OK" );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "btn_ok", btn_ok );
    gtk_widget_show( btn_ok );
    gtk_container_add( GTK_CONTAINER( hbuttonbox1 ), btn_ok );
    gtk_signal_connect( GTK_OBJECT( btn_ok ), "clicked", GTK_SIGNAL_FUNC( on_ok_clicked ), NULL );

    btn_cancel = gtk_button_new_with_label( "Cancel" );
    gtk_object_set_data( GTK_OBJECT( dlg_settings ), "btn_cancel", btn_cancel );
    gtk_widget_show( btn_cancel );
    gtk_container_add( GTK_CONTAINER( hbuttonbox1 ), btn_cancel );
    gtk_signal_connect( GTK_OBJECT( btn_cancel ), "clicked", GTK_SIGNAL_FUNC( on_cancel_clicked ), NULL );

    set_level(  );
    set_strategy(  );
    gtk_widget_show( dlg_settings );
}                               // dialog_settings
