

function Janela(url,largura,altura) {
   popupWin = window.open(url,"","width="+largura+",height="+altura+"");
}

function JanelaScroll(url,largura,altura) {
   popupWin = window.open(url,"","width="+largura+",height="+altura+",scrollbars=yes");
}

function VOTACAO(ativo) {

    if(ativo==1)
     {
        for (i=0; i < document.FORM_ENQUETE.VOTO.length; i++)
         {
           if (document.FORM_ENQUETE.VOTO[i].checked==1){
               Janela("jan_enquete.php?VOTO="+document.FORM_ENQUETE.VOTO[i].value,450,350);
               return; }
         }
       alert("Vote em uma opção!");
     }
    else
     {
        Janela("jan_enquete.php",400,350);
        return;
     }

}

