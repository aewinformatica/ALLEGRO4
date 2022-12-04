#include <allegro.h>
#include "timers.h"
#include "sprite.h"
#include "data.h"
#include "keymap.h"
#include "fighter.h"
#include "explosion.h"
#include "audio.h"
#include "power.h"
#include "avf.h"



static DATAFILE *data = NULL;
static BITMAP *fighter_buf = NULL;



void update_logic (FIGHTER *player1, FIGHTER *player2)
{
        update_fighter (player1);
        update_fighter (player2);
        play_sprite (player1->sprite);
        play_sprite (player2->sprite);
        update_explosion ();
}



void draw_fighter (BITMAP *bmp, FIGHTER *player)
{
        /* blit the segment of the image we need */
        stretch_blit (player->sprite->image, fighter_buf,
                      150 * player->sprite->current_frame, 0,
                      150, 150,
                      0, 0,
                      player->sprite->size.x, player->sprite->size.y);

        draw_power (fighter_buf, player);
        /* draw it on the buffer skipping transparent pixels */
        (player->dir ? draw_sprite_h_flip : draw_sprite)
        (bmp, fighter_buf, player->sprite->pos.x, player->sprite->pos.y);
}



void draw_ui (BITMAP *bmp, FIGHTER *player1, FIGHTER *player2)
{
        int y1 = 20;
        int y2 = 40;
        int hmargin = 20;
        int len = 250;
        float factor = (float) len / MAX_ENERGY;
        /* energy bars */
        rectfill (bmp, hmargin, y1, hmargin + len, y2, RED);
        rectfill (bmp, hmargin, y1, hmargin + player1->energy * factor, y2, YELLOW);
        rect (bmp, hmargin, y1, hmargin + len, y2, BLACK);
        rectfill (bmp, SCREEN_W - hmargin - len, y1, SCREEN_W - hmargin, y2, RED);
        rectfill (bmp, SCREEN_W - hmargin, y1, SCREEN_W - hmargin - player2->energy * factor, y2, YELLOW);
        rect (bmp, SCREEN_W - hmargin - len, y1, SCREEN_W - hmargin, y2, BLACK);
        /* timer */
        textprintf_centre (bmp, data [FONT].dat, SCREEN_W >> 1, 20, RED, "%d", seconds_timer);
        /* Fighter names */
        textprintf (bmp, data [FONT].dat, hmargin, y2 + 5, WHITE, player1->name);
        textprintf_right (bmp, data [FONT].dat, SCREEN_W - hmargin, y2 + 5, WHITE, player2->name);
        /* Dialogs */
        textprintf (bmp, data [FONT].dat, hmargin, SCREEN_H - 35, WHITE, player1->dialog);
        textprintf_right (bmp, data [FONT].dat, SCREEN_W - hmargin, SCREEN_H - 35, WHITE, player2->dialog);
}



void update_graphics (BITMAP *bmp, FIGHTER *player1, FIGHTER *player2)
{
        BITMAP *bg = (BITMAP *) data [BG].dat;
        stretch_blit (bg, bmp, 0, 0, bg->w, bg->h, 0, 0, SCREEN_W, SCREEN_H);
        draw_ui (bmp, player1, player2);
        draw_fighter (bmp, player2);
        draw_fighter (bmp, player1);
        draw_explosion (bmp);
}



void play_round (FIGHTER *player1, FIGHTER *player2)
{
        BITMAP *dbuf;
        int finish_time;
        if (!(dbuf = create_bitmap (SCREEN_W, SCREEN_H)))
                return;
        if (!(fighter_buf = create_bitmap (player1->sprite->size.x, player1->sprite->size.y)))
        {
                destroy_bitmap (dbuf);
                return;
        }
        install_interrupts ();
        init_audio ();
        init_power ();
        finish_time = 0;
        while (seconds_timer > finish_time)
        {
                while (logic_timer > 0)
                {
                        poll_keyboard ();
                        update_logic (player1, player2);
                        if ((!player1->energy || !player2->energy) && finish_time < 1)
                        {
                                finish_time = seconds_timer - 3;
                        }
                        update_audio ();
                        logic_timer--;
                }
                if (key [KEY_ESC])
                        break;
                fps_cnt++;
                update_graphics (dbuf, player1, player2);
                blit (dbuf, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
        end_power ();
        end_audio ();
        fade_out (2);
        remove_interrupts ();
        destroy_bitmap (dbuf);
        destroy_bitmap (fighter_buf);
}



#define TWO_PLAYERS  0
#define VS_ARINA     1
#define VS_JULIETTE  2
#define AUTO_FIGHT   3



void fight (int mode)
{
        FIGHTER *player1 = &arina;
        FIGHTER *player2 = &juliette;
        reset_fighter (player1);
        reset_fighter (player2);
        player1->init (player1);
        player2->init (player2);
        player1->enemy = player2;
        player2->enemy = player1;
        switch (mode)
        {
                case TWO_PLAYERS: player1->type = player2->type = HUMAN; break;
                case VS_ARINA: player1->type = CPU; player2->type = HUMAN; break;
                case VS_JULIETTE: player1->type = HUMAN; player2->type = CPU; break;
                case AUTO_FIGHT: player1->type = player2->type = CPU; break;
        }
        play_round (player1, player2);
        player1->destroy (player1);
        player2->destroy (player2);
}



void intro ()
{
        set_palette (black_palette);
        textprintf_centre (screen, data [FONT].dat, SCREEN_W >> 1, SCREEN_H >> 1, WHITE, "Unicornsoft presents");
        fade_in ((RGB *) data [MAIN_PALETTE].dat, 1);
        fade_out (1);
        clear_to_color (screen, BLACK);
}



#define NUM_OPTIONS   6



void draw_menu (BITMAP *bmp, int index)
{
        int i, x, y;
        BITMAP *logo = (BITMAP *) data [LOGO].dat;
        x = SCREEN_W >> 1;
        y = logo->h + 50;
        i = 0;
        draw_sprite (bmp, data [LOGO].dat, 50, 50);
        textprintf_centre (bmp, data [FONT].dat, x - 20, y - 110, BLACK, "FlGHTERS");
        textprintf_centre (bmp, data [FONT].dat, x, y + i * 30, index == i ? WHITE : GRAY, "[ 2 Players ]");
        i++;
        textprintf_centre (bmp, data [FONT].dat, x, y + i * 30, index == i ? WHITE : GRAY, "[ Player vs Arina ]");
        i++;
        textprintf_centre (bmp, data [FONT].dat, x, y + i * 30, index == i ? WHITE : GRAY, "[ Player vs Juliette ]");
        i++;
        textprintf_centre (bmp, data [FONT].dat, x, y + i * 30, index == i ? WHITE : GRAY, "[ Arina vs Juliette ]");
        i++;
        textprintf_centre (bmp, data [FONT].dat, x, y + i * 30, index == i ? WHITE : GRAY, "[ Sound: %d ]", sound_volume);
        i++;
        textprintf_centre (bmp, data [FONT].dat, x, y + i * 30, index == i ? WHITE : GRAY, "[ Exit ]");
        textprintf_centre (bmp, data [FONT].dat, SCREEN_W >> 1, SCREEN_H - 50, BLACK, "A Unicornsoft production");
}



int process_menu (int last_option)
{
        BITMAP *bmp;
        int index, pressed;
        if (!(bmp = create_bitmap (SCREEN_W, SCREEN_H)))
                exit (1);
        set_palette (data [MAIN_PALETTE].dat);
        index = last_option;
        pressed = 0;
        while (1)
        {
                poll_keyboard ();
                if (key [KEY_UP] && pressed != KEY_UP)
                {
                        index--;
                        if (index < 0)
                                index = NUM_OPTIONS - 1;
                        pressed = KEY_UP;
                }
                if (key [KEY_DOWN] && pressed != KEY_DOWN)
                {
                        index++;
                        if (index >= NUM_OPTIONS)
                                index = 0;
                        pressed = KEY_DOWN;
                }
                if (key [KEY_RIGHT] && pressed != KEY_RIGHT)
                {
                        if (index == NUM_OPTIONS - 2) // music volume
                        {
                                sound_volume++;
                                if (sound_volume >= 255)
                                        sound_volume = 255;
                        }
                        pressed = KEY_RIGHT;
                }
                if (key [KEY_LEFT] && pressed != KEY_LEFT)
                {
                        if (index == NUM_OPTIONS - 2) // music volume
                        {
                                sound_volume--;
                                if (sound_volume < 0)
                                        sound_volume = 0;
                        }
                        pressed = KEY_LEFT;
                }
                if (pressed == KEY_UP && !key [KEY_UP])
                        pressed = 0;
                if (pressed == KEY_DOWN && !key [KEY_DOWN])
                        pressed = 0;
                if (pressed == KEY_RIGHT)
                        pressed = 0;
                if (pressed == KEY_LEFT)
                        pressed = 0;
                if (key [KEY_ENTER])
                        break;
                clear_to_color (bmp, BGCOLOR);
                draw_menu (bmp, index);
                blit (bmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
        destroy_bitmap (bmp);
        return index;
}



void menu (void)
{
        int option = 0;
        do
        {
                option = process_menu (option);
                if (option == 0)
                        fight (TWO_PLAYERS);
                else if (option == 1)
                        fight (VS_ARINA);
                else if (option == 2)
                        fight (VS_JULIETTE);
                else if (option == 3)
                        fight (AUTO_FIGHT);
        }
        while (option != NUM_OPTIONS - 1);
}



void setup ()
{
        RGB *pal;
        allegro_init ();
        install_keyboard ();
        install_timer ();
        set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
        if (install_sound (DIGI_AUTODETECT, MIDI_NONE, NULL) != 0)
        {
                allegro_message ("Error installing sound driver: %s", allegro_error);
                exit (1);
        }
        use_audio = 1;
        if (digi_card == DIGI_NONE)
        {
                allegro_message ("Unable to find a sound driver: %s", allegro_error);
                use_audio = 0;
        }
        data = load_datafile ("data.dat");
        if (!data)
        {
                allegro_message ("Error loading datafile");
                exit (1);
        }
        pal = (RGB *) data [MAIN_PALETTE].dat;
        set_palette (pal);
        text_mode (-1);
}



void quit ()
{
        allegro_exit ();
}



int main (void)
{
        setup ();
        intro ();
        menu ();
        quit ();
        return 0;
}

END_OF_MAIN ()

