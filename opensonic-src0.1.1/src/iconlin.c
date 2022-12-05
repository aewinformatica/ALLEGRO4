/*
 * iconlin.c - icon file for GNU/Linux
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

#include <allegro.h>
/* XPM */
static const char *allegico_xpm[] = {
/* columns rows colors chars-per-pixel */
"16 14 13 1",
"  c #010101010101",
". c gray25",
"X c #808000000000",
"o c #A0A060604040",
"O c #20202020A0A0",
"+ c #20204040C0C0",
"@ c #40404040E0E0",
"# c #60606060E0E0",
"$ c #808080808080",
"% c #A0A0A0A0A0A0",
"& c #E0E0A0A08080",
"* c gray88",
"= c None",
/* pixels */
"================",
"=O++++@##=======",
"==OO+oo++++#=O==",
"===OOo&o@@+++O==",
"====Oo&@#@@@++==",
"==@@++++$$@@@$O=",
"=@+++++$%%#@@$O=",
"=+OO++#% .%@ %O=",
"@OOOO##% $*+ %#=",
"====OO#% .*#.%#=",
"==#+O o&&**  .==",
"=#+OO oo&&&&oo==",
"=+OOOO Xoo&oX===",
"=+==OOOXXXXX===="
};
#if defined ALLEGRO_WITH_XWINDOWS && defined ALLEGRO_USE_CONSTRUCTOR
extern void *allegro_icon;
CONSTRUCTOR_FUNCTION(static void _set_allegro_icon(void));
static void _set_allegro_icon(void)
{
    allegro_icon = allegico_xpm;
}
#endif
