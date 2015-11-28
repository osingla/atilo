/**
 * @file config.c
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#include "all.h"

static char **szaFontNames;
static int nb_fonts;
typedef struct {
    unsigned char width, height;
    char *name;
} FONT_INFO;
static FONT_INFO *font_infos;


#if 0
/**
 * TBD
 * 
 * @return TBD
 */
static char * fonts_filename( void ) {
    char *home, *cfg_name;
    int l;

    if ( ( ( home = getenv( "HOME" ) ) == NULL ) || ( ( l = strlen( home ) ) == 0 ) )
        return NULL;
    if ( ( cfg_name = malloc( strlen( home ) + 20 ) ) == NULL )
        return NULL;
    strcpy( cfg_name, home );
    if ( cfg_name[l - 1] != '/' )
        strcat( cfg_name, "/" );
    strcat( cfg_name, "atilo.fonts" );
    return cfg_name;

}                               // fonts_filename

/**
 * TBD
 */
static void write_fonts( void ) {
    char *fname;
    FILE *fp;
    FONT_INFO *font_info;
    int n;

    if ( ( ( fname = fonts_filename(  ) ) == NULL )
       || ( ( fp = fopen( fname, "w" ) ) == NULL ) ) {
        if ( fname )
            free( fname );
        return;
    }

    for ( font_info = font_infos, n = 0; n < nb_fonts; font_info++, n++ )
        fprintf( fp, "%d\t%d\t%s\n", font_info->width, font_info->height, font_info->name );

    fclose( fp );
    free( fname );

}                               // write_fonts

/**
 * TBD
 * 
 * @return TBD
 */
static int read_fonts( void ) {
    char *fname, name[301];
    FILE *fp;
    int width, height;
    int nb, n;
    FONT_INFO *font_info;

    if ( ( ( fname = fonts_filename(  ) ) == NULL )
       || ( ( fp = fopen( fname, "r" ) ) == NULL ) ) {
        if ( fname )
            free( fname );
        return FALSE;
    }

    for ( nb_fonts = 0;; ) {
        nb = fscanf( fp, "%d\t%d\t%[^\n]\n", &width, &height, name );
        if ( nb != 3 )
            break;
        nb_fonts++;
    }
    if ( nb_fonts == 0 ) {
        fclose( fp );
        return FALSE;
    }

    font_infos = malloc( sizeof( FONT_INFO ) * nb_fonts );
    rewind( fp );
    for ( font_info = font_infos, n = 0; n < nb_fonts; n++, font_info++ ) {
        nb = fscanf( fp, "%d\t%d\t%[^\n]\n", &width, &height, name );
        if ( nb != 3 )
            break;
        font_info->width = width;
        font_info->height = height;
        font_info->name = strdup( name );
    }

    fclose( fp );
    return TRUE;

}                               // read_fonts
#endif

/**
 * TBD
 */ 
void scan_fonts( void ) {
    int n, k;
    int sz, max_width, max_height;
    GdkFont *font;
    FONT_INFO *font_info;
    static char *hor[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
    static char *vert[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };

    /*
     * --- Get the font names --- 
     */
#if 0
    if ( !read_fonts(  ) )
#endif
    {

        szaFontNames = XListFonts( GDK_DISPLAY(  ), "*bold-r-normal-*-p*iso8859-1", 1000, &nb_fonts );
        if ( nb_fonts == 0 )
            szaFontNames = XListFonts( GDK_DISPLAY(  ), "*bold-r-normal-*iso8859-1", 1000, &nb_fonts );
        if ( nb_fonts == 0 )
            szaFontNames = XListFonts( GDK_DISPLAY(  ), "*-r-normal-*iso8859-1", 1000, &nb_fonts );
        font_infos = malloc( sizeof( FONT_INFO ) * nb_fonts );

        /*
         * --- Compute size for each font --- 
         */
        for ( font_info = font_infos, n = 0; n < nb_fonts; n++, font_info++ ) {
            font = gdk_font_load( szaFontNames[n] );
            for ( max_width = 0, k = 0; k < 8; k++ )
                if ( ( sz = gdk_string_width( font, hor[k] ) ) > max_width )
                    max_width = sz;
            for ( max_height = 0, k = 0; k < 8; k++ )
                if ( ( sz = gdk_string_height( font, vert[k] ) ) > max_height )
                    max_height = sz;
            font_info->width = max_width;
            font_info->height = max_height;
            font_info->name = strdup( szaFontNames[n] );
        }
        free( szaFontNames );

#if 0

        /*--- Write all fonts to a file ---*/
        write_fonts(  );
#endif

    }

}                               // scan_fonts

/**
 * TBD
 * 
 * @param width TBD
 * @param height TBD
 * @return TBD
 */
char *find_max_font( const int width, const int height ) {
    FONT_INFO *font_info, *info = NULL;
    int max_width, max_height, n;

    for ( max_width = max_height = 0, font_info = font_infos, n = 0; n < nb_fonts; n++, font_info++ ) {
        if ( ( font_info->width <= width ) && ( font_info->height <= height ) &&
           ( font_info->width > max_width ) && ( font_info->height > max_height ) ) {
            max_width = font_info->width;
            max_height = font_info->height;
            info = font_info;
        }
    }

    if ( info == NULL )
        return "*bold*-r*140*";
    else
        return info->name;
}                               // find_max_font

/**
 * TBD
 * 
 * @return FALSE if needs to reload all fonts
 */
int load_config( void ) {
    char fname[PATH_MAX];
    char *home;
    FILE *fp;

    /*
     * Default values
     */
    config.level = LEVEL2;
    config.strategy = STRATEGY4;
    config.mainwnd_w = 460;
    config.mainwnd_h = 400;

    /*
     * Read configuration file
     */
    home = getenv( "HOME" );
    if ( home == NULL )
        return FALSE;
    sprintf( fname, "%s/.atilo", home );
    fp = fopen( fname, "r" );
    if ( fp == NULL )
        return FALSE;
    fread( &config, sizeof( config ), 1, fp );
    fclose( fp );

    return FALSE;
}                               // load_config

/**
 * TBD
 */
void save_config( void ) {
    char fname[PATH_MAX];
    char *home;
    FILE *fp;

    home = getenv( "HOME" );
    if ( home == NULL )
        return;
    sprintf( fname, "%s/.atilo", home );
    fp = fopen( fname, "w" );
    if ( fp == NULL )
        return;
    fwrite( &config, sizeof( config ), 1, fp );
    fclose( fp );
}                               // save_config
