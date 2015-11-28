/**
 * @file util.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

/**
 * TBD
 * 
 * @param fp TBD
 * @return TBD
 */
long filelength( FILE * fp ) {
    struct stat sb;

    fstat( fileno( fp ), &sb );
    return sb.st_size;
}                               // filelength

/**
 * TBD
 * 
 * @param message TBD
 */
void warning_msg( const gchar *message ) {
    GtkWidget *dlg = gtk_message_dialog_new( GTK_WINDOW( main_wnd ),
       GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message );
    gtk_dialog_run( GTK_DIALOG( dlg ) );
    gtk_widget_destroy( dlg );
}                               // warning_msg
