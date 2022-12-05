/*
Autor 			: Adriano
Data  			: 18/08/2006
Descrição		: Recupera a posição do mouse
Utilização	: getMouse_Posicao(document.onmousemove);	
*/

var UA = navigator.userAgent;

var pos_mouse_x = 0;
var pos_mouse_y = 0;

if (UA.indexOf('MSIE') > -1) 
{
	var eventos_acionados = window.Event ? true : false; 
	
	if ( eventos_acionados == true )
	{ 
		document.captureEvents(event.mousemove);
		document.onmousemove = getMouse_Posicao;
	}
}
else
{
	document.onmousemove = mouseMove;
}

function mouseMove(ev)
{
	ev           = ev || window.event;
	var mousePos = mouseCoords(ev);
	
	pos_mouse_x = mousePos.pos_mouse_x;
	pos_mouse_y = mousePos.pos_mouse_y;	
}

function mouseCoords(ev){
	if(ev.pageX || ev.pageY){
		return {pos_mouse_x:ev.pageX, pos_mouse_y:ev.pageY};
	}
	return {
		pos_mouse_x:ev.clientX + document.body.scrollLeft - document.body.clientLeft,
		pos_mouse_y:ev.clientY + document.body.scrollTop  - document.body.clientTop
	};
}


	
function getMouse_Posicao( evento )	
{
	if (UA.indexOf('MSIE') > -1) 
	{
		pos_mouse_x = (event.clientX + document.body.scrollLeft);
		pos_mouse_y = (event.clientY + document.body.scrollTop);
	}
}

// função para teste
function alert_mouse()
{
	getMouse_Posicao(document.onmousemove);	
	alert( pos_mouse_x + "-" + pos_mouse_y );	
}



