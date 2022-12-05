#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>
#include <alpng.h>  
#define WHITE makecol(255,255,255)
#define BLACK makecol(0,0,0)
BITMAP *kitty[7];
char s[20];
int curframe=1, framedelay=25, framecount=0;
int x=100, y=200, n;
int main(void)
{
//initialize the program
allegro_init();
install_keyboard();
install_timer();
set_color_depth(16);
set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

BITMAP *Buffer =NULL;
       Buffer = create_bitmap(640,480);

textout_ex(screen, font, "AnimSprite Program (ESC to quit)",0, 0, WHITE,-1);

for (n=1; n<4; n++)
{
sprintf(s,"soco/soco%d.bmp",n+1);
kitty[n] = load_bitmap(s, NULL);
}
//main loop
while(!key[KEY_ESC])
{

clear(Buffer);

x += 2;
if (x > SCREEN_W - kitty[1]->w)
x = 0;

//update the frame
if (framecount++ > framedelay)
{
framecount = 1;
curframe++;
if (curframe >3)
curframe = 1;
}

//draw the sprite
draw_sprite(Buffer, kitty[curframe], x, y);
//display logistics
textprintf_ex(Buffer, font, 0, 20, WHITE,-1,
"Sprite X,Y: %3d,%3d", x, y);
textprintf_ex(Buffer, font, 0, 40, WHITE,-1,
"Frame,Count,Delay: %2d,%2d,%2d",
curframe, framecount, framedelay);
/*
acquire_screen();
release_screen();
*/
blit(Buffer,screen,0,0,0,0,640,480);

}
return 0;
}
END_OF_MAIN();
