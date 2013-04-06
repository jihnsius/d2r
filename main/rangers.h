#include "fix.h"

void init_rangers();

void send_timeout();
void do_timeout(char * buf);
void timeout_frame();
void send_ready();
void do_ready(char * buf);
void send_resume();
void do_resume();
void do_countdown();

void check_for_all_players_ready();

void timer_pause();
void timer_unpause();

void send_add_caller(char pnum);
void do_add_caller(char * buf);
void send_remove_caller(char pnum);
void do_remove_caller(char * buf);

int account_login();
int account_logout();
int account_login_name(int allow_abort);
int account_login_password(int allow_abort);
int account_login_access_token(int allow_abort);
int account_fetch_details();
int account_login_logout();

ubyte timeout_called;
ubyte pause_menu_page;