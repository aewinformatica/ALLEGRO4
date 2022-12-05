/*****************************************************
*
*	Javascript Colored Code (JsCC)
*	by Rogério Alencar Lino Filho
* reeditado e melhorado para a BDJOGOS.com por Adriano Waltrick 
*
*	This script convert all <pre> tags, with class name "code", in a "colored code"
*	following the Tokens list (keywords) below.
*
*	The style of the code displayed (in the HTML) can be changed easily in the CSS file (jscc.css).
*
*	(languages supported: Java, Javascript)
*
*	@version 0.1.1
*
******************************************************/


/*
*
*	TOKENS
*
*/
var KEYWORDS = "abstract and array Array as break case catch clone const continue declare default die do echo else elseif empty eval exception exit extends final for foreach function global if implements include interface isset new or print println private super protected public require return static switch this throw try use var while class void true false null int System out window alert bool NULL struct";

var TOKENS = KEYWORDS.split(" ");

// END TOKENS

var entrada 	= "";
var saida 		= "";
var posicao 	= 0;
var contador  = 0;

var proximoCaracter 	= "";
var tokenReconhecido 	= "";

var codigos = new Array();
var qtd_codigos = 0;

var mensagem = false;

//var regex = new RegExp(TOKENS);

function reconheceToken() 
{
	for (var i=0; i<TOKENS.length; i++) 
	{
		if (TOKENS[i] == tokenReconhecido) 
		{
			return true;
		}
	}
	
	return false;
}

function leProxCaractere() 
{
	proxCaractere = entrada.charAt(posicao++);
}

function inicio() 
{
	try 
	{
		leProxCaractere();
		
		if (proxCaractere == "/") 
		{
			tokenReconhecido += proxCaractere;
			leProxCaractere();
			
			if (proxCaractere == "*") 
			{
				tokenReconhecido += proxCaractere;
				leProxCaractere();
				comentario();
			} 
			else if (proxCaractere == "/") 
			{
				tokenReconhecido += proxCaractere;
				leProxCaractere();
				comentario_simples();
			}
		}
		
		if (proxCaractere == "\"") 
		{
			tokenReconhecido += proxCaractere;
			leProxCaractere();
			string();
		}
		
		if ((proxCaractere == '*') || (proxCaractere == ')') || (proxCaractere == '{') || (proxCaractere == '}') || (proxCaractere == '=') || (proxCaractere == '==') || (proxCaractere == '+') || (proxCaractere == '-') || (proxCaractere == '-')) 
		{
			operadores();
			leProxCaractere();
		}
		
		if ((proxCaractere == " ") || (proxCaractere == "\n") || (proxCaractere == "\t") || (proxCaractere == '.')  || (proxCaractere == '(') || (proxCaractere == ';'))
		{
			if ( reconheceToken() ) 
			{
				tokenReconhecido = "<span class='kwd'>"+tokenReconhecido+"</span>";
			}
			
			if ((proxCaractere == '.') || (proxCaractere == '(') || (proxCaractere == ';')) 
			{
				operadores();
			} 
			else 
			{
				tokenReconhecido += proxCaractere;
				saida += tokenReconhecido;
				tokenReconhecido = "";
			}
		
		} 
		else 
		{
			tokenReconhecido += proxCaractere;		
		}
		
		inicio();
		
	} 
	catch(e) 
	{
		proxCaractere = "EOF";
	}
}

function operadores() 
{
	saida += tokenReconhecido+"<span class='opr'>"+proxCaractere+"</span>";
	tokenReconhecido = "";
}

function string() 
{	
	if (proxCaractere == "\"") 
	{
		tokenReconhecido += proxCaractere;
		tokenReconhecido = "<span class='str'>"+tokenReconhecido+"</span>";
		saida += tokenReconhecido;
		tokenReconhecido = "";
		leProxCaractere();
	} 
	else 
	{
		tokenReconhecido += proxCaractere;
		leProxCaractere();
		string();
	}
}

function comentario() 
{	
	if (proxCaractere == "*") 
	{
		tokenReconhecido += proxCaractere;
		leProxCaractere();
		
		if (proxCaractere == "/") 
		{
			tokenReconhecido += proxCaractere;
			tokenReconhecido = "<span class='cmt2'>"+tokenReconhecido+"</span>";
			saida += tokenReconhecido;
			tokenReconhecido = "";
			leProxCaractere();
		} 
		else 
		{
			tokenReconhecido += proxCaractere;
			leProxCaractere();
			comentario();
		}
	} 
	else 
	{
		tokenReconhecido += proxCaractere;
		leProxCaractere();
		comentario();
	}
}

function comentario_simples() 
{
	if (proxCaractere == "\n") 
	{
		tokenReconhecido = "<span class='cmt1'>"+tokenReconhecido+"</span>";
		saida += tokenReconhecido;
		tokenReconhecido = "";
	} 
	else 
	{
		leProxCaractere();
		tokenReconhecido += proxCaractere;
		comentario_simples();
	}
}

function limpa() 
{
	entrada = "";
	saida = "";
	posicao = 0;
	proximoCaracter = "";
	tokenReconhecido = "";
}

function JsCC_convert() 
{
	var pre = document.getElementsByTagName("pre");
	
	for (var i=0; i<pre.length; i++) 
	{
		if ( (pre[i].getAttribute("class") == "code") || (pre[i].className == "code") ) 
		{
			
			alert( pre[i].innerHTML.toString() );
			
			entrada = pre[i].innerHTML+" ";
			//alert( entrada );
			
			entrada = pre[i].TextContent+" ";
			//alert( entrada );
			
			
			invl = entrada.indexOf( "<", 0 );
			while ( invl > 1 )
			{				
				entrada = entrada.replace("<", "&lt");	
				invl    = entrada.indexOf( "<", 0 );
			}
			
			
			invl = entrada.indexOf( ">", 0 );
			while ( invl > 1 )
			{
				entrada = entrada.replace(">", "&gt");	
				invl = entrada.indexOf( ">", 0 );
			}
			
			//entrada = entrada.replace(">/g", "&gt--");
			entrada = entrada.replace("&amp;", "&");			
			
			inicio();
			
			
			//if (document.all) 
			//{
				
				// o IE após a mudança não reconhece as quebras de linha (\n) e tabulações (\t)
				// então substui por tags HTML ("\n" por "<br />" e "\t" por "&nbsp;" - espaços em branco)
				saida = saida.replace(/\n/g, "<br> ");
				
				
				
				saida = saida.replace(/\t/g, "&nbsp;&nbsp;&nbsp;&nbsp;");
			//}
			
			pre[i].innerHTML = saida;
			limpa();
		}
	}
}

function numeracao()
{
	var pre = document.getElementsByTagName("pre");
	
	for (var i=0; i<pre.length; i++) 
	{
		contador = 1;
		
		if ( (pre[i].getAttribute("class") == "code") || (pre[i].className == "code") ) 
		{
			entrada = imprime_cab( i );
			entrada = entrada + "001.&nbsp;&nbsp;&nbsp;&nbsp;" + pre[i].innerHTML+" ";
			testeupp = entrada.toUpperCase();
			
			posicao = testeupp.indexOf("<BR>");
				
					
			while ( posicao > 0 )
			{				
				contador++;
				
				if ( contador < 10 )
				{
					vcont = "00"+contador;
				}
				
				if ( contador >= 10 && contador <= 99 )
				{
					vcont = "0"+contador;
				}
				
				entrada = entrada.substr( 0, posicao ) + "<br><font color='#000000'>" + vcont + ".</font>&nbsp;&nbsp;&nbsp;&nbsp;"+ entrada.substr( posicao+4, entrada.length );
				
				testeupp = entrada.toUpperCase();			
				posicao = testeupp.indexOf("<BR>", posicao+1 );		
				
			}
			
			
			pre[i].innerHTML = entrada + imprime_fim( i );
		}
	}
	
	
}


function copyText(theSel) 
{ 
	if (!document.all) return; // IE only 
	
	theForm = theSel.form; 
	theForm.copyArea.value = theSel.options[theSel.selectedIndex].value; 
	r = theForm.copyArea.createTextRange(); 
	r.select(); 
	r.execCommand('copy'); 
} 

function imprime_cab( id )
{
	var str = "";
	
	str = "<hr />";
	str = str + "<table width='100%' border='0' cellspacing='1' cellpadding='2'>"
	str = str + "<tr>";
	str = str + "<td width='44%'><span class='titulos_texto'>C&Oacute;DIGO... </span></td>";
	str = str + "<td width='2%'>&nbsp;</td>";
	str = str + "<td width='100%' align='right'>";
	str = str + "<img src='images/img_copiar_codigo.jpg' border='0' width='140' height='41' alt='Clique para copiar o código fonte inteiro'/ style='cursor:hand;' onclick='copyToClip("+ id +")'>";
	str = str + "</td></tr>";
	str = str + "</table>";
	str = str + "<hr />";
	
	return str;
}

function imprime_fim( id )
{
	var str = "";
	
	str = "<hr />";
	str = str + "<table width='100%' border='0' cellspacing='1' cellpadding='2'>"
	str = str + "<tr>";
	str = str + "<td width='44%'><span class='titulos_texto'>FIM DE C&Oacute;DIGO... </span></td>";
	str = str + "<td width='2%'>&nbsp;</td>";
	str = str + "<td width='100%' align='right'>";
	str = str + "<img src='images/img_copiar_codigo.jpg' border='0' width='140' height='41' alt='Clique para copiar o código fonte inteiro'/ style='cursor:hand;' onclick='copyToClip("+ id +")'>";
	str = str + "</td></tr>";
	str = str + "</table>";
	str = str + "<hr />";
	
	return str;
}

function copyToClip( id )
{
	if( window.clipboardData )
	{		
		window.clipboardData.setData('text', codigos[id] );  
		
		if ( mensagem == false )
		{
			alert('O Código foi copiado para o Clipboard com sucesso!\nAgora utilize a opções Colar ( CTRL+V ) em seu editor para colar o código fonte.\n\nTente ler e executar o fonte, estude sobre o conteúdo do tutorial e qualquer problema entre em contato pelo forum!!!');
			mensagem = true;
		}
	}
	else
	{		
		var wnd = window.open('','_blank','title=codigo, width=750, height=400, location=0, resizable=1, menubar=0, scrollbars=0');  
		wnd.document.write('<textarea rows="30" style="width:99%;  height:99%;  ">'+codigos[id]+'</textarea>');  
		wnd.document.close();  
	}	
}



function salva_codigo()
{
	var pre = document.getElementsByTagName("pre");

	for (var i=0; i<pre.length; i++) 
	{		
		if ( (pre[i].getAttribute("class") == "code") || (pre[i].className == "code") ) 
		{
			qtd_codigos++;
			codigos[i] = pre[i].innerHTML;
		}
	}
}

function recupera_codigo( valor )
{
	return codigos[ valor ];
}

function ver_codigos()
{
	for ( var i=0; i<codigos.length; i++ )
	{
		alert( codigos[i] );
	}
}
