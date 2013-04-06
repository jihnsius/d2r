

#include "rangers.h"
#include "rangers3.h"
#include "hudmsg.h"
#include "multi.h"
#include "game.h"
#include "timer.h"

void send_timeout()
{
	Netgame.scored_game = 1;
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	if (Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING) return;
	multibuf[0] = DO_TIMEOUT;
	multibuf[1] = Player_num;
	multi_send_data(multibuf, 2, 2);
	do_timeout(multibuf);
}

void do_timeout(char * buf)
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	if (timeout_called) return;
	int pnum = buf[1];

	HUD_init_message(HM_MULTI, "%s called a timeout!", Players[pnum].callsign);
	for (int i = 0; i < N_players; i++)
		Players[i].ready = 0;
		
	for (int i = 0; i < N_players; i++)
		if (Players[i].spec_flags & PLAYER_FLAGS_SPECTATING)
			Players[i].ready = 1;
		
	timeout_frame();	
}

void send_ready()
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	if (Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING) return;
	multibuf[0] = DO_READY;
	multibuf[1] = Player_num;
	multi_send_data(multibuf, 2, 2);
	do_ready(multibuf);
}

void do_ready(char * buf)
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	int pnum = buf[1];
	if (!timeout_called) return;
	
	HUD_init_message(HM_MULTI, "%s is ready!", Players[pnum].callsign);
	Players[pnum].ready = 1;
	check_for_all_players_ready();
}

void check_for_all_players_ready()
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	if (Player_num == multi_who_is_master())
	{
		for (int i = 0; i < N_players; i++)
		{
			if (Players[i].ready != 1) return;
		}
		send_resume();
	}
}

void send_resume()
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	multibuf[0] = DO_COUNTDOWN;
	multi_send_data(multibuf, 1, 2);
	do_countdown();
}

void do_countdown()
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	do_resume();
	HUD_init_message(HM_MULTI, "resume!");
}

void send_add_caller(char pnum)
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	multibuf[0] = ADD_CALLER;
	multibuf[1] = pnum;
	multi_send_data(multibuf, 2, 2);
	do_add_caller(multibuf);
}

void do_add_caller(char * buf)
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	int pnum = buf[1];
	Players[pnum].caller = 1;
	HUD_init_message(HM_MULTI, "%s is now a caller!", Players[pnum].callsign);
}

void send_remove_caller(char pnum)
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	int total_callers = 0;
	for (int i = 0; i < N_players; i++)
	{
		if (Players[i].caller)
			total_callers++;
	}
	if (total_callers==1)
	{
		HUD_init_message(HM_MULTI, "Must have at least one caller!");
		return;
	}
	multibuf[0] = REMOVE_CALLER;
	multibuf[1] = pnum;
	multi_send_data(multibuf, 2, 2);
	do_remove_caller(multibuf);
}

void do_remove_caller(char * buf)
{
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	int pnum = buf[1];
	Players[pnum].caller = 0;
	HUD_init_message(HM_MULTI, "%s is no longer a caller! :(", Players[pnum].callsign);
}