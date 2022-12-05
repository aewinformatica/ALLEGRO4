/******************************************/
/*              script_var.c              */
/*                                        */
/*       ˮ��֮Լ�еĽű����ͻ�����       */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/

/*��Ϊ�����ļ��еĺ���Ҫ�ı���ͻ���һЩ��־������д����ǰ������*/


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
/*�ű�״̬˵����SHUT �رսű�
                HANG ������Ϸִ�нű�
                RUN  ����ִ�У����ִ����
                WAIT �ȴ�ִ����һ���*/


/******************************************************************/

struct
{
 char filename[40];               /*�ļ���*/ 

 int size;                        /*�ű���С*/
 char command[10000];             /*��Žű�������*/

 char label_sign[32][32];          /*�����ת��������*/
 int label_offset[32];             /*�����תָ�������*/

 int temp[100];                    /*100����������õı���*/

 int cmd_offset;                 /*��ǰ�Ľű�ָ��ƫ��*/

 int cmd_stack[20];              /*��Žű�ָ��ƫ�ƵĶ�ջ*/
 int cmd_stack_off;              /*��ջָ��*/
 char fname_stack[20][40];       /*��Žű��ļ����Ķ�ջ*/
 int fname_stack_off;            /*��ջָ��*/
 char flag_stack[20];            /*״̬��ջ*/
 int flag_stack_off;             /*��ջָ��*/

 char script_flag;            /*��ǰ�ű����ͻ�״̬*/
}RPG_Script={0};

/*****************************************************/
struct
{
 MIDI *music;
 SAMPLE *sound;
}Voc={0};
