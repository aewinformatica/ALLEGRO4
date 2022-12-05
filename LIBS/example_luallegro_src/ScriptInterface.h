#ifndef SCRIPT_INTERFACE_H
#define SCRIPT_INTERFACE_H

#include <lua.hpp>
#include <allegro.h>


class ScriptInterface
{
public:
	static ScriptInterface *GetInstance();

	int DoScript(const char* fileName);
	void Update();
	void Draw();

	int CallFunction(const char *funcName); // Isso deve ser privado, ou nem mesmo existir.

	BITMAP *buffer;



	static int PrintMessage(lua_State *ls);
	static int showCircle(lua_State *ls);
	static int waitKey(lua_State *ls);
	static int readKey(lua_State *ls);
	static int GetActivePage(lua_State *ls);




private:
	lua_State* L;
	int linha;

	static ScriptInterface *instance;
	ScriptInterface(void);
	~ScriptInterface(void);

};

#endif
