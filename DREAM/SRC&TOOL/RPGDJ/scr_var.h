/******************************************/
/*              script_var.c              */
/*                                        */
/*       水晶之约中的脚本解释机变量       */
/*                                        */
/*             编译环境DJGPP              */
/*                                        */
/*        编译需allegro V4.1.13支持       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

/*因为其他文件中的函数要改变解释机的一些标志，所以写到最前面来了*/


#define SCR_TALK       "talk"
#define SCR_ADD        "add"
#define SCR_IF         "if"
#define SCR_SET        "set"
#define SCR_GOTO       "goto"
#define SCR_DELAY      "delay"
#define SCR_NPC_WALK   "NPC_walk"
#define SCR_NPC_STATUS "set_NPC_status"
#define SCR_NPC_SET    "NPC_set"
#define SCR_NPC_TURN   "NPC_turn"
#define SCR_SCN_STATUS "set_screen_status"
#define SCR_SCN_SET    "screen_set"
#define SCR_SCN_MOV    "screen_move"
#define SCR_GAME_STATUS "set_game_status"
#define SCR_LOAD_MAP   "load_map"
#define SCR_GO_INOUT   "go_inout"
#define SCR_END        "end"
#define SCR_CALL       "call"
#define SCR_RETURN     "return"
#define SCR_NPC_IMG    "load_NPC_image"
#define SCR_INIT_NPC   "init_NPC"
#define SCR_HOOK_DATA  "hook_data"
#define SCR_ADD_HOOK   "add_hook"
#define SCR_DEL_HOOK   "del_hook"
#define SCR_CLR_HOOK   "kill_all_hook"
#define SCR_CLR_NPC    "clr_all_NPC"
#define SCR_LOAD_PH    "load_photo"
#define SCR_CHS_PLR    "choose_player"
#define SCR_INIT_OBJ   "init_obj_lib"
#define SCR_INIT_ACTOR "init_actor_lib"
#define SCR_ADD_ACTOR  "add_actor"
#define SCR_ADD_OBJ    "add_obj"
#define SCR_WEATHER    "set_weather"
#define SCR_LIGHT      "set_light"
#define SCR_MAP_CHOICE "map_choice"
#define SCR_PLR_WALK   "player_walk"
#define SCR_GET_DIR    "get_NPC_dir"
#define SHOW_SCR_IMG   "show_screen_image"
#define INIT_SCR_IMG   "init_screen_image"
#define SHUT_SCR_IMG   "shut_screen_image"
#define PLAY_MIDI      "play_midi"
#define STOP_MIDI      "stop_midi"
#define PLAY_SOUND     "play_sound"
#define GET_NPC_X      "get_NPC_x"
#define GET_NPC_Y      "get_NPC_y"
#define SCR_KILL_OBJ   "kill_obj"
#define SCR_QUIT       "quit"
#define SCR_RGB        "rgb2color"
#define SCR_MIDI_OUT   "midi_out"


#define SCRIPT_HANG    -1
#define SCRIPT_SHUT    0
#define SCRIPT_BUSY    1
#define SCRIPT_WAIT    2
/*脚本状态说明：SHUT 关闭脚本
                HANG 挂起游戏执行脚本
                RUN  正常执行，语句执行中
                WAIT 等待执行下一语句*/


/******************************************************************/

struct
{
 char filename[40];               /*文件名*/ 

 int size;                        /*脚本大小*/
 char command[10000];             /*存放脚本的数组*/

 char label_sign[32][32];          /*存放跳转符的数组*/
 int label_offset[32];             /*存放跳转指针的数组*/

 int temp[100];                    /*100个给程序调用的变量*/

 int cmd_offset;                 /*当前的脚本指针偏移*/

 int cmd_stack[20];              /*存放脚本指针偏移的堆栈*/
 int cmd_stack_off;              /*堆栈指针*/
 char fname_stack[20][40];       /*存放脚本文件名的堆栈*/
 int fname_stack_off;            /*堆栈指针*/
 char flag_stack[20];            /*状态堆栈*/
 int flag_stack_off;             /*堆栈指针*/

 char script_flag;            /*当前脚本解释机状态*/
}RPG_Script={0};

/*****************************************************/
struct
{
 MIDI *music;
 SAMPLE *sound;
}Voc={0};
