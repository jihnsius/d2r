#define spec 1
#define DOOR_SPEC -1


int spec_trigger;

void multi_make_player_spec(int pnum);
void multi_send_spec_status (int pnum, int type);
void multi_do_spec_status (char * buf);
void multi_request_door_updates(int pnum);
void reset_stats_spec();

