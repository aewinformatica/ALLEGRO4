#include <math.h>
#include <ctime>
#include <iostream>
#include <string>
#include <allegro.h>

using namespace std;

int screen_h = 320; // screen height
int screen_w = 640; // screen width

char *thetime;
char *tempstring1;
char *tempstring2;
char *tempstring3;
char *endp;

int temptime;
int hours;
int mins;
int secs;

int secsx;
int secsy;
int minsx;
int minsy;
int hourx;
int houry;

float pi = 3.142;
float angle_stepsize = 0.1047;
int length = 100;

volatile long speed_counter = 0; 
volatile long counter = 1;// counter starts at 0

BITMAP *buffer; //create the buffer for drawing to the screen

void countdown() //this is the counter for the program
{
     counter++; // increment counter
     if (counter == 60)
     {
                 counter = 1;
     }
}

void Currenttime()
{
                      time_t date; // Make a time_t object that'll hold the date
                      time(&date); //  Set the date variable to the current date  
                      thetime = ctime(&date);
                      
                      for(int x=0; x < 6; x++) // this is a loop that cuts down the string to  
                      {
                           temptime -= uremove(thetime, -1); // this cuts off the trailing letters
                           temptime -= uremove(thetime, 0); // this cuts off the leading letters
                      }
                      
                      for(int x=0; x < 5; x++) // this loop finishes off the task.
                      {
                           temptime -= uremove(thetime, 0); 
                      }
                      
}

void Hours()
{
                      tempstring1 = ustrdup(thetime);
                      for(int x=0; x < 6; x++) // this is a loop that cuts down the string to  
                      {
                           temptime -= uremove(tempstring1, -1); // this cuts off the trailing letters
                      }
                      
                      double hours1 = ustrtod(tempstring1, &endp);
                      
                      char *endp, *string = tempstring1;
                      hours = ustrtol(string, &endp, 10);
}

void Mins()
{
                      tempstring2 = ustrdup(thetime);
                      for(int x=0; x < 3; x++) // this is a loop that cuts down the string to  
                      {
                           temptime -= uremove(tempstring2, 0); // this cuts off the trailing letters
                      }
   
                      for(int x=0; x < 3; x++) // this is a loop that cuts down the string to  
                      {
                           temptime -= uremove(tempstring2, -1); // this cuts off the trailing letters
                      }
                      
                      double mins1 = ustrtod(tempstring2, &endp);
                      
                      char *endp, *string = tempstring2;
                      mins = ustrtol(string, &endp, 10);
                      
}

void Secs()
{
                      tempstring3 = ustrdup(thetime);
                      for(int x=0; x < 6; x++) // this is a loop that cuts down the string to  
                      {
                           temptime -= uremove(tempstring3, 0); // this cuts off the leading letters
                      }
                      
                      double secs1 = ustrtod(tempstring3, &endp);
                      
                      char *endp, *string = tempstring3;
                      secs = ustrtol(string, &endp, 10);
}

void Draw_secs()
{
     int x, y;
     
     float angle = (secs * angle_stepsize) - (pi/2);
     x = length * cos (angle);
     y = length * sin (angle);
     
     line(buffer, screen_w/2, screen_h/2, (secsx + (screen_w/2)), (secsy + (screen_h/2)), makecol(0, 0, 0));
     line(buffer, screen_w/2, screen_h/2, (x + (screen_w/2)), (y + (screen_h/2)), makecol(255, 255, 255));
     secsx = x;
     secsy = y;
}

void Draw_mins()
{
     int x, y;
           
     float angle = (mins * angle_stepsize) - (pi/2);
     x = length * cos (angle);
     y = length * sin (angle);
     
     line(buffer, screen_w/2, screen_h/2, (minsx + (screen_w/2)), (minsy + (screen_h/2)), makecol(0, 0, 0));
     line(buffer, screen_w/2, screen_h/2, (x + (screen_w/2)), (y + (screen_h/2)), makecol(255, 0, 0));
     minsx = x;
     minsy = y;
}

void Draw_hours()
{
     int x, y;
     
     if (hours > 12)
     {
               hours -= 12;
     }
     
     float angle = (hours * (angle_stepsize * 5)) - (pi/2);
     x = length * cos (angle);
     y = length * sin (angle);
     hourx = x;
     houry = y;
     
     line(buffer, screen_w/2, screen_h/2, (hourx + (screen_w/2)), (houry + (screen_h/2)), makecol(0, 0, 0));
     line(buffer, screen_w/2, screen_h/2, (x + (screen_w/2)), (y + (screen_h/2)), makecol(0, 0, 255));
}
   
int main()   
{
    allegro_init();
    install_timer(); //installs the timer
    LOCK_VARIABLE(counter); 
    LOCK_FUNCTION(countdown);
    install_int_ex(countdown, SECS_TO_TIMER(1)); // update every second (the number indicates the number of seconds)
    install_keyboard();
    //install_mouse();
    //install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_w, screen_h, 0, 0);
    set_display_switch_mode(SWITCH_BACKGROUND);// keeps running in the background
    buffer = create_bitmap(screen_w, screen_h); 
    //srand(time(NULL));

while(!key[KEY_ESC])  
    {
                      
                      Currenttime();
                      Hours();
                      Mins();
                      Secs();
                      Draw_secs();
                      Draw_mins();
                      Draw_hours();
                      
                      textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255), -1, "time: %s", thetime);
                      
                      textprintf_ex(buffer, font, 10, 40, makecol(255, 255, 255), -1, "hours: %d", hours);
                      
                      textprintf_ex(buffer, font, 10, 60, makecol(255, 255, 255), -1, "mins: %d", mins);
                      
                      textprintf_ex(buffer, font, 10, 80, makecol(255, 255, 255), -1, "secs: %d", secs);
                      
                      draw_sprite(screen, buffer, 0, 0); 
                      
                      textprintf_ex(buffer, font, 10, 10, makecol(0, 0, 0), -1, "time: %s", thetime);
                      
                      textprintf_ex(buffer, font, 10, 40, makecol(0, 0, 0), -1, "hours: %d", hours);
                      
                      textprintf_ex(buffer, font, 10, 60, makecol(0, 0, 0), -1, "mins: %d", mins);
                      
                      textprintf_ex(buffer, font, 10, 80, makecol(0, 0, 0), -1, "secs: %d", secs);
                        
    }

    return 0;
}

END_OF_MAIN();
