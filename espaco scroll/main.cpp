#include <allegro.h>
#include <ctime>


int screen_h = 480; 
int screen_w = 640; 

volatile long speed_counter = 0; 

BITMAP *buffer;
BITMAP *layer1;
BITMAP *layer2;
BITMAP *background;

int l1x = 0;
int l1y = 0;
int l2x = 0;
int l2y = 0;
int bgx = 0;
int bgy = 0;

void increment_speed_counter() 
{
     speed_counter++;
}
                 
int main()   
{
    allegro_init();
    install_timer();
    LOCK_VARIABLE(speed_counter); 
    LOCK_FUNCTION(increment_speed_counter);
    install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_w, screen_h, 0, 0);
    buffer = create_bitmap(screen_w, screen_h);
    layer1 = create_bitmap(1280, 480);
    layer2 = create_bitmap(1280, 480);
    background = create_bitmap(1280, 480);
    clear_to_color(layer1, makecol(255, 0, 255)); 
    clear_to_color(layer2, makecol(255, 0, 255)); 
    clear(background);
    for(int i = 0; i < 200; i++)
    { 
      putpixel(layer1, rand() % 640, rand() % 480, makecol(255, 255, 255)); 
      //putpixel(layer2, rand() % 640, rand() % 480, makecol(255, 255, 255));
      circlefill(layer2, rand() %640, rand() % 480, 1, makecol(255, 255, 255));
      putpixel(background, rand() % 640, rand() % 480, makecol(255, 255, 255)); 
    }
    blit(layer1, layer1, 0, 0, 640, 0, 640, 480);
    blit(layer2, layer2, 0, 0, 640, 0, 640, 480);
    blit(background, background, 0, 0, 640, 0, 640, 480);
    
    while(!key[KEY_ESC])  
    {
        while(speed_counter > 0)  
        {
                            draw_sprite(buffer, background, bgx, bgy);
                            draw_sprite(buffer, layer1, l1x, l1y);
                            draw_sprite(buffer, layer2, l2x, l2y);
                            bgx -= 1;
                            l1x -= 2;
                            l2x -= 3;
                            if ((bgx+640) <= 0)
                               bgx = 0;
                            if ((l1x+640) <= 0)
                               l1x = 0;
                            if ((l2x+640) <= 0)
                               l2x = 0;

        speed_counter --;                   
        }
        draw_sprite(screen, buffer, 0, 0);    
        
    }
    return 0;
}
END_OF_MAIN();
