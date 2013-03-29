#define promod 0
#define DEFAULT_PROMOD_HAMFILE_REGISTERED "D2PROMOD.ham"
int PROMOD_REARM_TIME;

#define promod_homer_turnspeed 7 // (default is 8 on insane)

// default REARM_TIME = F1_0 = 65536 = 32768 + 32768

#define LASER_SWITCH		F1_0		/	2.2
//
#define VULCAN_SWITCH		F1_0		/	2.4
//
#define SPREAD_SWITCH		F1_0		/	2
//
#define PLASMA_SWITCH		F1_0		/	2
//
#define FUSION_SWITCH		F1_0		/	1.9
//
#define GAUSS_SWITCH		F1_0		/	1.8
//
#define HELIX_SWITCH			F1_0		/	1.8
//
#define PHOENIX_SWITCH	F1_0		/	1.7
//
#define OMEGA_SWITCH		F1_0		/	1.6
//
#define UNUSED_SWITCH		5000000
//set to some really high number in case this is actually called for, in which case it'll be easy to spot

#define CONC_SWITCH			F1_0		/	2
//
#define HOMER_SWITCH		F1_0		/	2
//
#define PROX_SWITCH			0								
// can be laid instantly anyway
#define SMART_SWITCH		F1_0		/ 	2
//
#define MEGA_SWITCH			F1_0		/	2
//
#define FLASH_SWITCH		F1_0		/	2
//
#define GUIDED_SWITCH		F1_0		/	1.75
//
#define SMINE_SWITCH		0
// can be laid instantly anyway
#define MERC_SWITCH			F1_0		/	1.9
//
#define SHAKER_SWITCH		F1_0		/	1.6
//
#define PHOENIX_LIFE_DIVISOR 			5
// the amount phoenix object's life is divided by each time it bounces
#define PHOENIX_DAMAGE_DIVISOR_1	3
// the amount phoenix object's damage is divided by after it bounced once
#define PHOENIX_DAMAGE_DIVISOR_2	7
// the amount phoenix object's damage is divided by after it bounced twice

//	WARNING: If you change DESIRED_OMEGA_DIST and MAX_OMEGA_BLOBS, you don't merely change the look of the cannon,
//	you change its range.  If you decrease DESIRED_OMEGA_DIST, you decrease how far the gun can fire.
#define OMEGA_BASE_TIME_PROMOD									F1_0/10 			// How many blobs per second!! No FPS-based blob creation anymore, no FPS-based damage anymore!
// default F1_0/10 for MP, F1_0/30 for SP
#define MIN_OMEGA_BLOBS_PROMOD									3					//	No matter how close the obstruction, at this many blobs created.
// default 3
#define MIN_OMEGA_DIST_PROMOD										F1_0*1.3		//	At least this distance between blobs, unless doing so would violate MIN_OMEGA_BLOBS
// default F1_0*3
#define DESIRED_OMEGA_DIST_PROMOD							F1_0*1.8		//	This is the desired distance between blobs.  For distances > MIN_OMEGA_BLOBS*DESIRED_OMEGA_DIST, but not very large, this will apply.
//default F1_0*5
#define MAX_OMEGA_BLOBS_PROMOD									16				//	No matter how far away the obstruction, this is the maximum number of blobs.
// default 16
#define MAX_OMEGA_DIST_PROMOD										(MAX_OMEGA_BLOBS_PROMOD * DESIRED_OMEGA_DIST_PROMOD)	//	Maximum extent of lightning blobs.
//	Additionally, several constants which apply to homing objects in general control the behavior of the Omega Cannon.
//	They are defined in laser.h.  They are copied here for reference.  These values are valid on 1/10/96:
//	If you want the Omega Cannon view cone to be different than the Homing Missile viewcone, contact MK to make the change.
//	(Unless you are a programmer, in which case, do it yourself!)
#define OMEGA_MIN_TRACKABLE_DOT_PROMOD				63*F1_0/64.5			//	Larger values mean narrower cone.  F1_0 means damn near impossible.  0 means 180 degree field of view.
// default 15*F1_0/16
#define OMEGA_MAX_TRACKABLE_DIST_PROMOD			MAX_OMEGA_DIST_PROMOD	//	An object must be at least this close to be tracked.
//	Note, you don't need to change these constants.  You can control damage and energy consumption by changing the
//	usual bitmaps.tbl parameters.
#define OMEGA_DAMAGE_SCALE_PROMOD							32		//	Controls how much damage is done.  This gets multiplied by the damage specified in bitmaps.tbl in the $WEAPON line.
// default 32
#define OMEGA_ENERGY_CONSUMPTION_PROMOD 		16		//	Controls how much energy is consumed.  This gets multiplied by the energy parameter from bitmaps.tbl. // pretty sure this doesn't do anything
// default 16 - pretty sure this variable is never used
#define	MIN_OMEGA_CHARGE_PROMOD								(MAX_OMEGA_CHARGE_PROMOD/8)
// default MAX_OMEGA_CHARGE_PROMOD/8
#define	OMEGA_CHARGE_SCALE_PROMOD						1			//default 4 //	FrameTime / OMEGA_CHARGE_SCALE added to Omega_charge every frame.
// default 4
#define MAX_OMEGA_CHARGE_PROMOD							  	F1_0
// default F1_0

#define NUM_SMART_CHILDREN_PROMOD				5
// default 6
#define NUM_EARTHSHAKER_CHILDREN_PROMOD 4
// default 6
#define NUM_SMARTMINE_CHILDREN_PROMOD		5
// default 6

#define VULCAN_SPREAD_VERTICAL_PROMOD		d_rand()/6 - 32767 / 12
// default d_rand()/8 - 32767 / 16
#define VULCAN_SPREAD_HORIZONTAL_PROMOD	d_rand()/6 - 32767 / 12
// default d_rand()/8 - 32767 / 16
#define HELIX_BLOBS_2_PROMOD	0.75
// default 1
#define HELIX_BLOBS_3_PROMOD	1.5
// default 2

#define SPREADFIRE_SPREAD_PROMOD	1

void promod_omega();		// jinx 02-14-13 promod
void init_promod_stuff();
int promod_first_time;