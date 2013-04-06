#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <setjmp.h>

#include "cJSON.h"

#include "rangers.h"
#include "rangers2.h"
#include "rangers3.h"

#include "player.h"
#include "hudmsg.h"
#include "multi.h"
#include "http_lib.h"
#include "sha256.h"

/* 
Anarchy = 1,
CaptureTheFlag = 2,
Cooperative = 3,
TeamAnarchy = 4,
RoboAnarchy = 5,
Entropy = 6,
Hoard = 7,
TeamHoard = 8,
HyperAnarchy = 9,
Monsterball = 10,
Bounty = 11

d2:
{
	"HashedAppToken":"7ee99601492050b4b095e504f45b60375ea6b211fb7f7efddf4c765cdfa0d0e3",
	"ExternalGameId":"Game ID from Rebirth (??)",
	"ReportingMemberId":"f4ef9dc6-f878-4470-b1ad-a16800cdfce8",
	"LevelName":"Minerva 3",
	"GameMode":4,
	"GameTimeInMinutesForCooperative":null,
	"NumberOfMembers":4,
	"NumberOfTeams":2,
	"Placements":
	[
		{"MemberId":"f4ef9dc6-f878-4470-b1ad-a16800cdfce8","Placement":1,"TeamPlacement":1},
		{"MemberId":"eab4e824-8ff7-4258-8fb9-a16800ce557e","Placement":2,"TeamPlacement":1},
		{"MemberId":"fcf76754-0397-431e-9890-a16800cdf551","Placement":3,"TeamPlacement":2},
		{"MemberId":"4996a5cd-c546-4afd-9f2b-a16800cdfcb7","Placement":4,"TeamPlacement":2}
	]
}
*/

cJSON *root;
static char *d2_pre_sha_hash =  "t6y27SeRJ7ir525" ;

void create_report()
{
	if (Player_num != multi_who_is_master()) return;
//	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	
	HUD_init_message(HM_CONSOLE, "d2 pre sha hash: %s", d2_pre_sha_hash);
	
	AppTokenHash(0, &d2_pre_sha_hash);
	
	HUD_init_message(HM_CONSOLE, "d2 post sha hash: %s", HashedAppToken);
	
	int N_players = 3;
	int GameID = 0;
	int N_teams = 0;
	
	Players[0].placement = 4;
	Players[1].placement = 3;
	Players[2].placement = 2;
	Players[3].placement = 1;
	
	Players[0].team_placement = 2;
	
	cJSON *plc_root;
		
	// if anything isn't applicable to the given gametype, set it to null (not 0)
	
	root=cJSON_CreateObject();	
	cJSON_AddStringToObject(root, "HashedAppToken", HashedAppToken);
HUD_init_message(HM_CONSOLE, "HashedAppToken: %s", HashedAppToken);
	cJSON_AddNumberToObject(root, "ExternalGameId", GameID);
	cJSON_AddStringToObject(root,"ReportingMemberId", RebirthAccessToken);
	cJSON_AddStringToObject(root,"LevelName", "Minerva 3");
	cJSON_AddNumberToObject(root,"GameMode", 4);
	cJSON_AddNumberToObject (root,"GameTimeInMinutesForCooperative", 0);
	cJSON_AddNumberToObject(root,"NumberOfMembers", N_players + 1); //
	cJSON_AddNumberToObject(root,"NumberOfTeams", N_teams);	// defaults to 2, set to 0 if non-team
	
	cJSON_AddItemToObject(root, "Placements", plc_root = cJSON_CreateArray());
	
	for (int i = 0; i < N_players; i++)
	{
		cJSON *plc_child[i];
		cJSON_AddItemToArray(plc_root, plc_child[i] = cJSON_CreateObject());
		cJSON_AddStringToObject(plc_child[i], "MemberId", Players[i].HashedAccessToken);
		cJSON_AddNumberToObject(plc_child[i],"Placement", Players[i].placement);
		cJSON_AddNumberToObject(plc_child[i],"TeamPlacement", Players[i].team_placement);
	}
		
	out=cJSON_Print(root);	// print to text
	dump_json_to_log();
	
	int verified = http_put(
	"/WebApi/DxxRebirth/v01/GameReport/ReportGame?AspxAutoDetectCookieSupport=1", 	// name of the file to create
	out, 	// data to send
	strlen(out), 	// length of data to send
	1, 	// overwrite data if it already exists
	0		// type of data
	);
	game_flush_inputs();
	
	//http_get(
	
	HUD_init_message(HM_CONSOLE, "verified? %d", verified);
	
	
	//	sending info to server and verification would happen here
	if (!server_verification())
	{
		error_verifying();
		return;
	}
	
	cJSON_Delete(root);	// delete the cJSON
	free(out);	// release the string
}

void dump_json_to_log()
{
	if (Player_num != multi_who_is_master()) return;
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	
	HUD_init_message(HM_CONSOLE, "Verification report saved as");
}

void error_verifying()
{
	if (Player_num != multi_who_is_master()) return;
	if (!Netgame.scored_game && !Netgame.ranked_game) return;
	
	HUD_init_message(HM_CONSOLE, "Error reporting the server.");
	HUD_init_message(HM_CONSOLE, "To retry, press f8 and type /report");
}

int server_verification()
{
	if (Player_num != multi_who_is_master()) return 0;
	if (!Netgame.scored_game && !Netgame.ranked_game) return 0;
	
	int tries = 0;

try_again:
	tries++;
	
	if (tries >= 4)
		return 0;
goto try_again;
}

static char *val =
{
    "7ee99601492050b4b095e504f45b60375ea6b211fb7f7efddf4c765cdfa0d0e3"
};

int AppTokenHash( int argc, char *argv[] )
{
	
	FILE *f;
    int i, j;
    //char output[65];
    sha256_context ctx;
    unsigned char buf[1000];
    unsigned char sha256sum[32];
	
     if( argc < 2 )
    {
        HUD_init_message(HM_CONSOLE, "SHA-256 Validation Tests:" );

        for( i = 0; i < 1; i++ )
        {
            //HUD_init_message(HM_CONSOLE, " Test %d ", i + 1 );

            sha256_starts( &ctx );

            if( i < 2 )
            {
                sha256_update( &ctx, (uint8 *) argv[i],
                               strlen( argv[i] ) );
            }
            else
            {
                memset( buf, 'a', 1000 );

                for( j = 0; j < 1000; j++ )
                {
                    sha256_update( &ctx, (uint8 *) buf, 1000 );
                }
            }

            sha256_finish( &ctx, sha256sum );

            for( j = 0; j < 32; j++ )
            {
                sprintf( HashedAppToken + j * 2, "%02x", sha256sum[j] );
            }

            if( memcmp( HashedAppToken, val, 64 ) )
            {
               HUD_init_message(HM_CONSOLE, "failed!" );
               return( 1 );
            }

            HUD_init_message(HM_CONSOLE,  "passed." );
        }
    }
    else
    {
        if( ! ( f = fopen( argv[1], "rb" ) ) )
        {
            perror( "fopen" );
            return( 1 );
        }

        sha256_starts( &ctx );

        while( ( i = fread( buf, 1, sizeof( buf ), f ) ) > 0 )
        {
            sha256_update( &ctx, buf, i );
        }

        sha256_finish( &ctx, sha256sum );

        for( j = 0; j < 32; j++ )
        {
            HUD_init_message(HM_CONSOLE,  "%02x", sha256sum[j] );
        }

        HUD_init_message(HM_CONSOLE, "  %s\n", argv[1] );
    }

    return( 0 );
}