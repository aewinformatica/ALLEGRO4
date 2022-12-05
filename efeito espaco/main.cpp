#include <math.h>
#include <allegro.h>
#define maxstars 500

int screen_h = 480; //screen height
int screen_w = 640; //screen width
volatile long speed_counter = 0;
float pi = 3.142; //definition of Pi

BITMAP *buffer; // create buffer

void increment_speed_counter() 
{
     speed_counter++;
}

class Star
{
      public:       
             int posx; //x position
             int posy;  //y position
             int speed; //there will be three speeds
             float length; //essentially how far from the origin the pixel is.
             float angle; // the direction the pixel is moving.
             Star();
             void movestars();
      private:  
};

Star::Star()
{
            angle = (rand() % 1256) * 0.005; //generate random angle 
            speed = (rand() % 5) + 1; // generate random speed for movement
            length = (rand() % 600); // generate random length
            // calculate x, y from a vector with known length and angle
            posx = length * cos (angle);
            posy = length * sin (angle);
                                 
            putpixel(buffer, posx + screen_w/2, posy + screen_h/2, makecol(255, 255, 255)); //place pixels in their start position
       
            
}

void Star::movestars()
{
          putpixel(buffer, posx + screen_w/2, posy + screen_h/2, makecol(0, 0, 0)); //mask old pixel position
          length += speed; //distance pixel is to move 
          if (length > 600) // if the pixel goes offscreen / generate a new position at the origin
          {
             length = (rand() % 100);
          }
          
          posx = length * cos (angle); //calculate new pixel positions
          posy = length * sin (angle);
          
          putpixel(buffer, posx + screen_w/2, posy + screen_h/2, makecol(255, 255, 255)); //place pixels in their start position
            
          
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
    srand(time(NULL));
    
    Star star[maxstars]; //use the class to initialise the stars and their positions
       
    while(!key[KEY_ESC])
    {
        while(speed_counter > 0)
        {
                            for(int i = 0; i < maxstars; ++i)//for every pixel
                            {
                                    star[i].movestars();               
                            }
                            speed_counter --;
        }
        draw_sprite(screen, buffer, 0, 0);
        
    }
    return 0;
}
END_OF_MAIN();