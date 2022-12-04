#ifndef TIMERS_H
#define TIMERS_H



extern volatile int seconds_timer;
extern volatile int logic_timer;
extern volatile int fps, fps_cnt;



void install_interrupts (void);
void remove_interrupts (void);



#endif

