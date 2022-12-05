// Colisão em uma função 
#include <allegro.h>

// função que verifica colisão
bool colide( int, int, int, int, int, int );

int main()
{
   allegro_init();
   set_color_depth(16);
   install_keyboard();
   set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0);
   
   // inicio da programação atual
   int x = 0;
   int y = 0;
   
   int x_atual = x;
   int y_atual = y;
   
   const int tamanho = 50;
   
   const int obj_tam = 100;
   const int obj_x = ( SCREEN_W - (obj_tam / 2) ) / 2;
   const int obj_y = ( SCREEN_H - (obj_tam / 2) ) / 2;
   
   bool colidindo = false;
   
   // Criando BUFFER para double buffer
   BITMAP *buffer = NULL;
   buffer = create_bitmap(800,800);
   
   
   // Laço principal
   while( !key[KEY_ESC] )
   {
      x_atual = x;
      y_atual = y,obj_y;
      
      
      if ( key[KEY_UP] )
      {
         y--;
         
      }
      
      if ( key[KEY_DOWN] )
      {
         y++;
         
      }
      
      if ( key[KEY_LEFT] )
      {
         x--;
      }
      
      if ( key[KEY_RIGHT] )
      {
         x++;
      }
      
      colidindo = colide( x, y, tamanho, obj_x, obj_y, obj_tam );
      
      
      if ( colidindo == false )
      {
         x_atual = x;
         y_atual = y;
      }
      else
      {
         // volta para os valores antigos
         x = x_atual;
         y = y_atual;
      }
      
      // limpa o nosso novo buffer
      clear( buffer );
      
      circlefill(buffer,obj_x,obj_y,tamanho,makecol(255,255,255));
      // escreve o quadrado no buffer
      rectfill( buffer, x_atual, y_atual, x_atual+tamanho, y_atual+tamanho, makecol(255, 0, 0) );
      
      rectfill( buffer, obj_x, obj_y, obj_x+obj_tam, obj_y+obj_tam, makecol(0, 0, 255) );
      
      // imprimimos as coordenadas x e y na tela para o usuário ver no buffer
      textprintf_ex( buffer, font, 10, 10, makecol(255,0,0), -1, "Variavel X: %d", x);
      textprintf_ex( buffer, font, 10, 20, makecol(255,0,0), -1, "Variavel Y: %d", y);
      
      textprintf_ex( buffer, font, 10, 30, makecol(255,0,0), -1, "obj X: %d", obj_x);
      textprintf_ex( buffer, font, 10, 40, makecol(255,0,0), -1, "obj Y: %d", obj_y);
      textprintf_ex( buffer, font, 10, 50, makecol(255,0,0), -1, "colide: %d", colidindo);
      
      
      // imprime o buffer na tela
      vsync();
      blit(buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
      
      // essa função faz o allegro esperar um pouco antes de voltar para o while
      rest(10);
   }
   
   
   allegro_exit();
   return 0;
}
END_OF_MAIN();
   
// função que verifica colisão
bool colide( int x, int y, int tamanho, int obj_x, int obj_y, int obj_tam )
{
   if ( x+tamanho > obj_x && x < obj_x+obj_tam && y+tamanho > obj_y && y < obj_y+obj_tam )
   {
      return true;
   }
   
   return false;
}
