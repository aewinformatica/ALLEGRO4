unit Exemplo07Prj;

// EXEMPLO 07

// ESTE EXEMPLO É UMA ENGINE DE JOGOS DE PLATAFORMA
// A CODIFICAÇÃO DESTE EXEMPLO É UMA POUCA MAIS COMPLEXA , POIS , DEVEMOS
// NOS PREOCUPAR COM ALGUMAS IMPORTANTES LEIS DA FÍSICA COMO : GRAVIDADE ,
// AREA DE CONTATO , DIREÇÃO , VELOCIDADE , ETC

// OS ELEMENTOS QUE FAZEM PARTE DESSE EXEMPLO SÃO :
// JOGADOR (1) - O PERSONAGEM QUE É CONTROLADO
// PLATAFORMAS (2) - O CHÃO E A PARTE DE BAIXO DO CHÃO
// EFEITOS (1) - PROJETIL DISPARADO PELO JOGADOR

// OUTRA IMPORTANTE OBSERVAÇÃO ENCONTRADA NESSE EXEMPLO É A CENTRALIZAÇÃO
// DE IMAGENS (CENTRO X E CENTRO Y) DO JOGADOR , PLATAFORMAS E EFEITOS ...
// É PREFERIVEL TER COMO PONTO DE REFERENCIA O CENTRO DE UMA FIGURA E
// NÃO A PARTE SUPERIOR ESQUERDA .

// TAMBÉM VEMOS NESSE EXEMPLO UM ALGORITMO BÁSICO PARA DISPARO DE PROJÉTIS
// E DE SALTOS ... REPAREM QUE PARA O JOGADOR PODER PULAR , ELE DEVERA
// ESTA PARADO NO CHÃO OU ANDANDO (POSIÇÕES 1 E 2 RESPECTIVAMENTE)
// ENQUANTO ESTA ATIRANDO , O PERSONAGEM NÃO PODE ANDAR 

// NESTE TIPO DE JOGO , USAMOS AS VARIAVEIS CAMERAX E CAMERAY PARA PODERMOS
// FOCAR QUALQUER PARTE DO MAPA (NESTE EXEMPLO , A CAMERA ESTA SEMPRE
// FOCADA NO JOGADOR)

// AS FIGURAS DO JOGADOR SÃO CARREGADAS EM TEMPO DE EXECUÇÃO ... REPAREM
// QUE FOI CRIADO UMA PEQUENA ROTINA PARA INVERTER AS FIGURAS E ALOCA-LAS
// EM OUTRO DXIMAGELIST

// O MAPA DO JOGO TAMBÉM É CARREGADO EM TEMPO DE EXECUÇÃO ... NESTE CASO
// UTILIZAMOS UM BITMAP (BMP) PARA QUARDAR TODAS AS INFORMAÇÕES DO MAPA
// (REPAREM QUE O MAPA CONTEM APENAS PONTOS VERMELHOS , QUE SÃO O CHÃO
// DO NOSSO JOGO)

// UTILIZAMOS TAMBÉM NESSA ENGINE O SCROLL HORIZONTAL , NO QUAL PODEMOS
// REPETIR INFINITAMENTE UM PEDAÇO DE CENARIO E ENCHER TODA A TELA ...
// (OBS - CARREGUE OUTRAS FIGURAS NO COMPONENTE DXIMAGELISTCENARIO E PODERÁ
// CRIAR OUTRAS FASES)

// APERTE AS TECLAS : Q , W , E  PARA PODER VISUALIZAR A COORDENADA E AREA DE CONTATO DO
// JOGADOR , PLATAFORMA E EFEITOS

// DICA - DEIXE SEMPRE OS ARQUIVOS DE SEUS JOGOS NOMEADOS CORRETAMENTE E
// COM UMA NUMERAÇÃO ... SE TIVER MUITOS ARQUIVOS DO MESMO TIPO (COMO POR
// EXEMPLO : EFEITOS , PLATAFORMAS , INIMIGOS) COLOQUE OS DENTRO DE PASTAS
// PARA FACILITAR O ACESSO A QUALQUER UM DELES


interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  DXClass, DXDraws, ExtCtrls, DXInput, DXSounds, DIB;

const
     maxplataforma=500;
     maxefeitos=10;


type
  TForm1 = class(TForm)
    DXDraw1: TDXDraw;
    DXTimer1: TDXTimer;
    DXImageListPlayerNormal: TDXImageList;
    BMP1: TImage;
    BMP2: TImage;
    DXInput1: TDXInput;
    DXImageListPlayerInvertido: TDXImageList;
    DXImageListPlataforma: TDXImageList;
    ImageMapa: TImage;
    DXImageListCenario: TDXImageList;
    DXImageListEfeitos: TDXImageList;
    DXImageListIcones: TDXImageList;
    DXWaveListSons: TDXWaveList;
    DXSound1: TDXSound;
    procedure barra(x1,y1,x2,y2:integer;cor:tcolor);
    procedure criaplataforma(num:byte;x,y:integer);
    procedure criamapa;
    procedure desenhaplataforma;
    procedure CopiaBmp(x1,y1,x2,y2:integer);
    procedure desenhacenario;
    procedure desenhajogador;
    procedure moverjogador(x,y:real);
    function contato(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2:integer):boolean;
    procedure criaefeito(num:byte;x,y:real);
    procedure desenhaefeitos;
    procedure FormCreate(Sender: TObject);
    procedure DXTimer1Timer(Sender: TObject; LagCount: Integer);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

// PLATAFORMAS
type
    plataforma=record
    x,y:real;          // COORDENADAS X E Y DA PLATAFORMA
    centrox,centroy:byte;    // CENTRO X E Y DA PLATAFORMA
    areax,areay:byte;  // AREA DE CONTATO DA PLATAFORMA
    num,desenho:byte;
end;

// EFEITOS
type
    efeito=record
    x,y:real;
    num,desenho:byte;
    direcao,velocidade:real;
    centrox,centroy:byte;    // CENTRO X E Y DA PLATAFORMA
end;

// JOGADOR
type
    jogador=record
    hp,hpmax:integer;     // VITALIDADE E VITALIDADE MAXIMA DO JOGADOR
    x,y:real;           // COORDENADAS X E Y DO JOGADOR
    velocidade,direcao,desenho:byte;
    centrox,centroy:byte;    // CENTRO X E Y DO JOGADOR
    ciclo:byte;           // USADO PARA MOVIMENTAR AS PERNAS E PULAR
    pos:byte;   // 1-PARADO 2-ANDANDO 3-PULANDO 4 E 5-ATIRANDO PROJETEIS
    areax,areay:byte;      // AREA DE CONTATO
    gravidade:real;     // GRAVIDADE DO PERSONAGEM
    bt1key,bt2key:boolean;    // VERIFICA SE UMA TECLA ESTA SENDO PRECIONADA
end;

// VARIAVEIS GLOBAIS
var
  Form1: TForm1;
  a:integer;
  gravidade,maxgravidade:real;     // GRAVIDADE DO JOGO
  camerax,cameray:integer;          // CAMERA
  jog:jogador;
  obj:array[1..maxplataforma] of plataforma;
  efts:array[1..maxefeitos] of efeito;
  detalhesjogador,detalhesplataforma,detalhesefeitos:boolean;

implementation

{$R *.DFM}

// DESENHA UMA BARRA NA TELA
procedure tform1.barra(x1,y1,x2,y2:integer;cor:tcolor);
begin
     dxdraw1.surface.canvas.brush.color:=cor;
     dxdraw1.surface.canvas.FillRect(rect(x1,y1,x2,y2));
     DXDraw1.surface.canvas.release;
end;

procedure tform1.CopiaBmp(x1,y1,x2,y2:integer);
var item:byte;
begin
     /////////////////////
     // IMAGEM NORMAL
     /////////////////////
     // ADICIONA UMA IMAGEM PARA O COMPONENTE DXIMAGELIST
     DXImageListPLayerNormal.Items.Add;

     // NUMERO DE FIGURAS DO COMPONENTE
     item:=DXImageListPlayerNormal.Items.Count;

     // RECORTA A AREA DO BMP E COPIA PARA O COMPONENTE DXIMAGELIST
     bmp2.Picture.Bitmap.width:=x2-x1;
     bmp2.Picture.Bitmap.height:=y2-y1;
     bmp2.Canvas.CopyRect(rect(0,0,x2-x1,y2-y1),bmp1.canvas,rect(x1,y1,x2,y2));
     DXImageListPlayerNormal.Items[item-1].Picture:=bmp2.Picture;
     DXImageListPlayerNormal.Items[item-1].Transparent:=true;
     DXImageListPlayerNormal.Items[item-1].TransparentColor:=rgb(255,255,255);


     /////////////////////
     // IMAGEM INVERTIDA
     /////////////////////
     // ADICIONA UMA IMAGEM PARA O COMPONENTE DXIMAGELIST
     DXImageListPLayerInvertido.Items.Add;

     // NUMERO DE FIGURAS DO COMPONENTE
     item:=DXImageListPlayerInvertido.Items.Count;

     // RECORTA A AREA DO BMP E COPIA PARA O COMPONENTE DXIMAGELIST
     bmp2.Picture.Bitmap.width:=x2-x1;
     bmp2.Picture.Bitmap.height:=y2-y1;
     bmp2.Canvas.CopyRect(rect(0,0,x2-x1,y2-y1),bmp1.canvas,rect(x2,y1,x1,y2));
     DXImageListPlayerInvertido.Items[item-1].Picture:=bmp2.Picture;
     DXImageListPlayerInvertido.Items[item-1].Transparent:=true;
     DXImageListPlayerInvertido.Items[item-1].TransparentColor:=rgb(255,255,255);

end;

// CRIA UMA PLATAFORMA DO MAPA
procedure tform1.criaplataforma(num:byte;x,y:integer);
var a:integer;
begin
     // PROCURA POR UMA PLATAFORMA QUE NÃO ESTEJA SENDO USADA
     for a:=1 to maxplataforma do
       if obj[a].num=0 then
       begin
            obj[a].num:=num;
            obj[a].x:=x;
            obj[a].y:=y;

            // APÓS ALOCAR UMA PLATAFORMA , PRECISAMOS SAIR DO LOOP
            break;
       end;
end;

// CRIA O MAPA DO JOGO
procedure tform1.criamapa;
var a,b:integer;
begin
     for a:=0 to ImageMapa.Picture.Width do
     for b:=0 to ImageMapa.Picture.Height do
     begin
          // CHÃO
          // OBS - SE ENCONTRARMOS UM PONTOS VERMELHO E ACIMA DELE
          // NÃO TIVER UM PONTO VERMELHO , SIGNIFICA QUE ESSA PLATAFORMA
          // ESTA NA SUPERFICIE
          if (ImageMapa.Canvas.Pixels[a,b]=rgb(255,0,0))
           and (ImageMapa.Canvas.Pixels[a,b-1]=rgb(255,255,255)) then criaplataforma(1,a*97,b*97);

          // EMBAIXO DO CHÃO
          // OBS - SE ENCONTRARMOS UM PONTOS VERMELHO E ACIMA DELE
          // TIVER UM PONTO VERMELHO , SIGNIFICA QUE ESSA PLATAFORMA
          // NÃO ESTA NA SUPERFICIE
          if (ImageMapa.Canvas.Pixels[a,b]=rgb(255,0,0))
           and (ImageMapa.Canvas.Pixels[a,b-1]=rgb(255,0,0)) then criaplataforma(2,a*97,b*97);
     end;

end;

// DESENHA TODAS AS PLATAFORMAS
procedure tform1.desenhaplataforma;
var a:integer;
    x,y:real;
begin
     for a:=1 to maxplataforma do
       if obj[a].num<>0 then
       begin
            // DETALHES - AREA DE CONTATO E COORDENADA DA PLATAFORMA
            if detalhesplataforma=true then
            begin
                 // AREA DE CONTATO
                 barra(round(obj[a].x-obj[a].areax-camerax),round(obj[a].y-obj[a].areay-cameray),
                   round(obj[a].x+obj[a].areax-camerax),round(obj[a].y+obj[a].areay-cameray),rgb(255,0,0));

                 // COORDENADAS DA PLATAFORMA
                 barra(round(obj[a].x-camerax),0,round(obj[a].x-camerax+1),600,rgb(255,255,255));
                 barra(0,round(obj[a].y-cameray),800,round(obj[a].y-cameray+1),rgb(255,255,255));
            end;

            // DESENHO DE CADA PLATAFORMA
            if obj[a].num=1 then obj[a].desenho:=0;
            if obj[a].num=2 then obj[a].desenho:=1;

            // CENTRO X E Y DAS PLATAFORMAS
            obj[a].centrox:=48;
            obj[a].centroy:=48;

            // AREA DE CONTATO DAS PLATAFORMAS
            obj[a].areax:=48;
            obj[a].areay:=40;

            // DESENHANDO A PLATAFORMA NA TELA
            x:=obj[a].x-obj[a].centrox-camerax;
            y:=obj[a].y-obj[a].centroy-cameray;

            if DXImageListPlataforma.Items.count>obj[a].desenho then
              DXImageListPlataforma.Items[obj[a].desenho].Draw(dxdraw1.surface,round(x),round(y),0);
       end;
end;

// DESENHA O CENARIO DO JOGO (FUNDO OU BACKGROUND)
procedure tform1.desenhacenario;
var a,tamx:integer;
    x:real;
begin
     // TAMANHO DO CENARIO
     tamx:=DXImageListCenario.Items[0].Width;

     // PREENCHENDO TODA A TELA COM A FIGURA DO CENARIO
     for a:=0 to round(800/tamx)+1 do
     begin
          x:=(a*tamx)-round(camerax/5) mod tamx;
          DXImageListCenario.Items[0].draw(dxdraw1.surface,round(x),0,0);
     end;
end;

// DESENHA O JOGADOR NA TELA
procedure tform1.desenhajogador;
var x,y:real;
    tamanhofigura:integer;
begin
     // DETALHES - AREA DE CONTATO E COORDENADA DO JOGADOR
     if detalhesjogador=true then
     begin
          // AREA DE CONTATO
          barra(round(jog.x-jog.areax-camerax),round(jog.y-jog.areay-cameray),
           round(jog.x+jog.areax-camerax),round(jog.y+jog.areay-cameray),rgb(255,0,0));

          // COORDENADAS DO JOGADOR
          barra(round(jog.x-camerax),0,round(jog.x-camerax+1),600,rgb(255,255,255));
          barra(0,round(jog.y-cameray),800,round(jog.y-cameray+1),rgb(255,255,255));
     end;

     // DESENHO DO JOGADOR
     // PARADO
     if jog.pos=1 then jog.desenho:=0;
     // ANDANDO
     if (jog.pos=2) and (jog.ciclo<5) then jog.desenho:=1;
     if (jog.pos=2) and (jog.ciclo>=5) and (jog.ciclo<10) then jog.desenho:=2;
     if (jog.pos=2) and (jog.ciclo>=10) and (jog.ciclo<15) then jog.desenho:=3;
     if (jog.pos=2) and (jog.ciclo>=15) and (jog.ciclo<20) then jog.desenho:=4;
     if (jog.pos=2) and (jog.ciclo>=20) and (jog.ciclo<25) then jog.desenho:=5;
     if (jog.pos=2) and (jog.ciclo>=25) and (jog.ciclo<30) then jog.desenho:=4;
     if (jog.pos=2) and (jog.ciclo>=30) and (jog.ciclo<35) then jog.desenho:=3;
     if (jog.pos=2) and (jog.ciclo>=35) and (jog.ciclo<40) then jog.desenho:=2;
     // PULANDO
     if (jog.pos=3) and (jog.ciclo<15) then jog.desenho:=6;
     if (jog.pos=3) and (jog.ciclo>=15) and (jog.ciclo<20) then jog.desenho:=7;
     if (jog.pos=3) and (jog.ciclo>=20) then jog.desenho:=8;
     // ATIRANDO NO CHÃO
     if jog.pos=4 then jog.desenho:=9;
     // ATIRANDO NO AR
     if jog.pos=5 then jog.desenho:=10;

     // DEPENDENDO DO DESENHO DO JOGADOR , O CENTROX DA FIGURA
     // SERA DIFERENTE
     if jog.desenho=0 then jog.centrox:=50;
     if jog.desenho=1 then jog.centrox:=60;
     if jog.desenho=2 then jog.centrox:=50;
     if jog.desenho=3 then jog.centrox:=50;
     if jog.desenho=4 then jog.centrox:=50;
     if jog.desenho=5 then jog.centrox:=60;
     if jog.desenho=6 then jog.centrox:=30;
     if jog.desenho=7 then jog.centrox:=40;
     if jog.desenho=8 then jog.centrox:=50;
     // DESENHO 9 - PEQUENA VARIAÇÃO RANDOMICA PARA DAR UM EFEITO DE
     // IMPACTO NO JOGADOR ENQUANTO ESTA ATIRANDO PROJETEIS
     if jog.desenho=9 then jog.centrox:=48+random(4);
     if jog.desenho=10 then jog.centrox:=30;

     // CENTROY DO JOGADOR
     jog.centroy:=90;
     if jog.desenho=6 then jog.centroy:=70;
     if jog.desenho=7 then jog.centroy:=70;
     if jog.desenho=8 then jog.centroy:=70;
     if jog.desenho=10 then jog.centroy:=70;

     // AREA DE CONTATO DO JOGADOR
     jog.areax:=25;
     jog.areay:=50;

     // DESENHANDO O JOGADOR NA TELA
     // DIREÇÃO - ESQUERDA (NORMAL)
     if (jog.direcao=1) and (DXImageListPlayerNormal.Items.count>jog.desenho) then
     begin
          x:=jog.x-jog.centrox-camerax;
          y:=jog.y-jog.centroy-cameray;

          DXImageListPlayerNormal.Items[jog.desenho].Draw(dxdraw1.surface,round(x),round(y),0);
     end;
     // DIREÇÃO - DIREITA (INVERTIDO)
     if (jog.direcao=2) and (DXImageListPlayerInvertido.Items.count>jog.desenho) then
     begin
          tamanhofigura:=DXImageListPlayerInvertido.Items[jog.desenho].width;
          x:=(jog.x-tamanhofigura)+jog.centrox-camerax;
          y:=jog.y-jog.centroy-cameray;

          DXImageListPlayerInvertido.Items[jog.desenho].Draw(dxdraw1.surface,round(x),round(y),0);
     end;

     // GRAVIDADE AGINDO SOBRE O JOGADOR
     jog.gravidade:=jog.gravidade+gravidade;
     if jog.gravidade>maxgravidade then jog.gravidade:=maxgravidade;

     // JOGADOR CAINDO DE UMA PLATAFORMA
     if ((jog.pos=1) or (jog.pos=2)) and (jog.gravidade>5) then
     begin
          jog.pos:=3;
          jog.ciclo:=25;
     end;

     // JOGADOR CAINDO , E VERIFICANDO SE HÁ ALGO EMBAIXO
     moverjogador(0,jog.gravidade);
end;

// MOVE O JOGADOR PARA CIMA,BAIXO,ESQUERDA E DIREITA
procedure tform1.moverjogador(x,y:real);
var a:integer;
begin
     jog.x:=jog.x+x;
     jog.y:=jog.y+y;

     if x>0 then jog.direcao:=1;
     if x<0 then jog.direcao:=2;

     // VERIFICA SE HOUVE COLISÃO COM UMA PLATAFORMA
     for a:=1 to maxplataforma do
      if obj[a].num<>0 then
       if contato(round(jog.x-jog.areax),round(jog.y-jog.areay),
        round(jog.x+jog.areax),round(jog.y+jog.areay),
         round(obj[a].x-obj[a].areax),round(obj[a].y-obj[a].areay),
          round(obj[a].x+obj[a].areax),round(obj[a].y+obj[a].areay)) then
          begin
               // PAREDES
               if x<0 then jog.x:=obj[a].x+obj[a].areax+jog.areax;
               if x>0 then jog.x:=obj[a].x-obj[a].areax-jog.areax;

               // CHÃO
               if y>0 then
               begin
                    jog.y:=obj[a].y-obj[a].areay-jog.areay;
                    if jog.pos<>4 then jog.pos:=1;
                    jog.gravidade:=0;
               end;

               // TETO
               if y<0 then
               begin
                    jog.y:=obj[a].y+obj[a].areay+jog.areay;
                    jog.pos:=3;
                    jog.ciclo:=25;
                    jog.gravidade:=0;
               end;
          end;

end;

// FUNÇÃO QUE VERIFICA A COLISÃO ENTRE 2 CORPOS (A E B)
function tform1.contato(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2:integer):boolean;
begin
     contato:=false;

     if (ax1<bx2) and (ax2>bx1) and (ay1<by2) and (ay2>by1) then contato:=true;
end;

// CRIA UM EFEITO
procedure tform1.criaefeito(num:byte;x,y:real);
var a:integer;
begin
     // PROCURA POR UM EFEITO QUE NÃO ESTEJA SENDO USADO
     for a:=1 to maxefeitos do
       if efts[a].num=0 then
       begin
            efts[a].num:=num;
            efts[a].x:=x;
            efts[a].y:=y;

            // EFEITO 1 - PROJETIL DO JOGADOR
            if efts[a].num=1 then
            begin
                 if jog.direcao=1 then efts[a].direcao:=0;
                 if jog.direcao=2 then efts[a].direcao:=3.14;
                 efts[a].velocidade:=18;
            end;

            // APÓS ALOCAR UMA PLATAFORMA , PRECISAMOS SAIR DO LOOP
            break;
       end;
end;

// DESENHA TODOS OS EFEITOS
procedure tform1.desenhaefeitos;
var a:integer;
    x,y:integer;
begin
     for a:=1 to maxefeitos do
       if efts[a].num<>0 then
       begin
            // DETALHES - AREA DE CONTATO E COORDENADA DOS EFEITOS
            if detalhesefeitos=true then
            begin
                 // AREA DE CONTATO
                 barra(round(efts[a].x-efts[a].centrox-camerax),round(efts[a].y-efts[a].centroy-cameray),
                   round(efts[a].x+efts[a].centrox-camerax),round(efts[a].y+efts[a].centroy-cameray),rgb(255,0,0));

                 // COORDENADAS DO JOGADOR
                 barra(round(efts[a].x-camerax),0,round(efts[a].x-camerax+1),600,rgb(255,255,255));
                 barra(0,round(efts[a].y-cameray),800,round(efts[a].y-cameray+1),rgb(255,255,255));
            end;

            // DESENHO DE CADA EFEITO
            if efts[a].num=1 then efts[a].desenho:=0;

            // CENTRO X E Y DOS EFEITOS
            if efts[a].desenho=0 then efts[a].centrox:=11;
            if efts[a].desenho=0 then efts[a].centroy:=10;

            // DESENHANDO O EFEITO NA TELA
            if DXImageListEfeitos.Items.count>efts[a].desenho then
            begin
                 x:=round(efts[a].x-camerax)-efts[a].centrox;
                 y:=round(efts[a].y-cameray)-efts[a].centroy;

                 DXImageListEfeitos.Items[efts[a].desenho].Draw(dxdraw1.surface,x,y,0);
            end;

            // EFEITOS SE MOVIMENTANDO
            efts[a].x:=efts[a].x+cos(efts[a].direcao)*efts[a].velocidade;
            efts[a].y:=efts[a].y-sin(efts[a].direcao)*efts[a].velocidade;

            // TIROS SAINDO DA TELA
            if x<-100 then efts[a].num:=0;
            if x>900 then efts[a].num:=0;
            if y<-100 then efts[a].num:=0;
            if y>700 then efts[a].num:=0;

       end;

end;

procedure TForm1.FormCreate(Sender: TObject);
begin
     // DADOS INICIAS DO JOGO
     gravidade:=2;
     maxgravidade:=17;

     // DADOS INICIAS DO JOGADOR
     jog.x:=50;
     jog.y:=80;
     jog.velocidade:=7;
     jog.direcao:=1;
     jog.hpmax:=800;
     jog.hp:=jog.hpmax;

     // DADOS DO FORM
     form1.width:=800;
     form1.height:=600;

     // ENCAIXA EXATAMENTE O DXDRAW NO FORM1
     DXDraw1.Top:=0;
     DXDraw1.Left:=0;
     DXDraw1.Width:=Form1.Width;
     DXDraw1.Height:=Form1.Height;

     // CARREGANDO O BITMAP INTEIRO (BMP1)
     bmp1.Picture.LoadFromFile('Player.bmp');

     // COPIANDO AS PARTES DO BMP PARA O COMPONENTE DXIMAGELISTPLAYER
     CopiaBmp(0,0,92,141);
     CopiaBmp(94,0,197,141);
     CopiaBmp(199,0,278,141);
     CopiaBmp(280,0,356,141);
     CopiaBmp(358,0,449,141);
     CopiaBmp(451,0,554,141);
     CopiaBmp(556,0,614,141);
     CopiaBmp(616,0,686,141);
     CopiaBmp(688,0,770,141);
     CopiaBmp(772,0,863,141);
     CopiaBmp(865,0,947,141);

     // CRIA O MAPA DO JOGO
     criamapa;

     // INICIALIZANDO OS EFEITOS SONOROS
     DXSound1.Initialize;
end;

procedure TForm1.DXTimer1Timer(Sender: TObject; LagCount: Integer);
begin
     // INDISPENSAVEL
     DXDraw1.Surface.Canvas.Release;

     // APAGA A TELA
     DXDraw1.Surface.Canvas.Brush.Color:=rgb(0,0,0);   // PRETO
     DXDraw1.Surface.Canvas.FillRect(rect(0,0,form1.width,form1.height));
     DXDraw1.Surface.Canvas.Release;

     // CAMERA FOCANDO O JOGADOR
     camerax:=round(jog.x-400);
     cameray:=round(jog.y-350);

     // LIMITES DA CAMERA
     if camerax<0 then camerax:=0;
     if cameray<0 then cameray:=0;
     if camerax>(ImageMapa.Picture.Width*97)-850 then camerax:=(ImageMapa.Picture.Width*97)-850;
     if cameray>(ImageMapa.Picture.Height*97)-650 then cameray:=(ImageMapa.Picture.Height*97)-650;

     // DESENHA O CENARIO
     desenhacenario;

     // DESENHA TODAS AS PLATAFORMAS
     desenhaplataforma;

     // DESENHA TODOS OS EFEITOS
     desenhaefeitos;

     // DESENHA O JOGADOR
     desenhajogador;

     // BARRA DE VITALIDADE
     if DXImageListIcones.Items.Count>0 then
       DXImageListIcones.Items[0].draw(dxdraw1.surface,10,100,0);
     barra(24,208-round(jog.hp/10),44,208,rgb(80,80,255));

     // JOGADOR PERDENDO VITALIDADE LENTAMENTE
     if jog.hp>1 then jog.hp:=jog.hp-1;

     // MOVENDO O JOGADOR
     dxinput1.update;

     if (jog.pos<>4) then
     begin
          if isleft in dxinput1.states then moverjogador(-jog.velocidade,0);
          if isright in dxinput1.states then moverjogador(jog.velocidade,0);
     end;

     // LIMITES DO JOGADOR
     if jog.x<20 then jog.x:=20;
     if jog.x>(ImageMapa.Picture.Width*97)-80 then jog.x:=(ImageMapa.Picture.Width*97)-80;


     // JOGADOR MOVENDO AS PERNAS
     if ((isleft in dxinput1.states) or (isright in dxinput1.states))
      and (jog.pos=1) then
      begin
           jog.pos:=2;
           jog.ciclo:=jog.ciclo+1;
           if jog.ciclo>40 then jog.ciclo:=0;
      end;

     // JOGADOR PARADO
     if (isleft in dxinput1.states=false) and (isright in dxinput1.states=false)
      and (jog.pos=2) then
      begin
           jog.pos:=1;
           jog.ciclo:=0;
      end;

     // JOGADOR ATIRANDO
     if (isbutton1 in dxinput1.states) and (jog.bt1key=false) then
     begin
          // NO CHÃO
          if (jog.pos=1) or (jog.pos=2) or (jog.pos=4) then
          begin
               if jog.direcao=1 then criaefeito(1,jog.x+45,jog.y-4);
               if jog.direcao=2 then criaefeito(1,jog.x-45,jog.y-4);
               jog.pos:=4;
          end;

          // NO AR
          if (jog.pos=3) or (jog.pos=5) then
          begin
               if jog.direcao=1 then criaefeito(1,jog.x+45,jog.y-17);
               if jog.direcao=2 then criaefeito(1,jog.x-45,jog.y-17);
               jog.pos:=5;
          end;

          jog.ciclo:=0;

          // SOM DE UM PROJETIL SENDO DISPARADO
          if DXWaveListSons.Items.Count>0 then
            DXWaveListSons.Items[0].Play(false);
     end;

     // CICLO DO JOGADOR ATIRANDO
     if jog.pos=4 then
     begin
          jog.ciclo:=jog.ciclo+1;
          if jog.ciclo>10 then jog.pos:=1;
     end;

     // PULANDO
     if (isbutton2 in dxinput1.states) and (jog.bt2key=false)
      and ((jog.pos=1) or (jog.pos=2)) then
      begin
           jog.pos:=3;
           jog.ciclo:=1;
           jog.gravidade:=-30;
      end;

     // CICLO DO PULO
     if jog.pos=3 then
     begin
          jog.ciclo:=jog.ciclo+1;
          if jog.ciclo>20 then jog.ciclo:=20;
     end;

     // DESENHA A TELA TODA DE UMA VEZ
     DXDraw1.Surface.Canvas.Release;
     DXDraw1.Flip;

     if isbutton1 in dxinput1.states then jog.bt1key:=true;
     if isbutton2 in dxinput1.states then jog.bt2key:=true;
     if isbutton1 in dxinput1.states=false then jog.bt1key:=false;
     if isbutton2 in dxinput1.states=false then jog.bt2key:=false;
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
     // SE APERTAR ESC , SAI DO PROGRAMA
     if key=27 then Application.Terminate;

     // LIGANDO E DESLIGANDO OS DETALHES DO JOGADOR
     if key=81 then
       if detalhesjogador=true then detalhesjogador:=false
       else detalhesjogador:=true;

     // LIGANDO E DESLIGANDO OS DETALHES DAS PLATAFORMAS
     if key=87 then
       if detalhesplataforma=true then detalhesplataforma:=false
       else detalhesplataforma:=true;

     // LIGANDO E DESLIGANDO OS DETALHES DOS EFEITOS
     if key=69 then
       if detalhesefeitos=true then detalhesefeitos:=false
       else detalhesefeitos:=true;
end;

end.
