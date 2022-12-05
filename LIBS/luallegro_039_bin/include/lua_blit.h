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





int l_blit                        (lua_State *L);
int l_masked_blit                 (lua_State *L);
int l_stretch_blit                (lua_State *L);
int l_masked_stretch_blit         (lua_State *L);
int l_draw_sprite                 (lua_State *L);
int l_stretch_sprite              (lua_State *L);
int l_draw_trans_sprite           (lua_State *L);
int l_draw_lit_sprite             (lua_State *L);
int l_draw_gouraud_sprite         (lua_State *L);
int l_draw_character_ex           (lua_State *L);
int l_draw_sprite_v_flip          (lua_State *L);
int l_draw_sprite_h_flip          (lua_State *L);
int l_draw_sprite_vh_flip         (lua_State *L);
int l_rotate_sprite               (lua_State *L);
int l_rotate_sprite_v_flip        (lua_State *L);
int l_rotate_scaled_sprite        (lua_State *L);
int l_rotate_scaled_sprite_v_flip (lua_State *L);
int l_pivot_sprite                (lua_State *L);
int l_pivot_sprite_v_flip         (lua_State *L);
int l_pivot_scaled_sprite         (lua_State *L);
int l_pivot_scaled_sprite_v_flip  (lua_State *L);





int luaopen_allegro_blit(lua_State *L);
