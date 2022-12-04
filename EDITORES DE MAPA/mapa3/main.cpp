# include <allegro.h>

struct grade{
int x1,y1;
int tipo,verificador;
}tile[21][24];

void allegro_inicia()
{
   allegro_init();
   set_color_depth(32);
   set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
   install_keyboard();
   install_mouse();

}

void editor()
{
   int fim_programa=0,i,j,x1=0,y1=75,tipo_sprite=0;

   BITMAP *buffer;
   BITMAP *mouse;
   BITMAP *map;
   BITMAP *tipo_1;
   BITMAP *tipo_1_b;
   BITMAP *tipo_2;
   BITMAP *menu_1;
   BITMAP *menu_2;
   BITMAP *menu_salvar;
   BITMAP *menu_carregar;

   tipo_2=create_bitmap(25,25);
   tipo_2=load_bitmap("sprites\\sprite_2.bmp",NULL);
   menu_carregar=create_bitmap(87,25);
   menu_carregar=load_bitmap("sprites\\menu_carregar.bmp",NULL);
   menu_salvar=create_bitmap(63,25);
   menu_salvar=load_bitmap("sprites\\menu_salvar.bmp",NULL);
   tipo_1=create_bitmap(25,25);
   tipo_1=load_bitmap("sprites\\sprite_1.bmp",NULL);
   
   tipo_1_b=create_bitmap(25,25);
   tipo_1_b=load_bitmap("sprites\\sprite_selecionado.bmp",NULL);
   
   menu_1=create_bitmap(200,525);
   menu_1=load_bitmap("sprites\\menu_1.bmp",NULL);
   menu_2=create_bitmap(40,25);
   menu_2=load_bitmap("sprites\\menu_2.bmp",NULL);
   mouse=create_bitmap(25,25);
   mouse=load_bitmap("sprites\\mouse.bmp",NULL);
   buffer=create_bitmap(800,600);
   map = create_bitmap(800,600);
   clear_to_color(map,makecol(0,0,0));

   //CRIA A GRID DE EDI플O
   for(i=0;i<21;i++)
   {
      for(j=0;j<24;j++)
      {
         tile[i][j].verificador=0;
         tile[i][j].tipo=0;
         tile[i][j].x1=x1;
         tile[i][j].y1=y1;
         blit(tipo_1,map,0,0,tile[i][j].x1,tile[i][j].y1,25,25);
         x1+=25;
      }
      y1+=25;
      x1=0;
   }

   while(fim_programa==0)
   {


    //MODIFICA TILE SE MOUSE EST� EM CIMA
    for(i=0;i<24;i++)
    {
     for(j=0;j<24;j++)
     {
      if((mouse_x<(tile[i][j].x1+25) && (mouse_x>tile[i][j].x1)) && (mouse_y<(tile[i][j].y1+25) && (mouse_y>tile[i][j].y1)))
      {
      tile[i][j].tipo=1;
      tile[i][j].verificador=1;
      }
      else
      tile[i][j].tipo=0;
      x1+=25;
     }
      y1+=25;
      x1=0;
    }
    for(i=0;i<24;i++)
    {
     for(j=0;j<24;j++)
     {
      if(tile[i][j].tipo==1)
       masked_blit(tipo_1_b,map,0,0,tile[i][j].x1,tile[i][j].y1,25,25);
      else
      if(tile[i][j].verificador==1)
      {
       blit(tipo_1,map,0,0,tile[i][j].x1,tile[i][j].y1,25,25);
       tile[i][j].verificador=0;
      }
     }
    }
    if((mouse_x<740 && mouse_x>700) && (mouse_y<50 && mouse_y>25) && (mouse_b & 1))
    { //SAI DO EDITOR
      fim_programa=1;
    }

    //SELE플O DE TILES
    //=================================================================================
    //SELECIONA TILE VERDE
    if((mouse_x<650) && (mouse_x>625) && (mouse_y<150) && (mouse_y>125) && (mouse_b & 1))
    {
     tipo_sprite=1;
    }

    //MENU DE EDI플O
    masked_blit(menu_1,map,0,0,600,75,200,525);
    masked_blit(menu_carregar,map,0,0,525,25,87,25);
    masked_blit(menu_2,map,0,0,700,25,40,25);
    masked_blit(menu_salvar,map,0,0,625,25,63,25);
    textprintf_ex(map,font,625,575,makecol(255,255,255),-1,"x=%d y=%d",mouse_x,mouse_y);
    textout_ex(map,font,"Tiles:",625,100,makecol(255,255,255),-1);
    blit(tipo_2,map,0,0,625,125,25,25);


      //DESENHA NA TELA
      clear_to_color(buffer,makecol(0,0,0));
      blit(map,buffer,0,0,0,0,800,600);
      masked_blit(mouse,buffer,0,0,mouse_x,mouse_y,25,25);
      blit(buffer,screen,0,0,0,0,800,600);

   }

   destroy_bitmap(buffer);
   destroy_bitmap(tipo_1);
   destroy_bitmap(tipo_1_b);
   destroy_bitmap(map);
   destroy_bitmap(mouse);
   destroy_bitmap(menu_1);
   destroy_bitmap(menu_2);
   destroy_bitmap(menu_salvar);
   destroy_bitmap(menu_carregar);
   //destroy_bitmap();

}

int main()
{
allegro_inicia();
editor();
allegro_exit();
return 0;
}
END_OF_MAIN();


