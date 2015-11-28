/**
 * @file config.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef CONFIG_H_
#  define CONFIG_H_

/* config.c */
void scan_fonts( void );
char *find_max_font( const int width, const int height );
int load_config( void );
void save_config( void );

#endif                         /*CONFIG_H_ */
