#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>



#include "instagib.h"
#include "ctfc.h"
#include "spec.h"
#include "d1.h"
#include "jinx.h"
#include "promod.h"
#include "rangers.h"

#include "gamefont.h"
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

void drop_player_eggs(object *player); // from collide.c

// ************************************
// jinx 01-25-12 spectator block start
// ************************************

void multi_make_player_spec()
{
	if (!spec) return;
	if (Players[Player_num].flags & PLAYER_FLAGS_FLAG)
	{
		HUD_init_message(HM_MULTI, "Cannot join spectators while holding a flag");
		return;
	}

	int type;
	object *obj;
	obj = &Objects[Players[Player_num].objnum];
	if ((obj->type != OBJ_PLAYER) && (obj->type != OBJ_CAMERA) && (obj->type != OBJ_GHOST) && (obj->type != OBJ_CAMERA)) return;
	
	in_free = 1;
	piggy_num = 0;
	
	if (!(Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING))
	{
		obj->type = OBJ_CAMERA;
		obj->render_type = RT_NONE;
		obj->control_type	= CT_FLYING;
		obj->movement_type	= MT_PHYSICS;
		multi_reset_player_object(obj);
		Players[Player_num].spec_flags |= PLAYER_FLAGS_SPECTATING;
		multi_send_position(Players[Player_num].objnum);
		drop_player_eggs(ConsoleObject);
		multi_send_player_explode(MULTI_PLAYER_DROP);
		reset_stats_spec();
		in_free = 1;
		piggy_num = Player_num;
		HUD_init_message(HM_MULTI, "You are now spectating", Players[Player_num].callsign);
		Players[Player_num].ready = 1;
		do_resume();
		type = 1;
	}
	else if ((Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING))
	{
		obj->type = OBJ_PLAYER;
		obj->render_type = RT_POLYOBJ;
		obj->control_type	= CT_FLYING;
		obj->movement_type	= MT_PHYSICS;
		Players[Player_num].spec_flags &= ~PLAYER_FLAGS_SPECTATING;
		send_i_am_no_longer_spectating_you(piggy_num);
		HUD_init_message(HM_MULTI, "You are no longer spectating", Players[Player_num].callsign);
		timeout_frame();
		type = 0;
	}
	
	if (Game_mode & GM_MULTI_ROBOTS)
			multi_strip_robots(Player_num);
	
	kill_all_objects_linked_to_player();
	spec_kill_movement();
	
	multi_send_spec_status(type);
}

void multi_send_spec_status (int type)
{
	multibuf[0]=MULTI_DO_SPEC_STATUS;
	multibuf[1]=Player_num;
	multibuf[2]=type;
	
	multi_send_data(multibuf, 3, 2);
}

void multi_do_spec_status (char * buf)
{
	object *obj;
	
	int pnum = buf[1];
	int type = buf[2];
	
	obj = &Objects[Players[pnum].objnum];
	
	if ((obj->type != OBJ_CAMERA) && (obj->type != OBJ_PLAYER) && (obj->type != OBJ_GHOST) && (obj->type != OBJ_CAMERA))
	{
		HUD_init_message(HM_CONSOLE, "multi_do_spec_status() passed an illegal object #%d", Objects[Players[pnum].objnum]);
		return;
	}
	
	if (type == 1)
	{
		obj->type = OBJ_CAMERA;
		obj->render_type = RT_NONE;
		multi_reset_player_object(obj);
		Players[pnum].spec_flags |= PLAYER_FLAGS_SPECTATING;
		HUD_init_message(HM_MULTI, "%s is now spectating", Players[pnum].callsign);
		Players[pnum].ready = 1;
		check_for_all_players_ready();
	}
	if (type == 0)
	{
		obj->type = OBJ_PLAYER;
		obj->render_type = RT_POLYOBJ;
		multi_reset_player_object(obj);
		Players[pnum].spec_flags &= ~PLAYER_FLAGS_SPECTATING;
		HUD_init_message(HM_MULTI, "%s is no longer spectating", Players[pnum].callsign);
		Players[pnum].ready = 0;
		send_timeout();
	}
		
	if (Game_mode & GM_MULTI_ROBOTS)
			multi_strip_robots(pnum);
}

void reset_stats_spec()
{
	int i;

		if (Newdemo_state == ND_STATE_RECORDING)
		{
			newdemo_record_laser_level(Players[Player_num].laser_level, 0);
			newdemo_record_player_weapon(0, 0);
			newdemo_record_player_weapon(1, 0);
		}
		Primary_weapon = 0;
		Secondary_weapon = 0;
		for (i=0; i<MAX_PRIMARY_WEAPONS; i++)
			Primary_last_was_super[i] = 0;
		for (i=1; i<MAX_SECONDARY_WEAPONS; i++)
			Secondary_last_was_super[i] = 0;
		Player_eggs_dropped = 0;
		Global_laser_firing_count=0;
	
	Players[Player_num].laser_level = 0;
	for (i=0; i<MAX_PRIMARY_WEAPONS; i++)
		Players[Player_num].primary_ammo[i] = 0;
	for (i=1; i<MAX_SECONDARY_WEAPONS; i++)
		Players[Player_num].secondary_ammo[i] = 0;
	if (!Netgame.Instagib)
		Players[Player_num].secondary_ammo[0] = 2 + NDL - Difficulty_level;
	Players[Player_num].primary_weapon_flags = HAS_LASER_FLAG;
	if (!Netgame.Instagib)
		Players[Player_num].secondary_weapon_flags = HAS_CONCUSSION_FLAG;
	Players[Player_num].flags &= ~( PLAYER_FLAGS_QUAD_LASERS | PLAYER_FLAGS_AFTERBURNER | PLAYER_FLAGS_CLOAKED | PLAYER_FLAGS_INVULNERABLE | PLAYER_FLAGS_MAP_ALL | PLAYER_FLAGS_CONVERTER | PLAYER_FLAGS_AMMO_RACK | PLAYER_FLAGS_HEADLIGHT | PLAYER_FLAGS_HEADLIGHT_ON | PLAYER_FLAGS_FLAG);
	Players[Player_num].cloak_time = 0;
	Players[Player_num].invulnerable_time = 0;
	Players[Player_num].homing_object_dist = -F1_0; 
	digi_kill_sound_linked_to_object(Players[Player_num].objnum);
	killstreak = 0;		// jinx 01-22-13 instagib
	highest_killstreak = 0;
	last_killed = 0;
	game_flush_inputs();
}

void send_i_am_spectating_you(int pnum)
{
	if (!display_spec_text) return;
	multibuf[0]=DO_I_AM_SPECTATING_YOU;
	multibuf[1]=Player_num;
	multibuf[2]=pnum;
	multi_send_data(multibuf, 3, 1);
}

void do_i_am_spectating_you(char * buf)
{
	if (!display_spec_text) return;
	int pnum = buf[2];
	if (pnum != Player_num) return;
	int spec_num = buf[1];

	object *obj;
	obj = &Objects[Players[spec_num].objnum];
	obj->type = OBJ_GHOST;
	obj->render_type = RT_NONE;
	Players[spec_num].spec_flags |= PLAYER_FLAGS_SPECTATING_ME;
	Players[spec_num].spec_flags |= PLAYER_FLAGS_SPECTATING;
	HUD_init_message(HM_MULTI, "%s is now spectating you", Players[spec_num].callsign);
}

void send_i_am_no_longer_spectating_you(int pnum)
{
	if (!display_spec_text) return;
	multibuf[0]=DO_I_AM_NO_LONGER_SPECTATING_YOU;
	multibuf[1]=Player_num;
	multibuf[2]=pnum;
	multi_send_data(multibuf, 3, 1);
}

void do_i_am_no_longer_spectating_you(char * buf)
{
	if (!display_spec_text) return;
	int pnum = buf[2];
	if (pnum != Player_num) return;
	int spec_num = buf[1];
	Players[spec_num].spec_flags &= ~PLAYER_FLAGS_SPECTATING_ME;
	HUD_init_message(HM_MULTI, "%s is no longer spectating you", Players[spec_num].callsign);
}

void make_all_spectators_invisible()
{
/*
	for (int i = 0; i < N_players; i++)
	{
		if (Players[i].spec_flags & PLAYER_FLAGS_SPECTATING)
		{
			object * obj;
			obj = &Objects[Players[i].objnum];
	
			obj->type = OBJ_GHOST;
			obj->render_type = RT_NONE;
		}
	}	
*/
}

void switch_between_piggy_and_free()
{
	piggy_and_free_switch = 0;
	if ((in_free == 0) && (piggy_and_free_switch == 0))
	{
		in_free = 1;
		send_i_am_no_longer_spectating_you(piggy_num);
	}
	else if ((in_free == 1) && (piggy_and_free_switch == 0))
	{
		in_free = 0;
		switch_between_piggies();
	}
	kill_all_objects_linked_to_player();
	spec_kill_movement();
	game_flush_inputs();
}

void switch_between_piggies()
{
	if (in_free) return;
/*
	for (int i = last_piggy_num + 1; i <= Highest_object_index; i++)
		if (Objects[i].type == OBJ_PLAYER)
		{
			piggy_num = i;
			break;
		}
*/
		piggy_num++;
		last_piggy_num = piggy_num;
		in_free = 0;
		send_i_am_no_longer_spectating_you(last_piggy_num);
		send_i_am_spectating_you(piggy_num);		
		
	HUD_init_message(HM_MULTI, "now spectating %s", Players[piggy_num].callsign);

	game_flush_inputs();
	kill_all_objects_linked_to_player();
}

void render_spec_status()
{
	if (!(Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING)) return;
	if (!display_spec_text) return;
	
	gr_set_curfont(GAME_FONT);
	gr_set_fontcolor(BM_XRGB(0,63,0),-1);
	if (in_free == 1)
		gr_printf(0x8000, (LINE_SPACING*6)+FSPACY(1), "free fly mode");
	if (in_free == 0)
		gr_printf(0x8000, (LINE_SPACING*6)+FSPACY(1), "spectating %s", Players[piggy_num].callsign);
}

void make_spectator_ghost_follow_piggy(int pnum)
{
/*	
	Objects[Players[Player_num].objnum].pos =  Objects[Players[piggy_num].objnum].pos;
	Objects[Players[Player_num].objnum].orient =  Objects[Players[piggy_num].objnum].orient;
	Objects[Players[Player_num].objnum].segnum =  Objects[Players[piggy_num].objnum].segnum;
*/
	Objects[Players[Player_num].objnum].pos =  Objects[piggy_num].pos;
	Objects[Players[Player_num].objnum].orient =  Objects[piggy_num].orient;
	Objects[Players[Player_num].objnum].segnum =  Objects[piggy_num].segnum;
}

void spec_kill_movement()
{
	object * obj;
	obj = &Objects[Players[Player_num].objnum];

	obj->mtype.phys_info.velocity.x 	=	0;
	obj->mtype.phys_info.velocity.y	=	0;
	obj->mtype.phys_info.velocity.z 	=	0;
	obj->mtype.phys_info.thrust.x   	= 	0;
	obj->mtype.phys_info.thrust.y   	= 	0;
	obj->mtype.phys_info.thrust.z   	= 	0;
	obj->mtype.phys_info.brakes     	=	0;
	obj->mtype.phys_info.rotvel.x   	= 	0;
	obj->mtype.phys_info.rotvel.y   	= 	0;
	obj->mtype.phys_info.rotvel.z   	= 	0;
	obj->mtype.phys_info.rotthrust.x	=	0;
	obj->mtype.phys_info.rotthrust.y	=	0;
	obj->mtype.phys_info.rotthrust.z	=	0;
	obj->mtype.phys_info.turnroll    	= 	0;
	game_flush_inputs();
}

void do_spectator_frame()
{
	if (!(Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING)) return;
	if ((!in_free) && !(Players[Player_num].spec_flags & PLAYER_FLAGS_SPECTATING))	
	{
		make_spectator_ghost_follow_piggy(piggy_num);			
	}
	make_all_spectators_invisible();
}

void kill_all_objects_linked_to_player()
{
	for (int i=0;i<=Highest_object_index;i++)
	{
		object * linked;
		linked = &Objects[i];
		if ((linked->ctype.laser_info.parent_num == Players[Player_num].objnum) && (Players[Player_num].objnum != i) && ((Objects[i].type == OBJ_WEAPON) || (Objects[i].type == OBJ_FLARE)))
		{
			linked->flags |= OF_SHOULD_BE_DEAD;
			multi_send_remobj(linked-Objects);
		}
	}
}

void init_all_spectator_stuff()
{
	make_all_spectators_invisible();
}

void multi_do_spec_flags (char *buf)
{
	char pnum=buf[1];
	uint spec_flags;

	spec_flags = GET_INTEL_INT(buf + 2);
	if (pnum!=Player_num)
		Players[(int)pnum].spec_flags = spec_flags;
}

void multi_send_spec_flags (char pnum)
{
	multibuf[0]=MULTI_SPEC_FLAGS;
	multibuf[1]=pnum;
	PUT_INTEL_INT(multibuf+2, Players[(int)pnum].spec_flags);

	multi_send_data(multibuf, 6, 2);
	multi_do_spec_flags(multibuf);
}

// ************************************
// jinx 01-25-13 spectator block end
// ************************************