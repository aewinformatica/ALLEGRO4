

#include "warbirds.h"


//reuse our friendly tile grabber from chapter 9
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


void loadsprites(void)
{
    int n;

    //load progress bar
    temp = load_bitmap("progress.bmp", NULL);
    progress = grabframe(temp,130,14,0,0,1,0);
    bar = grabframe(temp,6,10,130,2,1,0);
    destroy_bitmap(temp);

    //load bonus shot
    bonus_shot_image = load_bitmap("bonusshot.bmp", NULL);
    bonus_shot = malloc(sizeof(SPRITE));
    bonus_shot->alive=0;
    bonus_shot->x = 0;
    bonus_shot->y = 0;
    bonus_shot->width = bonus_shot_image->w;
    bonus_shot->height = bonus_shot_image->h;
    bonus_shot->xdelay = 0;
    bonus_shot->ydelay = 2;
    bonus_shot->xcount = 0;
    bonus_shot->ycount = 0;
    bonus_shot->xspeed = 0;
    bonus_shot->yspeed = 1;
    bonus_shot->curframe = 0;
    bonus_shot->maxframe = 0;
    bonus_shot->framecount = 0;
    bonus_shot->framedelay = 0;


    //load player airplane sprite
    temp = load_bitmap("p38.bmp", NULL);
    for (n=0; n<3; n++)
        player_images[n] = grabframe(temp,64,64,0,0,3,n);
    destroy_bitmap(temp);

    //initialize the player's sprite
    player = malloc(sizeof(SPRITE));
    player->x = 320-32;
    player->y = 400;
    player->width = player_images[0]->w;
    player->height = player_images[0]->h;
    player->xdelay = 1;
    player->ydelay = 0;
    player->xcount = 0;
    player->ycount = 0;
    player->xspeed = 0;
    player->yspeed = 0;
    player->curframe = 0;
    player->maxframe = 2;
    player->framecount = 0;
    player->framedelay = 10;
    player->animdir = 1;

    //load bullet images
    bullet_images[0] = load_bitmap("bullets.bmp", NULL);

    //initialize the bullet sprites
    for (n=0; n<MAX_BULLETS; n++)
    {
        bullets[n] = malloc(sizeof(SPRITE));
        bullets[n]->alive = 0;
        bullets[n]->x = 0;
        bullets[n]->y = 0;
        bullets[n]->width = bullet_images[0]->w;
        bullets[n]->height = bullet_images[0]->h;
        bullets[n]->xdelay = 0;
        bullets[n]->ydelay = 0;
        bullets[n]->xcount = 0;
        bullets[n]->ycount = 0;
        bullets[n]->xspeed = 0;
        bullets[n]->yspeed = -2;
        bullets[n]->curframe = 0;
        bullets[n]->maxframe = 0;
        bullets[n]->framecount = 0;
        bullets[n]->framedelay = 0;
        bullets[n]->animdir = 0;
    }

    //load enemy plane sprites
    temp = load_bitmap("enemyplane1.bmp", NULL);
    for (n=0; n<3; n++)
        enemy_plane_images[n] = grabframe(temp,32,32,0,0,3,n);
    destroy_bitmap(temp);

    //initialize the enemy planes
    for (n=0; n<MAX_ENEMIES; n++)
    {
        enemy_planes[n] = malloc(sizeof(SPRITE));
        enemy_planes[n]->alive = 0;
        enemy_planes[n]->x = rand() % 100 + 50;
        enemy_planes[n]->y = 0;
        enemy_planes[n]->width = enemy_plane_images[0]->w;
        enemy_planes[n]->height = enemy_plane_images[0]->h;
        enemy_planes[n]->xdelay = 4;
        enemy_planes[n]->ydelay = 4;
        enemy_planes[n]->xcount = 0;
        enemy_planes[n]->ycount = 0;
        enemy_planes[n]->xspeed = (rand() % 2 - 3);
        enemy_planes[n]->yspeed = 1;
        enemy_planes[n]->curframe = 0;
        enemy_planes[n]->maxframe = 2;
        enemy_planes[n]->framecount = 0;
        enemy_planes[n]->framedelay = 10;
        enemy_planes[n]->animdir = 1;
    }

    //load explosion sprites
    temp = load_bitmap("explosion.bmp", NULL);
    for (n=0; n<6; n++)
        explosion_images[n] = grabframe(temp,32,32,0,0,6,n);
    destroy_bitmap(temp);

    //initialize the sprites
    for (n=0; n<MAX_EXPLOSIONS; n++)
    {
        explosions[n] = malloc(sizeof(SPRITE));
        explosions[n]->alive = 0;
        explosions[n]->x = 0;
        explosions[n]->y = 0;
        explosions[n]->width = explosion_images[0]->w;
        explosions[n]->height = explosion_images[0]->h;
        explosions[n]->xdelay = 0;
        explosions[n]->ydelay = 8;
        explosions[n]->xcount = 0;
        explosions[n]->ycount = 0;
        explosions[n]->xspeed = 0;
        explosions[n]->yspeed = -1;
        explosions[n]->curframe = 0;
        explosions[n]->maxframe = 5;
        explosions[n]->framecount = 0;
        explosions[n]->framedelay = 15;
        explosions[n]->animdir = 1;
    }

    //load explosion sprites
    temp = load_bitmap("bigexplosion.bmp", NULL);
    for (n=0; n<8; n++)
        bigexp_images[n] = grabframe(temp,64,64,0,0,7,n);
    destroy_bitmap(temp);

    //initialize the sprites
    bigexp = malloc(sizeof(SPRITE));
    bigexp->alive = 0;
    bigexp->x = 0;
    bigexp->y = 0;
    bigexp->width = bigexp_images[0]->w;
    bigexp->height = bigexp_images[0]->h;
    bigexp->xdelay = 0;
    bigexp->ydelay = 8;
    bigexp->xcount = 0;
    bigexp->ycount = 0;
    bigexp->xspeed = 0;
    bigexp->yspeed = -1;
    bigexp->curframe = 0;
    bigexp->maxframe = 6;
    bigexp->framecount = 0;
    bigexp->framedelay = 10;
    bigexp->animdir = 1;

}

int inside(int x,int y,int left,int top,int right,int bottom)
{
    if (x > left && x < right && y > top && y < bottom)
        return 1;
    else
        return 0;
}

void updatesprite(SPRITE *spr)
{
    //update x position
    if (++spr->xcount > spr->xdelay)
    {
        spr->xcount = 0;
        spr->x += spr->xspeed;
    }

    //update y position
    if (++spr->ycount > spr->ydelay)
    {
        spr->ycount = 0;
        spr->y += spr->yspeed;
    }

    //update frame based on animdir
    if (++spr->framecount > spr->framedelay)
    {
        spr->framecount = 0;
        if (spr->animdir == -1)
        {
            if (--spr->curframe < 0)
                spr->curframe = spr->maxframe;
        }
        else if (spr->animdir == 1)
        {
            if (++spr->curframe > spr->maxframe)
                spr->curframe = 0;
        }
    }
}

void startexplosion(int x, int y)
{
    int n;
    for (n=0; n<MAX_EXPLOSIONS; n++)
    {
        if (!explosions[n]->alive)
        {
            explosions[n]->alive++;
            explosions[n]->x = x;
            explosions[n]->y = y;
            break;
        }
    }

    //launch bonus shot if ready
    if (!bonus_shot->alive)
    {
        bonus_shot->alive++;
        bonus_shot->x = x;
        bonus_shot->y = y;
    }
}

void updateexplosions()
{
    int n, c=0;

    for (n=0; n<MAX_EXPLOSIONS; n++)
    {
        if (explosions[n]->alive)
        {
            c++;
            updatesprite(explosions[n]);
            draw_sprite(buffer, explosion_images[explosions[n]->curframe],
                explosions[n]->x, explosions[n]->y);

            if (explosions[n]->curframe >= explosions[n]->maxframe)
            {
                explosions[n]->curframe=0;
                explosions[n]->alive=0;
            }
        }
    }
    textprintf(buffer,font,0,430,WHITE,"explosions %d", c);

    //update the big "player" explosion if needed
    if (bigexp->alive)
    {
        updatesprite(bigexp);
        draw_sprite(buffer, bigexp_images[bigexp->curframe], 
            bigexp->x, bigexp->y);
        if (bigexp->curframe >= bigexp->maxframe)
        {
            bigexp->curframe=0;
            bigexp->alive=0;
        }
    }
}

void updatebonuses()
{
    int x,y,x1,y1,x2,y2;

    //add more bonuses here

    //update bonus shot if alive
    if (bonus_shot->alive)
    {
        updatesprite(bonus_shot);
        draw_sprite(buffer, bonus_shot_image, bonus_shot->x, bonus_shot->y);
        if (bonus_shot->y > HEIGHT)
            bonus_shot->alive=0;

        //see if player got the bonus
        x = bonus_shot->x + bonus_shot->width/2;
        y = bonus_shot->y + bonus_shot->height/2;
        x1 = player->x;
        y1 = player->y;
        x2 = x1 + player->width;
        y2 = y1 + player->height;

        if (inside(x,y,x1,y1,x2,y2))
        {
            //increase firing rate
            if (firedelay>20) firedelay-=2;

            bonus_shot->alive=0;
        }
    }

}

void updatebullet(SPRITE *spr)
{
    int n,x,y;
    int x1,y1,x2,y2;

    //move the bullet
    updatesprite(spr);

    //check bounds
    if (spr->y < 0)
    {
        spr->alive = 0;
        return;
    }

    for (n=0; n<MAX_ENEMIES; n++)
    {
        if (enemy_planes[n]->alive)
        {
            //find center of bullet
            x = spr->x + spr->width/2;
            y = spr->y + spr->height/2;

            //get enemy plane bounding rectangle
            x1 = enemy_planes[n]->x;
            y1 = enemy_planes[n]->y - yoffset;
            x2 = x1 + enemy_planes[n]->width;
            y2 = y1 + enemy_planes[n]->height;

            //check for collisions
            if (inside(x, y, x1, y1, x2, y2))
            {
                enemy_planes[n]->alive=0;
                spr->alive=0;
                startexplosion(spr->x+16, spr->y);
                score+=2;
                break;
            }
        }
    }
}

void updatebullets()
{
    int n;
    //update/draw bullets
    for (n=0; n<MAX_BULLETS; n++)
        if (bullets[n]->alive)
        {
            updatebullet(bullets[n]);
            draw_sprite(buffer,bullet_images[0], bullets[n]->x, bullets[n]->y);
        }
}

void bouncex_warpy(SPRITE *spr)
{
    //bounces x off bounds
    if (spr->x < 0 - spr->width)
    {
        spr->x = 0 - spr->width + 1;
        spr->xspeed *= -1;
    }

    else if (spr->x > SCREEN_W)
    {
        spr->x = SCREEN_W - spr->xspeed;
        spr->xspeed *= -1;
    }

    //warps y if plane has passed the player
    if (spr->y > yoffset + 2000)
    {
        //respawn enemy plane
        spr->y = yoffset - 1000 - rand() % 1000;
        spr->alive++;
        spr->x = rand() % WIDTH;
    }

    //warps y from bottom to top of level
    if (spr->y < 0)
    {
        spr->y = 0;
    }

    else if (spr->y > 48000)
    {
        spr->y = 0;
    }

}


void fireatenemy()
{
    int n;
    for (n=0; n<MAX_BULLETS; n++)
    {
        if (!bullets[n]->alive)
        {
            bullets[n]->alive++;
            bullets[n]->x = player->x;
            bullets[n]->y = player->y;
            return;
        }
    }
}

void displayprogress(int life)
{
    int n;
    draw_sprite(buffer,progress,490,15);
    
    for (n=0; n<life; n++)
        draw_sprite(buffer,bar,492+n*5,17);
    
}

void updateenemyplanes()
{
    int n, c=0;

    //update/draw enemy planes
    for (n=0; n<MAX_ENEMIES; n++)
    {
        if (enemy_planes[n]->alive)
        {
            c++;
            updatesprite(enemy_planes[n]);
            bouncex_warpy(enemy_planes[n]);

            //is plane visible on screen?
            if (enemy_planes[n]->y > yoffset-32 && enemy_planes[n]->y < yoffset + HEIGHT+32)
            {
                //draw enemy plane
                draw_sprite(buffer, enemy_plane_images[enemy_planes[n]->curframe],
                    enemy_planes[n]->x, enemy_planes[n]->y - yoffset);
            }
        }
        //reset plane
        else
        {
            enemy_planes[n]->alive++;
            enemy_planes[n]->x = rand() % 100 + 50;
            enemy_planes[n]->y = yoffset - 2000 + rand() % 2000;
        }
    }
    textprintf(buffer,font,0,470,WHITE,"enemies %d", c);
}

void updatescroller()
{
    //make sure it doesn't scroll beyond map edge
    if (yoffset < 5) 
    {
        //level is over
        yoffset = 5;
        textout_centre(buffer, font, "END OF LEVEL", SCREEN_W/2, SCREEN_H/2, WHITE);
    }
    if (yoffset > BOTTOM) yoffset = BOTTOM;


    //scroll map up 1 pixel
    yoffset-=1;

    //draw map with single layer
    MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W-1, SCREEN_H-1);


}

void updateplayer()
{
    int n,x,y,x1,y1,x2,y2;

    //update/draw player sprite
    updatesprite(player);
    draw_sprite(buffer, player_images[player->curframe], 
        player->x, player->y);

    //check for collision with enemy planes
    x = player->x + player->width/2;
    y = player->y + player->height/2;
    for (n=0; n<MAX_ENEMIES; n++)
    {
        if (enemy_planes[n]->alive)
        {
            x1 = enemy_planes[n]->x;
            y1 = enemy_planes[n]->y - yoffset;
            x2 = x1 + enemy_planes[n]->width;
            y2 = y1 + enemy_planes[n]->height;
            if (inside(x,y,x1,y1,x2,y2))
            {
                enemy_planes[n]->alive=0;
                if (health > 0) health--;
                bigexp->alive++;
                bigexp->x = player->x;
                bigexp->y = player->y;
                score++;
            }
        }
    }
}

void displaystats()
{
    //display some status information
    textprintf(buffer,font,0,420,WHITE,"firing rate %d", firedelay);
    textprintf(buffer,font,0,440,WHITE,"yoffset %d",yoffset);
    textprintf(buffer,font,0,450,WHITE,"counter %d", counter);
    textprintf(buffer,font,0,460,WHITE,"framerate %d", framerate);

    //display score
    textprintf(buffer,font,22,22,GRAY,"SCORE: %d", score);
    textprintf(buffer,font,20,20,RED,"SCORE: %d", score);
}

void checkinput()
{
    //check for keyboard input
    if (key[KEY_UP])
    {
        player->y -= 1;
        if (player->y < 100)
            player->y = 100;
    }
    if (key[KEY_DOWN])
    {
        player->y += 1;
        if (player->y > HEIGHT-65)
            player->y = HEIGHT-65;
    }
    if (key[KEY_LEFT])
    {
        player->x -= 1;
        if (player->x < 0)
            player->x = 0;
    }
    if (key[KEY_RIGHT])
    {
        player->x += 1;
        if (player->x > WIDTH-65)
            player->x = WIDTH-65;
    }

    if (key[KEY_SPACE])
    {
        if (firecount > firedelay)
        {
            firecount = 0;
            fireatenemy();
        }
    }

}

//calculate framerate every second
void timer1(void)
{
    counter++;
    framerate = ticks;
    ticks=0;
//    rest(2);
}
END_OF_FUNCTION(timer1)

void initialize()
{
    //initialize program
	allegro_init();	
	install_timer();
	install_keyboard();
    set_color_depth(16);
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    text_mode(-1);
    srand(time(NULL));

    //create the double buffer and clear it
	buffer = create_bitmap(SCREEN_W, SCREEN_H);	
	if (buffer==NULL) 
    { 
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Error creating double buffer");
        return;
    }
	clear(buffer);

    //load the Mappy file
	if (MapLoad("level1.fmp"))
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message ("Can't find level1.fmp");
		return;
	}

    //set palette
    MapSetPal8();

    //identify variables used by interrupt function
    LOCK_VARIABLE(counter);
    LOCK_VARIABLE(framerate);
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(timer1);

    //create new interrupt handler
    install_int(timer1, 1000);
}


void main (void)
{
    int n;

    //init game
    initialize();
    loadsprites();

    //main loop
    while (!key[KEY_ESC])
	{
        checkinput();

        updatescroller();

        updateplayer();
        updateenemyplanes();

        updatebullets();            
        updateexplosions();
        updatebonuses();

        displayprogress(health);
        displaystats();

        //blit the double buffer
        acquire_screen();
		blit (buffer, screen, 0, 0, 0, 0, SCREEN_W-1, SCREEN_H-1);
        release_screen();

        ticks++;
        firecount++;
	}

    //delete the Mappy level
	MapFreeMem();

    //delete bitmaps
	destroy_bitmap(buffer);
    destroy_bitmap(progress);
    destroy_bitmap(bar);

    for (n=0; n<6; n++)
        destroy_bitmap(explosion_images[n]);

    for (n=0; n<3; n++)
    {
        destroy_bitmap(player_images[n]);
        destroy_bitmap(bullet_images[n]);
        destroy_bitmap(enemy_plane_images[n]);
    }

    //delete sprites
    free(player);
    for (n=0; n<MAX_EXPLOSIONS; n++)
        free(explosions[n]);
    for (n=0; n<MAX_BULLETS; n++)
        free(bullets[n]);
    for (n=0; n<MAX_ENEMIES; n++)
        free(enemy_planes[n]);

	allegro_exit();
	return;
}

END_OF_MAIN()

