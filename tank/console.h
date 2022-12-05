//============================================================================
// Author(s) : F�bio Reis Cecin
// Module    : ColoniesConsole Header
// Contents  : Game Console Class Interface
// Notes     : (none)
// Date      : 19.03.2000 (all dates in french notation)
//============================================================================

/*
================================================================================

Classe:

	ColoniesConsole ("base console")

Descri��o:	

	Implementa o console do Colonies. O console � um bloco ("buffer de sa�da")
	de mem�ria cont�nuo, com gera��o de apontadores auxiliares para visualiza��o.
	Possui um "buffer de entrada" separado, que recebe caracteres de edi��o do
	usu�rio e, ao receber o "enter", o texto � tratado com um comando e enviado
	para o buffer de sa�da, junto com a sa�da gerada pelo comando. O buffer do
	console � circular; quando a sua capacidade � esgotada, as primeiras linhas
	armazenadas v�o sendo apagadas.
	Esta classe implementa tamb�m o interpretador de comandos.

================================================================================
*/

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define CONSOLE_PRINTF_MAXPRINTMSGSIZE 4096

/*==============================================================
****************************************************************

	class ColoniesConsole ("base console")

****************************************************************
==============================================================*/

class console_c
{
protected:

	// con: dados gerais do console
	//
	bool	conDisplay;				// se false, significa que console nao tem onde se desenhar atualmente

	int		conCols;		      // largura do display do console, em colunas
	int		conRows;		      // altura do display do console, em linhas

	bool	conInputRedrawFlag;	// linha de input alterada, necessita redesenho (na p�gina virtual)
	bool	conOutputRedrawFlag;	// linha de output alterada, necessita redesenho (na p�gina virtual)
	bool	conDeveloperMode; 		// flag: imprime mensagens de debug (DPrintf)?

	// output: buffer de sa�da (mensagens do console)
	//
	char*	outBuffer;   // buffer de sa�da do console
	int		outSize;     // tamanho em bytes alocado para o buffer de sa�da
	int		outStart;	 // ponteiro relativo p/ o 1o caractere (valido) do buffer. -1 = vazio
	int		outEnd;		 // ponteiro relativo p/ o ultimo caractere (valido) do buffer. -1 = vazio
	int		outCount;	 // contador de caracteres usados
		
	int		outRows;		   // n� linhas reservadas para output (= conRows - 1)
	bool	outIndexValid; // TRUE=pode desenhar direto FALSE=necessita scan
	bool	outScrolled;   // TRUE=deu pageups FALSE=grudado no fim do console
	int		outPageStart;	// offset do primeiro caractere (vis�vel)
	int		outPageEnd;	   // offset do �ltimo caractere (vis�vel)

	typedef struct { int start; int count; } tagLineIndexRecord;
	
	tagLineIndexRecord* outLineIndex; // vetor de informa��es sobre as linhas vis�veis, tipo um "cache"

	// input: digita��o de comandos
	//
	char*	inBuffer;	// buffer de entrada
	int		inSize;		// tamanho em bytes alocado
	int		inCount;		// contador de caracteres usados

	// history: historico de comandos digitados na entrada
	//
	char**	hisList;    // lista de strings
	int			hisSize;    // numero maximo de strings (capacidade) da history
	int			hisSpaceLeft;// numero de strings ainda nao preenchidas na history
	int			hisFirst;   // proxima string a ser mostrada quando acessar history
	int			hisLast;    // posicao da ultima string (onde escreve a proxima entrada)
	int			hisCurrent; // posicao corrente do history sendo acessada

	// estrutura para retorno da busca de linhas no outbuffer
	typedef struct { int charOfs, charCount, resultCode; } tagGetResult;

	// busca a pr�xima linha no buffer
	// par�metros:
	//	  firstCharOffset: primeiro caractere da linha atual
	//   maxWidth: se -1: ignorar largura do console, sen�o, a funcao para quando a linha completar maxWidth colunas
	// retorno:
	//   r.charPos = se (r.resultCode) charPos = posi��o do 1o caractere (in�cio) da pr�xima linha encontrada
	//   r.charCount = numero de caracteres da linha ATUAL (n�o da pr�xima)
	//   r.resultCode = 0:falha (fim buffer) 1:ok, parou por \n 2: ok, parou por width
	tagGetResult get_next_line(int firstCharOffset, int maxWidth);

	// busca a linha anterior no buffer (este s� para em "\n"s e fim do buffer!)
	// par�metros:
	//   firstCharOffset: caractere onde inicia a busca
	//   alreadyAtEnd: TRUE se firstCharOffset j� aponta para o �ltimo caractere
	//    da linha "anterior" em quest�o.  FALSE se ainda aponta para o in�cio da
	//    linha seguinte
	// retorno:
	//   r.charPos = se (r.resultCode) charPos = posi��o do 1o caractere (in�cio) da linha anterior encontrada
	//   r.charCount = numero de caracteres da linha ANTERIOR ENCONTRADA
	//   r.resultCode = 0:falha (n�o h� linha anterior) 1:ok (achou)
	tagGetResult get_previous_line(int firstCharOffset, bool alreadyAtEnd);

	// Realiza varredura da tela atual vis�vel (out), atualizando a estrutura outLineIndex
	void update_output_line_index();

	// Renderiza uma linha na tela
	void render_line(char* buffer, int firstCharOffset, int lineNumber, int charCount);

	// adiciona um comando 'a history list
	void history_add(char* inputstr);

	// altera o numero de linhas/colunas visiveis em uma pagina do console
	void set_con_rows(int newConsoleDisplayRows);
	void set_con_cols(int newConsoleDisplayColumns);

	// processamento de comandos
	// *** TO BE IMPLEMENTED IN SUBCLASS ***
	virtual void interprete_command(char* cmdstr) { printf("interprete_command(\"%s\");\n", cmdstr); }

	// render a console line. line: line offset onscreen. buf: line of text.
	// *** TO BE IMPLEMENTED IN SUBCLASS ***
	virtual void draw_line(int line, char *buf) { }

  // called when console must be redrawn.
	// *** TO BE IMPLEMENTED IN SUBCLASS ***
	virtual void clear() { }

  // called when console input must be redrawn.
	// *** TO BE IMPLEMENTED IN SUBCLASS ***
	virtual void clear_prompt() { }

public:

	// constructor : console + display default
	console_c(int outputBufferSizeInKilobytes, int consoleDisplayColumns, int consoleDisplayRows);

	// constructor : console sem display, precisa invocar enable_display() depois
	console_c(int outputBufferSizeInKilobytes);

	// Destructor do console (libera buffers)
	virtual ~console_c();

	// faz com que o console passe a se desenhar (novamente?)
	void enable_display(int cols, int rows);

	// desliga rotinas de desenho do console
	void disable_display();

	// troca developer mode
	void set_debug_mode(bool devmode) { conDeveloperMode = devmode; };

	// Sequencia de caracteres (char*) enviado para o console
	// retorna false se nao escreveu (por algum problema)
	virtual bool write_string(const char* outstr);

	// Escreve sa�da formatada (printf)
	void printf(char* formatstr, ...); 
	void dprintf(char* formatstr, ...);		// apenas p/ mensagens de debug

	// Input de string (char*) pelo usu�rio ou por um script
	void read_string(char* instr, bool isScript = false);

	// Input de caractere (char) pelo usu�rio ou por um script
	void read_char(char inchar, bool isScript = false);

	// Redesenha o console inteiro. se force == true, ent�o for�a as flags
	// "conInputRedrawFlag" e "conOutputRedrawFlag" para true (atualiza tudo)
	void draw_page(bool force = false);

	// Scroll
	void scroll_page_up();
	void scroll_page_down();
	void scroll_all_down();

	// coloca a proxima/anterior linhas de history na entrada
	void history_back();    
	void history_forward();

	// ha' algum script sendo executado? (bloqueia input do usuario)
	bool no_script_running();

private:

	// set defaults (usado pelos ctors)
	void defaults();
};

#endif
