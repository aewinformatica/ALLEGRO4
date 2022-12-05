#include "ScriptInterface.h"
#include <lua_alleg.h>

static const struct luaL_reg	scriptInterface_lib[] =
{
	{ "Circulo", ScriptInterface::showCircle },
	{ "Escreva", ScriptInterface::PrintMessage },
	{ "EsperaTecla", ScriptInterface::waitKey },
	{ "LeTecla", ScriptInterface::readKey },
	{ "GetActivePage", ScriptInterface::GetActivePage }, // muito importante
	{ NULL, NULL }	/* sentinel */
};


ScriptInterface *ScriptInterface::	instance = NULL;

/* */
ScriptInterface *ScriptInterface::GetInstance()
{
	if (!instance)
		instance = new ScriptInterface();
	return instance;
}


/// Construtor
/// Registra a biblioteca LuAlleg e algumas funções
ScriptInterface::ScriptInterface(void)
{
	L = lua_open();		///< \deprecated O manual diz que essa função, lua_open() foi substituida por lua_newstate()
	luaL_openlibs(L);	// load Lua base libraries

	//~~~~~~~~~~~~~~~~~~~~
	luaopen_allegro(L);
	//~~~~~~~~~~~~~~~~~~~~

	luaL_register(L, "script_interface", scriptInterface_lib);

	buffer = create_bitmap(SCREEN_W, SCREEN_H);
}


/* */
ScriptInterface::~ScriptInterface(void)
{
	lua_close(L);
	destroy_bitmap(buffer);
}


/// Update the logic
void ScriptInterface::Update()
{
	lua_getglobal(L, "Update");
	lua_pcall(L, 0, 0, 0);
}


/// Carrega um arquivo de script.
/// \retval int Retorna \c (!0) se o arquivo for carregado com sucesso e \c 0 em caso contrário.
/// \todo (H8OR#5#): Melhorar o tratamento de erros.
int ScriptInterface::DoScript(const char *fileName)
{
	luaL_dofile(L, fileName);
	if (!L)
		return -1;
	lua_getglobal(L, "Construct");
	lua_pcall(L, 0, 0, 0);
	return 0;
}


/// Chama a função do script, depois desenha o conteúdo de buffer na tela.
void ScriptInterface::Draw()
{
	clear_to_color(buffer, 0);	// Isso não deveria estar dentro do script?
	lua_getglobal(L, "Draw");
	lua_pcall(L, 0, 0, 0);
}


//###########################################################################

//! \retval int Retorna 0 se a função for executada com sucesso e "qualquer outra coisa" em caso contrário.
int ScriptInterface::CallFunction(const char *funcName)
{
	lua_getglobal(L, funcName);
	return lua_pcall(L, 0, 0, 0);
}


//###########################################################################
int ScriptInterface::readKey(lua_State *ls)
{
	int l_args = lua_gettop(ls);
	l_args = (int) lua_tonumber(ls, 1);
	lua_pushboolean(ls, (key[l_args] ? true : false));
	return 1;
}


/* */
int ScriptInterface::waitKey(lua_State *ls)
{
	int l_args = lua_gettop(ls);
	l_args = (int) lua_tonumber(ls, 1);
	while (!key[l_args]);

	// o processo de otimização do GCC elimina laços vazios
	while (key[l_args]);
	return 0;
}


/* */
int ScriptInterface::showCircle(lua_State *ls)
{
	int l_args = lua_gettop(ls);
	if (l_args < 6)
	{	allegro_message("Erro ao chamar função");
		exit(1);
	}

	circle(instance->buffer, (int) lua_tonumber(ls, 1), (int) lua_tonumber(ls, 2), (int) lua_tonumber(ls, 3),
		   makecol((int) lua_tonumber(ls, 4), (int) lua_tonumber(ls, 5), (int) lua_tonumber(ls, 6)));
	return 0;
}


/* */
int ScriptInterface::PrintMessage(lua_State *ls)
{
	int l_args = lua_gettop(ls);
	int x = lua_tointeger(ls, 1);
	int y = lua_tointeger(ls, 2);
	for (int i = 3; i <= l_args; i++)
		textout_ex(instance->buffer, font, lua_tostring(ls, i), x, y + (i - 3) * 8, makecol(255, 0, 0), -1);
	return 0;
}

/// \retval BITMAP* Retorna um ponteiro para a página ativa. One deve ser denhado.
int ScriptInterface::GetActivePage(lua_State *L)
{
	AUD *ad;
	ad = (AUD *) lua_newuserdata(L, sizeof(AUD));
	if (!ad)
	{	destroy_bitmap(instance->buffer);
		return 0;
	}

	luaL_getmetatable(L, "allegro_meta");
	lua_setmetatable(L, -2);

	ad->DataType = AL_BITMAP;
	ad->DataPtr = instance->buffer;

	return 1;
	//lua_pushlightuserdata(ls, (void *) instance->buffer);
	//return 1;
}
