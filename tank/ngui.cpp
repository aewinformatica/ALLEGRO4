/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      The core GUI routines.
 *
 *      By Shawn Hargreaves.
 *
 *      Peter Pavlovic modified the drawing and positioning of menus.
 *
 *      Menu auto-opening added by Angelo Mottola.
 *
 *      Eric Botcazou added the support for non-blocking menus.
 *
 *      See readme.txt for copyright information.
 */


#include <limits.h>

#include <allegro.h>
#include "allegro/internal/aintern.h"

#include "scorch.h"



/* if set, the input focus follows the mouse pointer */
//int gui_mouse_focus = TRUE;


/* font alignment value */
//int gui_font_baseline = 0;


/* pointer to the currently active dialog and menu objects */
static DIALOG_PLAYER *active_dialog_player = NULL;
static MENU_PLAYER *active_menu_player = NULL;
static int active_menu_player_zombie = FALSE;
//DIALOG *active_dialog = NULL;
//MENU *active_menu = NULL;


/* list of currently active (initialized) dialog players */
struct al_active_dialog_player {
   DIALOG_PLAYER *player;
   struct al_active_dialog_player *next;
};

static struct al_active_dialog_player *first_active_dialog_player = 0;
static struct al_active_dialog_player *current_active_dialog_player = 0;



/* hook function for reading the mouse x position */
static int default_mouse_x(void)
{
   if (mouse_needs_poll())
      poll_mouse();

   return mouse_x;
}

END_OF_STATIC_FUNCTION(default_mouse_x);



/* hook function for reading the mouse y position */
static int default_mouse_y(void)
{
   if (mouse_needs_poll())
      poll_mouse();

   return mouse_y;
}

END_OF_STATIC_FUNCTION(default_mouse_y);



/* hook function for reading the mouse z position */
static int default_mouse_z(void)
{
   if (mouse_needs_poll())
      poll_mouse();

   return mouse_z;
}

END_OF_STATIC_FUNCTION(default_mouse_z);



/* hook function for reading the mouse button state */
static int default_mouse_b(void)
{
   if (mouse_needs_poll())
      poll_mouse();

   return mouse_b;
}

END_OF_STATIC_FUNCTION(default_mouse_b);



/* hook functions for reading the mouse state */
/*int (*gui_mouse_x)(void) = default_mouse_x;
int (*gui_mouse_y)(void) = default_mouse_y;
int (*gui_mouse_z)(void) = default_mouse_z;
int (*gui_mouse_b)(void) = default_mouse_b;*/


/* timer to handle menu auto-opening */
static int gui_timer;

static int gui_menu_opening_delay;


/* Checking for double clicks is complicated. The user could release the
 * mouse button at almost any point, and I might miss it if I am doing some
 * other processing at the same time (eg. sending the single-click message).
 * To get around this I install a timer routine to do the checking for me,
 * so it will notice double clicks whenever they happen.
 */

static volatile int dclick_status, dclick_time;

static int gui_install_count = 0;
static int gui_install_time = 0;

#define DCLICK_START      0
#define DCLICK_RELEASE    1
#define DCLICK_AGAIN      2
#define DCLICK_NOT        3



/* dclick_check:
 *  Double click checking user timer routine.
 */
static void dclick_check(void)
{
   gui_timer++;
   
   if (dclick_status==DCLICK_START) {              /* first click... */
      if (!gui_mouse_b()) {
	 dclick_status = DCLICK_RELEASE;           /* aah! released first */
	 dclick_time = 0;
	 return;
      }
   }
   else if (dclick_status==DCLICK_RELEASE) {       /* wait for second click */
      if (gui_mouse_b()) {
	 dclick_status = DCLICK_AGAIN;             /* yes! the second click */
	 dclick_time = 0;
	 return;
      }
   }
   else
      return;

   /* timeout? */
   if (dclick_time++ > 10)
      dclick_status = DCLICK_NOT;
}

END_OF_STATIC_FUNCTION(dclick_check);



/* gui_switch_callback:
 *  Sets the dirty flags whenever a display switch occurs.
 */
static void gui_switch_callback(void)
{
   if (active_dialog_player)
      active_dialog_player->res |= D_REDRAW_ALL;
}



/* position_dialog:
 *  Moves all the objects in a dialog to the specified position.
 */
/*void position_dialog(DIALOG *dialog, int x, int y)
{
   int min_x = INT_MAX;
   int min_y = INT_MAX;
   int xc, yc;
   int c;
   ASSERT(dialog);

   // locate the upper-left corner 
   for (c=0; dialog[c].proc; c++) {
      if (dialog[c].x < min_x) 
	 min_x = dialog[c].x;

      if (dialog[c].y < min_y) 
	 min_y = dialog[c].y;
   }

   // move the dialog 
   xc = min_x - x;
   yc = min_y - y;

   for (c=0; dialog[c].proc; c++) {
      dialog[c].x -= xc;
      dialog[c].y -= yc;
   }
}*/



/* centre_dialog:
 *  Moves all the objects in a dialog so that the dialog is centered in
 *  the screen.
 */
/*void centre_dialog(DIALOG *dialog)
{
   int min_x = INT_MAX;
   int min_y = INT_MAX;
   int max_x = INT_MIN;
   int max_y = INT_MIN;
   int xc, yc;
   int c;
   ASSERT(dialog);

   // find the extents of the dialog (done in many loops due to a bug
   // in MSVC that prevents the more sensible version from working)
   // 
   for (c=0; dialog[c].proc; c++) {
      if (dialog[c].x < min_x)
	 min_x = dialog[c].x;
   }

   for (c=0; dialog[c].proc; c++) {
      if (dialog[c].y < min_y)
	 min_y = dialog[c].y;
   }

   for (c=0; dialog[c].proc; c++) {
      if (dialog[c].x + dialog[c].w > max_x)
	 max_x = dialog[c].x + dialog[c].w;
   }

   for (c=0; dialog[c].proc; c++) {
      if (dialog[c].y + dialog[c].h > max_y)
	 max_y = dialog[c].y + dialog[c].h;
   }

   // how much to move by? 
   xc = (SCREENWIDTH - (max_x - min_x)) / 2 - min_x;
   yc = (SCREENHEIGHT - (max_y - min_y)) / 2 - min_y;

   // move it 
   for (c=0; dialog[c].proc; c++) {
      dialog[c].x += xc;
      dialog[c].y += yc;
   }
}*/



/* set_dialog_color:
 *  Sets the foreground and background colors of all the objects in a dialog.
 */
/*void set_dialog_color(DIALOG *dialog, int fg, int bg)
{
   int c;
   ASSERT(dialog);

   for (c=0; dialog[c].proc; c++) {
      dialog[c].fg = fg;
      dialog[c].bg = bg;
   }
}*/



/* find_dialog_focus:
 *  Searches the dialog for the object which has the input focus, returning
 *  its index, or -1 if the focus is not set. Useful after do_dialog() exits
 *  if you need to know which object was selected.
 */
/*int find_dialog_focus(DIALOG *dialog)
{
   int c;
   ASSERT(dialog);

   for (c=0; dialog[c].proc; c++)
      if (dialog[c].flags & D_GOTFOCUS)
	 return c;

   return -1;
}*/



/* object_message:
 *  Sends a message to a widget, automatically scaring and unscaring
 *  the mouse if the message is MSG_DRAW.
 */
/*int object_message(DIALOG *dialog, int msg, int c)
{
#ifdef ALLEGRO_WINDOWS
   // exported address of d_clear_proc 
   extern int (*_d_clear_proc)(int, DIALOG *, int);
#endif

   int ret;
   ASSERT(dialog);

   if (msg == MSG_DRAW) {
      if (dialog->flags & D_HIDDEN)
	 return D_O_K;

#ifdef ALLEGRO_WINDOWS
      if (dialog->proc == _d_clear_proc)
#else
      if (dialog->proc == d_clear_proc)
#endif
	 scare_mouse();
      else
	 scare_mouse_area(dialog->x, dialog->y, dialog->w, dialog->h);

      acquire_screen();
   }

   ret = dialog->proc(msg, dialog, c);

   if (msg == MSG_DRAW) {
      release_screen();
      unscare_mouse();
   }

   if (ret & D_REDRAWME) {
      dialog->flags |= D_DIRTY;
      ret &= ~D_REDRAWME;
   }

   return ret;
}*/



/* dialog_message:
 *  Sends a message to all the objects in a dialog. If any of the objects
 *  return values other than D_O_K, returns the value and sets obj to the 
 *  object which produced it.
 */
/*int dialog_message(DIALOG *dialog, int msg, int c, int *obj)
{
   int count, res, r, force, try;
   ASSERT(dialog);

   if (msg == MSG_DRAW)
      acquire_screen();

   force = ((msg == MSG_START) || (msg == MSG_END));

   res = D_O_K;

   // If a menu spawned by a d_menu_proc object is active, the dialog engine
   // has effectively been shutdown for the sake of safety. This means that
   // we can't send the message to the other objects in the dialog. So try
   // first to send the message to the d_menu_proc object and, if the menu
   // is then not active anymore, send it to the other objects as well.
   //
   if (active_menu_player)
      try = 2;
   else
      try = 1;

   for (; try > 0; try--) {
      for (count=0; dialog[count].proc; count++) {
         if ((try == 2) && (&dialog[count] != active_menu_player->dialog))
	    continue;

	 if ((force) || (!(dialog[count].flags & D_HIDDEN))) {
	    r = object_message(&dialog[count], msg, c);

	    if (r != D_O_K) {
	       res |= r;
	       if (obj)
		  *obj = count;
	    }

	    if ((msg == MSG_IDLE) && (dialog[count].flags & (D_DIRTY | D_HIDDEN)) == D_DIRTY) {
	       dialog[count].flags &= ~D_DIRTY;
	       object_message(dialog+count, MSG_DRAW, 0);
	    }
	 }
      }

      if (active_menu_player)
	 break;
   }

   if (msg == MSG_DRAW)
      release_screen();

   return res;
}*/



/* broadcast_dialog_message:
 *  Broadcasts a message to all the objects in the active dialog. If any of
 *  the dialog procedures return values other than D_O_K, it returns that
 *  value.
 */
/*int broadcast_dialog_message(int msg, int c)
{
   int nowhere;

   if (active_dialog)
      return dialog_message(active_dialog, msg, c, &nowhere);
   else
      return D_O_K;
}*/



/* find_mouse_object:
 *  Finds which object the mouse is on top of.
 */
static int find_mouse_object(DIALOG *d)
{
   int mouse_object = -1;
   int c;
   ASSERT(d);

   for (c=0; d[c].proc; c++)
      if ((gui_mouse_x() >= d[c].x) && (gui_mouse_y() >= d[c].y) &&
	  (gui_mouse_x() < d[c].x + d[c].w) && (gui_mouse_y() < d[c].y + d[c].h) &&
	  (!(d[c].flags & (D_HIDDEN | D_DISABLED))))
	 mouse_object = c;

   return mouse_object;
}



/* offer_focus:
 *  Offers the input focus to a particular object.
 */
/*int offer_focus(DIALOG *dialog, int obj, int *focus_obj, int force)
{
   int res = D_O_K;
   ASSERT(dialog);
   ASSERT(focus_obj);

   if ((obj == *focus_obj) || 
       ((obj >= 0) && (dialog[obj].flags & (D_HIDDEN | D_DISABLED))))
      return D_O_K;

   // check if object wants the focus 
   if (obj >= 0) {
      res = object_message(dialog+obj, MSG_WANTFOCUS, 0);
      if (res & D_WANTFOCUS)
	 res ^= D_WANTFOCUS;
      else
	 obj = -1;
   }

   if ((obj >= 0) || (force)) {
      // take focus away from old object 
      if (*focus_obj >= 0) {
	 res |= object_message(dialog+*focus_obj, MSG_LOSTFOCUS, 0);
	 if (res & D_WANTFOCUS) {
	    if (obj < 0)
	       return D_O_K;
	    else
	       res &= ~D_WANTFOCUS;
	 }
	 dialog[*focus_obj].flags &= ~D_GOTFOCUS;
	 res |= object_message(dialog+*focus_obj, MSG_DRAW, 0);
      }

      *focus_obj = obj;

      // give focus to new object 
      if (obj >= 0) {
	 dialog[obj].flags |= D_GOTFOCUS;
	 res |= object_message(dialog+obj, MSG_GOTFOCUS, 0);
	 res |= object_message(dialog+obj, MSG_DRAW, 0);
      }
   }

   return res;
}*/



#define MAX_OBJECTS     512

typedef struct OBJ_LIST
{
   int index;
   int diff;
} OBJ_LIST;



/* obj_list_cmp:
 *  Callback function for qsort().
 */
static int obj_list_cmp(AL_CONST void *e1, AL_CONST void *e2)
{
   return (((OBJ_LIST *)e1)->diff - ((OBJ_LIST *)e2)->diff);
}



/* cmp_tab:
 *  Comparison function for tab key movement.
 */
static int cmp_tab(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2)
{
   int ret = (int)((AL_CONST unsigned long)d2 - (AL_CONST unsigned long)d1);

   if (ret < 0)
      ret += 0x10000;

   return ret;
}



/* cmp_shift_tab:
 *  Comparison function for shift+tab key movement.
 */
static int cmp_shift_tab(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2)
{
   int ret = (int)((AL_CONST unsigned long)d1 - (AL_CONST unsigned long)d2);

   if (ret < 0)
      ret += 0x10000;

   return ret;
}



/* cmp_right:
 *  Comparison function for right arrow key movement.
 */
static int cmp_right(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2)
{
   int ret = (d2->x - d1->x) + ABS(d1->y - d2->y) * 8;

   if (d1->x >= d2->x)
      ret += 0x10000;

   return ret;
}



/* cmp_left:
 *  Comparison function for left arrow key movement.
 */
static int cmp_left(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2)
{
   int ret = (d1->x - d2->x) + ABS(d1->y - d2->y) * 8;

   if (d1->x <= d2->x)
      ret += 0x10000;

   return ret;
}



/* cmp_down:
 *  Comparison function for down arrow key movement.
 */
static int cmp_down(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2)
{
   int ret = (d2->y - d1->y) + ABS(d1->x - d2->x) * 8;

   if (d1->y >= d2->y)
      ret += 0x10000;

   return ret;
}



/* cmp_up:
 *  Comparison function for up arrow key movement.
 */
static int cmp_up(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2)
{
   int ret = (d1->y - d2->y) + ABS(d1->x - d2->x) * 8;

   if (d1->y <= d2->y)
      ret += 0x10000;

   return ret;
}



/* move_focus:
 *  Handles arrow key and tab movement through a dialog, deciding which
 *  object should be given the input focus.
 */
static int move_focus(DIALOG *d, int ascii, int scan, int *focus_obj)
{
   int (*cmp)(AL_CONST DIALOG *d1, AL_CONST DIALOG *d2);
   OBJ_LIST obj[MAX_OBJECTS];
   int obj_count = 0;
   int fobj, c;
   int res = D_O_K;

   /* choose a comparison function */ 
   switch (scan) {
      case KEY_TAB:   cmp = (ascii == '\t') ? cmp_tab : cmp_shift_tab; break;
      case KEY_RIGHT: cmp = cmp_right; break;
      case KEY_LEFT:  cmp = cmp_left;  break;
      case KEY_DOWN:  cmp = cmp_down;  break;
      case KEY_UP:    cmp = cmp_up;    break;
      default:        return D_O_K;
   }

   /* fill temporary table */
   for (c=0; d[c].proc; c++) {
      if ((*focus_obj < 0) || (c != *focus_obj)) {
	 obj[obj_count].index = c;
	 if (*focus_obj >= 0)
	    obj[obj_count].diff = cmp(d+*focus_obj, d+c);
	 else
	    obj[obj_count].diff = c;
	 obj_count++;
	 if (obj_count >= MAX_OBJECTS)
	    break;
      }
   }

   /* sort table */
   qsort(obj, obj_count, sizeof(OBJ_LIST), obj_list_cmp);

   /* find an object to give the focus to */
   fobj = *focus_obj;
   for (c=0; c<obj_count; c++) {
      res |= offer_focus(d, obj[c].index, focus_obj, FALSE);
      if (fobj != *focus_obj)
	 break;
   } 

   return res;
}



#define MESSAGE(i, msg, c) {                       \
   r = object_message(player->dialog+i, msg, c);   \
   if (r != D_O_K) {                               \
      player->res |= r;                            \
      player->obj = i;                             \
   }                                               \
}



/* do_dialog:
 *  The basic dialog manager. The list of dialog objects should be
 *  terminated by one with a null dialog procedure. Returns the index of 
 *  the object which caused it to exit.
 */
/*int do_dialog(DIALOG *dialog, int focus_obj)
{
   BITMAP *mouse_screen = _mouse_screen;
   int screen_count = _gfx_mode_set_count;
   void *player;
   ASSERT(dialog);

   if (!is_same_bitmap(_mouse_screen, screen))
      show_mouse(screen);

   player = init_dialog(dialog, focus_obj);

   while (update_dialog(player))
      ;

   if (_gfx_mode_set_count == screen_count)
      show_mouse(mouse_screen);

   return shutdown_dialog(player);
}*/



/* popup_dialog:
 *  Like do_dialog(), but it stores the data on the screen before drawing
 *  the dialog and restores it when the dialog is closed. The screen area
 *  to be stored is calculated from the dimensions of the first object in
 *  the dialog, so all the other objects should lie within this one.
 */
/*int popup_dialog(DIALOG *dialog, int focus_obj)
{
   BITMAP *bmp;
   int ret;
   ASSERT(dialog);

   bmp = create_bitmap(dialog->w, dialog->h); 

   if (bmp) {
      scare_mouse_area(dialog->x, dialog->y, dialog->w, dialog->h);
      blit(screen, bmp, dialog->x, dialog->y, 0, 0, dialog->w, dialog->h);
      unscare_mouse();
   }
   else
      *allegro_errno = ENOMEM;

   ret = do_dialog(dialog, focus_obj);

   if (bmp) {
      scare_mouse_area(dialog->x, dialog->y, dialog->w, dialog->h);
      blit(bmp, screen, 0, 0, dialog->x, dialog->y, dialog->w, dialog->h);
      unscare_mouse();
      destroy_bitmap(bmp);
   }

   return ret;
}*/



/* init_dialog:
 *  Sets up a dialog, returning a player object that can be used with
 *  the update_dialog() and shutdown_dialog() functions.
 */
/*DIALOG_PLAYER *init_dialog(DIALOG *dialog, int focus_obj)
{
   DIALOG_PLAYER *player;
   struct al_active_dialog_player *n;
   char tmp1[64], tmp2[64];
   int c;
   ASSERT(dialog);

   player = malloc(sizeof(DIALOG_PLAYER));
   if (!player) {
      *allegro_errno = ENOMEM;
      return NULL;
   }

   // append player to the list 
   n = malloc(sizeof(struct al_active_dialog_player));
   if (!n) {
      *allegro_errno = ENOMEM;
      free (player);
      return NULL;
   }

   n->next = NULL;
   n->player = player;

   if (!current_active_dialog_player) {
      current_active_dialog_player = first_active_dialog_player = n;
   }
   else {
      current_active_dialog_player->next = n;
      current_active_dialog_player = n;
   }

   player->res = D_REDRAW;
   player->joy_on = TRUE;
   player->click_wait = FALSE;
   player->dialog = dialog;
   player->obj = -1;
   player->mouse_obj = -1;
   player->mouse_oz = gui_mouse_z();
   player->mouse_b = gui_mouse_b();

   // set up the global  dialog pointer 
   active_dialog_player = player;
   active_dialog = dialog;

   // set up dclick checking code 
   if (gui_install_count <= 0) {
      LOCK_VARIABLE(gui_timer);
      LOCK_VARIABLE(dclick_status);
      LOCK_VARIABLE(dclick_time);
      LOCK_VARIABLE(gui_mouse_x);
      LOCK_VARIABLE(gui_mouse_y);
      LOCK_VARIABLE(gui_mouse_z);
      LOCK_VARIABLE(gui_mouse_b);
      LOCK_FUNCTION(default_mouse_x);
      LOCK_FUNCTION(default_mouse_y);
      LOCK_FUNCTION(default_mouse_z);
      LOCK_FUNCTION(default_mouse_b);
      LOCK_FUNCTION(dclick_check);

      install_int(dclick_check, 20);

      switch (get_display_switch_mode()) {
         case SWITCH_AMNESIA:
         case SWITCH_BACKAMNESIA:
            set_display_switch_callback(SWITCH_IN, gui_switch_callback);
      }

      // gets menu auto-opening delay (in milliseconds) from config file 
      gui_menu_opening_delay = get_config_int(uconvert_ascii("system", tmp1), uconvert_ascii("menu_opening_delay", tmp2), 300);
      if (gui_menu_opening_delay >= 0) {
         // adjust for actual timer speed 
         gui_menu_opening_delay /= 20;
      }
      else {
         // no auto opening 
         gui_menu_opening_delay = -1;
      }
      
      gui_install_count = 1;
      gui_install_time = _allegro_count;
   }
   else
      gui_install_count++;

   // initialise the dialog 
   set_clip(screen, 0, 0, SCREENWIDTH-1, SCREENHEIGHT-1);
   player->res |= dialog_message(dialog, MSG_START, 0, &player->obj);

   player->mouse_obj = find_mouse_object(dialog);
   if (player->mouse_obj >= 0)
      dialog[player->mouse_obj].flags |= D_GOTMOUSE;

   for (c=0; dialog[c].proc; c++)
      dialog[c].flags &= ~D_GOTFOCUS;

   if (focus_obj >= 0)
      c = focus_obj;
   else
      c = player->mouse_obj;

   if ((c >= 0) && ((object_message(dialog+c, MSG_WANTFOCUS, 0)) & D_WANTFOCUS)) {
      dialog[c].flags |= D_GOTFOCUS;
      player->focus_obj = c;
   }
   else
      player->focus_obj = -1;

   return player;
}



/* check_for_redraw:
 *  Checks whether any parts of the current dialog need to be redraw.
 */
static void check_for_redraw(DIALOG_PLAYER *player)
{
   struct al_active_dialog_player *iter;
   int c, r;
   ASSERT(player);

   /* need to redraw all active dialogs? */
   if (player->res & D_REDRAW_ALL) {
      for (iter = first_active_dialog_player; iter != current_active_dialog_player; iter = iter->next)
	 dialog_message(iter->player->dialog, MSG_DRAW, 0, NULL);

      player->res &= ~D_REDRAW_ALL;
      player->res |= D_REDRAW;
   }

   /* need to draw it? */
   if (player->res & D_REDRAW) {
      player->res ^= D_REDRAW;
      player->res |= dialog_message(player->dialog, MSG_DRAW, 0, &player->obj);
   }

   /* check if any widget has to be redrawn */
   for (c=0; player->dialog[c].proc; c++) {
      if ((player->dialog[c].flags & (D_DIRTY | D_HIDDEN)) == D_DIRTY) {
	 player->dialog[c].flags &= ~D_DIRTY;
	 MESSAGE(c, MSG_DRAW, 0);
      }
   }
}



/* update_dialog:
 *  Updates the status of a dialog object returned by init_dialog(),
 *  returning TRUE if it is still active or FALSE if it has finished.
 */

//static int	nojoytime=0; // not required -- defined in Keycfg.cpp   // ???

int new_update_dialog(DIALOG_PLAYER *player)
{
   int c, cascii, cscan, ccombo, r, ret, nowhere, z;
   int new_mouse_b;
   ASSERT(player);
   
   /* redirect to update_menu() whenever a menu is activated */
   if (active_menu_player) {
      if (!active_menu_player_zombie) {
	 if (update_menu(active_menu_player))
	    return TRUE;
      }

      /* make sure all buttons are released before folding the menu */
      if (gui_mouse_b()) {
	 active_menu_player_zombie = TRUE;
	 return TRUE;
      }
      else {
	 active_menu_player_zombie = FALSE;

	 for (c=0; player->dialog[c].proc; c++)
	    if (&player->dialog[c] == active_menu_player->dialog)
	       break;

	 MESSAGE(c, MSG_LOSTMOUSE, 0);
	 goto getout;
      }
   }
       
   if (player->res & D_CLOSE)
      return FALSE;

   /* deal with mouse buttons presses and releases */
   new_mouse_b = gui_mouse_b();
   if (new_mouse_b != player->mouse_b) {
      player->res |= offer_focus(player->dialog, player->mouse_obj, &player->focus_obj, FALSE);

      if (player->mouse_obj >= 0) {
	 /* send press and release messages */
         if ((new_mouse_b & 1) && !(player->mouse_b & 1))
	    MESSAGE(player->mouse_obj, MSG_LPRESS, new_mouse_b);
         if (!(new_mouse_b & 1) && (player->mouse_b & 1))
	    MESSAGE(player->mouse_obj, MSG_LRELEASE, new_mouse_b);

         if ((new_mouse_b & 4) && !(player->mouse_b & 4))
	    MESSAGE(player->mouse_obj, MSG_MPRESS, new_mouse_b);
         if (!(new_mouse_b & 4) && (player->mouse_b & 4))
	    MESSAGE(player->mouse_obj, MSG_MRELEASE, new_mouse_b);

         if ((new_mouse_b & 2) && !(player->mouse_b & 2))
	    MESSAGE(player->mouse_obj, MSG_RPRESS, new_mouse_b);
         if (!(new_mouse_b & 2) && (player->mouse_b & 2))
	    MESSAGE(player->mouse_obj, MSG_RRELEASE, new_mouse_b);

         player->mouse_b = new_mouse_b;
      }
      else
	 dialog_message(player->dialog, MSG_IDLE, 0, &nowhere);
   }

   /* need to reinstall the dclick and switch handlers? */
   if (gui_install_time != _allegro_count) {
      install_int(dclick_check, 20);

      switch (get_display_switch_mode()) {
         case SWITCH_AMNESIA:
         case SWITCH_BACKAMNESIA:
            set_display_switch_callback(SWITCH_IN, gui_switch_callback);
      }

      gui_install_time = _allegro_count;
   }

   /* are we dealing with a mouse click? */
   if (player->click_wait) {
      if ((ABS(player->mouse_ox-gui_mouse_x()) > 8) || 
	  (ABS(player->mouse_oy-gui_mouse_y()) > 8))
	 dclick_status = DCLICK_NOT;

      /* waiting... */
      if ((dclick_status != DCLICK_AGAIN) && (dclick_status != DCLICK_NOT)) {
	 dialog_message(player->dialog, MSG_IDLE, 0, &nowhere);
	 check_for_redraw(player);
	 return TRUE;
      }

      player->click_wait = FALSE;

      /* double click! */
      if ((dclick_status==DCLICK_AGAIN) &&
	  (gui_mouse_x() >= player->dialog[player->mouse_obj].x) && 
	  (gui_mouse_y() >= player->dialog[player->mouse_obj].y) &&
	  (gui_mouse_x() <= player->dialog[player->mouse_obj].x + player->dialog[player->mouse_obj].w) &&
	  (gui_mouse_y() <= player->dialog[player->mouse_obj].y + player->dialog[player->mouse_obj].h)) {
	 MESSAGE(player->mouse_obj, MSG_DCLICK, 0);
      }

      goto getout;
   }

   player->res &= ~D_USED_CHAR;

   /* need to give the input focus to someone? */
   if (player->res & D_WANTFOCUS) {
      player->res ^= D_WANTFOCUS;
      player->res |= offer_focus(player->dialog, player->obj, &player->focus_obj, FALSE);
   }

   /* has mouse object changed? */
   c = find_mouse_object(player->dialog);
   if (c != player->mouse_obj) {
      if (player->mouse_obj >= 0) {
	 player->dialog[player->mouse_obj].flags &= ~D_GOTMOUSE;
	 MESSAGE(player->mouse_obj, MSG_LOSTMOUSE, 0);
      }
      if (c >= 0) {
	 player->dialog[c].flags |= D_GOTMOUSE;
	 MESSAGE(c, MSG_GOTMOUSE, 0);
      }
      player->mouse_obj = c; 

      /* move the input focus as well? */
      if ((gui_mouse_focus) && (player->mouse_obj != player->focus_obj))
	 player->res |= offer_focus(player->dialog, player->mouse_obj, &player->focus_obj, TRUE);
   }

   /* deal with mouse button clicks */
   if (new_mouse_b) {
      player->res |= offer_focus(player->dialog, player->mouse_obj, &player->focus_obj, FALSE);

      if (player->mouse_obj >= 0) {
	 dclick_time = 0;
	 dclick_status = DCLICK_START;
	 player->mouse_ox = gui_mouse_x();
	 player->mouse_oy = gui_mouse_y();

	 /* send click message */
	 MESSAGE(player->mouse_obj, MSG_CLICK, new_mouse_b);

	 if (player->res == D_O_K)
	    player->click_wait = TRUE;
      }
      else
	 player->res |= dialog_message(player->dialog, MSG_IDLE, 0, &nowhere);

      /* goto getout; */  /* to avoid an updating delay */
   }

   /* deal with mouse wheel clicks */
   z = gui_mouse_z();

   if (z != player->mouse_oz) {
      player->res |= offer_focus(player->dialog, player->mouse_obj, &player->focus_obj, FALSE);

      if (player->mouse_obj >= 0) {
	 MESSAGE(player->mouse_obj, MSG_WHEEL, z-player->mouse_oz);
      }
      else
	 player->res |= dialog_message(player->dialog, MSG_IDLE, 0, &nowhere);

      player->mouse_oz = z;

      /* goto getout; */  /* to avoid an updating delay */
   }

   cascii = cscan = 0;

   /*if (nojoytime <= 0)
   {

   // fake joystick input by converting it to key presses 
   if (player->joy_on)
   {
      rest(20);
	   //nojoytime = 5;
   }

   //poll_joystick();

   if (player->joy_on) 
   {
      if ((!joy[0].stick[0].axis[0].d1) && (!joy[0].stick[0].axis[0].d2) && 
	  (!joy[0].stick[0].axis[1].d1) && (!joy[0].stick[0].axis[1].d2) &&
	  (!joy[0].button[0].b) && (!joy[0].button[1].b)) 
	  {
		player->joy_on = FALSE;
		
		//nojoytime = 5;
		rest(20);
      }

      cascii = cscan = 0;
   }
   else 
   {
      //if (joy[0].stick[0].axis[0].d1) 
	  
	  // Going left
	  //if (IsJoyStickMoved(0, 0, 0, 1) && (joy[0].stick[0].axis[0].pos) < 0)
	  if (joy[0].stick[0].axis[0].d1) 
	  {
		cascii = 0;
		cscan = KEY_LEFT;
		player->joy_on = TRUE;
      }
      //else if (joy[0].stick[0].axis[0].d2) 

	  // Going right
	  //else if (IsJoyStickMoved(0, 0, 0, 1) && joy[0].stick[0].axis[0].pos > 0)
	  else if (joy[0].stick[0].axis[0].d2) 
	  {
		cascii = 0;
		cscan = KEY_RIGHT;
		player->joy_on = TRUE;
      }
      
	  //else if (joy[0].stick[0].axis[1].d1) 

	  // Going up
	  //else if (IsJoyStickMoved(0, 0, 1, 1) && (joy[0].stick[0].axis[1].pos) < 0)
	  else if (joy[0].stick[0].axis[1].d1) 
	  {
		cascii = 0;
		cscan = KEY_UP;
		player->joy_on = TRUE;
      }
      
	  // Going down
	  //else if (IsJoyStickMoved(0, 0, 1))
	  //else if (IsJoyStickMoved(0, 0, 1, 1) && (joy[0].stick[0].axis[1].pos) > 0)
	  else if (joy[0].stick[0].axis[1].d2) 
	  {
		cascii = 0;
		cscan = KEY_DOWN;
		player->joy_on = TRUE;
      }
      //else if ((joy[0].button[0].b) || (joy[0].button[1].b)) 
	  else if (IsJoyButtonDn(0, 0) || IsJoyButtonDn(0, 1))
	  {
		cascii = ' ';
		cscan = KEY_SPACE;
		player->joy_on = TRUE;
      }
      else
		cascii = cscan = 0;
   }

   }
   else
   {
	   nojoytime--;
	   if (nojoytime < 0)
		   nojoytime = 0;
   }*/

   /* deal with keyboard input */
   if ((cascii) || (cscan) || (keypressed())) 
   {
      if ((!cascii) && (!cscan))
		cascii = ureadkey(&cscan);

      ccombo = (cscan<<8) | ((cascii <= 255) ? cascii : '^');

      /* let object deal with the key */
      if (player->focus_obj >= 0) 
	  {
		MESSAGE(player->focus_obj, MSG_CHAR, ccombo);
		if (player->res & D_USED_CHAR)
			goto getout;

		MESSAGE(player->focus_obj, MSG_UCHAR, cascii);
		if (player->res & D_USED_CHAR)
			goto getout;
      }

      /* keyboard shortcut? */
      for (c=0; player->dialog[c].proc; c++) 
	  {
		if ((((cascii > 0) && (cascii <= 255) && 
	       (utolower(player->dialog[c].key) == utolower((cascii)))) ||
	      ((!cascii) && (player->dialog[c].key == (cscan<<8)))) && 
	     (!(player->dialog[c].flags & (D_HIDDEN | D_DISABLED)))) 
		{
			MESSAGE(c, MSG_KEY, ccombo);
			goto getout;
		}
      }

      /* broadcast in case any other objects want it */
      for (c=0; player->dialog[c].proc; c++) {
	 if (!(player->dialog[c].flags & (D_HIDDEN | D_DISABLED))) {
	    MESSAGE(c, MSG_XCHAR, ccombo);
	    if (player->res & D_USED_CHAR)
	       goto getout;
	 }
      }

      /* pass <CR> or <SPACE> to selected object */
      if (((cascii == '\r') || (cascii == '\n') || (cascii == ' ')) && 
	  (player->focus_obj >= 0)) {
	 MESSAGE(player->focus_obj, MSG_KEY, ccombo);
	 goto getout;
      }

      /* ESC closes dialog */
      if (cascii == 27) {
	 player->res |= D_CLOSE;
	 player->obj = -1;
	 goto getout;
      }

      /* move focus around the dialog */
      player->res |= move_focus(player->dialog, cascii, cscan, &player->focus_obj);
   }

   /* redraw? */
   check_for_redraw(player);

   /* send idle messages */
   player->res |= dialog_message(player->dialog, MSG_IDLE, 0, &player->obj);

   getout:

   ret = (!(player->res & D_CLOSE));
   player->res &= ~D_CLOSE;
   return ret;
}



/* shutdown_dialog:
 *  Destroys a dialog object returned by init_dialog(), returning the index
 *  of the object that caused it to exit.
 */
/*int shutdown_dialog(DIALOG_PLAYER *player)
{
   struct al_active_dialog_player *iter, *prev;
   int obj;
   ASSERT(player);

   // send the finish messages /
   dialog_message(player->dialog, MSG_END, 0, &player->obj);

   // remove the double click handler/
   gui_install_count--;

   if (gui_install_count <= 0) {
      remove_int(dclick_check);
      remove_display_switch_callback(gui_switch_callback);
   }

   if (player->mouse_obj >= 0)
      player->dialog[player->mouse_obj].flags &= ~D_GOTMOUSE;

   // remove dialog player from the list of active players /
   for (iter = first_active_dialog_player, prev = 0; iter != 0; prev = iter, iter = iter->next) {
      if (iter->player == player) {
	 if (prev)
	    prev->next = iter->next;
	 else
	    first_active_dialog_player = iter->next;

	 if (iter == current_active_dialog_player)
	    current_active_dialog_player = prev;

	 free (iter);
	 break;
      }
   }

   if (current_active_dialog_player)
      active_dialog_player = current_active_dialog_player->player;
   else
      active_dialog_player = NULL;

   if (active_dialog_player)
      active_dialog = active_dialog_player->dialog;
   else
      active_dialog = NULL;

   obj = player->obj;

   free(player);

   return obj;
}*/



