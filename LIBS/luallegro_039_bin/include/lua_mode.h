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



int l_get_color_depth       (lua_State *L);
int l_set_color_depth       (lua_State *L);
int l_get_refresh_rate      (lua_State *L);
int l_request_refresh_rate  (lua_State *L);
int l_set_gfx_mode          (lua_State* L);
int l_scroll_screen         (lua_State* L);
int l_request_scroll        (lua_State* L);
int l_poll_scroll           (lua_State* L);
int l_show_video_bitmap     (lua_State* L);
int l_request_video_bitmap  (lua_State* L);
int l_vsync                 (lua_State* L);
int l_gfx_capabilities      (lua_State* L);
int l_get_gfx_mode_list     (lua_State *L);

int luaopen_allegro_mode(lua_State *L);
