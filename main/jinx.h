#define jinxmod 1
#define new_kill_text 0
#define testing 1
#define settings_leave_menu_when_hit 0
#define assault 0

int first_frame_of_this_life;
int skip_message_send;

void is_it_working();

void load_hamfile(char hamfile);
int load_new_ham;
char new_ham;
char new_hamfile;
extern char *killer_weapon[];

int killer_weapon_id;
int killer_weapon_type;
void set_killer_weapon(int wid, int wtype);
void show_killer_weapon_type();
void do_normal_player_killed_player_with_message(int killer_pnum, int killed_pnum, int kwid);

int console_kill;

int laser_total;
int plasma_total;

void multi_new_do_kill(char *buf);
void multi_new_send_kill();
void multi_new_compute_kill(int killer, int killed, int kwid, int kwtype);

void multi_new_bounty_target_new();
int multi_new_bounty_seed();

void reload_hamfile_first_time();