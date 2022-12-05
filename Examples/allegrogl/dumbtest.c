#ifndef DEBUGMODE
#define DEBUGMODE
#endif

#include <allegro.h>
#include "alleggl.h"

volatile int chrono = 0;

void the_timer(void) {
	chrono++;
} END_OF_FUNCTION(the_timer);


int main()
{
	float rotate = 0;
	BITMAP *save_bmp;
	BITMAP *sub_screen, *sub_screen2, *vid_bitmap, *mem_bitmap, *vid_bitmap2;
	int keypress;
	int frame_count = 0, frame_count_time = 0;
	float fps_rate = 0.0;

	allegro_init();
	install_allegro_gl();
	install_timer();
	
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_WINDOWED, TRUE);
	allegro_gl_set(AGL_COLOR_DEPTH, 16);
	allegro_gl_set(AGL_SUGGEST, AGL_DOUBLEBUFFER | AGL_WINDOWED
	             | AGL_COLOR_DEPTH);

	if(set_gfx_mode(GFX_OPENGL, 640, 480, 0, 0)<0) {
		allegro_message ("Error setting OpenGL graphics mode:\n%s\n"
		                 "Allegro GL error : %s\n",
		                 allegro_error, allegro_gl_error);
		exit(0);
	}

	save_bmp = create_bitmap (640, 480);
	ASSERT (save_bmp);
	
	sub_screen  = create_sub_bitmap(screen, 320, 0, 128, 96);
	sub_screen2 = create_sub_bitmap(screen, 448, 0, 128, 96);
	ASSERT(sub_screen);
	ASSERT(sub_screen2);
	
	vid_bitmap = create_video_bitmap(50, 50);
	clear(vid_bitmap);
	rect(vid_bitmap, 10, 10, 39, 39, makecol(255, 255, 255));
	rectfill(vid_bitmap, 12, 12, 37, 37, makecol(255, 0, 0));
	triangle(vid_bitmap, 9, 9, 46, 3, 0, 16, makecol(0, 255, 0));
	
	mem_bitmap = create_bitmap(50, 50);
	clear(mem_bitmap);
	blit(vid_bitmap, mem_bitmap, 25, 25, 10, 10, 50, 50);
	save_bitmap("dumbtest.bmp", mem_bitmap, NULL);
	
	vid_bitmap2 = create_video_bitmap(50, 50);
	clear(vid_bitmap2);
	blit(mem_bitmap, vid_bitmap2, 0, 0, 10, 10, 50, 50);

	install_keyboard();
	
	LOCK_VARIABLE(chrono);
	LOCK_FUNCTION(the_timer);
	
	install_int(the_timer, 5);
	
	/* Setup OpenGL like we want */
	glEnable(GL_TEXTURE_2D);

	
	TRACE("Starting main loop\n");

	do {
		if (keypressed())
			keypress = readkey();
		else
			keypress = 0;

		rotate+=1.0;
		if(rotate>360.0) rotate=0;

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glPushMatrix();
		glRotatef(45, 1, 0, 0);
		glRotatef(rotate, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glColor3f(1.0, 0.0, 1.0);
			glVertex3f(0.1, 0.1, 0.1);
			glVertex3f(0.1, -0.1, 0.1);
			glVertex3f(-0.1, -0.1, 0.1);
			glVertex3f(-0.1, 0.1, 0.1);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0.1, 0.1, -0.1);
			glVertex3f(0.1, -0.1, -0.1);
			glVertex3f(-0.1, -0.1, -0.1);
			glVertex3f(-0.1, 0.1, -0.1);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0.1, 0.1, 0.1);
			glVertex3f(0.1, -0.1, 0.1);
			glVertex3f(0.1, -0.1, -0.1);
			glVertex3f(0.1, 0.1, -0.1);
		glEnd();
		glPopMatrix();
			
		frame_count++;
		
		if (frame_count >= 20) {
			if (chrono - frame_count_time)
				fps_rate = frame_count * 200.0 / (chrono - frame_count_time);
			else
				fps_rate = frame_count * 200.0;
			frame_count_time = chrono;
			frame_count = 0;
		}

		allegro_gl_set_allegro_mode();
		line(screen, 0, 0, 640, 480, makecol(0xff, 0, 0xff));
		triangle(screen, 115, 21, 230, 120, 5, 90, makecol(0xff, 0, 0));
		rect(screen, 0, 0, 639, 479, makecol(0xff, 0xbf, 0x00));
		putpixel(screen, 0, 0, makecol(0xff, 0xff, 0xff));
		putpixel(screen, 639, 0, makecol(0xff, 0xff, 0xff));
		putpixel(screen, 0, 479, makecol(0xff, 0xff, 0xff));
		putpixel(screen, 639, 479, makecol(0xff, 0xff, 0xff));
		putpixel(screen, 0, 478, makecol(0xff, 0xff, 0xff));
		putpixel(screen, 639, 478, makecol(0xff, 0xff, 0xff));
		rectfill(screen, 16, 240, 256, 360, makecol(0xff, 0xff, 0xff));

#if (((ALLEGRO_VERSION << 16) | (ALLEGRO_SUB_VERSION << 8) | ALLEGRO_WIP_VERSION) >= 0x040104)
		textout_ex(screen, font, "Partially cut off text", -3, 400,
		        makecol(255,255,255), -1);
		textout_ex(screen, font, "Hello World!", 70, 256, makecol(0,0,0), -1);
		textprintf_ex(screen, font, 70, 270, makecol(0, 0, 0), -1,
		           "FPS: %.2f", fps_rate);
#else
		text_mode(-1);
		textout(screen, font, "Partially cut off text", -3, 400,
		        makecol(255,255,255));
		textout(screen, font, "Hello World!", 70, 256, makecol(0,0,0));
		textprintf(screen, font, 70, 270, makecol(0, 0, 0),
		           "FPS: %.2f", fps_rate);
#endif
		
		/* Now here's the interesting part. This section of code tests
		 * the various blit combinations supported by AllegroGL. That is:
		 *                 screen -> sub-bitmap of screen
		 *   sub-bitmap of screen -> sub-bitmap of screen
		 *                 screen -> screen
		 *           video bitmap -> screen
		 *                 screen -> memory bitmap
		 *          memory bitmap -> screen
		 *
		 *   Other combinations are tested either at the init
		 *   (memory <-> video) or aren't available (video -> video).
		 */
		blit (screen, sub_screen, 256, 192, 0, 0, 128, 96);
		blit (sub_screen, sub_screen2, 0, 0, 0, 0, 128, 96);
		blit (screen, screen, 0, 0, 400, 240, 90, 90);
		blit (vid_bitmap, screen, 0, 0, 0, 0, 50, 50);
		blit (vid_bitmap2, screen, 0, 0, 50, 0, 50, 50);
		blit (screen, mem_bitmap, 0, 0, 0, 0, 50, 50);
		blit (mem_bitmap, screen, 0, 0, 100, 0, 50, 50);
		allegro_gl_unset_allegro_mode();

		if ((keypress & 0xdf) == 'S')
			blit (screen, save_bmp, 0, 0, 0, 0, 640, 480);

		glFlush();
		allegro_gl_flip();

		if ((keypress & 0xdf) == 'S')
			save_bitmap ("screen.tga", save_bmp, desktop_palette);

	}while(!key[KEY_ESC]);

	allegro_gl_set_allegro_mode();
	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_FRONT);
	
#if (((ALLEGRO_VERSION << 16) | (ALLEGRO_SUB_VERSION << 8) | ALLEGRO_WIP_VERSION) >= 0x040104)
	textout_ex(screen, font, "Press any key to exit...", 0, 470,
	        makecol(255, 255, 255), -1);
#else
	textout(screen, font, "Press any key to exit...", 0, 470,
	        makecol(255, 255, 255));
#endif

	/* check the dialog saved the background OK */
	readkey();

	destroy_bitmap(save_bmp);
	destroy_bitmap(sub_screen);
	destroy_bitmap(sub_screen2);
	destroy_bitmap(mem_bitmap);
	destroy_bitmap(vid_bitmap);
	destroy_bitmap(vid_bitmap2);
	return 0;
}

END_OF_MAIN();

