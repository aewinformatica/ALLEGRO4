/*
 * Example program to show how to use EBox to determine when two bitmaps collide.
 * Jon Rafkind
 *
 * This code is freely distributable and can be changed or used any way you want.
 * There is no guarantee this code will work properly. Use at your own risk.
 */


#include "ebox.h"
#include "allegro.h"
#include <stdlib.h>
#include <time.h>

volatile int speed_counter = 0;

void inc_speed_counter(){
	speed_counter++;
}
END_OF_FUNCTION( inc_speed_counter );

//some online guide said this is the best way to get a random number using the rand()
//function. i dont know why, i just use it.
int rnd( int q ){
	return (int)( q * ( (double)rand() / ( (double)RAND_MAX + 1.0 ) ) );
}

void init(){
	allegro_init();
	srand( time( NULL ) );
	install_timer();
	install_keyboard();
	set_color_depth( 16 );
	set_gfx_mode( GFX_AUTODETECT, 640, 480, 0, 0 );
	text_mode( -1 );
	LOCK_VARIABLE( speed_counter );
	LOCK_FUNCTION( (void *)inc_speed_counter );
	install_int_ex( inc_speed_counter, MSEC_TO_TIMER( 40 ) );
}

struct object{
	int x, y; //position of object
	BITMAP * bmp; //bitmap of object
	ECollide * col; //collide object
	bool collided; //remember if we collided with something

	int dx, dy; //direction object is moving in
};

int main(){
	init();

	//initialize objects
	object olist[10];
	for ( int q = 0; q < 10; q++ ){
		olist[q].x = rnd( 640 );
		olist[q].y = rnd( 480 );
		olist[q].bmp = create_bitmap( 90, 90 );
		clear_to_color( olist[q].bmp, makecol(255,0,255) );
		olist[q].collided = false;
		olist[q].dx = rnd(2) * 2 - 1;
		olist[q].dy = rnd(2) * 2 - 1;
		//place random circles on the bitmap
		for ( int z = 0; z < 5; z++ )
			circlefill( olist[q].bmp, rnd(90), rnd(90), rnd(20)+10, makecol(rnd(128)+50,20,rnd(255) ) );

		// change any of these variables to see what happens with the collide object
		int size = 5; //randge 1-infinity
		double percent = 45; //range 1-100

		olist[q].col = new ECollide( olist[q].bmp, size, makecol(255,0,255), percent );
	}

	BITMAP * work = create_bitmap( 640, 480 );
	clear( work );

	bool draw_boxes = true;
	bool hold = false;
	while ( !key[KEY_ESC] ){

		if ( key[KEY_D] && !hold ){
			draw_boxes = !draw_boxes;
			hold = true;
		}
		if ( !key[KEY_D] ) hold = false;

		while ( speed_counter > 0 ){

			for ( int q = 0; q < 10; q++ )olist[q].collided = false;

			for ( int q = 0; q < 10; q++ ){

				olist[q].x += olist[q].dx*2;
				olist[q].y += olist[q].dy*2;
				if( olist[q].x < 0 || olist[q].x > 640 ) olist[q].dx = -olist[q].dx;
				if( olist[q].y < 0 || olist[q].y > 480 ) olist[q].dy = -olist[q].dy;

				//test to see if the current object collided with any other object
				for ( int m = 0; m < 10; m++ )
					if ( m != q )
						if ( olist[q].col->Collision(olist[m].col,olist[q].x,
							olist[q].y, olist[m].x, olist[m].y ) ){
							olist[q].collided = true;
							olist[m].collided = true;
						}
				
			}
			
			speed_counter--;
		}

		for ( int q = 0; q < 10; q++  ){
			int x = olist[q].x - olist[q].bmp->w/2;
			int y = olist[q].y - olist[q].bmp->h/2;
			draw_sprite( work, olist[q].bmp, x, y );
			if ( draw_boxes )
			olist[q].col->display( work, olist[q].x, olist[q].y );
			textprintf( work, font, olist[q].x, olist[q].y, makecol(255,255,255), "Box:%d", q );
			if ( olist[q].collided ) 
				textprintf(work,font,1,q*10,makecol(255,255,255),"Box %d collided with something!", q );
			else	textprintf(work,font,1,q*10,makecol(255,255,255),"Box %d has not collided");
		}
		textprintf(work,font,1,470,makecol(255,255,255),"Press D to toggle drawing of collision boxes");
		blit( work, screen, 0, 0, 0, 0, 640, 480 );
		clear( work );
		
	}
	destroy_bitmap( work );
	for ( int q = 0; q < 10; q++ ){
		destroy_bitmap( olist[q].bmp );
		delete olist[q].col;
	}

}
END_OF_MAIN();


