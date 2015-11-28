/**
 * @file all.h
 *
 * Atilo: Othello/Reversi Game \n
 * Copyright (C) 1999-2007  Olivier Singla \n
 * http://atilo.sourceforge.net/ \n
 */

#ifndef ALL_H_
#define ALL_H_

#define __STDC_CONSTANT_MACROS 	/**< TBD **/
#define _GNU_SOURCE				/**< TBD **/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <gdk/gdkx.h>
#include <X11/Xlib.h>

#include "atilo.h"

#include "config.h"
#include "endgame.h"
#include "game.h"
#include "layout.h"
#include "loadsave.h"
#include "menu.h"
#include "play.h"
#include "settings.h"
#include "status.h"
#include "util.h"

#endif                         /*ALL_H_ */
