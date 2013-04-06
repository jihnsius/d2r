#include "collide.h"

// yo dawg i heard you like capture the flag classic so i put some classic in your flag so you can capture the flag classic while you capture the classic flag
// original d2 ctf is now called cctf (crappy ctf)
// ctfc by JinX.
// started: 16 December 2012 with protocol number 573

#define ctfc 1

int current_time;

//misc
ubyte force_respawn;
ubyte is_flag;
int ignore_last_kill_text;
ubyte first_spawns;

//flag spawns
int red_flag_spawn;
vms_vector red_flag_pos;
int blue_flag_spawn;
vms_vector blue_flag_pos;

//timers
int blue_flag_timer;
int red_flag_timer;
int player_spawn_timer;
int red_flag_respawn_countdown;
int blue_flag_respawn_countdown;
int blue_flag_smooth_timer;
int red_flag_smooth_timer;

void set_flag_home();
void multi_send_flag_orients();
void multi_do_flag_orients(char * buf);

void multi_send_return_bonus(char pnum);
void request_flag_spawns();
void multi_do_autoreturn_red();
void multi_do_autoreturn_blue();
void multi_send_autoreturn_red();
void multi_send_autoreturn_blue();

void multi_do_flag_disappearance(char * buf);
void multi_send_flag_disappearance(object * obj);
void multi_send_capture_bonus(char pnum);
void multi_send_capture_flag(char pnum);
void multi_send_return_flag(object *obj, char pnum);
void multi_do_return_flag(char *buf);
void multi_do_capture_flag(char * buf);
void multi_send_update_stats(int pnum, int type);
void multi_do_update_stats(char * buf);

void multi_send_flag_drop(int pnum);
void multi_do_flag_drop(char * buf);

void ctfc_fontcolor();