/*
 * global.h - global definitions
 * Copyright (C) 2008-2009  Alexandre Martins < alemartf [.a.t.] gmail [.d.o.t.] com >
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H

/* Game data */
#define GAME_UNIXNAME           "opensonic"
#define GAME_TITLE              "Open Sonic"
#define GAME_VERSION            0
#define GAME_SUB_VERSION        1
#define GAME_WIP_VERSION        1
#define GAME_WINDOWTITLE        "%s %d.%d.%d"
#define GAME_WEBSITE            "http://opensnc.sourceforge.net"


/* Global definitions and constants */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
#define TRUE                    -1
#define FALSE                   0
#define EPSILON                 0.00001f
#define PI                      3.14159265

#endif
