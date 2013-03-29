#define instagib 0
#define misc 1
#define instagun_speed 11128
#define instagun_firespeed 86384
#define instagun_mass 5000
#define max_fix 2147483647

#define firespeed_bonus 1.5
#define gib_timer 2

#define tracer_type 9
#define bolt_type 3

#define last_killed_minimum 90
#define instagib_killstreak_divisor 5

void instagun_tracer();
void multi_send_killstreak(int killstreak);
void multi_do_killstreak(char * buf);
void multi_send_last_killed (int last_killed); 
void multi_do_last_killed(char * buf);

void do_last_killed();

int killstreak;
int highest_killstreak;
int gib_time;
int last_killed;
int last_killed_ticker;
int last_killed_timer;
int total_shots;
