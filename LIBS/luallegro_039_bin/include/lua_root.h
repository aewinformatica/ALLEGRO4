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

#ifndef _LUA_ROOT_H_
#define _LUA_ROOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "algif.h"

int l_allegro_init           (lua_State* L);
int l_allegro_exit           (lua_State* L);
int l_allegro_message        (lua_State* L);
int l_allegro_id             (lua_State* L);
int l_allegro_error          (lua_State* L);
int l_os_type                (lua_State* L);
int l_os_version             (lua_State* L);
int l_os_revision            (lua_State* L);
int l_os_multitasking        (lua_State* L);
int l_set_window_title       (lua_State* L);
int l_desktop_color_depth    (lua_State* L);
int l_get_desktop_resolution (lua_State* L);
int l_check_cpu              (lua_State* L);
int l_close_button_pressed	(lua_State* L);

int luaopen_allegro_root            (lua_State *L);

#ifdef __cplusplus
}
#endif

#endif // _LUA_ROOT_H_
