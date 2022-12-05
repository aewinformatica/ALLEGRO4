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



int l_install_mouse          (lua_State* L);
int l_remove_mouse           (lua_State* L);
int l_poll_mouse             (lua_State* L);
int l_mouse_needs_poll       (lua_State* L);
int l_mouse_position         (lua_State* L);
int l_get_mouse_mickeys      (lua_State* L);
int l_mouse_set_position     (lua_State* L);
int l_set_mouse_range        (lua_State* L);
int l_set_mouse_speed        (lua_State* L);
int l_show_mouse             (lua_State* L);
int l_hide_mouse             (lua_State* L);
int l_scare_mouse            (lua_State* L);
int l_unscare_mouse          (lua_State* L);
int l_set_mouse_sprite       (lua_State* L);
int l_show_os_cursor         (lua_State* L);
int l_select_mouse_cursor    (lua_State* L);
int l_enable_hardware_cursor (lua_State* L);

int luaopen_allegro_mouse(lua_State *L);
