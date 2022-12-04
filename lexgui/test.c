#include <allegro.h>
#include <stdio.h>
#include <ctype.h>
#include "lexgui.h"

#define WIDTH  800
#define HEIGHT 600

BITMAP *Buffer = NULL;
SAMPLE *sound;
char textbuff[80] = "WAGNER";
BITMAP *bg;
        
int buttonCallback(int id);
int buttonCallback2(int id);
int onThemeChange(int id);

int colors[3];

void firstTimeInit() {

	int mode = GFX_AUTODETECT_WINDOWED	; //GFX_AUTODETECT

    allegro_init();
    install_mouse();
    install_keyboard();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    set_color_depth(32);
    set_gfx_mode(mode, WIDTH, HEIGHT, 0, 0);
      

    Buffer = create_bitmap(WIDTH, HEIGHT);
    set_uformat(U_ASCII);

    bg = load_bitmap("bg.bmp", NULL);

    sound = load_sample("win.wav");
}

char* listbox_callback(int index, int *size) {
    static char buffer[20];
    if (index < 0) {
        *size = 10;
        return NULL;
    }
    sprintf(buffer, "Line %03i", index);
    return buffer;
}


 DIALOG the_dialog[] =
{
    //(dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)     (d1)                    (d2)  (dp)              (dp2) (dp3) */
   { lex_dialog_proc,   50,  20,  440,  480,   -1,  -1,    0,    0,          0,                      0,    "Dialog",    NULL, NULL  },
   { lex_button_proc,   110,  130,  150,   24,    0,  -1,   't',   D_EXIT,    -1,                     0,    "&Sound && Dialog",    buttonCallback, NULL  },
   
   { lex_radio_proc,    110,  160,   80 ,  20,    0,  -1,    0,    D_SELECTED, 1,                      0,    "Radio 1",        NULL, NULL  },
   { lex_radio_proc,    110,  180,   80 ,  20,    0,  -1,    0,    0,          1,                      0,    "Radio 2",        NULL, NULL  },
   { lex_radio_proc,    110,  200,   80 ,  20,    0,  -1,    0,    0,          1,                      0,    "Radio 3",        NULL, NULL  },
   
   { lex_check_proc,    270,  160,   80 ,  20,    0,  -1,    0,    D_SELECTED, 0,                      0,    "Check 1",        NULL, NULL  },
   { lex_check_proc,    270,  180,   80 ,  20,    0,  -1,    0,    0,          0,                      0,    "Check 2",        NULL, NULL  },
   { lex_check_proc,    270,  200,   80 ,  20,    0,  -1,    0,    0,          0,                      0,    "Check 3",        NULL, NULL  },
   
   
   
   { lex_edit_proc ,    360,  130,  120 ,  24,    0,  -1,    0,    0,          80,                     0,   textbuff ,        NULL, NULL  },
   { lex_label_proc ,   300,  130,  120 ,  24,    0,  -1,    0,    0,          80,                     0,   textbuff ,        NULL, NULL  },

   { lex_list_proc ,    360,  160,  120 ,  90,    0  ,-1,    0,    0,          0,                      0,   listbox_callback,        NULL, NULL  },

   { lex_slider_proc,   110,  230,  200,   20,   0 , -1,    0,    0,       100,                    0,    NULL,             NULL, NULL  },
      
   { lex_button_proc,   110,  264,   60,   24,    0,  -1,    'y',  D_EXIT,    0,                     0,    "&Aqua",     onThemeChange, NULL  },   
   { lex_button_proc,   180,  264,   60,   24,    0,  -1,    'b',  D_EXIT,    1,                     0,    "&Bill",     onThemeChange, NULL  },   
   { lex_button_proc,   250,  264,   60,   24,    0,  -1,    'c',  D_EXIT,    2,                     0,    "&Yellow",     onThemeChange, NULL  },   
   
   { lex_button_proc,   420,  460,   50,   24,    0,  -1,    'x',  D_EXIT,    -1,                     0,    "E&xit",     NULL, NULL  },
      
   { NULL,              0,    0,    0,    0,    0  ,   0,    0,    0,          0,                     0,    NULL,         NULL, NULL  },
};

DIALOG the_dialog2[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)     (d1)                    (d2)  (dp)              (dp2) (dp3) */
   { lex_dialog_proc,   100,  100,  240,  100,   -1,  -1,    0,    0,          0,                      0,    "Dialog",    NULL, NULL  }, 
   { lex_button_proc,   110,  130,  120,   24,    0,  -1,   't',   D_EXIT,    -1,                      0,    "&Play Sound",    buttonCallback2, NULL  },  
   { lex_button_proc,   280,  160,   50,   24,    0,  -1,    'x',  D_EXIT,    -1,                      0,    "&Close",     NULL, NULL  },
   
   { NULL,              0,    0,    0,    0,    0  ,   0,    0,    0,          0,                      0,    NULL,         NULL, NULL  },
};





void updateScreen(BITMAP *buffer) {
    scare_mouse();
    blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
    unscare_mouse();
}

int buttonCallback(int id) {
    play_sample(sound, 255, 128, 1000, 0);     
    lex_do_dialog(the_dialog2, -1, Buffer, updateScreen);    
    return D_O_K;
}

int buttonCallback2(int id) {
    play_sample(sound, 255, 128, 1000, 0);
    return D_O_K;
}


int onThemeChange(int id) {
    static char* themeNames[] = {
        "./aqua.skin",
        "./bill32.skin",
        "./yellow.skin",
    };
    static int cur = 2;

    if (id != cur) {
        cur = id;
        lex_load_skin(themeNames[id]);
        lex_init_dialog(the_dialog, TRUE);
    }
    
    return D_O_K;
}


int main(int argc, char** argv) {
    firstTimeInit();    

    lex_load_skin("./yellow.skin");  
    
    stretch_blit(bg, Buffer, 0, 0, bg->w, bg->h, 0, 0, Buffer->w, Buffer->h);    
    lex_do_dialog(the_dialog, -1, Buffer, updateScreen);    

    lex_gui_shutdown();
    return 0;
}
END_OF_MAIN();


