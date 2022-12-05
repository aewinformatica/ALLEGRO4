/****************************************************************************
**                                                                         **
**                  LuAllegro library - Lua port of Allegro                **
**                       Copyright (C) 2005 Peter Jamroz                   **
**                 e-mail: peterjam at poczta dot onet dot pl              **
**                                                                         **
**        Permission to use, copy, modify, publish this software           **
**    is hereby granted to any person obtaining a copy of this software,   **
**                              FREE OF CHARGE,                            **
**                     under the following conditions:                     **
**                                                                         **
** 1:  The above copyright notice and this permission notice shall be      **
**     included in all copies or substantial portions of the Software.     **
**                                                                         **
** 2:  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,     **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.**
**   IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                **
**                                                                         **
****************************************************************************/

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
//#include <lstate.h>




int l_clearscreen      (lua_State *L);
int l_clear_bitmap     (lua_State *L);
int l_clear_to_color   (lua_State *L);
int l_putpixel         (lua_State *L);
int l_getpixel         (lua_State *L);
int l_line             (lua_State *L);
int l_triangle         (lua_State *L);
int l_polygon          (lua_State *L);
int l_arc              (lua_State *L);
int l_rect             (lua_State *L);
int l_rectfill         (lua_State *L);
int l_circle           (lua_State *L);
int l_circlefill       (lua_State *L);
int l_ellipse          (lua_State *L);
int l_ellipsefill      (lua_State *L);
int l_floodfill        (lua_State *L);
int l_hline            (lua_State *L);
int l_vline            (lua_State *L);
int l_spline           (lua_State *L);


#define FORMAT_FLAT 1
#define FORMAT_12   2
#define FORMAT_XY   3

int get_points_xy(lua_State* L,int* points,int tablecount);
int get_points_12(lua_State* L,int* points,int tablecount);
int get_points_flat(lua_State* L,int* points,int tablecount);
int get_format_xy(lua_State* L);

int luaopen_allegro_prim      (lua_State *L);
