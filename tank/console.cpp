//============================================================================
// Author(s) : F�bio Reis Cecin
// Module    : console
// Contents  : Game Console Class Implementation
// Notes     : (none)
// Date      : 19.03.2000 (all dates in french notation)
//============================================================================

/*

2 DO:

- reescrever pagedown p/ usar as funcoes "GetLine"

- fazer 2 funcoes auxiliares para "push" de linha no index
  (para que pgup/pgdn mantenham o index v�lido enquanto buscam)

- implementar check de scroll ficando inv�lido ao
		adicionar conte�do e atropelar o in�cio do conte�do
		apontado pelo scroll (no caso, da� scroll = new_start)

- input fixes:
  -// FIXME: fazer uso de um RenderChar() para desenhar o "]" e a "_" 
   // da linha, a� n�o precisa desse buf[256] feio a� de baixo e o 
   // desenho fica mais turbo

  -// FIXME: o input n�o deve ter o "]" se o offset estiver habilitado 
   // (o prompt deve sumir quando ocorre rolagem do input)

- OTIMIZA��O "FUTURA": n�o invalidar/redesenhar console inteiro
  quando input alterado (nesse caso, fazer um atalho para o render_line
  do input, j� que o out n�o � afetado.)

- POSSIVEL OTIMIZA��O: pagedown quando "bate no fim" invalida Index,
  mas n�o precisava (causa um re-scan in�til)

*/

// External Headers
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//#include "console.h"
#include "scorch.h"

// tamanho da lista de historico do input do console
#define  CONSOLE_HISTORY_LINES               32

//---- CODE SECTION ----------------------------------------------------------

// constructor 
console_c::console_c(int outputBufferSizeInKilobytes, int consoleDisplayColumns, int consoleDisplayRows)
{
	// salva largura/altura vis�veis do console
	conDisplay = true;
	conCols = consoleDisplayColumns;
	conRows = consoleDisplayRows;

	//codigo comum dos constructors
	outSize = outputBufferSizeInKilobytes * 1024;
	defaults();
}

// constructor 
console_c::console_c(int outputBufferSizeInKilobytes)
{
	// sem largura/altura vis�veis do console
	conDisplay = false;
	conCols = 10;				//apenas alguns defauls pra prevenir...
	conRows = 10;

	//codigo comum dos constructors
	outSize = outputBufferSizeInKilobytes * 1024;
	defaults();
}

// set defaults (usado pelos ctors)
void console_c::defaults() {

	// developer mode desligado (mensagens Dprintf s�o ignoradas por default)
	conDeveloperMode = false;

	// console redraw inicialmente ligado
	conInputRedrawFlag = true;
	conOutputRedrawFlag = true;

	// aloca �rea de mem�ria para o output do console
	outBuffer = new char[outSize]; 

	// inicializa��es arcanas, n�o mexa!
	outStart = 0;
	outEnd = -1;	
	outCount = 0;

	// outRows: conRows - 1 (do input)
	outRows = conRows - 1;

	// scroll inicialmente desativado
	outScrolled = false;

	// inicializa index
	outIndexValid = false;	// line index inv�lido
	outLineIndex = new tagLineIndexRecord[outRows];

	// aloca �rea para o input
	inSize = 1023;	// area US�VEL (alocado + 1 p/ eventual \0 quando usa todos os chars)
	inBuffer = new char[inSize + 1];
	inCount = 0; // n� caracteres v�lidos dentro do input (0=vazio)

	// aloca espaco e inicializa history
	hisSize = hisSpaceLeft = CONSOLE_HISTORY_LINES;
	hisList = new char*[hisSize];
	hisFirst = 0;
	hisLast = -1;
	hisCurrent = -1; // -1 = inicio da history, que e' a "linha vazia"
}

// Caractere (char) enviado para o console
bool console_c::write_string(const char* outstr)
{
	int len = strlen(outstr); // len = comprimento do bloco a ser inserido

	if (len < 1) return false; // se n�o h� nada: return

	if (len >= outSize) // caso raro (s� pra n�o deixar nada sem tratamento):
	{						  // tamanho maior ou igual ao pr�prio console. o console fica cheio com a parte final de outstr
		outEnd = outCount = outSize;
		outEnd--;
		outStart = 0;
		memcpy(outBuffer, outstr + len - outSize, outSize);
		outScrolled = false;		// conScroll = -1;
		outIndexValid = false;	// avacalha com o desenho, pois troca tudo
		return true;
	}

	int new_outEnd = (outEnd + len) % outSize; // new_outEnd - onde o end vai parar

	int new_outCount = outCount + len; // new_outCount - novo n� de caracteres

	// ops: tem mais caracteres do que cabe no buffer = atropelou start, ent�o conserta o count (start = f(count,end))
	// rola o start pra frente do novo End, at� achar um in�cio de linha v�lido (um ap�s um \n == 10)
	// FIX: continua tentando (while) at� conseguir, se a 1a linha n�o resolver
	while (new_outCount > outSize) 
	{										 
		int start = new_outEnd;
		new_outCount = outSize;
		do
		{
			new_outCount--; // esse � o quente
			start++;
			if (start >= outSize) start = 0; // wrap around
		}
		while (((char)*(outBuffer + start) != '\n'));
	}

	// �ndices OK, realiza a c�pia
	//	
	// size1 = tamanho da 1a metade, � igual ao resto at� o fim do buffer ou
	//			  o tamanho de outstr, o que for menor, pode ser zero se 
	//			  outEnd for a �ltima posi��o (outSize-1)
	//
	// size2 = tamanho da 2a metade, >0 somente se size1 � menor que o
	//			  tamanho de outstr

	int size1, size2;
	
	size1 = outSize - outEnd - 1; // tamanho at� o final
	if (size1 > len) // n�o copia mais do que tem
		size1 = len;
	size2 = len - size1;	// pega a sobra

	if (size1 > 0) memcpy(outBuffer + outEnd + 1, outstr, size1);
	if (size2 > 0) memcpy(outBuffer, outstr + size1, size2);

	// guarda novos valores
	outCount = new_outCount;
	outEnd = new_outEnd;

	// recalcula o start
	int new_outStart = outEnd - outCount + 1;
	if (new_outStart < 0) new_outStart += outSize;

	// checa para ver se a mudan�a do START n�o avacalha
	// com o SCROLL atual
	// se SIM, scroll = novo start
	//
	// FIXME, do it! tem new_outStart, outStart e conScroll.
	// agora FAZ!
	//

	// guarda novos valores
	outStart = new_outStart;

	// invalida o Index, se o scroll estiver grudado no fim
	// (sen�o, continua valido pq n�o mexeu na tela)
	if (!outScrolled) outIndexValid = false;

	// ok
	return true;
}

// Input de string (char*)
void console_c::read_string(char* instr, bool isScript)
{
	// simplesmente digita cada caracter no input, como se fosse a pessoa digitando
	char* scan = instr;			   
	while ((char)*scan != '\0') read_char((char)*scan++, isScript);
}

// Input de caractere (char)
void console_c::read_char(char inchar, bool isScript)
{
	// sinalizar redraw da linha de input necess�rio
	conInputRedrawFlag = true;

	switch (inchar)
	{
		case 8:	// backspace
			if (inCount > 0) inCount--;
			break;

		case '\n':	// new line		 (10)
		case '\r':	// enter/return (13)

			// envia echo do comando para o console (com \n\0 no final)
			//if (NoScriptRunning())
			if (!isScript)
			{
				inBuffer[inCount] = '\n';
				inBuffer[inCount+1] = '\0';
				printf("]%s", inBuffer);
			}

			// interpreta comando
			inBuffer[inCount] = '\0'; // tira o \n
			interprete_command(inBuffer); // interpreta comando

			// adiciona a string ao history, se foi pedido pra fazer isso..
			//if ((NoScriptRunning()) && (logHistory))
			if (!isScript)
				history_add(inBuffer); 

			// continua executando p/ reset input:

		case 27:	// ESC (limpa input)
			inCount = 0;
			break;

		default:   // caractere normal
			if (inCount < inSize)
				inBuffer[inCount++] = inchar;
			//else
			// FIXME: beep, aumentar �rea de armazenamento, etc. ou "do nothing" mesmo
			break;
	}
}

// Renderiza uma linha na tela
void console_c::render_line(char* buffer, int firstCharOffset, int lineNumber, int charCount)
{
	char drawLine[512];

	int size1, size2;

	size1 = outSize - firstCharOffset;
	if (size1 > charCount)
		size1 = charCount;
	size2 = charCount - size1;

	if (size1 > 0) memcpy(drawLine, buffer + firstCharOffset, size1);
	if (size2 > 0) memcpy(drawLine + size1, buffer, size2);

	drawLine[charCount] = '\0';

	//call line-draw implementation of subclass
	draw_line(lineNumber, (char*)drawLine);
}

// busca a pr�xima linha no buffer
// par�metros:
//	  firstCharOffset: primeiro caractere da linha atual
//   maxWidth: se -1: ignorar largura do console, sen�o, a funcao para quando a linha completar maxWidth colunas
// retorno:
//   r.charPos = se (r.resultCode) charPos = posi��o do 1o caractere (in�cio) da pr�xima linha encontrada
//   r.charCount = numero de caracteres da linha ATUAL (n�o da pr�xima)
//   r.resultCode = 0:falha (fim buffer) 1:ok, parou por \n 2: ok, parou por width
console_c::tagGetResult console_c::get_next_line(int firstCharOffset, int maxWidth)
{
	int  charPos    = firstCharOffset; // caractere onde inicia a busca
	int  resultCode = 0;		// condi��o de parada da busca (0=fim buffer 1="\n" 2=width)
	int  charCount  = 0;		// ordem do caractere atual na linha atual
	bool notFound   = true;	// ainda n�o achou (permanece no while)

	do
	{
		// pega o char da posi��o
		char ch = (char)*(outBuffer + charPos);

		// contador de caracteres v�lidos
		if (ch != '\n') charCount++;

		// se for o �ltimo do buffer o atual, (==end), ent�o n�o
		// h� proxima linha, pq este pode ser:
		//   \n: fim DESTA linha
		//   char de pos=maxwidth: ultimo caractere DESTA linha desenh�vel
		if (charPos == outEnd)
		{
			resultCode = 0;
			notFound   = false;
		}
		else // o atual n�o � o �ltimo, ou seja, h� pr�ximo depois dele
			  // para poder ser uma nextline, eventualmente
		{
			// verifica casos de parada (1=\n ou 2=width)
			if (ch == '\n') 
				resultCode = 1;
			else
			{
				if (charCount == maxWidth) 
					resultCode = 2;
			}

			// pr�ximo � in�cio de pr�xima linha (o charPos++ abaixo � proposital)
			if (resultCode) notFound = false;
		}

		// procura a pr�xima posi��o (se vai parar o loop, este charPos++ final faz
		// o papel de avan�ar para o primeiro caractere da pr�xima linha, pois
		// charPos agora aponta para o �ltimo da linha anterior
		charPos++;
		if (charPos >= outSize) charPos -= outSize;
	}
	while (notFound);

	// retorna resultados
	tagGetResult ret;
	ret.charOfs    = charPos;
	ret.charCount  = charCount;	// VALIDOS (print�veis fora \n que � um separador)
	ret.resultCode = resultCode;
	return ret;
}

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
console_c::tagGetResult console_c::get_previous_line(int firstCharOffset, bool alreadyAtEnd)
{
	int  charPos    = firstCharOffset; // caractere onde inicia a busca
	int  resultCode = 0;		// condi��o de parada da busca (0=fim buffer 1="\n" 2=width)
	int  charCount  = 0;		// ordem do caractere atual na linha atual
	bool notFound   = true;	// ainda n�o achou (permanece no while)

	int  nlCount    = 0;		// remendo, para detectar linhas nulas (sequencia de \n\n\n\...)

	tagGetResult ret;			// para retorno
	
	// est� no in�cio de uma linha v�lida
	// busca um atr�s. \n ou char, pertence � linha anterior
	if (!alreadyAtEnd)
	{
		// charPos == outStart. n�o tem o que buscar, man�.
		if (charPos == outStart)
		{
			ret.resultCode = 0;
			return ret;
		}

		// \n ou char, desde que exista, � o fim da outra linha
		charPos--;
		if (charPos < 0) charPos += outSize;
	}

	do
	{
		// pega o char da posi��o
		char ch = (char)*(outBuffer + charPos);

		// contador de caracteres v�lidos
		if (ch != '\n') charCount++;

		// se for o �ltimo do buffer o atual, (==start)
		// PODE HAVER PR�XIMA LINHA se == '\n' = � linha nula anterior
		// caso contr�rio, N�O H�
		if (((ch == '\n') || (charPos == outStart)) && (nlCount || charCount)) // primeiro � ignorado (tinha que estar l�)
		{
			// fim da linha - corrige charPos
			if (ch == '\n')
			{
				charPos++;
				if (charPos >= outSize) charPos -= outSize;
			}

			resultCode = 1;
			notFound = false;
		}
		else
		{
			// procura a pr�xima posi��o 
			charPos--;
			if (charPos < 0) charPos += outSize;
		}

		// para detec��o de \n\n\n\ consecutivos
		if (ch == '\n') nlCount++;
	}
	while (notFound);

	// retorna resultados
	ret.charOfs    = charPos;
	ret.charCount  = charCount;	// VALIDOS (print�veis fora \n que � um separador)
	ret.resultCode = resultCode;
	return ret;
}

// Realiza varredura da tela atual vis�vel (out), atualizando outLineIndex
void console_c::update_output_line_index()
{
	// se est� v�lido, retorna (n�o precisa fazer nada)
	if (outIndexValid) return;

	// ser� necess�rio redesenhar o output, j� que est� sendo
	// necess�rio alterar o �ndex
	conOutputRedrawFlag = true;

	// caso buffer n�o preencha a tela, o index fica com linhas default nulas
int i;
	for (i = 0; i < outRows; i++)
	{
		outLineIndex[i].count = 0;
		outLineIndex[i].start = -1;
	}

	// casos.. "especiais".. :-)
	if (outCount < 2) {
		conOutputRedrawFlag = false;
		return;	// 0 ou 1 caracteres: n�o escreve nada no console.
	}

	//
	//	Atualiza o Index
	// CASO 1: scroll fixo no fim. faz busca de tr�s-pra-frente
	//
	if (!outScrolled)
	{
		bool notAtEndAnymore = false;

		int lineCount = outRows - 1; // proxima linha livre a ser desenhada
		int charPos   = outEnd;

		tagGetResult res;

		while (lineCount >= 0)
		{
			bool alreadyAtEnd;

			if ((charPos == outEnd) && (!notAtEndAnymore))
			{
				alreadyAtEnd = true;
				notAtEndAnymore = true;
			}
			else
				alreadyAtEnd = false;

			// procura-se a pr�xima, obtendo o count da atual
			res = get_previous_line(charPos, alreadyAtEnd);

			// se achou linha anterior, processa
			if (res.resultCode) 
			{
				int linespan   = 0;					// contador de multilinhas (0=linha �nica)
				int totalcount = res.charCount;	// total de caracteres na biglinha

				// calcula linespan, deixando o "resto de caracteres da �ltima linha" no totalcount
				while (totalcount > conCols)
				{
					totalcount -= conCols;
					linespan++;
				}

				// startofs - contador/indicador de ofs.inicial da linha
				int startofs = res.charOfs + res.charCount - totalcount;
				if (startofs < 0) startofs += outSize;			// sei l� onde vai parar isso... limita dos dois lados
				if (startofs >= outSize) startofs -= outSize;

				// preenche a "�ltima"
				outLineIndex[lineCount].start = startofs;
				outLineIndex[lineCount].count = totalcount;

				// preenche as outras, se existirem
				while ((linespan) && (lineCount > 0))
				{
					linespan--;  // menos uma
					lineCount--; // menos uma
			
					startofs -= conCols;	// calcula start da linha
					if (startofs < 0) startofs += outSize;

					// desenha linha (existe pq linecount > 0 e com o -- fica no m�nimo 0)
					outLineIndex[lineCount].start = startofs;
					outLineIndex[lineCount].count = conCols;
				}
	
				// pr�xima linha a ser desenhada na tela
				charPos = startofs;
				lineCount--;
			} 
			else // n�o tem mais pr�xima linha
				break;
		}
		
		// atualiza PageStart - � o primeiro start v�lido do �ndex 
		// FIXME: e se n�o tem nenhum?? (out vazio)
		//outPageStart = charPos;
		for (i = 0; i < outRows; i++)
			if (outLineIndex[i].start != -1)
			{ 
				outPageStart = outLineIndex[i].start; 
				break; 
			}
	}
	//
	//	Atualiza o Index
	// CASO 2: console scrolled up
	//
	else
	{
		tagGetResult res;

		int lineCount = 0;				// pr�xima linha livre a ser desenhada na tela
		int charPos   = outPageStart;	// caractere inicial de busca

		while (lineCount < outRows)   // 0..outRows - 1
		{
			// grava no Index parte da linha atual que se sabe
			outLineIndex[lineCount].start = charPos;

			// procura-se a pr�xima, obtendo o count da atual
			res = get_next_line(charPos, conCols);

			// grava no Index parte da linha atual que se descobriu agora
			outLineIndex[lineCount].count = res.charCount;

			// pr�xima
			charPos = res.charOfs;
			lineCount++;
		}

		// atualiza indices "Page"
		outPageEnd   = outLineIndex[outRows-1].start + outLineIndex[outRows-1].count;
	}

	// valida
	outIndexValid = true;
}

// Redesenha o console inteiro
void console_c::draw_page(bool force)
{
	// verifica se possui display
	if (!conDisplay)
		return;

	// chama a varredura de p�gina
	update_output_line_index();

	// desenha output
	// se n�o � necess�rio, n�o mexe
	if ((conOutputRedrawFlag) || (force))
	{
		// limpeza da �rea de desenho
		clear();

		// renderiza as linhas j� indexadas
		for (int j = 0; j < outRows; j++)
			render_line(outBuffer, outLineIndex[j].start, j, outLineIndex[j].count);

		// desenhado ok
		conOutputRedrawFlag = false;
	}

	// desenha a linha de input, a �ltima do console (conRows-1)
	// se n�o � necess�rio, n�o mexe

	// FIXME: fazer uso de um RenderChar() para
	//  desenhar o "]" e a "_" da linha, a� n�o
	//  precisa desse buf[256] feio a� de baixo
	//  e o desenho fica mais turbo

	// FIXME: o input n�o deve ter o "]" se o offset estiver habilitado 
	// (o prompt deve sumir quando ocorre rolagem do input)

	if ((conInputRedrawFlag) || (force))
	{
		char buf[256];

		int offset, count;

		offset = inCount - conCols + 2; // +2 por causa do ] e do _
		if (offset < 0) offset = 0;

		count = inCount;
		if (count > conCols - 2) count = conCols - 2; // -2 por causa do ] e do _

		buf[0] = ']';
		memcpy(buf + 1, inBuffer + offset, count);
		buf[count+1] = '_';

		// limpeza da �rea de desenho
		clear_prompt();

		// 0 porque o "buf" j� tem a linha pronta, copia do come�o (FIXME acima muda isso)
		// conRows-1 porque � a localiza��o fixa da linha de input na tela
		// +2 por causa do ] e do _ 
		render_line(buf, 0, conRows - 1, count + 2); 

		// desenhado ok
		conInputRedrawFlag = false;
	}
}

// Scroll up
void console_c::scroll_page_up()
{
	// scroll j� no topo
	if (outPageStart == outStart) return; //if (conScroll == outStart) return;

	// invalida o display (independente de chamar o reindexador)
	conOutputRedrawFlag = true;

	int lineCount = 0;			 // contador de linhas "subidas"
	int charPos = outPageStart; // 1o caractere atual

	if (!outScrolled) // se scroll no fim, ent�o tem que come�ar do �ltimo
	{
		charPos = outEnd;
		lineCount -= outRows; // remendo: 1o pageup tem que subir 2 p�ginas, pois come�a o scan da mais de baixo.
	}

	int new_outPageStart = -1;

	bool notAtEndAnymore = false;

	do
	{
		tagGetResult res;

		bool alreadyAtEnd;

		if ((charPos == outEnd) && (!notAtEndAnymore))
		{
			alreadyAtEnd = true;
			notAtEndAnymore = true;
		}
		else
			alreadyAtEnd = false;
	
		// busca o in�cio da anterior 
		res = get_previous_line(charPos, alreadyAtEnd); //(charPos == outEnd)//((!outScrolled) && 

		// v� se n�o acabou
		if (res.resultCode)
		{
			int linespan   = 0;					// contador de multilinhas (0=linha �nica)
			int totalcount = res.charCount;	// total de caracteres na biglinha

			// calcula linespan, deixando o "resto de caracteres da �ltima linha" no totalcount
			while (totalcount > conCols)
			{
				totalcount -= conCols;
				linespan++;
			}

			// startofs - contador/indicador de ofs.inicial da linha
			int startofs = res.charOfs + res.charCount - totalcount;
			if (startofs < 0) startofs += outSize;			// sei l� onde vai parar isso... limita dos dois lados
			if (startofs >= outSize) startofs -= outSize;

			// preenche a "�ltima"
			// push line
			for (int i = outRows-1; i > 0; i--)
			{
				outLineIndex[i].start = outLineIndex[i-1].start;
				outLineIndex[i].count = outLineIndex[i-1].count;
			}
			outLineIndex[0].start = startofs;
			outLineIndex[0].count = totalcount;
	
			// preenche as outras, se existirem
			while ((linespan) && (lineCount < outRows)) // FIXME
			{
				linespan--;  // menos uma
				lineCount++; // menos uma
			
				startofs -= conCols;	// calcula start da linha
				if (startofs < 0) startofs += outSize;

				// push line 
				for (int i = outRows-1; i > 0; i--)
				{
					outLineIndex[i].start = outLineIndex[i-1].start;
					outLineIndex[i].count = outLineIndex[i-1].count;
				}
				outLineIndex[0].start = startofs;
				outLineIndex[0].count = conCols;
			}
	
			// pr�xima linha a ser desenhada na tela
			charPos = startofs;
			lineCount++;

			// condi��o de parada
			if (lineCount >= outRows) new_outPageStart = charPos;
		}
		else
		{
			// n�o tem anterior, esta � a �ltima, e � o start
			new_outPageStart = charPos;
		}
		
		charPos = res.charOfs; // pr�xima posi��o
	}
	while (new_outPageStart == -1);

	outScrolled = true;					// scrollUp ativado
	outIndexValid = true;				// pageup j� atualiza as linhas encontradas
	outPageStart = new_outPageStart; // salva os novos PageStart,PageEnd encontrados
	outPageEnd = outLineIndex[outRows-1].start + outLineIndex[outRows-1].count;
}

// Scroll down
// FIXME: reescrever fun��o para usar a funcao auxiliar get_next_line()
void console_c::scroll_page_down()
{
	// scroll j� no fim
	if (!outScrolled) return; //if (conScroll == -1) return;

	// invalida o display (independente de chamar o reindexador)
	conOutputRedrawFlag = true;

	// primeiro: Index tem que estar v�lido para a p�gina atual
	// chama a varredura de p�gina
	update_output_line_index();

	// segundo vai avan�ando linhas, at� 
	//   I- acabar o console (scroll=false, mas ainda valido, start e end infer�veis)
	//  II- preencher as linhas necess�rias

	int lineCount = 0;
	int colCount = 0;
	
	// FIXME: fazer mais amiguinho
	int charPos = outPageEnd + 1; 
	if (charPos >= outSize)
		charPos -= outSize;

		if ((char)*(outBuffer + charPos) == '\n') charPos++;

	// FIXME: REMENDO DE BUG
	int firstLineInARow = 1;

	while (lineCount < outRows)
	{
		char ch = (char)*(outBuffer + charPos);

		// INICIO
		if (ch != '\n') colCount++;

		if ((ch == '\n') || (colCount == conCols))
		{
			// chpos � o charPos s� que voltando ao in�cio da linha e wrap-aware
			int chpos = charPos - colCount;

			// FIXME: REMENDO DE BUG -- INICIO --
			if (ch == '\n') firstLineInARow = 1;
			if ((firstLineInARow) && (colCount == conCols))
			{
				firstLineInARow = 0;
				chpos++;
			}
			// FIXME: REMENDO DE BUG -- FIM --

			if (chpos < 0) chpos += outSize;

			//render_line(outBuffer, chpos, lineCount, colCount);
			// GRAVA no Index
			// neste caso, primeiro faz um "shift" e grava no �ltimo
			
			for (int l = 0; l < outRows-1; l++)
			{
				outLineIndex[l].start = outLineIndex[l+1].start;
				outLineIndex[l].count = outLineIndex[l+1].count;
			}
			outLineIndex[outRows-1].start = chpos;
			outLineIndex[outRows-1].count = colCount;

			colCount = 0;	// reseta
			lineCount++;	// avan�a
		}

		charPos++; // pr�ximo...
		if (charPos >= outSize) charPos = 0; // ...wrapando

		// FIM PASTE

		if (charPos == outEnd) // escaneando perto do fim, n�o interessa: bang!
									  // OTIMIZA��O: n�o precisava, vai causar 1 scan in�til;
									  // (devia continuar v�lido)
		{
			outScrolled = false;
			outIndexValid = false;
			outPageEnd = outEnd;
			return;
		}

	}

	// linecount atingido aqui
	// como Index=valid...:
	outPageStart = outLineIndex[0].start;
	outPageEnd = outLineIndex[outRows-1].start + outLineIndex[outRows-1].count;
}

// scroll page down at� o fim
void console_c::scroll_all_down()
{
	outScrolled = false;
	outIndexValid = false;
	outPageEnd = outEnd;
}

// altera o numero de linhas visiveis em uma pagina do console
void console_c::set_con_rows(int newConsoleDisplayRows)
{
	conRows = newConsoleDisplayRows;

	// novo outRows: re-inicializa index
	outRows = conRows - 1;
	delete [] outLineIndex;
	outIndexValid = false;	// line index inv�lido
	outLineIndex = new tagLineIndexRecord[outRows];
	
	// scroll no fim
	outScrolled = false;
	outIndexValid = false;
	outPageEnd = outEnd;

	conOutputRedrawFlag = true;
}

// altera o numero de colunas visiveis em uma pagina do console
void console_c::set_con_cols(int newConsoleDisplayColumns)
{
	conCols = newConsoleDisplayColumns;
	
	// scroll no fim
	outScrolled = false;
	outIndexValid = false;
	outPageEnd = outEnd;

	conInputRedrawFlag = true;
	conOutputRedrawFlag = true;
}

// faz com que o console passe a se desenhar (novamente?)
void console_c::enable_display(int cols, int rows) {

	conDisplay = true;
	set_con_cols(cols);
	set_con_rows(rows);
}

// desliga rotinas de desenho do console
void console_c::disable_display() {

	conDisplay = false;
}

// Destructor do console (libera buffers)
console_c::~console_c()
{
	delete[] outBuffer;		
	delete[] outLineIndex;
	delete[] inBuffer;
}

void console_c::printf(char* formatstr, ...)
{
	va_list		argptr;
	static char			msg[CONSOLE_PRINTF_MAXPRINTMSGSIZE];
	
	va_start (argptr, formatstr);
	vsprintf (msg, formatstr, argptr);
	va_end (argptr);
	
	write_string(msg);
}

void console_c::dprintf(char* formatstr, ...)
{
	va_list		argptr;
	static char			msg[CONSOLE_PRINTF_MAXPRINTMSGSIZE];
	
	if (!conDeveloperMode) return;	// ignore

	va_start (argptr, formatstr);
	vsprintf (msg, formatstr, argptr);
	va_end (argptr);

	printf("%s", msg);
}

// adiciona um comando 'a history list
void console_c::history_add(char* inputstr)
{	
	// hisFirst: PRIMEIRO a ser colocado (mais velho)
	// hisLast: ++ quando adiciona 1 string!
	//
	hisLast++;						   // avanca last
	if (hisSpaceLeft > 0)
		hisSpaceLeft--;				// menos um espaco livre
	else
	{
		if (hisLast == hisSize) hisLast -= hisSize;	// wrapa last
		delete hisList[hisLast];	   // deleta o que estiver no last (atropela)
		hisFirst++;						   // avanca first c/ wrap
		if (hisFirst == hisSize) hisFirst -= hisSize;
	}
	
	// adiciona na history
	hisList[hisLast] = strdup(inputstr);

	// reseta hisCurrent (volta pro inicio)
	hisCurrent = -1;
}

// coloca a "proxima" linha de history na entrada
void console_c::history_back()
{
	// se nao ha' strings, nao faz nada
	if (hisSpaceLeft == hisSize) return; // ou: if (hisLast == -1)

	// se esta' na primeira string (a mais velha), nao faz nada
	if (hisCurrent == hisFirst) return;

	// se esta' no comeco, vai para a ultima string digitada (last), senao volta um.
	if (hisCurrent == -1)
		hisCurrent = hisLast;
	else
	{
		hisCurrent--;
		if (hisCurrent < 0) hisCurrent += hisSize;
	}
	// coloca a string no inputbuffer
	strcpy(inBuffer, hisList[hisCurrent]);
	inCount = strlen(hisList[hisCurrent]);
	conInputRedrawFlag = true;
}

// coloca a linha "anterior" de history na entrada
// OBS: a linha "anterior" a "ultima" (mais recente) eh a linha vazia
void console_c::history_forward()
{
	// se nao ha' strings, nao faz nada
	if (hisSpaceLeft == hisSize) return; // ou: if (hisLast == -1)

	// se esta' mostrando a string vazia, nao faz nada
	if (hisCurrent == -1) return;

	// se ja' esta' mostrando a mais recente, agora mostrara' a string vazia,
	if (hisCurrent == hisLast)
	{
		hisCurrent = -1;
		inCount = 0;
	}
	// senao vai 1 pra frente e mostra essa
	else
	{
		hisCurrent++;	// avanca com wrap
		if (hisCurrent == hisSize) hisCurrent -= hisSize;
		// coloca a string no inputbuffer
		strcpy(inBuffer, hisList[hisCurrent]);
		inCount = strlen(hisList[hisCurrent]);
	}

	conInputRedrawFlag = true;
}

// ha' algum script sendo executado? (bloqueia input do usuario)
bool console_c::no_script_running()
{
	return true;			//FIXME
}


void conalleg_c::draw_line(int line, char *buf) 
{
	int col = tcol;
	if (line >= conRows - 1) 
		col = pcol;

	textprintf(bmp, cfont, 10, 10 + line * 10, col, buf);
}


void conalleg_c::clear() 
{ 
	//clear_to_color(bmp, bcol);
	rectfill(bmp, 0, 0, conbuffer->w, conbuffer->h, bcol);
}

void conalleg_c::clear_prompt() 
{ 
	rectfill(bmp, 10, conRows * 10, 10 + conCols * 8, conRows * 10 + 8, bcol);
}


bool conalleg_c::read_keyboard(int special_key)
{
		bool special_hit = false;

		if (keypressed()) 
		{
			int key = readkey();
			int ascii = key & 0xff;
			key = key >> 8;
			
			if (key == KEY_PGUP)
				con->scroll_page_up();
			else if (key == KEY_PGDN)
				con->scroll_page_down();
			else if (key == KEY_END)
				con->scroll_all_down();
			else if (key == KEY_UP)
				con->history_back();
			else if (key == KEY_DOWN)
				con->history_forward();
			else if (key == special_key)
				special_hit = true;
			else if (ascii > 0)
				con->read_char(ascii);
		}

		return special_hit;
}
