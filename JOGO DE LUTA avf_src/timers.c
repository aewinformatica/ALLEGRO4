#include <allegro.h>
#include "timers.h"



volatile int seconds_timer;
volatile int logic_timer;
volatile int fps, fps_cnt;



void inc_logic_timer (void)
{
        logic_timer++;
}

END_OF_FUNCTION (inc_logic_timer);



void inc_fps_cnt (void)
{
        fps = fps_cnt;
        fps_cnt = 0;
        seconds_timer--;
}

END_OF_FUNCTION (inc_fps_cnt);



void install_interrupts (void)
{
        LOCK_VARIABLE (logic_timer);
        LOCK_FUNCTION (inc_logic_timer);
        install_int_ex (inc_logic_timer, BPS_TO_TIMER (60));
        logic_timer = 0;

        LOCK_VARIABLE (fps_cnt);
        LOCK_VARIABLE (fps);
        LOCK_FUNCTION (inc_fps_cnt);
        install_int_ex (inc_fps_cnt, SECS_TO_TIMER (1));
        fps_cnt = 0;
        fps = 0;
        seconds_timer = 60;
}



void remove_interrupts (void)
{
        remove_int (inc_logic_timer);
        remove_int (inc_fps_cnt);
}

