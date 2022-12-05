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

#include "lua_alleg.h"

int l_keypressed          (lua_State* L);
int l_readkey             (lua_State* L);
int l_ureadkey            (lua_State* L);
int l_keyboard_needs_poll (lua_State* L);
int l_poll_keyboard       (lua_State* L);
int l_install_keyboard    (lua_State* L);
int l_remove_keyboard     (lua_State* L);
int l_clear_keybuf        (lua_State* L);
int l_simulate_keypress   (lua_State* L);
int l_simulate_ukeypress  (lua_State* L);
int l_keystate            (lua_State* L);
int l_three_finger_flag   (lua_State* L);
int l_scancode_to_ascii   (lua_State* L);
int l_scancode_to_name    (lua_State* L);


int luaopen_allegro_kbd          (lua_State *L);
