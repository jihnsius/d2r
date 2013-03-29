#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


// jinx
#include "instagib.h"
#include "ctfc.h"
#include "spec.h"
#include "d1.h"
#include "jinx.h"
#include "promod.h"

#include "gamefont.h"
// jinx

#include "u_mem.h"
#include "strutil.h"
#include "game.h"
#include "multi.h"
#include "object.h"
#include "laser.h"
#include "fuelcen.h"
#include "scores.h"
#include "gauges.h"
#include "collide.h"
#include "error.h"
#include "fireball.h"
#include "newmenu.h"
#include "console.h"
#include "wall.h"
#include "cntrlcen.h"
#include "powerup.h"
#include "polyobj.h"
#include "bm.h"
#include "endlevel.h"
#include "key.h"
#include "playsave.h"
#include "timer.h"
#include "digi.h"
#include "sounds.h"
#include "kconfig.h"
#include "newdemo.h"
#include "text.h"
#include "kmatrix.h"
#include "multibot.h"
#include "gameseq.h"
#include "physics.h"
#include "config.h"
#include "ai.h"
#include "switch.h"
#include "textures.h"
#include "byteswap.h"
#include "sounds.h"
#include "args.h"
#include "effects.h"
#include "iff.h"
#include "state.h"
#include "automap.h"
#ifdef USE_UDP
#include "net_udp.h"
#endif

// ************************************
// jinx 12-23-12 ctfc block start
// ************************************

void multi_do_return_flag(char *buf)
{
	if (!(multi_i_am_master())) return;
	int pnum = buf[1];
	int powerup = buf[2];
	if (powerup==47)	// if pow_flag_red (not including powerup.h)
	{
		maybe_drop_net_powerup (POW_FLAG_RED);
	}
	if (powerup==46)	// if pow_flag_blue (not including powerup.h)
	{
		maybe_drop_net_powerup (POW_FLAG_BLUE);
	}
	multi_send_return_bonus(pnum);
}

void multi_do_capture_flag(char * buf)
{
	if (!(multi_i_am_master())) return;
	
	int pnum = buf[1];
	
	if (get_team(pnum)==TEAM_RED)
	{
		maybe_drop_net_powerup (POW_FLAG_BLUE);
	}
	if (get_team(pnum)==TEAM_BLUE)
	{
		maybe_drop_net_powerup (POW_FLAG_RED);
	}
	multi_send_capture_bonus (pnum);
}

void multi_send_return_flag(object *obj, char pnum)
{
	multibuf[0]=MULTI_SEND_RETURN_FLAG;
	multibuf[1]=pnum;
	multibuf[2]=obj->id;
	//obj->flags |= OF_SHOULD_BE_DEAD;
	multi_send_data(multibuf, 3, 2);
	multi_do_return_flag(multibuf);
}

void multi_send_capture_flag(char pnum)
{
	multibuf[0]=MULTI_SEND_CAPTURE_FLAG;
	multibuf[1]=pnum;
	multi_send_data(multibuf, 2, 2);
	multi_do_capture_flag(multibuf);
}

void set_flag_home() 
{
	if (multi_i_am_master())
	{
		for (int i=0;i<=Highest_object_index;i++)
		{
			if (Objects[i].id == POW_FLAG_RED) 
			{
			red_flag_spawn = Objects[i].segnum;
			red_flag_pos = Objects[i].pos;
			}
			if (Objects[i].id == POW_FLAG_BLUE) 
			{
			blue_flag_spawn = Objects[i].segnum;
			blue_flag_pos = Objects[i].pos;
			}
		}
	}
}

void multi_send_flag_drop(int pnum)
{
	multibuf[0]=MULTI_DO_FLAG_DROP;
	multibuf[1]=pnum;
	multi_send_data(multibuf, 2, 2);
	multi_do_flag_drop(multibuf);
}

void multi_do_flag_drop(char * buf)
{
	int pnum = buf[1];
	
	if ((pnum == Player_num) && (get_team(Player_num)==TEAM_RED))
		HUD_init_message(HM_MULTI, "You dropped the blue flag!");
	else if ((pnum == Player_num) && (get_team(Player_num)==TEAM_BLUE))
		HUD_init_message(HM_MULTI, "You dropped the red flag!");
	
	if ((get_team(pnum)==TEAM_RED) && (Player_num != pnum)) 
		HUD_init_message(HM_MULTI, "%s dropped the blue flag!", Players[pnum].callsign);
	else if ((get_team(pnum)==TEAM_BLUE) && (Player_num != pnum))
		HUD_init_message(HM_MULTI, "%s dropped the red flag!", Players[pnum].callsign);
	
	Players[pnum].flags &= ~PLAYER_FLAGS_FLAG;
	
}

void multi_do_flag_disappearance(char * buf)
{
	int obj = buf[1];
	object_create_explosion(Objects[obj].segnum, &Objects[obj].pos, F1_0*7/2, VCLIP_POWERUP_DISAPPEARANCE );
}

void multi_send_flag_disappearance(object * obj)
{
	multibuf[0]=DO_FLAG_DISAPPEAR;
	multibuf[1]=obj-Objects;
	multi_send_data(multibuf, 2, 2);
	multi_do_flag_disappearance(multibuf);
}

// ************************************
// jinx 12-23-12 ctfc block end
// ************************************