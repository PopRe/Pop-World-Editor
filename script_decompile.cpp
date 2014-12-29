
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "pop.h"
#include "popedt.h"
#include "script.h"



int  ScriptDecompile_SkipError();
bool ScriptDecompile_GetField(char *sz);
int  ScriptDecompile_BEGIN();
int  ScriptDecompile_DO();
int  ScriptDecompile_EVERY();
int  ScriptDecompile_SET();
int  ScriptDecompile_MULTIPLY_DEVIDE();
int  ScriptDecompile_test();
int  ScriptDecompile_logic();
int  ScriptDecompile_condition();
int  ScriptDecompile_IF();



bool ScriptDecompile(SCRIPTINFO *scriptinfo, HANDLE hfile)
{
	script_fail   = false;
	script_h      = hfile;
	script_indent = 0;
	script_p      = 0;
	script_si     = scriptinfo;

	if(script_si->Codes[0] != SCRIPT_VERSION)
	{
		sprintf(str, SZ_SCRIPT_ERR_WRONG_VERSION, script_si->Codes[script_p]);
		ScriptWriteSz(str);
	}
	if(!Script_inc_si()) return false;

	if(ScriptDecompile_BEGIN() != RES_OK) return false;

	if(script_code != TOKEN_SCRIPT_END)
	{
		sprintf(str, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND, SZ_TOKEN_SCRIPT_END, TOKEN_SCRIPT_END, script_code);
		ScriptWriteSz(str);
		return false;
	}

	ScriptWriteToken(SZ_TOKEN_SCRIPT_END);

	return !script_fail;
}


bool ScriptDecompile_GetField(char *sz)
{
	if(script_code >= MAX_FIELDS || script_code < 0)
	{
		sprintf(sz, SZ_SCRIPT_ERR_FIELD_OVERFLOW, script_code);
		return false;
	}

	FIELDINFO *fi = &script_si->Fields[script_code];

	switch(fi->Type)
	{
	case CONSTANT_VAL:
		sprintf(sz, "%d", fi->Value);
		return true;

	case USER_VAL:
		if(fi->Index >= MAX_VARS || fi->Index < 0)
		{
			sprintf(sz, SZ_SCRIPT_ERR_USERVAL_OVERFLOW, fi->Index);
			return false;
		}
		sprintf(sz, VAR_PREFIX "%d", fi->Index);
		return true;

	case INTERNAL_VAL:
		{
			switch(fi->Index)
			{
			case INT_M_SPELL_BURN_COST:						strcpy(sz, SZ_INT_M_SPELL_BURN_COST);					break;
			case INT_M_SPELL_BLAST_COST:					strcpy(sz, SZ_INT_M_SPELL_BLAST_COST);					break;
			case INT_M_SPELL_LIGHTNING_BOLT_COST:			strcpy(sz, SZ_INT_M_SPELL_LIGHTNING_BOLT_COST);			break;
			case INT_M_SPELL_WHIRLWIND_COST:				strcpy(sz, SZ_INT_M_SPELL_WHIRLWIND_COST);				break;
			case INT_M_SPELL_INSECT_PLAGUE_COST:			strcpy(sz, SZ_INT_M_SPELL_INSECT_PLAGUE_COST);			break;
			case INT_M_SPELL_INVISIBILITY_COST:				strcpy(sz, SZ_INT_M_SPELL_INVISIBILITY_COST);			break;
			case INT_M_SPELL_HYPNOTISM_COST:				strcpy(sz, SZ_INT_M_SPELL_HYPNOTISM_COST);				break;
			case INT_M_SPELL_FIRESTORM_COST:				strcpy(sz, SZ_INT_M_SPELL_FIRESTORM_COST);				break;
			case INT_M_SPELL_GHOST_ARMY_COST:				strcpy(sz, SZ_INT_M_SPELL_GHOST_ARMY_COST);				break;
			case INT_M_SPELL_EROSION_COST:					strcpy(sz, SZ_INT_M_SPELL_EROSION_COST);				break;
			case INT_M_SPELL_SWAMP_COST:					strcpy(sz, SZ_INT_M_SPELL_SWAMP_COST);					break;
			case INT_M_SPELL_LAND_BRIDGE_COST:				strcpy(sz, SZ_INT_M_SPELL_LAND_BRIDGE_COST);			break;
			case INT_M_SPELL_ANGEL_OF_DEATH_COST:			strcpy(sz, SZ_INT_M_SPELL_ANGEL_OF_DEATH_COST);			break;
			case INT_M_SPELL_EARTHQUAKE_COST:				strcpy(sz, SZ_INT_M_SPELL_EARTHQUAKE_COST);				break;
			case INT_M_SPELL_FLATTEN_COST:					strcpy(sz, SZ_INT_M_SPELL_FLATTEN_COST);				break;
			case INT_M_SPELL_VOLCANO_COST:					strcpy(sz, SZ_INT_M_SPELL_VOLCANO_COST);				break;
			case INT_M_SPELL_WRATH_OF_GOD_COST:				strcpy(sz, SZ_INT_M_SPELL_WRATH_OF_GOD_COST);			break;
			case INT_M_SPELL_SHIELD_COST:					strcpy(sz, SZ_INT_M_SPELL_SHIELD_COST);					break;
			case INT_MY_MANA:								strcpy(sz, SZ_INT_MY_MANA);								break;
			//
			case INT_M_BUILDING_TEPEE:						strcpy(sz, SZ_INT_M_BUILDING_TEPEE);					break;
			case INT_M_BUILDING_HUT:						strcpy(sz, SZ_INT_M_BUILDING_HUT);						break;
			case INT_M_BUILDING_FARM:						strcpy(sz, SZ_INT_M_BUILDING_FARM);						break;
			case INT_M_BUILDING_DRUM_TOWER:					strcpy(sz, SZ_INT_M_BUILDING_DRUM_TOWER);				break;
			case INT_M_BUILDING_TEMPLE:						strcpy(sz, SZ_INT_M_BUILDING_TEMPLE);					break;
			case INT_M_BUILDING_SPY_TRAIN:					strcpy(sz, SZ_INT_M_BUILDING_SPY_TRAIN);				break;
			case INT_M_BUILDING_WARRIOR_TRAIN:				strcpy(sz, SZ_INT_M_BUILDING_WARRIOR_TRAIN);			break;
			case INT_M_BUILDING_FIREWARRIOR_TRAIN:			strcpy(sz, SZ_INT_M_BUILDING_FIREWARRIOR_TRAIN);		break;
			case INT_M_BUILDING_RECONVERSION:				strcpy(sz, SZ_INT_M_BUILDING_RECONVERSION);				break;
			case INT_M_BUILDING_WALL_PIECE:					strcpy(sz, SZ_INT_M_BUILDING_WALL_PIECE);				break;
			case INT_M_BUILDING_GATE:						strcpy(sz, SZ_INT_M_BUILDING_GATE);						break;
			case INT_M_BUILDING_CURR_OE_SLOT:				strcpy(sz, SZ_INT_M_BUILDING_CURR_OE_SLOT);				break;
			case INT_M_BUILDING_BOAT_HUT_1:					strcpy(sz, SZ_INT_M_BUILDING_BOAT_HUT_1);				break;
			case INT_M_BUILDING_BOAT_HUT_2:					strcpy(sz, SZ_INT_M_BUILDING_BOAT_HUT_2);				break;
			case INT_M_BUILDING_AIRSHIP_HUT_1:				strcpy(sz, SZ_INT_M_BUILDING_AIRSHIP_HUT_1);			break;
			case INT_M_BUILDING_AIRSHIP_HUT_2:				strcpy(sz, SZ_INT_M_BUILDING_AIRSHIP_HUT_2);			break;
			//
			case INT_B_BUILDING_TEPEE:						strcpy(sz, SZ_INT_B_BUILDING_TEPEE);					break;
			case INT_B_BUILDING_HUT:						strcpy(sz, SZ_INT_B_BUILDING_HUT);						break;
			case INT_B_BUILDING_FARM:						strcpy(sz, SZ_INT_B_BUILDING_FARM);						break;
			case INT_B_BUILDING_DRUM_TOWER:					strcpy(sz, SZ_INT_B_BUILDING_DRUM_TOWER);				break;
			case INT_B_BUILDING_TEMPLE:						strcpy(sz, SZ_INT_B_BUILDING_TEMPLE);					break;
			case INT_B_BUILDING_SPY_TRAIN:					strcpy(sz, SZ_INT_B_BUILDING_SPY_TRAIN);				break;
			case INT_B_BUILDING_WARRIOR_TRAIN:				strcpy(sz, SZ_INT_B_BUILDING_WARRIOR_TRAIN);			break;
			case INT_B_BUILDING_FIREWARRIOR_TRAIN:			strcpy(sz, SZ_INT_B_BUILDING_FIREWARRIOR_TRAIN);		break;
			case INT_B_BUILDING_RECONVERSION:				strcpy(sz, SZ_INT_B_BUILDING_RECONVERSION);				break;
			case INT_B_BUILDING_WALL_PIECE:					strcpy(sz, SZ_INT_B_BUILDING_WALL_PIECE);				break;
			case INT_B_BUILDING_GATE:						strcpy(sz, SZ_INT_B_BUILDING_GATE);						break;
			case INT_B_BUILDING_CURR_OE_SLOT:				strcpy(sz, SZ_INT_B_BUILDING_CURR_OE_SLOT);				break;
			case INT_B_BUILDING_BOAT_HUT_1:					strcpy(sz, SZ_INT_B_BUILDING_BOAT_HUT_1);				break;
			case INT_B_BUILDING_BOAT_HUT_2:					strcpy(sz, SZ_INT_B_BUILDING_BOAT_HUT_2);				break;
			case INT_B_BUILDING_AIRSHIP_HUT_1:				strcpy(sz, SZ_INT_B_BUILDING_AIRSHIP_HUT_1);			break;
			case INT_B_BUILDING_AIRSHIP_HUT_2:				strcpy(sz, SZ_INT_B_BUILDING_AIRSHIP_HUT_2);			break;
			//
			case INT_R_BUILDING_TEPEE:						strcpy(sz, SZ_INT_R_BUILDING_TEPEE);					break;
			case INT_R_BUILDING_HUT:						strcpy(sz, SZ_INT_R_BUILDING_HUT);						break;
			case INT_R_BUILDING_FARM:						strcpy(sz, SZ_INT_R_BUILDING_FARM);						break;
			case INT_R_BUILDING_DRUM_TOWER:					strcpy(sz, SZ_INT_R_BUILDING_DRUM_TOWER);				break;
			case INT_R_BUILDING_TEMPLE:						strcpy(sz, SZ_INT_R_BUILDING_TEMPLE);					break;
			case INT_R_BUILDING_SPY_TRAIN:					strcpy(sz, SZ_INT_R_BUILDING_SPY_TRAIN);				break;
			case INT_R_BUILDING_WARRIOR_TRAIN:				strcpy(sz, SZ_INT_R_BUILDING_WARRIOR_TRAIN);			break;
			case INT_R_BUILDING_FIREWARRIOR_TRAIN:			strcpy(sz, SZ_INT_R_BUILDING_FIREWARRIOR_TRAIN);		break;
			case INT_R_BUILDING_RECONVERSION:				strcpy(sz, SZ_INT_R_BUILDING_RECONVERSION);				break;
			case INT_R_BUILDING_WALL_PIECE:					strcpy(sz, SZ_INT_R_BUILDING_WALL_PIECE);				break;
			case INT_R_BUILDING_GATE:						strcpy(sz, SZ_INT_R_BUILDING_GATE);						break;
			case INT_R_BUILDING_CURR_OE_SLOT:				strcpy(sz, SZ_INT_R_BUILDING_CURR_OE_SLOT);				break;
			case INT_R_BUILDING_BOAT_HUT_1:					strcpy(sz, SZ_INT_R_BUILDING_BOAT_HUT_1);				break;
			case INT_R_BUILDING_BOAT_HUT_2:					strcpy(sz, SZ_INT_R_BUILDING_BOAT_HUT_2);				break;
			case INT_R_BUILDING_AIRSHIP_HUT_1:				strcpy(sz, SZ_INT_R_BUILDING_AIRSHIP_HUT_1);			break;
			case INT_R_BUILDING_AIRSHIP_HUT_2:				strcpy(sz, SZ_INT_R_BUILDING_AIRSHIP_HUT_2);			break;
			//
			case INT_Y_BUILDING_TEPEE:						strcpy(sz, SZ_INT_Y_BUILDING_TEPEE);					break;
			case INT_Y_BUILDING_HUT:						strcpy(sz, SZ_INT_Y_BUILDING_HUT);						break;
			case INT_Y_BUILDING_FARM:						strcpy(sz, SZ_INT_Y_BUILDING_FARM);						break;
			case INT_Y_BUILDING_DRUM_TOWER:					strcpy(sz, SZ_INT_Y_BUILDING_DRUM_TOWER);				break;
			case INT_Y_BUILDING_TEMPLE:						strcpy(sz, SZ_INT_Y_BUILDING_TEMPLE);					break;
			case INT_Y_BUILDING_SPY_TRAIN:					strcpy(sz, SZ_INT_Y_BUILDING_SPY_TRAIN);				break;
			case INT_Y_BUILDING_WARRIOR_TRAIN:				strcpy(sz, SZ_INT_Y_BUILDING_WARRIOR_TRAIN);			break;
			case INT_Y_BUILDING_FIREWARRIOR_TRAIN:			strcpy(sz, SZ_INT_Y_BUILDING_FIREWARRIOR_TRAIN);		break;
			case INT_Y_BUILDING_RECONVERSION:				strcpy(sz, SZ_INT_Y_BUILDING_RECONVERSION);				break;
			case INT_Y_BUILDING_WALL_PIECE:					strcpy(sz, SZ_INT_Y_BUILDING_WALL_PIECE);				break;
			case INT_Y_BUILDING_GATE:						strcpy(sz, SZ_INT_Y_BUILDING_GATE);						break;
			case INT_Y_BUILDING_CURR_OE_SLOT:				strcpy(sz, SZ_INT_Y_BUILDING_CURR_OE_SLOT);				break;
			case INT_Y_BUILDING_BOAT_HUT_1:					strcpy(sz, SZ_INT_Y_BUILDING_BOAT_HUT_1);				break;
			case INT_Y_BUILDING_BOAT_HUT_2:					strcpy(sz, SZ_INT_Y_BUILDING_BOAT_HUT_2);				break;
			case INT_Y_BUILDING_AIRSHIP_HUT_1:				strcpy(sz, SZ_INT_Y_BUILDING_AIRSHIP_HUT_1);			break;
			case INT_Y_BUILDING_AIRSHIP_HUT_2:				strcpy(sz, SZ_INT_Y_BUILDING_AIRSHIP_HUT_2);			break;
			//
			case INT_G_BUILDING_TEPEE:						strcpy(sz, SZ_INT_G_BUILDING_TEPEE);					break;
			case INT_G_BUILDING_HUT:						strcpy(sz, SZ_INT_G_BUILDING_HUT);						break;
			case INT_G_BUILDING_FARM:						strcpy(sz, SZ_INT_G_BUILDING_FARM);						break;
			case INT_G_BUILDING_DRUM_TOWER:					strcpy(sz, SZ_INT_G_BUILDING_DRUM_TOWER);				break;
			case INT_G_BUILDING_TEMPLE:						strcpy(sz, SZ_INT_G_BUILDING_TEMPLE);					break;
			case INT_G_BUILDING_SPY_TRAIN:					strcpy(sz, SZ_INT_G_BUILDING_SPY_TRAIN);				break;
			case INT_G_BUILDING_WARRIOR_TRAIN:				strcpy(sz, SZ_INT_G_BUILDING_WARRIOR_TRAIN);			break;
			case INT_G_BUILDING_FIREWARRIOR_TRAIN:			strcpy(sz, SZ_INT_G_BUILDING_FIREWARRIOR_TRAIN);		break;
			case INT_G_BUILDING_RECONVERSION:				strcpy(sz, SZ_INT_G_BUILDING_RECONVERSION);				break;
			case INT_G_BUILDING_WALL_PIECE:					strcpy(sz, SZ_INT_G_BUILDING_WALL_PIECE);				break;
			case INT_G_BUILDING_GATE:						strcpy(sz, SZ_INT_G_BUILDING_GATE);						break;
			case INT_G_BUILDING_CURR_OE_SLOT:				strcpy(sz, SZ_INT_G_BUILDING_CURR_OE_SLOT);				break;
			case INT_G_BUILDING_BOAT_HUT_1:					strcpy(sz, SZ_INT_G_BUILDING_BOAT_HUT_1);				break;
			case INT_G_BUILDING_BOAT_HUT_2:					strcpy(sz, SZ_INT_G_BUILDING_BOAT_HUT_2);				break;
			case INT_G_BUILDING_AIRSHIP_HUT_1:				strcpy(sz, SZ_INT_G_BUILDING_AIRSHIP_HUT_1);			break;
			case INT_G_BUILDING_AIRSHIP_HUT_2:				strcpy(sz, SZ_INT_G_BUILDING_AIRSHIP_HUT_2);			break;
			//
			case INT_M_PERSON_BRAVE:						strcpy(sz, SZ_INT_M_PERSON_BRAVE);						break;
			case INT_M_PERSON_WARRIOR:						strcpy(sz, SZ_INT_M_PERSON_WARRIOR);					break;
			case INT_M_PERSON_RELIGIOUS:					strcpy(sz, SZ_INT_M_PERSON_RELIGIOUS);					break;
			case INT_M_PERSON_SPY:							strcpy(sz, SZ_INT_M_PERSON_SPY);						break;
			case INT_M_PERSON_FIREWARRIOR:					strcpy(sz, SZ_INT_M_PERSON_FIREWARRIOR);				break;
			case INT_M_PERSON_SHAMAN:						strcpy(sz, SZ_INT_M_PERSON_SHAMAN);						break;
			//
			case INT_B_PERSON_BRAVE:						strcpy(sz, SZ_INT_B_PERSON_BRAVE);						break;
			case INT_B_PERSON_WARRIOR:						strcpy(sz, SZ_INT_B_PERSON_WARRIOR);					break;
			case INT_B_PERSON_RELIGIOUS:					strcpy(sz, SZ_INT_B_PERSON_RELIGIOUS);					break;
			case INT_B_PERSON_SPY:							strcpy(sz, SZ_INT_B_PERSON_SPY);						break;
			case INT_B_PERSON_FIREWARRIOR:					strcpy(sz, SZ_INT_B_PERSON_FIREWARRIOR);				break;
			case INT_B_PERSON_SHAMAN:						strcpy(sz, SZ_INT_B_PERSON_SHAMAN);						break;
			//
			case INT_R_PERSON_BRAVE:						strcpy(sz, SZ_INT_R_PERSON_BRAVE);						break;
			case INT_R_PERSON_WARRIOR:						strcpy(sz, SZ_INT_R_PERSON_WARRIOR);					break;
			case INT_R_PERSON_RELIGIOUS:					strcpy(sz, SZ_INT_R_PERSON_RELIGIOUS);					break;
			case INT_R_PERSON_SPY:							strcpy(sz, SZ_INT_R_PERSON_SPY);						break;
			case INT_R_PERSON_FIREWARRIOR:					strcpy(sz, SZ_INT_R_PERSON_FIREWARRIOR);				break;
			case INT_R_PERSON_SHAMAN:						strcpy(sz, SZ_INT_R_PERSON_SHAMAN);						break;
			//
			case INT_Y_PERSON_BRAVE:						strcpy(sz, SZ_INT_Y_PERSON_BRAVE);						break;
			case INT_Y_PERSON_WARRIOR:						strcpy(sz, SZ_INT_Y_PERSON_WARRIOR);					break;
			case INT_Y_PERSON_RELIGIOUS:					strcpy(sz, SZ_INT_Y_PERSON_RELIGIOUS);					break;
			case INT_Y_PERSON_SPY:							strcpy(sz, SZ_INT_Y_PERSON_SPY);						break;
			case INT_Y_PERSON_FIREWARRIOR:					strcpy(sz, SZ_INT_Y_PERSON_FIREWARRIOR);				break;
			case INT_Y_PERSON_SHAMAN:						strcpy(sz, SZ_INT_Y_PERSON_SHAMAN);						break;
			//
			case INT_G_PERSON_BRAVE:						strcpy(sz, SZ_INT_G_PERSON_BRAVE);						break;
			case INT_G_PERSON_WARRIOR:						strcpy(sz, SZ_INT_G_PERSON_WARRIOR);					break;
			case INT_G_PERSON_RELIGIOUS:					strcpy(sz, SZ_INT_G_PERSON_RELIGIOUS);					break;
			case INT_G_PERSON_SPY:							strcpy(sz, SZ_INT_G_PERSON_SPY);						break;
			case INT_G_PERSON_FIREWARRIOR:					strcpy(sz, SZ_INT_G_PERSON_FIREWARRIOR);				break;
			case INT_G_PERSON_SHAMAN:						strcpy(sz, SZ_INT_G_PERSON_SHAMAN);						break;
			//
			case INT_M_VEHICLE_BOAT_1:						strcpy(sz, SZ_INT_M_VEHICLE_BOAT_1);					break;
			case INT_M_VEHICLE_AIRSHIP_1:					strcpy(sz, SZ_INT_M_VEHICLE_AIRSHIP_1);					break;
			case INT_B_VEHICLE_BOAT_1:						strcpy(sz, SZ_INT_B_VEHICLE_BOAT_1);					break;
			case INT_B_VEHICLE_AIRSHIP_1:					strcpy(sz, SZ_INT_B_VEHICLE_AIRSHIP_1);					break;
			case INT_R_VEHICLE_BOAT_1:						strcpy(sz, SZ_INT_R_VEHICLE_BOAT_1);					break;
			case INT_R_VEHICLE_AIRSHIP_1:					strcpy(sz, SZ_INT_R_VEHICLE_AIRSHIP_1);					break;
			case INT_Y_VEHICLE_BOAT_1:						strcpy(sz, SZ_INT_Y_VEHICLE_BOAT_1);					break;
			case INT_Y_VEHICLE_AIRSHIP_1:					strcpy(sz, SZ_INT_Y_VEHICLE_AIRSHIP_1);					break;
			case INT_G_VEHICLE_BOAT_1:						strcpy(sz, SZ_INT_G_VEHICLE_BOAT_1);					break;
			case INT_G_VEHICLE_AIRSHIP_1:					strcpy(sz, SZ_INT_G_VEHICLE_AIRSHIP_1);					break;
			//
			case INT_BLUE_KILLED_BY_ME:						strcpy(sz, SZ_INT_BLUE_KILLED_BY_ME);
			case INT_RED_KILLED_BY_ME:						strcpy(sz, SZ_INT_RED_KILLED_BY_ME);					break;
			case INT_YELLOW_KILLED_BY_ME:					strcpy(sz, SZ_INT_YELLOW_KILLED_BY_ME);					break;
			case INT_GREEN_KILLED_BY_ME:					strcpy(sz, SZ_INT_GREEN_KILLED_BY_ME);					break;
			//
			case INT_MY_NUM_KILLED_BY_BLUE:					strcpy(sz, SZ_INT_MY_NUM_KILLED_BY_BLUE);				break;
			case INT_MY_NUM_KILLED_BY_RED:					strcpy(sz, SZ_INT_MY_NUM_KILLED_BY_RED);				break;
			case INT_MY_NUM_KILLED_BY_YELLOW:				strcpy(sz, SZ_INT_MY_NUM_KILLED_BY_YELLOW);				break;
			case INT_MY_NUM_KILLED_BY_GREEN:				strcpy(sz, SZ_INT_MY_NUM_KILLED_BY_GREEN);				break;
			//
			case INT_ATTR_EXPANSION:						strcpy(sz, SZ_INT_ATTR_EXPANSION);						break;
			case INT_ATTR_PREF_SPY_TRAINS:					strcpy(sz, SZ_INT_ATTR_PREF_SPY_TRAINS);				break;
			case INT_ATTR_PREF_RELIGIOUS_TRAINS:			strcpy(sz, SZ_INT_ATTR_PREF_RELIGIOUS_TRAINS);			break;
			case INT_ATTR_PREF_WARRIOR_TRAINS:				strcpy(sz, SZ_INT_ATTR_PREF_WARRIOR_TRAINS);			break;
			case INT_ATTR_PREF_FIREWARRIOR_TRAINS:			strcpy(sz, SZ_INT_ATTR_PREF_FIREWARRIOR_TRAINS);		break;
			case INT_ATTR_PREF_SPY_PEOPLE:					strcpy(sz, SZ_INT_ATTR_PREF_SPY_PEOPLE);				break;
			case INT_ATTR_PREF_RELIGIOUS_PEOPLE:			strcpy(sz, SZ_INT_ATTR_PREF_RELIGIOUS_PEOPLE);			break;
			case INT_ATTR_PREF_WARRIOR_PEOPLE:				strcpy(sz, SZ_INT_ATTR_PREF_WARRIOR_PEOPLE);			break;
			case INT_ATTR_PREF_FIREWARRIOR_PEOPLE:			strcpy(sz, SZ_INT_ATTR_PREF_FIREWARRIOR_PEOPLE);		break;
			case INT_ATTR_MAX_BUILDINGS_ON_GO:				strcpy(sz, SZ_INT_ATTR_MAX_BUILDINGS_ON_GO);			break;
			case INT_ATTR_HOUSE_PERCENTAGE:					strcpy(sz, SZ_INT_ATTR_HOUSE_PERCENTAGE);				break;
			case INT_ATTR_AWAY_BRAVE:						strcpy(sz, SZ_INT_ATTR_AWAY_BRAVE);						break;
			case INT_ATTR_AWAY_WARRIOR:						strcpy(sz, SZ_INT_ATTR_AWAY_WARRIOR);					break;
			case INT_ATTR_AWAY_RELIGIOUS:					strcpy(sz, SZ_INT_ATTR_AWAY_RELIGIOUS);					break;
			case INT_ATTR_DEFENSE_RAD_INCR:					strcpy(sz, SZ_INT_ATTR_DEFENSE_RAD_INCR);				break;
			case INT_ATTR_MAX_DEFENSIVE_ACTIONS:			strcpy(sz, SZ_INT_ATTR_MAX_DEFENSIVE_ACTIONS);			break;
			case INT_ATTR_AWAY_SPY:							strcpy(sz, SZ_INT_ATTR_AWAY_SPY);						break;
			case INT_ATTR_AWAY_FIREWARRIOR:					strcpy(sz, SZ_INT_ATTR_AWAY_FIREWARRIOR);				break;
			case INT_ATTR_ATTACK_PERCENTAGE:				strcpy(sz, SZ_INT_ATTR_ATTACK_PERCENTAGE);				break;
			case INT_ATTR_AWAY_SHAMAN:						strcpy(sz, SZ_INT_ATTR_AWAY_SHAMAN);					break;
			case INT_ATTR_PEOPLE_PER_BOAT:					strcpy(sz, SZ_INT_ATTR_PEOPLE_PER_BOAT);				break;
			case INT_ATTR_PEOPLE_PER_BALLOON:				strcpy(sz, SZ_INT_ATTR_PEOPLE_PER_BALLOON);				break;
			case INT_ATTR_MAX_SPY_ATTACKS:					strcpy(sz, SZ_INT_ATTR_MAX_SPY_ATTACKS);				break;
			case INT_ATTR_ENEMY_SPY_MAX_STAND:				strcpy(sz, SZ_INT_ATTR_ENEMY_SPY_MAX_STAND);			break;
			case INT_ATTR_MAX_ATTACKS:						strcpy(sz, SZ_INT_ATTR_MAX_ATTACKS);					break;
			case INT_ATTR_SPY_CHECK_FREQUENCY:				strcpy(sz, SZ_INT_ATTR_SPY_CHECK_FREQUENCY);			break;
			case INT_ATTR_RETREAT_VALUE:					strcpy(sz, SZ_INT_ATTR_RETREAT_VALUE);					break;
			case INT_ATTR_SHAMEN_BLAST:						strcpy(sz, SZ_INT_ATTR_SHAMEN_BLAST);					break;
			case INT_ATTR_MAX_TRAIN_AT_ONCE:				strcpy(sz, SZ_INT_ATTR_MAX_TRAIN_AT_ONCE);				break;
			case INT_ATTR_GROUP_OPTION:						strcpy(sz, SZ_INT_ATTR_GROUP_OPTION);					break;
			case INT_ATTR_PREF_BOAT_HUTS:					strcpy(sz, SZ_INT_ATTR_PREF_BOAT_HUTS);					break;
			case INT_ATTR_PREF_BALLOON_HUTS:				strcpy(sz, SZ_INT_ATTR_PREF_BALLOON_HUTS);				break;
			case INT_ATTR_PREF_BOAT_DRIVERS:				strcpy(sz, SZ_INT_ATTR_PREF_BOAT_DRIVERS);				break;
			case INT_ATTR_PREF_BALLOON_DRIVERS:				strcpy(sz, SZ_INT_ATTR_PREF_BALLOON_DRIVERS);			break;
			case INT_ATTR_FIGHT_STOP_DISTANCE:				strcpy(sz, SZ_INT_ATTR_FIGHT_STOP_DISTANCE);			break;
			case INT_ATTR_SPY_DISCOVER_CHANCE:				strcpy(sz, SZ_INT_ATTR_SPY_DISCOVER_CHANCE);			break;
			case INT_ATTR_COUNT_PREACH_DAMAGE:				strcpy(sz, SZ_INT_ATTR_COUNT_PREACH_DAMAGE);			break;
			case INT_ATTR_USE_PREACHER_FOR_DEFENCE:			strcpy(sz, SZ_INT_ATTR_USE_PREACHER_FOR_DEFENCE);		break;
			case INT_ATTR_RANDOM_BUILD_SIDE:				strcpy(sz, SZ_INT_ATTR_RANDOM_BUILD_SIDE);				break;
			case INT_ATTR_BASE_UNDER_ATTACK_RETREAT:		strcpy(sz, SZ_INT_ATTR_BASE_UNDER_ATTACK_RETREAT);		break;
			case INT_ATTR_EMPTY_AT_WAYPOINT:				strcpy(sz, SZ_INT_ATTR_EMPTY_AT_WAYPOINT);				break;
			case INT_ATTR_DONT_USE_BOATS:					strcpy(sz, SZ_INT_ATTR_DONT_USE_BOATS);					break;
			case INT_ATTR_DONT_GROUP_AT_DT:					strcpy(sz, SZ_INT_ATTR_DONT_GROUP_AT_DT);				break;
			case INT_ATTR_SPELL_DELAY:						strcpy(sz, SZ_INT_ATTR_SPELL_DELAY);					break;
			case INT_ATTR_DONT_DELETE_USELESS_BOAT_HOUSE:	strcpy(sz, SZ_INT_ATTR_DONT_DELETE_USELESS_BOAT_HOUSE);	break;
			case INT_ATTR_BOAT_HOUSE_BROKEN:				strcpy(sz, SZ_INT_ATTR_BOAT_HOUSE_BROKEN);				break;
			case INT_ATTR_DONT_AUTO_TRAIN_PREACHERS:		strcpy(sz, SZ_INT_ATTR_DONT_AUTO_TRAIN_PREACHERS);		break;
			case INT_ATTR_SPARE_6:							strcpy(sz, SZ_INT_ATTR_SPARE_6);						break;
			//
			case INT_NO_SPECIFIC_SPELL:						strcpy(sz, SZ_INT_NO_SPECIFIC_SPELL);					break;
			case INT_BURN:									strcpy(sz, SZ_INT_BURN);								break;
			case INT_BLAST:									strcpy(sz, SZ_INT_BLAST);								break;
			case INT_LIGHTNING_BOLT:						strcpy(sz, SZ_INT_LIGHTNING_BOLT);						break;
			case INT_WHIRLWIND:								strcpy(sz, SZ_INT_WHIRLWIND);							break;
			case INT_INSECT_PLAGUE:							strcpy(sz, SZ_INT_INSECT_PLAGUE);						break;
			case INT_INVISIBILITY:							strcpy(sz, SZ_INT_INVISIBILITY);						break;
			case INT_HYPNOTISM:								strcpy(sz, SZ_INT_HYPNOTISM);							break;
			case INT_FIRESTORM:								strcpy(sz, SZ_INT_FIRESTORM);							break;
			case INT_GHOST_ARMY:							strcpy(sz, SZ_INT_GHOST_ARMY);							break;
			case INT_EROSION:								strcpy(sz, SZ_INT_EROSION);								break;
			case INT_SWAMP:									strcpy(sz, SZ_INT_SWAMP);								break;
			case INT_LAND_BRIDGE:							strcpy(sz, SZ_INT_LAND_BRIDGE);							break;
			case INT_ANGEL_OF_DEATH:						strcpy(sz, SZ_INT_ANGEL_OF_DEATH);						break;
			case INT_EARTHQUAKE:							strcpy(sz, SZ_INT_EARTHQUAKE);							break;
			case INT_FLATTEN:								strcpy(sz, SZ_INT_FLATTEN);								break;
			case INT_VOLCANO:								strcpy(sz, SZ_INT_VOLCANO);								break;
			case INT_WRATH_OF_GOD:							strcpy(sz, SZ_INT_WRATH_OF_GOD);						break;
			case INT_SHIELD:								strcpy(sz, SZ_INT_SHIELD);								break;
			case INT_CONVERT:								strcpy(sz, SZ_INT_CONVERT);								break;
			case INT_TELEPORT:								strcpy(sz, SZ_INT_TELEPORT);							break;
			case INT_BLOODLUST:								strcpy(sz, SZ_INT_BLOODLUST);							break;
			//
			case INT_NO_SPECIFIC_PERSON:					strcpy(sz, SZ_INT_NO_SPECIFIC_PERSON);					break;
			case INT_BRAVE:									strcpy(sz, SZ_INT_BRAVE);								break;
			case INT_WARRIOR:								strcpy(sz, SZ_INT_WARRIOR);								break;
			case INT_RELIGIOUS:								strcpy(sz, SZ_INT_RELIGIOUS);							break;
			case INT_SPY:									strcpy(sz, SZ_INT_SPY);									break;
			case INT_FIREWARRIOR:							strcpy(sz, SZ_INT_FIREWARRIOR);							break;
			case INT_SHAMAN:								strcpy(sz, SZ_INT_SHAMAN);								break;
			//
			case INT_NO_SPECIFIC_BUILDING:					strcpy(sz, SZ_INT_NO_SPECIFIC_BUILDING);				break;
			case INT_TARGET_SHAMAN:							strcpy(sz, SZ_INT_TARGET_SHAMAN);						break;
			case INT_TEPEE:									strcpy(sz, SZ_INT_TEPEE);								break;
			case INT_HUT:									strcpy(sz, SZ_INT_HUT);									break;
			case INT_FARM:									strcpy(sz, SZ_INT_FARM);								break;
			case INT_DRUM_TOWER:							strcpy(sz, SZ_INT_DRUM_TOWER);							break;
			case INT_TEMPLE:								strcpy(sz, SZ_INT_TEMPLE);								break;
			case INT_SPY_TRAIN:								strcpy(sz, SZ_INT_SPY_TRAIN);							break;
			case INT_WARRIOR_TRAIN:							strcpy(sz, SZ_INT_WARRIOR_TRAIN);						break;
			case INT_FIREWARRIOR_TRAIN:						strcpy(sz, SZ_INT_FIREWARRIOR_TRAIN);					break;
			case INT_WALL_PIECE:							strcpy(sz, SZ_INT_WALL_PIECE);							break;
			case INT_GATE:									strcpy(sz, SZ_INT_GATE);								break;
			case INT_BOAT_HUT_1:							strcpy(sz, SZ_INT_BOAT_HUT_1);							break;
			case INT_BOAT_HUT_2:							strcpy(sz, SZ_INT_BOAT_HUT_2);							break;
			case INT_AIRSHIP_HUT_1:							strcpy(sz, SZ_INT_AIRSHIP_HUT_1);						break;
			case INT_AIRSHIP_HUT_2:							strcpy(sz, SZ_INT_AIRSHIP_HUT_2);						break;
			//
			case INT_CP_FREE_ENTRIES:						strcpy(sz, SZ_INT_CP_FREE_ENTRIES);						break;
			case INT_RANDOM_100:							strcpy(sz, SZ_INT_RANDOM_100);							break;
			//
			case INT_NUM_SHAMEN_DEFENDERS:					strcpy(sz, SZ_INT_NUM_SHAMEN_DEFENDERS);				break;
			case INT_CAMERA_ANGLE:							strcpy(sz, SZ_INT_CAMERA_ANGLE);						break;
			case INT_CAMERA_X:								strcpy(sz, SZ_INT_CAMERA_X);							break;
			case INT_CAMERA_Z:								strcpy(sz, SZ_INT_CAMERA_Z);							break;
			//
			case INT_GAME_TURN:								strcpy(sz, SZ_INT_GAME_TURN);							break;
			case INT_MY_NUM_PEOPLE:							strcpy(sz, SZ_INT_MY_NUM_PEOPLE);						break;
			case INT_BLUE_PEOPLE:							strcpy(sz, SZ_INT_BLUE_PEOPLE);							break;
			case INT_RED_PEOPLE:							strcpy(sz, SZ_INT_RED_PEOPLE);							break;
			case INT_YELLOW_PEOPLE:							strcpy(sz, SZ_INT_YELLOW_PEOPLE);						break;
			case INT_GREEN_PEOPLE:							strcpy(sz, SZ_INT_GREEN_PEOPLE);						break;
			case INT_MY_NUM_KILLED_BY_HUMAN:				strcpy(sz, SZ_INT_MY_NUM_KILLED_BY_HUMAN);				break;
			case INT_RED_KILLED_BY_HUMAN:					strcpy(sz, SZ_INT_RED_KILLED_BY_HUMAN);					break;
			case INT_YELLOW_KILLED_BY_HUMAN:				strcpy(sz, SZ_INT_YELLOW_KILLED_BY_HUMAN);				break;
			case INT_GREEN_KILLED_BY_HUMAN:					strcpy(sz, SZ_INT_GREEN_KILLED_BY_HUMAN);				break;
			case INT_WILD_PEOPLE:							strcpy(sz, SZ_INT_WILD_PEOPLE);							break;
			case INT_BLUE_MANA:								strcpy(sz, SZ_INT_BLUE_MANA);							break;
			case INT_RED_MANA:								strcpy(sz, SZ_INT_RED_MANA);							break;
			case INT_YELLOW_MANA:							strcpy(sz, SZ_INT_YELLOW_MANA);							break;
			case INT_GREEN_MANA:							strcpy(sz, SZ_INT_GREEN_MANA);							break;
			//
			default:
				sprintf(sz, SZ_SCRIPT_ERR_UNKNOW_INT_VAR, fi->Index);
				return false;
			}
		}
		return true;

	default:
		sprintf(sz, SZ_SCRIPT_ERR_UNKNOW_VAR_TYPE, fi->Type);
		return false;
	}
}


int ScriptDecompile_SkipError()
{
	do
	{
		if(!Script_inc_si()) return RES_FATAL;
		if(script_code >= TOKEN_OFFSET || script_code == 0) return RES_OK;
	}
	while(script_p < MAX_CODES);

	return RES_OK;
}


int ScriptDecompile_BEGIN()
{
	if(script_code != TOKEN_BEGIN)
	{
		sprintf(str, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND, SZ_TOKEN_BEGIN, TOKEN_BEGIN, script_code);
		ScriptWriteSz(str);
		return RES_FATAL;
	}
	if(!Script_inc_si()) return RES_FATAL;

	ScriptWriteToken(SZ_TOKEN_BEGIN);
	script_indent++;

	for(;;)
	{
		if(script_p >= MAX_CODES)
		{
			ScriptWriteSz(SZ_SCRIPT_ERR_CODE_EOF);
			return RES_FATAL;
		}

		if(script_code == TOKEN_END)
		{
			if(script_indent > 0) script_indent--;
			ScriptWriteToken(SZ_TOKEN_END);
			if(!Script_inc_si()) return RES_FATAL;
			return RES_OK;
		}

		switch(script_code)
		{
		case 0:
			ScriptWriteSz(SZ_SCRIPT_ERR_PREMATURE_END);
			return RES_FATAL;
		case TOKEN_DO:
			{
				rs = ScriptDecompile_DO();

				if(rs == RES_FATAL)
					return RES_FATAL;
				else if(rs == RES_ERROR)
					if(ScriptDecompile_SkipError() != RES_OK) return RES_FATAL;
			}			
			break;
		case TOKEN_EVERY:
			{
				rs = ScriptDecompile_EVERY();

				if(rs == RES_FATAL)
					return RES_FATAL;
				else if (rs == RES_ERROR)
					if(ScriptDecompile_SkipError() != RES_OK) return RES_FATAL;
			}
			break;
		case TOKEN_INCREMENT:
		case TOKEN_DECREMENT:
		case TOKEN_SET:
			{
				rs = ScriptDecompile_SET();

				if(rs == RES_FATAL)
					return RES_FATAL;
				else if(rs == RES_ERROR)
					if(ScriptDecompile_SkipError() != RES_OK) return RES_FATAL;
			}
			break;
		case TOKEN_MULTIPLY:
		case TOKEN_DIVIDE:
			{
				rs = ScriptDecompile_MULTIPLY_DEVIDE();

				if(rs == RES_FATAL)
					return RES_FATAL;
				else if(rs == RES_ERROR)
					if(ScriptDecompile_SkipError() != RES_OK) return RES_FATAL;
			}
			break;
		case TOKEN_IF:
			{
				rs = ScriptDecompile_IF();

				if(rs == RES_FATAL)
					return RES_FATAL;
				else if(rs == RES_ERROR)
					if(ScriptDecompile_SkipError() != RES_OK) return RES_FATAL;
			}
			break;
		default:
			sprintf(str, SZ_SCRIPT_ERR_UNKNOW_COMMAND, script_code);
			ScriptWriteSz(str);
			if(ScriptDecompile_SkipError() != RES_OK) return RES_FATAL;
			break;
		}
	}
}


int ScriptDecompile_DO()
{
	if(script_code != TOKEN_DO)
	{
		sprintf(str, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND, SZ_TOKEN_DO, TOKEN_DO, script_code);
		ScriptWriteSz(str);
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;

	strcpy(str, SZ_TOKEN_DO);
	strcat(str, " ");

	switch(script_code)
	{
	case TOKEN_CONSTRUCT_BUILDING:
		strcat(str, SZ_TOKEN_CONSTRUCT_BUILDING);
		goto _on_off;
	case TOKEN_FETCH_WOOD:
		strcat(str, SZ_TOKEN_FETCH_WOOD);
		goto _on_off;
	case TOKEN_SHAMAN_GET_WILDS:
		strcat(str, SZ_TOKEN_SHAMAN_GET_WILDS);
		goto _on_off;
	case TOKEN_HOUSE_A_PERSON:
		strcat(str, SZ_TOKEN_HOUSE_A_PERSON);
		goto _on_off;
	case TOKEN_SEND_GHOSTS:
		strcat(str, SZ_TOKEN_SEND_GHOSTS);
		goto _on_off;
	case TOKEN_BRING_NEW_PEOPLE_BACK:
		strcat(str, SZ_TOKEN_BRING_NEW_PEOPLE_BACK);
		goto _on_off;
	case TOKEN_TRAIN_PEOPLE:
		strcat(str, SZ_TOKEN_TRAIN_PEOPLE);
		goto _on_off;
	case TOKEN_POPULATE_DRUM_TOWER:
		strcat(str, SZ_TOKEN_POPULATE_DRUM_TOWER);
		goto _on_off;
	case TOKEN_DEFEND:
		strcat(str, SZ_TOKEN_DEFEND);
		goto _on_off;
	case TOKEN_DEFEND_BASE:
		strcat(str, SZ_TOKEN_DEFEND_BASE);
		goto _on_off;
	case TOKEN_PREACH:
		strcat(str, SZ_TOKEN_PREACH);
		goto _on_off;
	case TOKEN_BUILD_WALLS:
		strcat(str, SZ_TOKEN_BUILD_WALLS);
		goto _on_off;
	case TOKEN_SABOTAGE:
		strcat(str, SZ_TOKEN_SABOTAGE);
		goto _on_off;
	case TOKEN_SPELL_OFFENSIVE:
		strcat(str, SZ_TOKEN_SPELL_OFFENSIVE);
		goto _on_off;
	case TOKEN_FIREWARRIOR_DEFEND:
		strcat(str, SZ_TOKEN_FIREWARRIOR_DEFEND);
		goto _on_off;
	case TOKEN_BUILD_VEHICLE:
		strcat(str, SZ_TOKEN_BUILD_VEHICLE);
		goto _on_off;
	case TOKEN_FETCH_LOST_PEOPLE:
		strcat(str, SZ_TOKEN_FETCH_LOST_PEOPLE);
		goto _on_off;
	case TOKEN_FETCH_LOST_VEHICLE:
		strcat(str, SZ_TOKEN_FETCH_LOST_VEHICLE);
		goto _on_off;
	case TOKEN_FETCH_FAR_VEHICLE:
		strcat(str, SZ_TOKEN_FETCH_FAR_VEHICLE);
		goto _on_off;
	case TOKEN_AUTO_ATTACK:
		strcat(str, SZ_TOKEN_AUTO_ATTACK);
		goto _on_off;
	case TOKEN_FLATTEN_BASE:
		strcat(str, SZ_TOKEN_FLATTEN_BASE);
		goto _on_off;
	case TOKEN_BUILD_OUTER_DEFENCES:
		strcat(str, SZ_TOKEN_BUILD_OUTER_DEFENCES);
		goto _on_off;
	case TOKEN_SET_AUTO_BUILD:
		strcat(str, SZ_TOKEN_SET_AUTO_BUILD);
		goto _on_off;
	case TOKEN_SET_AUTO_HOUSE:
		strcat(str, SZ_TOKEN_SET_AUTO_HOUSE);
		goto _on_off;
	case TOKEN_DONT_HOUSE_SPECIALISTS:
		strcat(str, SZ_TOKEN_DONT_HOUSE_SPECIALISTS);
		goto _on_off;
	case TOKEN_SET_REINCARNATION:
		strcat(str, SZ_TOKEN_SET_REINCARNATION);
		goto _on_off;
	case TOKEN_EXTRA_WOOD_COLLECTION:
		strcat(str, SZ_TOKEN_EXTRA_WOOD_COLLECTION);
		goto _on_off;
	case TOKEN_SET_BUCKET_USAGE:
		strcat(str, SZ_TOKEN_SET_BUCKET_USAGE);
		goto _on_off;
	case TOKEN_SET_SPECIAL_NO_BLDG_PANEL:
		strcat(str, SZ_TOKEN_SET_SPECIAL_NO_BLDG_PANEL);
		goto _on_off;
	case TOKEN_TURN_PUSH:
		strcat(str, SZ_TOKEN_TURN_PUSH);
		goto _on_off;
	case TOKEN_FLYBY_ALLOW_INTERRUPT:
		strcat(str, SZ_TOKEN_FLYBY_ALLOW_INTERRUPT);
		goto _on_off;
	case TOKEN_GIVE_UP_AND_SULK:
		strcat(str, SZ_TOKEN_GIVE_UP_AND_SULK);
		goto _on_off;
	case TOKEN_AUTO_MESSAGES:
		strcat(str, SZ_TOKEN_AUTO_MESSAGES);
		goto _on_off;

	case TOKEN_ATTACK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_ATTACK);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // ppl
					if(!Script_inc_si()) return RES_FATAL; // target
					if(!Script_inc_si()) return RES_FATAL; // model
					if(!Script_inc_si()) return RES_FATAL; // damage
					if(!Script_inc_si()) return RES_FATAL; // spell
					if(!Script_inc_si()) return RES_FATAL; // spell
					if(!Script_inc_si()) return RES_FATAL; // spell
					if(!Script_inc_si()) return RES_FATAL; // attack type
					if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
					if(!Script_inc_si()) return RES_FATAL; // marker
					if(!Script_inc_si()) return RES_FATAL; // marker
					if(!Script_inc_si()) return RES_FATAL; // marker
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// ppl
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // ppl
				if(!Script_inc_si()) return RES_FATAL; // target
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // damage
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// target
			switch(script_code)
			{
			case TOKEN_ATTACK_MARKER:   strcat(str, SZ_TOKEN_ATTACK_MARKER);   break;
			case TOKEN_ATTACK_BUILDING: strcat(str, SZ_TOKEN_ATTACK_BUILDING); break;
			case TOKEN_ATTACK_PERSON:   strcat(str, SZ_TOKEN_ATTACK_PERSON);   break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // target
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // damage
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_TARGET_TYPE_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// model
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // damage
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// damage
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // damage
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// spell 1
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// spell 2
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// spell 3
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// attack type
			switch(script_code)
			{
			case TOKEN_ATTACK_NORMAL:	  strcat(str, SZ_TOKEN_ATTACK_NORMAL);     break;
			case TOKEN_ATTACK_BY_BOAT:    strcat(str, SZ_TOKEN_ATTACK_BY_BOAT);    break;
			case TOKEN_ATTACK_BY_BALLOON: strcat(str, SZ_TOKEN_ATTACK_BY_BALLOON); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // attack type
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_ATTACK_TYPE_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// bring back vehicles
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // bring back vehicles
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker, attack via
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker, cast spell at
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker, direction to cast spell at
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SPELL_ATTACK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SPELL_ATTACK);

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // direction
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // direction
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// direction
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // direction
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_RESET_BASE_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_RESET_BASE_MARKER);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_BASE_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_BASE_MARKER);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_BASE_RADIUS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_BASE_RADIUS);

			// radius
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_COUNT_PEOPLE_IN_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_COUNT_PEOPLE_IN_MARKER);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:       strcat(str, SZ_TOKEN_BLUE);       break;
			case TOKEN_RED:        strcat(str, SZ_TOKEN_RED);        break;
			case TOKEN_YELLOW:     strcat(str, SZ_TOKEN_YELLOW);     break;
			case TOKEN_GREEN:      strcat(str, SZ_TOKEN_GREEN);      break;
			case TOKEN_COUNT_WILD: strcat(str, SZ_TOKEN_COUNT_WILD); break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // marker
					if(!Script_inc_si()) return RES_FATAL; // radius
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// radius
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_DRUM_TOWER_POS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_DRUM_TOWER_POS);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CONVERT_AT_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CONVERT_AT_MARKER);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_PREACH_AT_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_PREACH_AT_MARKER);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SEND_GHOST_PEOPLE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SEND_GHOST_PEOPLE);

			// num
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_SPELLS_CAST:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_SPELLS_CAST);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // spell
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_NUM_ONE_OFF_SPELLS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_NUM_ONE_OFF_SPELLS);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // spell
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_ATTACK_VARIABLE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_ATTACK_VARIABLE);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_BUILD_DRUM_TOWER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_BUILD_DRUM_TOWER);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GUARD_AT_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GUARD_AT_MARKER);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// num braves
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num braves
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num warriors
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num firewarriors
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num preachers
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// guard
			switch(script_code)
			{
			case TOKEN_GUARD_NORMAL:      strcat(str, SZ_TOKEN_GUARD_NORMAL);      break;
			case TOKEN_GUARD_WITH_GHOSTS: strcat(str, SZ_TOKEN_GUARD_WITH_GHOSTS); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_GUARD_TYPE_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GUARD_BETWEEN_MARKERS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GUARD_BETWEEN_MARKERS);

			// marker 1
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker 1
				if(!Script_inc_si()) return RES_FATAL; // marker 2
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// marker 2
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker 2
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num braves
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num braves
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num warriors
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num firewarriors
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num preachers
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// guard
			switch(script_code)
			{
			case TOKEN_GUARD_NORMAL:      strcat(str, SZ_TOKEN_GUARD_NORMAL);      break;
			case TOKEN_GUARD_WITH_GHOSTS: strcat(str, SZ_TOKEN_GUARD_WITH_GHOSTS); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // guard
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_GUARD_TYPE_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SPELL_DEFENCE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SPELL_DEFENCE);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // y
				if(!Script_inc_si()) return RES_FATAL; // on/off
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// y
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // y
				if(!Script_inc_si()) return RES_FATAL; // on/off
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// on/off
			switch(script_code)
			{
			case TOKEN_ON:  strcat(str, SZ_TOKEN_ON);  break;
			case TOKEN_OFF: strcat(str, SZ_TOKEN_OFF); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // on/off
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_ON_OFF_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_HEIGHT_AT_POS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_HEIGHT_AT_POS);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SEND_ALL_PEOPLE_TO_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SEND_ALL_PEOPLE_TO_MARKER);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_RESET_CONVERT_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_RESET_CONVERT_MARKER);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_CONVERT_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_CONVERT_MARKER);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_MARKER_ENTRY:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MARKER_ENTRY);

			// entry
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // entry
				if(!Script_inc_si()) return RES_FATAL; // marker1
				if(!Script_inc_si()) return RES_FATAL; // marker2
				if(!Script_inc_si()) return RES_FATAL; // num braves
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// marker1
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker1
				if(!Script_inc_si()) return RES_FATAL; // marker2
				if(!Script_inc_si()) return RES_FATAL; // num braves
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker2
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker2
				if(!Script_inc_si()) return RES_FATAL; // num braves
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num braves
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num braves
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num warriors
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num warriors
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num firewarriors
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num firewarriors
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// num preachers
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num preachers
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_MARKER_ENTRIES:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_MARKER_ENTRIES);

			//
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; //
				if(!Script_inc_si()) return RES_FATAL; //
				if(!Script_inc_si()) return RES_FATAL; //
				if(!Script_inc_si()) return RES_FATAL; //
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			//
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; //
				if(!Script_inc_si()) return RES_FATAL; //
				if(!Script_inc_si()) return RES_FATAL; //
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			//
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; //
				if(!Script_inc_si()) return RES_FATAL; //
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			//
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; //
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CLEAR_GUARDING_FROM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CLEAR_GUARDING_FROM);

			// entry1
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // entry1
				if(!Script_inc_si()) return RES_FATAL; // entry2
				if(!Script_inc_si()) return RES_FATAL; // entry3
				if(!Script_inc_si()) return RES_FATAL; // entry4
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// entry2
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // entry2
				if(!Script_inc_si()) return RES_FATAL; // entry3
				if(!Script_inc_si()) return RES_FATAL; // entry4
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// entry3
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // entry3
				if(!Script_inc_si()) return RES_FATAL; // entry4
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// entry4
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // entry4
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_BUILDING_DIRECTION:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_BUILDING_DIRECTION);

			// dir
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TRAIN_PEOPLE_NOW:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRAIN_PEOPLE_NOW);

			// num
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num
				if(!Script_inc_si()) return RES_FATAL; // model
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// model
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // model
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_PRAY_AT_HEAD:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_PRAY_AT_HEAD);

			// num ppl
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num ppl
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_PUT_PERSON_IN_DT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_PUT_PERSON_IN_DT);

			// person type
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // person type
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_I_HAVE_ONE_SHOT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_I_HAVE_ONE_SHOT);
			strcat(str, " ");

			switch(script_code)
			{
			case TOKEN_SPELL_TYPE:    strcat(str, SZ_TOKEN_SPELL_TYPE);    break;
			case TOKEN_BUILDING_TYPE: strcat(str, SZ_TOKEN_BUILDING_TYPE); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // spell/building
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_SPELL_BUILDING_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// model
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_BOAT_PATROL:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_BOAT_PATROL);

			// num ppl
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // num ppl
				if(!Script_inc_si()) return RES_FATAL; // marker1
				if(!Script_inc_si()) return RES_FATAL; // marker2
				if(!Script_inc_si()) return RES_FATAL; // marker3
				if(!Script_inc_si()) return RES_FATAL; // marker4
				if(!Script_inc_si()) return RES_FATAL; // vehicle type
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker1
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker1
				if(!Script_inc_si()) return RES_FATAL; // marker2
				if(!Script_inc_si()) return RES_FATAL; // marker3
				if(!Script_inc_si()) return RES_FATAL; // marker4
				if(!Script_inc_si()) return RES_FATAL; // vehicle type
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker2
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker2
				if(!Script_inc_si()) return RES_FATAL; // marker3
				if(!Script_inc_si()) return RES_FATAL; // marker4
				if(!Script_inc_si()) return RES_FATAL; // vehicle type
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker3
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker3
				if(!Script_inc_si()) return RES_FATAL; // marker4
				if(!Script_inc_si()) return RES_FATAL; // vehicle type
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// marker4
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker4
				if(!Script_inc_si()) return RES_FATAL; // vehicle type
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// vehicle type
			switch(script_code)
			{
			case TOKEN_BOAT_TYPE:    strcat(str, SZ_TOKEN_BOAT_TYPE);    break;
			case TOKEN_BALLOON_TYPE: strcat(str, SZ_TOKEN_BALLOON_TYPE); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // vehicle type
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_BOAT_BALLON_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_DEFEND_SHAMEN:
		{
			strcat(str, SZ_TOKEN_DEFEND_SHAMEN);
			if(!Script_inc_si()) return RES_FATAL;

			// num ppl
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SEND_SHAMEN_DEFENDERS_HOME:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SEND_SHAMEN_DEFENDERS_HOME);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_IS_BUILDING_NEAR:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_BUILDING_NEAR);

			// building model
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // building model
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // team
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // team
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // team
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // radius
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// radius
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_BUILD_AT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_BUILD_AT);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // building model
				if(!Script_inc_si()) return RES_FATAL; // settlement number
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // building model
				if(!Script_inc_si()) return RES_FATAL; // settlement number
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// building model
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // building model
				if(!Script_inc_si()) return RES_FATAL; // settlement number
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// settlement number
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // settlement number
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_SPELL_ENTRY:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_SPELL_ENTRY);

			// entry
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // entry
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // min mana
				if(!Script_inc_si()) return RES_FATAL; // frequency
				if(!Script_inc_si()) return RES_FATAL; // min ppl
				if(!Script_inc_si()) return RES_FATAL; // base spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // min mana
				if(!Script_inc_si()) return RES_FATAL; // frequency
				if(!Script_inc_si()) return RES_FATAL; // min ppl
				if(!Script_inc_si()) return RES_FATAL; // base spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// min mana
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // min mana
				if(!Script_inc_si()) return RES_FATAL; // frequency
				if(!Script_inc_si()) return RES_FATAL; // min ppl
				if(!Script_inc_si()) return RES_FATAL; // base spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// frequency
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // frequency
				if(!Script_inc_si()) return RES_FATAL; // min ppl
				if(!Script_inc_si()) return RES_FATAL; // base spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// min ppl
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // min ppl
				if(!Script_inc_si()) return RES_FATAL; // base spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// base spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // base spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_DELAY_MAIN_DRUM_TOWER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DELAY_MAIN_DRUM_TOWER);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_BUILD_MAIN_DRUM_TOWER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_BUILD_MAIN_DRUM_TOWER);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_ZOOM_TO:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_ZOOM_TO);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_DISABLE_USER_INPUTS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DISABLE_USER_INPUTS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_ENABLE_USER_INPUTS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_ENABLE_USER_INPUTS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_OPEN_DIALOG:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_OPEN_DIALOG);

			// index
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GIVE_ONE_SHOT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GIVE_ONE_SHOT);

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // team
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CLEAR_STANDING_PEOPLE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CLEAR_STANDING_PEOPLE);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_ONLY_STAND_AT_MARKERS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_ONLY_STAND_AT_MARKERS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_NAV_CHECK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_NAV_CHECK);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // target
					if(!Script_inc_si()) return RES_FATAL; // model
					if(!Script_inc_si()) return RES_FATAL; // remember
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");

			// type
			switch(script_code)
			{
			case TOKEN_ATTACK_MARKER:   strcat(str, SZ_TOKEN_ATTACK_MARKER);   break;
			case TOKEN_ATTACK_BUILDING: strcat(str, SZ_TOKEN_ATTACK_BUILDING); break;
			case TOKEN_ATTACK_PERSON:   strcat(str, SZ_TOKEN_ATTACK_PERSON);   break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // target
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // remember
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_TARGET_TYPE_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// model
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // model
				if(!Script_inc_si()) return RES_FATAL; // remember
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// remember
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // remember
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TARGET_S_WARRIORS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TARGET_S_WARRIORS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_DONT_TARGET_S_WARRIORS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DONT_TARGET_S_WARRIORS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_TARGET_BLUE_SHAMAN:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TARGET_BLUE_SHAMAN);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_DONT_TARGET_BLUE_SHAMAN:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DONT_TARGET_BLUE_SHAMAN);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_TARGET_BLUE_DRUM_TOWERS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TARGET_BLUE_DRUM_TOWERS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_DONT_TARGET_BLUE_DRUM_TOWERS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DONT_TARGET_BLUE_DRUM_TOWERS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_HAS_BLUE_KILLED_A_GHOST:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_HAS_BLUE_KILLED_A_GHOST);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_COUNT_GUARD_FIRES:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_COUNT_GUARD_FIRES);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// rad
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // rad
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_HEAD_TRIGGER_COUNT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_HEAD_TRIGGER_COUNT);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_MOVE_SHAMAN_TO_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_MOVE_SHAMAN_TO_MARKER);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TRACK_SHAMAN_TO_ANGLE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRACK_SHAMAN_TO_ANGLE);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TRACK_SHAMAN_EXTRA_BOLLOCKS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRACK_SHAMAN_EXTRA_BOLLOCKS);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_IS_SHAMAN_AVAILABLE_FOR_ATTACK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_SHAMAN_AVAILABLE_FOR_ATTACK);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_PARTIAL_BUILDING_COUNT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_PARTIAL_BUILDING_COUNT);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SEND_BLUE_PEOPLE_TO_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SEND_BLUE_PEOPLE_TO_MARKER);

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GIVE_MANA_TO_PLAYER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GIVE_MANA_TO_PLAYER);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // mana
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// mana
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // mana
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_IS_PLAYER_IN_WORLD_VIEW:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_PLAYER_IN_WORLD_VIEW);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_DESELECT_ALL_BLUE_PEOPLE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DESELECT_ALL_BLUE_PEOPLE);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_FLASH_BUTTON:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLASH_BUTTON);

			// id
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // id
				if(!Script_inc_si()) return RES_FATAL; // on/off
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);
			strcat(str, " ");

			// on/off
			switch(script_code)
			{
			case TOKEN_ON:  strcat(str, SZ_TOKEN_ON);  break;
			case TOKEN_OFF: strcat(str, SZ_TOKEN_OFF); break;
			default:
				if(!Script_inc_si()) return RES_FATAL; // on/off
				ScriptWriteToken(str);
				sprintf(str, SZ_SCRIPT_ERR_ON_OFF_EXPECTED, script_code);
				ScriptWriteSz(str);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TURN_PANEL_ON:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TURN_PANEL_ON);

			// id
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GIVE_PLAYER_SPELL:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GIVE_PLAYER_SPELL);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // spell
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_HAS_PLAYER_BEEN_IN_ENCYC:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_HAS_PLAYER_BEEN_IN_ENCYC);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_IS_BLUE_SHAMAN_SELECTED:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_BLUE_SHAMAN_SELECTED);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CLEAR_SHAMAN_LEFT_CLICK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CLEAR_SHAMAN_LEFT_CLICK);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_CLEAR_SHAMAN_RIGHT_CLICK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CLEAR_SHAMAN_LEFT_CLICK);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_IS_SHAMAN_ICON_LEFT_CLICKED:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_SHAMAN_ICON_LEFT_CLICKED);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_IS_SHAMAN_ICON_RIGHT_CLICKED:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_SHAMAN_ICON_RIGHT_CLICKED);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TRIGGER_THING:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRIGGER_THING);

			// idx
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TRACK_TO_MARKER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRACK_TO_MARKER);

			// idx
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CAMERA_ROTATION:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CAMERA_ROTATION);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_STOP_CAMERA_ROTATION:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_STOP_CAMERA_ROTATION);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_COUNT_BLUE_SHAPES:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_COUNT_BLUE_SHAPES);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_COUNT_BLUE_IN_HOUSES:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_COUNT_BLUE_IN_HOUSES);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_HAS_HOUSE_INFO_BEEN_SHOWN:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_HAS_HOUSE_INFO_BEEN_SHOWN);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CLEAR_HOUSE_INFO_FLAG:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CLEAR_HOUSE_INFO_FLAG);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_COUNT_BLUE_WITH_BUILD_COMMAND:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_COUNT_BLUE_WITH_BUILD_COMMAND);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TARGET_PLAYER_DT_AND_S:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TARGET_PLAYER_DT_AND_S);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_REMOVE_PLAYER_THING:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_REMOVE_PLAYER_THING);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // spell
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_WOOD_COLLECTION_RADII:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_WOOD_COLLECTION_RADII);

			// min
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // min
				if(!Script_inc_si()) return RES_FATAL; // max
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// max
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // max
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_NUM_PEOPLE_CONVERTED:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_NUM_PEOPLE_CONVERTED);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_NUM_PEOPLE_BEING_PREACHED:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_NUM_PEOPLE_BEING_PREACHED);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_TRIGGER_LEVEL_LOST:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRIGGER_LEVEL_LOST);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_TRIGGER_LEVEL_WON:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_TRIGGER_LEVEL_WON);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_REMOVE_HEAD_AT_POS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_REMOVE_HEAD_AT_POS);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_BUCKET_COUNT_FOR_SPELL:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_BUCKET_COUNT_FOR_SPELL);

			// spell
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // spell
				if(!Script_inc_si()) return RES_FATAL; // mult
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// mult
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // mult
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2, 60);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CREATE_MSG_NARRATIVE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CREATE_MSG_NARRATIVE);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CREATE_MSG_OBJECTIVE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CREATE_MSG_OBJECTIVE);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CREATE_MSG_INFORMATION:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CREATE_MSG_INFORMATION);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CREATE_MSG_INFORMATION_ZOOM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CREATE_MSG_INFORMATION_ZOOM);

			// idx
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // idx
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_MSG_ZOOM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_ZOOM);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_MSG_TIMEOUT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_TIMEOUT);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_MSG_DELETE_ON_OK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_DELETE_ON_OK);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_RETURN_ON_OK:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_RETURN_ON_OK);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_DELETE_ON_RMB_ZOOM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_DELETE_ON_RMB_ZOOM);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_OPEN_DLG_ON_RMB_ZOOM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_OPEN_DLG_ON_RMB_ZOOM);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_CREATE_RETURN_MSG_ON_RMB_ZOOM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_CREATE_RETURN_MSG_ON_RMB_ZOOM);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_OPEN_DLG_ON_RMB_DELETE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_OPEN_DLG_ON_RMB_DELETE);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_ZOOM_ON_LMB_OPEN_DLG:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_ZOOM_ON_LMB_OPEN_DLG);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_AUTO_OPEN_DLG:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_AUTO_OPEN_DLG);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_OK_SAVE_EXIT_DLG:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_OK_SAVE_EXIT_DLG);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_FIX_WILD_IN_AREA:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FIX_WILD_IN_AREA);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// rad
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CHECK_IF_PERSON_PREACHED_TO:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CHECK_IF_PERSON_PREACHED_TO);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				if(!Script_inc_si()) return RES_FATAL;
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_COUNT_ANGELS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_COUNT_ANGELS);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_NO_BLUE_REINC:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_NO_BLUE_REINC);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_IS_SHAMAN_IN_AREA:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_SHAMAN_IN_AREA);
			strcat(str, " ");

			// team
			switch(script_code)
			{
			case TOKEN_BLUE:   strcat(str, SZ_TOKEN_BLUE);   break;
			case TOKEN_RED:    strcat(str, SZ_TOKEN_RED);    break;
			case TOKEN_YELLOW: strcat(str, SZ_TOKEN_YELLOW); break;
			case TOKEN_GREEN:  strcat(str, SZ_TOKEN_GREEN);  break;
			default:
				if(!ScriptDecompile_GetField(str2))
				{
					if(!Script_inc_si()) return RES_FATAL; // team
					if(!Script_inc_si()) return RES_FATAL; // marker
					if(!Script_inc_si()) return RES_FATAL; // radius
					if(!Script_inc_si()) return RES_FATAL; // user var
					ScriptWriteToken(str);
					ScriptWriteSz(str2);
					script_fail = true;
					return RES_OK;
				}
				strcat(str, str2);
				break;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");

			// marker
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // marker
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// radius
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // radius
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // user var
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FORCE_TOOLTIP:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FORCE_TOOLTIP);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // head / building
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // head / building
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// head / building
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // head / building
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// duration
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_DEFENCE_RADIUS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_DEFENCE_RADIUS);

			// rad
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_MARVELLOUS_HOUSE_DEATH:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_MARVELLOUS_HOUSE_DEATH);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_CALL_TO_ARMS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CALL_TO_ARMS);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_DELETE_SMOKE_STUFF:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_DELETE_SMOKE_STUFF);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// rad
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_SET_TIMER_GOING:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_TIMER_GOING);

			// time
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_REMOVE_TIMER:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_REMOVE_TIMER);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_HAS_TIMER_REACHED_ZERO:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_HAS_TIMER_REACHED_ZERO);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_START_REINC_NOW:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_START_REINC_NOW);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_FLYBY_CREATE_NEW:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_CREATE_NEW);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_FLYBY_START:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_START);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_FLYBY_STOP:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_STOP);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_FLYBY_SET_EVENT_POS:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_EVENT_POS);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// duration
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FLYBY_SET_EVENT_ANGLE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_EVENT_ANGLE);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// duration
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FLYBY_SET_EVENT_ZOOM:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_EVENT_ZOOM);

			// zoom
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // zoom
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// duration
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FLYBY_SET_EVENT_INT_POINT:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_EVENT_INT_POINT);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// duration
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FLYBY_SET_EVENT_TOOLTIP:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_EVENT_TOOLTIP);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // code
				if(!Script_inc_si()) return RES_FATAL; // start
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // code
				if(!Script_inc_si()) return RES_FATAL; // start
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// code
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // code
				if(!Script_inc_si()) return RES_FATAL; // start
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// start
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // start
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// duration
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // duration
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FLYBY_SET_END_TARGET:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_END_TARGET);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // zoom
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // zoom
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// angle
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // angle
				if(!Script_inc_si()) return RES_FATAL; // zoom
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// zoom
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // zoom
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_FLYBY_SET_MESSAGE:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_FLYBY_SET_MESSAGE);

			// idx
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // idx
				if(!Script_inc_si()) return RES_FATAL; // start
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcata(str, str2);

			// start
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // start
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_KILL_TEAM_IN_AREA:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_KILL_TEAM_IN_AREA);

			// x
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // x
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// z
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // z
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			// rad
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL; // rad
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_CLEAR_ALL_MSG:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_CLEAR_ALL_MSG);
			ScriptWriteToken(str);
			return RES_OK;
		}

	case TOKEN_SET_MSG_ID:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_SET_MSG_ID);

			// id
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_GET_MSG_ID:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_GET_MSG_ID);

			// id
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_KILL_ALL_MSG_ID:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_KILL_ALL_MSG_ID);

			// id
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	case TOKEN_IS_PRISON_ON_LEVEL:
		{
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, SZ_TOKEN_IS_PRISON_ON_LEVEL);

			// user var
			if(!ScriptDecompile_GetField(str2))
			{
				if(!Script_inc_si()) return RES_FATAL;
				ScriptWriteToken(str);
				ScriptWriteSz(str2);
				script_fail = true;
				return RES_OK;
			}
			if(!Script_inc_si()) return RES_FATAL;
			strcat(str, " ");
			strcat(str, str2);

			ScriptWriteToken(str);

			return RES_OK;
		}

	default:
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_UNKNOW_DO, script_code);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}

_on_off:
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	if(script_code == TOKEN_ON)
	{
		strcata(str, SZ_TOKEN_ON);
	}
	else if(script_code == TOKEN_OFF)
	{
		strcata(str, SZ_TOKEN_OFF);
	}
	else
	{
		if(!Script_inc_si()) return RES_FATAL;
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_ON_OFF_EXPECTED, script_code);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;
	ScriptWriteToken(str);

	return RES_OK;
}


int ScriptDecompile_EVERY()
{
	if(script_code != TOKEN_EVERY)
	{
		sprintf(str, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND, SZ_TOKEN_EVERY, TOKEN_EVERY, script_code);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcpy(str, SZ_TOKEN_EVERY);

	// 1st value

	if(script_code >= 512 || script_code < 0)
	{
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_FIELD_OVERFLOW, script_code);
		ScriptWriteSz(str);
		return RES_ERROR;
	}

	FIELDINFO *fi = &script_si->Fields[script_code];

	if(fi->Type != CONSTANT_VAL)
	{
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_CONST_EXPECTED, fi->Type);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}

	if(!Script_inc_si()) return RES_FATAL;
	sprintf(str2, "%d", fi->Value+1);
	strcat(str, " ");
	strcat(str, str2);

	// 2nd value

	if(script_code != TOKEN_BEGIN)
	{
		if(script_code >= 512 || script_code < 0)
		{
			ScriptWriteToken(str);
			sprintf(str, SZ_SCRIPT_ERR_FIELD_OVERFLOW, script_code);
			ScriptWriteSz(str);
			return RES_ERROR;
		}

		fi = &script_si->Fields[script_code];

		if(fi->Type != CONSTANT_VAL)
		{
			ScriptWriteToken(str);
			sprintf(str, SZ_SCRIPT_ERR_CONST_EXPECTED, fi->Type);
			ScriptWriteSz(str);
			script_fail = true;
			return RES_ERROR;
		}

		if(!Script_inc_si()) return RES_FATAL;
		sprintf(str2, "%d", fi->Value+1);
		strcat(str, " ");
		strcat(str, str2);
	}

	ScriptWriteToken(str);

	return ScriptDecompile_BEGIN();
}


int ScriptDecompile_SET()
{
	bool isset = false;

	switch(script_code)
	{
	case TOKEN_SET:
		strcpy(str, SZ_TOKEN_SET);
		isset = true;
		break;
	case TOKEN_INCREMENT:
		strcpy(str, SZ_TOKEN_INCREMENT);
		break;
	case TOKEN_DECREMENT:
		strcpy(str, SZ_TOKEN_DECREMENT);
		break;
	default:
		sprintf(str, SZ_SCRIPT_ERR_SET_INC_DEC_EXPECTED, script_code);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;

	if(!ScriptDecompile_GetField(str2))
	{
		if(!Script_inc_si()) return RES_FATAL; // param 1
		if(!Script_inc_si()) return RES_FATAL; // param 2
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_OK;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	strcat(str, str2);

	if(!ScriptDecompile_GetField(str2))
	{
		if(!Script_inc_si()) return RES_FATAL; // param 2
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_OK;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");

	if(isset)
		strcata(str, str2);
	else
		strcat(str, str2);

	ScriptWriteToken(str);

	return RES_OK;
}


int ScriptDecompile_MULTIPLY_DEVIDE()
{
	switch(script_code)
	{
	case TOKEN_MULTIPLY:
		strcpy(str, SZ_TOKEN_MULTIPLY);
		break;
	case TOKEN_DIVIDE:
		strcpy(str, SZ_TOKEN_DIVIDE);
		break;
	default:
		sprintf(str, SZ_SCRIPT_ERR_MULTIPLY_DIVIDE_EXPECTED, script_code);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;

	if(!ScriptDecompile_GetField(str2))
	{
		if(!Script_inc_si()) return RES_FATAL;
		if(!Script_inc_si()) return RES_FATAL;
		if(!Script_inc_si()) return RES_FATAL;
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_OK;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	strcat(str, str2);

	if(!ScriptDecompile_GetField(str2))
	{
		if(!Script_inc_si()) return RES_FATAL;
		if(!Script_inc_si()) return RES_FATAL;
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_OK;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	strcat(str, str2);

	if(!ScriptDecompile_GetField(str2))
	{
		if(!Script_inc_si()) return RES_FATAL;
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_OK;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	strcat(str, str2);

	ScriptWriteToken(str);

	return RES_OK;
}


int ScriptDecompile_test()
{
	WORD o = script_code;
	if(!Script_inc_si()) return RES_FATAL;

	if(!ScriptDecompile_GetField(str2))
	{
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	strcat(str, str2);

	strcat(str, " ");
	switch(o)
	{
	case TOKEN_GREATER_THAN:
		strcat(str, SZ_TOKEN_GREATER_THAN);
		break;
	case TOKEN_LESS_THAN:
		strcat(str, SZ_TOKEN_LESS_THAN);
		break;
	case TOKEN_EQUAL_TO:
		strcat(str, SZ_TOKEN_EQUAL_TO);
		break;
	case TOKEN_NOT_EQUAL_TO:
		strcat(str, SZ_TOKEN_NOT_EQUAL_TO);
		break;
	case TOKEN_GREATER_THAN_EQUAL_TO:
		strcat(str, SZ_TOKEN_GREATER_THAN_EQUAL_TO);
		break;
	case TOKEN_LESS_THAN_EQUAL_TO:
		strcat(str, SZ_TOKEN_LESS_THAN_EQUAL_TO);
		break;
	default:
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_UNKNOW_OPERATOR, o);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}

	if(!ScriptDecompile_GetField(str2))
	{
		ScriptWriteToken(str);
		ScriptWriteSz(str2);
		script_fail = true;
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcat(str, " ");
	strcat(str, str2);

	return RES_OK;
}


int ScriptDecompile_logic()
{
	WORD o = script_code;
	if(!Script_inc_si()) return RES_FATAL;

	if(rs = ScriptDecompile_condition() != RES_OK) return rs;

	strcat(str, " ");
	if(o == TOKEN_AND)
		strcat(str, SZ_TOKEN_AND);
	else if(o == TOKEN_OR)
		strcat(str, SZ_TOKEN_OR);
	else
	{
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_AND_OR_EXPECTED, o);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}

	if(rs = ScriptDecompile_condition() != RES_OK) return rs;

	return RES_OK;
}


int ScriptDecompile_condition()
{
	switch(script_code)
	{
	case TOKEN_AND:
	case TOKEN_OR:
		if(rs = ScriptDecompile_logic() != RES_OK) return rs;
		break;
	case TOKEN_GREATER_THAN:
	case TOKEN_LESS_THAN:
	case TOKEN_EQUAL_TO:
	case TOKEN_NOT_EQUAL_TO:
	case TOKEN_GREATER_THAN_EQUAL_TO:
	case TOKEN_LESS_THAN_EQUAL_TO:
		if(rs = ScriptDecompile_test() != RES_OK) return rs;
		break;
	default:
		ScriptWriteToken(str);
		sprintf(str, SZ_SCRIPT_ERR_UNKNOW_OPERATOR, script_code);
		ScriptWriteSz(str);
		script_fail = true;
		return RES_ERROR;
	}
	return RES_OK;
}


int ScriptDecompile_IF()
{
	if(script_code != TOKEN_IF)
	{
		sprintf(str, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND, SZ_TOKEN_IF, TOKEN_IF, script_code);
		ScriptWriteSz(str);
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;
	strcpy(str, SZ_TOKEN_IF);
	strcat(str, " (");

	if(rs = ScriptDecompile_condition() != RES_OK) return rs;

	strcat(str, " )");
	ScriptWriteToken(str);

	if(ScriptDecompile_BEGIN() != RES_OK) return RES_FATAL;

	if(script_code == TOKEN_ELSE)
	{
		if(!Script_inc_si()) return RES_FATAL;
		ScriptWriteToken(SZ_TOKEN_ELSE);

		if(ScriptDecompile_BEGIN() != RES_OK) return RES_FATAL;
	}

	if(script_code != TOKEN_ENDIF)
	{
		sprintf(str, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND, SZ_TOKEN_ENDIF, TOKEN_ENDIF, script_code);
		ScriptWriteSz(str);
		return RES_ERROR;
	}
	if(!Script_inc_si()) return RES_FATAL;

	ScriptWriteToken(SZ_TOKEN_ENDIF);

	return RES_OK;
}

