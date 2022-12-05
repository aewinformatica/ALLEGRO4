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

#ifndef _LUA_ALLEG_H_
#define _LUA_ALLEG_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>



//#include <lstate.h>
#ifndef LUALLEGRO_VERSION
#define LUALLEGRO_VERSION	0.392
#endif

//Declarations of types const and structures used by LuAllegro library
#define max(a, b)	(((a) > (b)) ? (a) : (b))
#define min(a, b)	(((a) < (b)) ? (a) : (b))

typedef struct
{
	int		DataType;
	void	*DataPtr;
} AUD;

enum
{
	AL_BITMAP				= 1,
	AL_ZBUFFER,
	AL_FONT,
	AL_PALETTE,
	AL_RLE_SPRITE,
	AL_COMPILED_SPRITE,
	AL_SAMPLE,
	AL_MIDI,
	AL_AUDIOSTREAM,
	AL_JOYSTICK_INFO,
	AL_JOYSTICK_BUTTON_INFO,
	AL_JOYSTICK_STICK_INFO,
	AL_JOYSTICK_AXIS_INFO,
	AL_GFX_MODE_LIST,
	AL_GFX_MODE,
	AL_RGB,
	AL_V3D,
	AL_V3D_f,
	AL_COLOR_MAP,
	AL_RGB_MAP,
	AL_al_ffblk,
	AL_DATAFILE,
	AL_MATRIX,
	AL_MATRIX_f,
	AL_QUAT,
	AL_DIALOG,
	AL_MENU,
	AL_DIALOG_PLAYER,
	AL_PACKFILE,
	AL_PACKFILE_VTABLE,
	AL_LZSS_PACK_DATA,
	AL_LZSS_UNPACK_DATA
};

//------------------------------------------------------------------------------------------
#define get_first_param(au, dest, i) \
	{	i = 0; \
		au = (AUD *) lua_touserdata(L, 1); \
		if (au && au->DataType == AL_BITMAP) \
		{	dest = (BITMAP *) au->DataPtr; \
			i = 1; \
		} \
		else \
		{	dest = screen; \
			if (lua_type(L, 1) == LUA_TNIL) \
				i = 1; \
		} \
	}

//------------------------------------------------------------------------------------------
#define get_param(a)	a = lua_tonumber(L, ++i)
#define get_param_def(a, b) \
	if (lua_type(L, i + 1) == LUA_TNUMBER) \
		get_param(a); \
	else \
	{	i++; \
		a = b; \
	}

void				setfield(lua_State *L, const char *key, int value);

int					luallegro_set_close_button_callback(void);

LUA_API lua_Number	lua_tonumber_def(lua_State *L, int idx, lua_Number def_val);

extern int			force_exit;

//--- The most important function: ---
int					luaopen_allegro(lua_State *L);
int					luaopen_luallegro(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif // #ifndef  _LUA_ALLEG_H_
