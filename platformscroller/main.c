

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define JUMPIT 1600

//define the sprite structure
typedef struct SPRITE
{
    int dir, alive;
    int x,y;
    int width,height;
    int xspeed,yspeed;
    int xdelay,ydelay;
    int xcount,ycount;
    int curframe,maxframe,animdir;
    int framecount,framedelay;
}SPRITE;

//declare the bitmaps and sprites
BITMAP *player_image[8];
SPRITE *player;
BITMAP *buffer;	
BITMAP *temp;


//tile grabber
BITMAP *grabframe(BITMAP *source, 
                  int width, int height, 
                  int startx, int starty, 
                  int columns, int frame)
{
    BITMAP *temp = create_bitmap(width,height);
    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    blit(source,temp,x,y,0,0,width,height);
    return temp;
}


int collided(int x, int y)
{
    BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tl;
}


int main (void)
{
    int mapxoff, mapyoff;
    int oldpy, oldpx;
    int facing = 0;
    int jump = JUMPIT;
    int n;

	allegro_init();	
	install_timer();
	install_keyboard();


	set_color_depth(16);
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);


    temp = load_bitmap("guy.bmp", NULL);
    for (n=0; n<8; n++)
        player_image[n] = grabframe(temp,50,64,0,0,8,n);
    destroy_bitmap(temp);

    player = malloc(sizeof(SPRITE));
    player->x = 80;
    player->y = 100;
    player->curframe=0;
    player->framecount=0;
    player->framedelay=6;
    player->maxframe=7;
    player->width=player_image[0]->w;
    player->height=player_image[0]->h;


    //load the map
	if (MapLoad("sample.fmp")) exit(0);	

    //create the double buffer
	buffer = create_bitmap (WIDTH, HEIGHT);
	clear(buffer);

    //main loop
	while (!key[KEY_ESC])
	{

		oldpy = player->y; 
        oldpx = player->x;

		if (key[KEY_RIGHT]) 
        { 
            facing = 1; 
            player->x+=2; 
            if (++player->framecount > player->framedelay)
            {
                player->framecount=0;
                if (++player->curframe > player->maxframe)
                    player->curframe=1;
            }
        }
        else if (key[KEY_LEFT]) 
        { 
            facing = 0; 
            player->x-=2; 
            if (++player->framecount > player->framedelay)
            {
                player->framecount=0;
                if (++player->curframe > player->maxframe)
                    player->curframe=1;
            }
        }
        else player->curframe=0;

        //handle jumping
        if (jump==JUMPIT)
        { 
            if (!collided(player->x + player->width/2, 
                player->y + player->height + 5))
                jump = 0; 

		    if (key[KEY_SPACE]) 
                jump = 30;
        }
        else
        {
            player->y -= jump/3; 
            jump--; 
        }

		if (jump<0) 
        { 
            if (collided(player->x + player->width/2, 
                player->y + player->height))
			{ 
                jump = JUMPIT; 
                while (collided(player->x + player->width/2, 
                    player->y + player->height))
                    player->y -= 2; 
            } 
        }

        //check for collided with foreground tiles
		if (!facing) 
        { 
            if (collided(player->x, player->y + player->height)) 
                player->x = oldpx; 
        }
		else 
        { 
            if (collided(player->x + player->width, 
                player->y + player->height)) 
                player->x = oldpx; 
        }
		
        //update the map scroll position
		mapxoff = player->x + player->width/2 - WIDTH/2 + 10;
		mapyoff = player->y + player->height/2 - HEIGHT/2 + 10;


        //avoid moving beyond the map edge
		if (mapxoff < 0) mapxoff = 0;
		if (mapxoff > (mapwidth * mapblockwidth - WIDTH))
            mapxoff = mapwidth * mapblockwidth - WIDTH;
		if (mapyoff < 0) 
            mapyoff = 0;
		if (mapyoff > (mapheight * mapblockheight - HEIGHT)) 
            mapyoff = mapheight * mapblockheight - HEIGHT;

        //draw the background tiles
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1);

        //draw foreground tiles
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1, 0);

        //draw the player's sprite
		if (facing) 
            draw_sprite(buffer, player_image[player->curframe], 
                (player->x-mapxoff), (player->y-mapyoff+1));
		else 
            draw_sprite_h_flip(buffer, player_image[player->curframe], 
                (player->x-mapxoff), (player->y-mapyoff));

        //blit the double buffer 
		vsync();
        acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH-1, HEIGHT-1);
        release_screen();

	} //while

    for (n=0; n<9; n++)
        destroy_bitmap(player_image[n]);
    free(player);
	destroy_bitmap(buffer);
	MapFreeMem ();
	allegro_exit();
	return 0;
}

END_OF_MAIN();

