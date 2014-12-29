
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "pop.h"
#include "popedt.h"
#include "script.h"



#define COMPILE_ADD_CMD(cmd)															\
{																						\
	script_si->Codes[script_p] = cmd;													\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_PARAM()															\
{																						\
	if(!ScriptSkipToParam())															\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);					\
		ScriptWriteSz(str);																\
		script_fail = true;																\
		ScriptSkipToReturn();															\
		return RES_OK;																	\
	}																					\
	ScriptWordRead(str);																\
}
	
#define COMPILE_CMD_END()																\
{																						\
	if(ScriptSkipToReturn())															\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str, SZ_SCRIPT_ERR_NO_MORE_PARAMS_EXPECTED, script_line, str2);			\
		ScriptWriteSz(str);																\
		script_fail = true;																\
	}																					\
	return RES_OK;																		\
}

#define COMPILE_READ_FIELD()															\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	rs = ScriptCompile_GetField(&script_si->Codes[script_p]);							\
	if(rs == RES_FATAL)																	\
	{																					\
		return RES_FATAL;																\
	}																					\
	else if(rs == RES_ERROR)															\
	{																					\
		ScriptSkipToReturn();															\
		script_fail = true;																\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_VAR_FIELD()														\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	rs = ScriptCompile_GetField(&script_si->Codes[script_p]);							\
	if(rs == RES_FATAL)																	\
	{																					\
		return RES_FATAL;																\
	}																					\
	else if(rs == RES_ERROR)															\
	{																					\
		ScriptSkipToReturn();															\
		script_fail = true;																\
		return RES_OK;																	\
	}																					\
	if(script_si->Fields[script_si->Codes[script_p]].Type != USER_VAL)					\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str3, SZ_SCRIPT_ERR_PARAM_NEED_BE_VAR, str, script_line, str2);			\
		ScriptWriteSz(str3);															\
		script_fail = true;																\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_TEAM()																\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_BLUE) == 0)												\
	{																					\
		script_si->Codes[script_p] = TOKEN_BLUE;										\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_RED) == 0)											\
	{																					\
		script_si->Codes[script_p] = TOKEN_RED;											\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_YELLOW) == 0)										\
	{																					\
		script_si->Codes[script_p] = TOKEN_YELLOW;										\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_GREEN) == 0)											\
	{																					\
		script_si->Codes[script_p] = TOKEN_GREEN;										\
	}																					\
	else																				\
	{																					\
		rs = ScriptCompile_GetField(&script_si->Codes[script_p]);						\
		if(rs == RES_FATAL)																\
		{																				\
			return RES_FATAL;															\
		}																				\
		else if(rs == RES_ERROR)														\
		{																				\
			ScriptSkipToReturn();														\
			script_fail = true;															\
			return RES_OK;																\
		}																				\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_TEAM_WILD()														\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_BLUE) == 0)												\
	{																					\
		script_si->Codes[script_p] = TOKEN_BLUE;										\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_RED) == 0)											\
	{																					\
		script_si->Codes[script_p] = TOKEN_RED;											\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_YELLOW) == 0)										\
	{																					\
		script_si->Codes[script_p] = TOKEN_YELLOW;										\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_GREEN) == 0)											\
	{																					\
		script_si->Codes[script_p] = TOKEN_GREEN;										\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_COUNT_WILD) == 0)									\
	{																					\
		script_si->Codes[script_p] = TOKEN_COUNT_WILD;									\
	}																					\
	else																				\
	{																					\
		rs = ScriptCompile_GetField(&script_si->Codes[script_p]);						\
		if(rs == RES_FATAL)																\
		{																				\
			return RES_FATAL;															\
		}																				\
		else if(rs == RES_ERROR)														\
		{																				\
			ScriptSkipToReturn();														\
			script_fail = true;															\
			return RES_OK;																\
		}																				\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_TARGET_TYPE()														\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_ATTACK_MARKER) == 0)										\
	{																					\
		script_si->Codes[script_p] = TOKEN_ATTACK_MARKER;								\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_ATTACK_BUILDING) == 0)								\
	{																					\
		script_si->Codes[script_p] = TOKEN_ATTACK_BUILDING;								\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_ATTACK_PERSON) == 0)									\
	{																					\
		script_si->Codes[script_p] = TOKEN_ATTACK_PERSON;								\
	}																					\
	else																				\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str3, SZ_SCRIPT_ERR_TARGET_TYPE_EXPECTED_LINE, str, script_line, str2);	\
		ScriptWriteSz(str3);															\
		script_fail = true;																\
		ScriptSkipToReturn();															\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_ATTACK_TYPE()														\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_ATTACK_NORMAL) == 0)										\
	{																					\
		script_si->Codes[script_p] = TOKEN_ATTACK_NORMAL;								\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_ATTACK_BY_BOAT) == 0)								\
	{																					\
		script_si->Codes[script_p] = TOKEN_ATTACK_BY_BOAT;								\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_ATTACK_BY_BALLOON) == 0)								\
	{																					\
		script_si->Codes[script_p] = TOKEN_ATTACK_BY_BALLOON;							\
	}																					\
	else																				\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str3, SZ_SCRIPT_ERR_ATTACK_TYPE_EXPECTED_LINE, str, script_line, str2);	\
		ScriptWriteSz(str3);															\
		script_fail = true;																\
		ScriptSkipToReturn();															\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_GUARD_TYPE()														\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_GUARD_NORMAL) == 0)										\
	{																					\
		script_si->Codes[script_p] = TOKEN_GUARD_NORMAL;								\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_GUARD_WITH_GHOSTS) == 0)								\
	{																					\
		script_si->Codes[script_p] = TOKEN_GUARD_WITH_GHOSTS;							\
	}																					\
	else																				\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str3, SZ_SCRIPT_ERR_GUARD_TYPE_EXPECTED_LINE, str, script_line, str2);	\
		ScriptWriteSz(str3);															\
		script_fail = true;																\
		ScriptSkipToReturn();															\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_ONE_SHOT_TYPE()													\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_SPELL_TYPE) == 0)											\
	{																					\
		script_si->Codes[script_p] = TOKEN_SPELL_TYPE;									\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_BUILDING_TYPE) == 0)									\
	{																					\
		script_si->Codes[script_p] = TOKEN_BUILDING_TYPE;								\
	}																					\
	else																				\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str3, SZ_SCRIPT_ERR_SPELL_BUILDING_EXPECTED_LINE, str, script_line, str2);	\
		ScriptWriteSz(str3);															\
		script_fail = true;																\
		ScriptSkipToReturn();															\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}

#define COMPILE_READ_VEHICLE_TYPE()														\
{																						\
	COMPILE_READ_PARAM();																\
																						\
	if(lstrcmpi(str, SZ_TOKEN_BOAT_TYPE) == 0)											\
	{																					\
		script_si->Codes[script_p] = TOKEN_BOAT_TYPE;									\
	}																					\
	else if(lstrcmpi(str, SZ_TOKEN_BALLOON_TYPE) == 0)									\
	{																					\
		script_si->Codes[script_p] = TOKEN_BALLOON_TYPE;								\
	}																					\
	else																				\
	{																					\
		ScriptLineRead(str2);															\
		sprintf(str3, SZ_SCRIPT_ERR_BOAT_BALLON_EXPECTED_LINE, str, script_line, str2);	\
		ScriptWriteSz(str3);															\
		script_fail = true;																\
		ScriptSkipToReturn();															\
		return RES_OK;																	\
	}																					\
	if(!Script_inc_si()) return RES_FATAL;												\
}



struct VAR_NAME
{
	char name[VAR_MAX_NAME_LENGTH+1];
};

VAR_NAME script_var_names[MAX_VARS];



int  ScriptCompile_GetField(WORD *field);
int  ScriptCompile_BEGIN();
int  ScriptCompile_DO();
int  ScriptCompile_EVERY();
int  ScriptCompile_SET(int n);
int  ScriptCompile_MULTIPLY_DIVIDE(bool mul);
int  ScriptCompile_IF();



bool ScriptCompile(SCRIPTINFO *scriptinfo, char *szf, HANDLE hlog)
{
	script_fail   = false;
	script_h      = hlog;
	script_p      = 1;
	script_p2     = 0;
	script_linep  = 0;
	script_line   = 1;
	script_begin  = 0;
	script_fields = 0;
	script_vars   = 0;
	script_si     = scriptinfo;

	memset(script_var_names, 0, sizeof(script_var_names));

	script_szf = szf;
	script_size = lstrlen(szf);

	memset(script_si, 0, sizeof(SCRIPTINFO));
	script_si->Codes[0] = SCRIPT_VERSION;

	if(ScriptCompile_BEGIN() != RES_OK) return false;

	ScriptSkipToCmd();
	ScriptWordRead(str);

	if(lstrcmpi(str, SZ_TOKEN_SCRIPT_END) != 0)
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_SCRIPTEND_EXPECTED, str, script_line, str2);
		ScriptWriteSz(str3);
		return false;
	}

	script_si->Codes[script_p] = TOKEN_SCRIPT_END;
	if(!Script_inc_si()) return false;

	return !script_fail;
}


int ScriptCompile_GetField(WORD *field)
{
	FIELDINFO fi;
	char *s;

	if(!str[0]) goto _field_error;

	// const
	{
		s = str;
		bool  positive = true;
		SLONG n = 0;

		if(*s == '-')
		{
			positive = false;
			s++;
		}
		else if(*s == '+')
		{
			positive = true;
			s++;
		}

		while(*s)
		{
			switch(*s)
			{
			case '0': n = n * 10 + 0; break;
			case '1': n = n * 10 + 1; break;
			case '2': n = n * 10 + 2; break;
			case '3': n = n * 10 + 3; break;
			case '4': n = n * 10 + 4; break;
			case '5': n = n * 10 + 5; break;
			case '6': n = n * 10 + 6; break;
			case '7': n = n * 10 + 7; break;
			case '8': n = n * 10 + 8; break;
			case '9': n = n * 10 + 9; break;
			default:
				goto _skip_const;
			}
			s++;
		}
		if(!positive) n *= -1;

		fi.Type = CONSTANT_VAL;
		fi.Value = n;

		goto _ok;
	}
_skip_const:

	// vars
	{
		s = str + VAR_PREFIX_LENGTH;
		char c = *s;
		*s = 0;

		if(lstrcmpi(str, VAR_PREFIX) != 0)
		{
			*s = c;
			goto _skip_var;
		}
		*s = c;

		int l = lstrlen(s);

		if(l == 0) goto _field_error;

		if(l > VAR_MAX_NAME_LENGTH)
		{
			ScriptLineRead(str2);
			sprintf(str, SZ_SCRIPT_ERR_VAR_NAME_LENGTH, script_line, str2);
			ScriptWriteSz(str);
			return RES_ERROR;
		}

		for(WORD i = 0; i < script_vars; i++)
		{
			if(lstrcmpi(s, script_var_names[i].name) == 0)
			{
				fi.Type  = USER_VAL;
				fi.Value = i;
				goto _ok;
			}
		}

		if(script_vars < MAX_VARS)
		{
			strcpy(script_var_names[script_vars].name, s);
			fi.Type  = USER_VAL;
			fi.Value = script_vars;
			script_vars++;
			goto _ok;
		}

		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MAX_VARS_REACHED, script_line, str2);
		ScriptWriteSz(str);
		return RES_FATAL;
	}
_skip_var:

	// internal

	{
		fi.Type = INTERNAL_VAL;

		if(lstrcmpi(str, SZ_INT_M_SPELL_BURN_COST) == 0)
		{
			fi.Value = INT_M_SPELL_BURN_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_BLAST_COST) == 0)
		{
			fi.Value = INT_M_SPELL_BLAST_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_LIGHTNING_BOLT_COST) == 0)
		{
			fi.Value = INT_M_SPELL_LIGHTNING_BOLT_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_WHIRLWIND_COST) == 0)
		{
			fi.Value = INT_M_SPELL_WHIRLWIND_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_INSECT_PLAGUE_COST) == 0)
		{
			fi.Value = INT_M_SPELL_INSECT_PLAGUE_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_INVISIBILITY_COST) == 0)
		{
			fi.Value = INT_M_SPELL_INVISIBILITY_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_HYPNOTISM_COST) == 0)
		{
			fi.Value = INT_M_SPELL_HYPNOTISM_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_FIRESTORM_COST) == 0)
		{
			fi.Value = INT_M_SPELL_FIRESTORM_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_GHOST_ARMY_COST) == 0)
		{
			fi.Value = INT_M_SPELL_GHOST_ARMY_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_EROSION_COST) == 0)
		{
			fi.Value = INT_M_SPELL_EROSION_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_SWAMP_COST) == 0)
		{
			fi.Value = INT_M_SPELL_SWAMP_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_LAND_BRIDGE_COST) == 0)
		{
			fi.Value = INT_M_SPELL_LAND_BRIDGE_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_ANGEL_OF_DEATH_COST) == 0)
		{
			fi.Value = INT_M_SPELL_ANGEL_OF_DEATH_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_EARTHQUAKE_COST) == 0)
		{
			fi.Value = INT_M_SPELL_EARTHQUAKE_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_FLATTEN_COST) == 0)
		{
			fi.Value = INT_M_SPELL_FLATTEN_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_VOLCANO_COST) == 0)
		{
			fi.Value = INT_M_SPELL_VOLCANO_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_WRATH_OF_GOD_COST) == 0)
		{
			fi.Value = INT_M_SPELL_WRATH_OF_GOD_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_SPELL_SHIELD_COST) == 0)
		{
			fi.Value = INT_M_SPELL_SHIELD_COST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_MY_MANA) == 0)
		{
			fi.Value = INT_MY_MANA;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_M_BUILDING_TEPEE) == 0)
		{
			fi.Value = INT_M_BUILDING_TEPEE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_HUT) == 0)
		{
			fi.Value = INT_M_BUILDING_HUT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_FARM) == 0)
		{
			fi.Value = INT_M_BUILDING_FARM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_DRUM_TOWER) == 0)
		{
			fi.Value = INT_M_BUILDING_DRUM_TOWER;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_TEMPLE) == 0)
		{
			fi.Value = INT_M_BUILDING_TEMPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_SPY_TRAIN) == 0)
		{
			fi.Value = INT_M_BUILDING_SPY_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_WARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_M_BUILDING_WARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_FIREWARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_M_BUILDING_FIREWARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_RECONVERSION) == 0)
		{
			fi.Value = INT_M_BUILDING_RECONVERSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_WALL_PIECE) == 0)
		{
			fi.Value = INT_M_BUILDING_WALL_PIECE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_GATE) == 0)
		{
			fi.Value = INT_M_BUILDING_GATE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_CURR_OE_SLOT) == 0)
		{
			fi.Value = INT_M_BUILDING_CURR_OE_SLOT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_BOAT_HUT_1) == 0)
		{
			fi.Value = INT_M_BUILDING_BOAT_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_BOAT_HUT_2) == 0)
		{
			fi.Value = INT_M_BUILDING_BOAT_HUT_2;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_AIRSHIP_HUT_1) == 0)
		{
			fi.Value = INT_M_BUILDING_AIRSHIP_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_BUILDING_AIRSHIP_HUT_2) == 0)
		{
			fi.Value = INT_M_BUILDING_AIRSHIP_HUT_2;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_B_BUILDING_TEPEE) == 0)
		{
			fi.Value = INT_B_BUILDING_TEPEE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_HUT) == 0)
		{
			fi.Value = INT_B_BUILDING_HUT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_FARM) == 0)
		{
			fi.Value = INT_B_BUILDING_FARM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_DRUM_TOWER) == 0)
		{
			fi.Value = INT_B_BUILDING_DRUM_TOWER;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_TEMPLE) == 0)
		{
			fi.Value = INT_B_BUILDING_TEMPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_SPY_TRAIN) == 0)
		{
			fi.Value = INT_B_BUILDING_SPY_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_WARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_B_BUILDING_WARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_FIREWARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_B_BUILDING_FIREWARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_RECONVERSION) == 0)
		{
			fi.Value = INT_B_BUILDING_RECONVERSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_WALL_PIECE) == 0)
		{
			fi.Value = INT_B_BUILDING_WALL_PIECE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_GATE) == 0)
		{
			fi.Value = INT_B_BUILDING_GATE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_CURR_OE_SLOT) == 0)
		{
			fi.Value = INT_B_BUILDING_CURR_OE_SLOT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_BOAT_HUT_1) == 0)
		{
			fi.Value = INT_B_BUILDING_BOAT_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_BOAT_HUT_2) == 0)
		{
			fi.Value = INT_B_BUILDING_BOAT_HUT_2;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_AIRSHIP_HUT_1) == 0)
		{
			fi.Value = INT_B_BUILDING_AIRSHIP_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_BUILDING_AIRSHIP_HUT_2) == 0)
		{
			fi.Value = INT_B_BUILDING_AIRSHIP_HUT_2;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_R_BUILDING_TEPEE) == 0)
		{
			fi.Value = INT_R_BUILDING_TEPEE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_HUT) == 0)
		{
			fi.Value = INT_R_BUILDING_HUT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_FARM) == 0)
		{
			fi.Value = INT_R_BUILDING_FARM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_DRUM_TOWER) == 0)
		{
			fi.Value = INT_R_BUILDING_DRUM_TOWER;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_TEMPLE) == 0)
		{
			fi.Value = INT_R_BUILDING_TEMPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_SPY_TRAIN) == 0)
		{
			fi.Value = INT_R_BUILDING_SPY_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_WARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_R_BUILDING_WARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_FIREWARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_R_BUILDING_FIREWARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_RECONVERSION) == 0)
		{
			fi.Value = INT_R_BUILDING_RECONVERSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_WALL_PIECE) == 0)
		{
			fi.Value = INT_R_BUILDING_WALL_PIECE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_GATE) == 0)
		{
			fi.Value = INT_R_BUILDING_GATE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_CURR_OE_SLOT) == 0)
		{
			fi.Value = INT_R_BUILDING_CURR_OE_SLOT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_BOAT_HUT_1) == 0)
		{
			fi.Value = INT_R_BUILDING_BOAT_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_BOAT_HUT_2) == 0)
		{
			fi.Value = INT_R_BUILDING_BOAT_HUT_2;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_AIRSHIP_HUT_1) == 0)
		{
			fi.Value = INT_R_BUILDING_AIRSHIP_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_BUILDING_AIRSHIP_HUT_2) == 0)
		{
			fi.Value = INT_R_BUILDING_AIRSHIP_HUT_2;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_TEPEE) == 0)
		{
			fi.Value = INT_Y_BUILDING_TEPEE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_HUT) == 0)
		{
			fi.Value = INT_Y_BUILDING_HUT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_FARM) == 0)
		{
			fi.Value = INT_Y_BUILDING_FARM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_DRUM_TOWER) == 0)
		{
			fi.Value = INT_Y_BUILDING_DRUM_TOWER;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_TEMPLE) == 0)
		{
			fi.Value = INT_Y_BUILDING_TEMPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_SPY_TRAIN) == 0)
		{
			fi.Value = INT_Y_BUILDING_SPY_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_WARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_Y_BUILDING_WARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_FIREWARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_Y_BUILDING_FIREWARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_RECONVERSION) == 0)
		{
			fi.Value = INT_Y_BUILDING_RECONVERSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_WALL_PIECE) == 0)
		{
			fi.Value = INT_Y_BUILDING_WALL_PIECE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_GATE) == 0)
		{
			fi.Value = INT_Y_BUILDING_GATE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_CURR_OE_SLOT) == 0)
		{
			fi.Value = INT_Y_BUILDING_CURR_OE_SLOT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_BOAT_HUT_1) == 0)
		{
			fi.Value = INT_Y_BUILDING_BOAT_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_BOAT_HUT_2) == 0)
		{
			fi.Value = INT_Y_BUILDING_BOAT_HUT_2;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_AIRSHIP_HUT_1) == 0)
		{
			fi.Value = INT_Y_BUILDING_AIRSHIP_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_BUILDING_AIRSHIP_HUT_2) == 0)
		{
			fi.Value = INT_Y_BUILDING_AIRSHIP_HUT_2;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_G_BUILDING_TEPEE) == 0)
		{
			fi.Value = INT_G_BUILDING_TEPEE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_HUT) == 0)
		{
			fi.Value = INT_G_BUILDING_HUT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_FARM) == 0)
		{
			fi.Value = INT_G_BUILDING_FARM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_DRUM_TOWER) == 0)
		{
			fi.Value = INT_G_BUILDING_DRUM_TOWER;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_TEMPLE) == 0)
		{
			fi.Value = INT_G_BUILDING_TEMPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_SPY_TRAIN) == 0)
		{
			fi.Value = INT_G_BUILDING_SPY_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_WARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_G_BUILDING_WARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_FIREWARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_G_BUILDING_FIREWARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_RECONVERSION) == 0)
		{
			fi.Value = INT_G_BUILDING_RECONVERSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_WALL_PIECE) == 0)
		{
			fi.Value = INT_G_BUILDING_WALL_PIECE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_GATE) == 0)
		{
			fi.Value = INT_G_BUILDING_GATE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_CURR_OE_SLOT) == 0)
		{
			fi.Value = INT_G_BUILDING_CURR_OE_SLOT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_BOAT_HUT_1) == 0)
		{
			fi.Value = INT_G_BUILDING_BOAT_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_BOAT_HUT_2) == 0)
		{
			fi.Value = INT_G_BUILDING_BOAT_HUT_2;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_AIRSHIP_HUT_1) == 0)
		{
			fi.Value = INT_G_BUILDING_AIRSHIP_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_BUILDING_AIRSHIP_HUT_2) == 0)
		{
			fi.Value = INT_G_BUILDING_AIRSHIP_HUT_2;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_M_PERSON_BRAVE) == 0)
		{
			fi.Value = INT_M_PERSON_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_PERSON_WARRIOR) == 0)
		{
			fi.Value = INT_M_PERSON_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_PERSON_RELIGIOUS) == 0)
		{
			fi.Value = INT_M_PERSON_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_PERSON_SPY) == 0)
		{
			fi.Value = INT_M_PERSON_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_PERSON_FIREWARRIOR) == 0)
		{
			fi.Value = INT_M_PERSON_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_PERSON_SHAMAN) == 0)
		{
			fi.Value = INT_M_PERSON_SHAMAN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_B_PERSON_BRAVE) == 0)
		{
			fi.Value = INT_B_PERSON_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_PERSON_WARRIOR) == 0)
		{
			fi.Value = INT_B_PERSON_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_PERSON_RELIGIOUS) == 0)
		{
			fi.Value = INT_B_PERSON_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_PERSON_SPY) == 0)
		{
			fi.Value = INT_B_PERSON_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_PERSON_FIREWARRIOR) == 0)
		{
			fi.Value = INT_B_PERSON_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_PERSON_SHAMAN) == 0)
		{
			fi.Value = INT_B_PERSON_SHAMAN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_R_PERSON_BRAVE) == 0)
		{
			fi.Value = INT_R_PERSON_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_PERSON_WARRIOR) == 0)
		{
			fi.Value = INT_R_PERSON_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_PERSON_RELIGIOUS) == 0)
		{
			fi.Value = INT_R_PERSON_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_PERSON_SPY) == 0)
		{
			fi.Value = INT_R_PERSON_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_PERSON_FIREWARRIOR) == 0)
		{
			fi.Value = INT_R_PERSON_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_PERSON_SHAMAN) == 0)
		{
			fi.Value = INT_R_PERSON_SHAMAN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_Y_PERSON_BRAVE) == 0)
		{
			fi.Value = INT_Y_PERSON_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_PERSON_WARRIOR) == 0)
		{
			fi.Value = INT_Y_PERSON_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_PERSON_RELIGIOUS) == 0)
		{
			fi.Value = INT_Y_PERSON_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_PERSON_SPY) == 0)
		{
			fi.Value = INT_Y_PERSON_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_PERSON_FIREWARRIOR) == 0)
		{
			fi.Value = INT_Y_PERSON_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_PERSON_SHAMAN) == 0)
		{
			fi.Value = INT_Y_PERSON_SHAMAN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_G_PERSON_BRAVE) == 0)
		{
			fi.Value = INT_G_PERSON_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_PERSON_WARRIOR) == 0)
		{
			fi.Value = INT_G_PERSON_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_PERSON_RELIGIOUS) == 0)
		{
			fi.Value = INT_G_PERSON_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_PERSON_SPY) == 0)
		{
			fi.Value = INT_G_PERSON_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_PERSON_FIREWARRIOR) == 0)
		{
			fi.Value = INT_G_PERSON_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_PERSON_SHAMAN) == 0)
		{
			fi.Value = INT_G_PERSON_SHAMAN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_M_VEHICLE_BOAT_1) == 0)
		{
			fi.Value = INT_M_VEHICLE_BOAT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_M_VEHICLE_AIRSHIP_1) == 0)
		{
			fi.Value = INT_M_VEHICLE_AIRSHIP_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_VEHICLE_BOAT_1) == 0)
		{
			fi.Value = INT_B_VEHICLE_BOAT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_B_VEHICLE_AIRSHIP_1) == 0)
		{
			fi.Value = INT_B_VEHICLE_AIRSHIP_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_VEHICLE_BOAT_1) == 0)
		{
			fi.Value = INT_R_VEHICLE_BOAT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_R_VEHICLE_AIRSHIP_1) == 0)
		{
			fi.Value = INT_R_VEHICLE_AIRSHIP_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_VEHICLE_BOAT_1) == 0)
		{
			fi.Value = INT_Y_VEHICLE_BOAT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_Y_VEHICLE_AIRSHIP_1) == 0)
		{
			fi.Value = INT_Y_VEHICLE_AIRSHIP_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_VEHICLE_BOAT_1) == 0)
		{
			fi.Value = INT_G_VEHICLE_BOAT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_G_VEHICLE_AIRSHIP_1) == 0)
		{
			fi.Value = INT_G_VEHICLE_AIRSHIP_1;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_BLUE_KILLED_BY_ME) == 0)
		{
			fi.Value = INT_BLUE_KILLED_BY_ME;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_RED_KILLED_BY_ME) == 0)
		{
			fi.Value = INT_RED_KILLED_BY_ME;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_YELLOW_KILLED_BY_ME) == 0)
		{
			fi.Value = INT_YELLOW_KILLED_BY_ME;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_GREEN_KILLED_BY_ME) == 0)
		{
			fi.Value = INT_GREEN_KILLED_BY_ME;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_MY_NUM_KILLED_BY_BLUE) == 0)
		{
			fi.Value = INT_MY_NUM_KILLED_BY_BLUE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_MY_NUM_KILLED_BY_RED) == 0)
		{
			fi.Value = INT_MY_NUM_KILLED_BY_RED;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_MY_NUM_KILLED_BY_YELLOW) == 0)
		{
			fi.Value = INT_MY_NUM_KILLED_BY_YELLOW;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_MY_NUM_KILLED_BY_GREEN) == 0)
		{
			fi.Value = INT_MY_NUM_KILLED_BY_GREEN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_ATTR_EXPANSION) == 0)
		{
			fi.Value = INT_ATTR_EXPANSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_SPY_TRAINS) == 0)
		{
			fi.Value = INT_ATTR_PREF_SPY_TRAINS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_RELIGIOUS_TRAINS) == 0)
		{
			fi.Value = INT_ATTR_PREF_RELIGIOUS_TRAINS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_WARRIOR_TRAINS) == 0)
		{
			fi.Value = INT_ATTR_PREF_WARRIOR_TRAINS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_FIREWARRIOR_TRAINS) == 0)
		{
			fi.Value = INT_ATTR_PREF_FIREWARRIOR_TRAINS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_SPY_PEOPLE) == 0)
		{
			fi.Value = INT_ATTR_PREF_SPY_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_RELIGIOUS_PEOPLE) == 0)
		{
			fi.Value = INT_ATTR_PREF_RELIGIOUS_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_WARRIOR_PEOPLE) == 0)
		{
			fi.Value = INT_ATTR_PREF_WARRIOR_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_FIREWARRIOR_PEOPLE) == 0)
		{
			fi.Value = INT_ATTR_PREF_FIREWARRIOR_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_MAX_BUILDINGS_ON_GO) == 0)
		{
			fi.Value = INT_ATTR_MAX_BUILDINGS_ON_GO;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_HOUSE_PERCENTAGE) == 0)
		{
			fi.Value = INT_ATTR_HOUSE_PERCENTAGE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_AWAY_BRAVE) == 0)
		{
			fi.Value = INT_ATTR_AWAY_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_AWAY_WARRIOR) == 0)
		{
			fi.Value = INT_ATTR_AWAY_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_AWAY_RELIGIOUS) == 0)
		{
			fi.Value = INT_ATTR_AWAY_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_DEFENSE_RAD_INCR) == 0)
		{
			fi.Value = INT_ATTR_DEFENSE_RAD_INCR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_MAX_DEFENSIVE_ACTIONS) == 0)
		{
			fi.Value = INT_ATTR_MAX_DEFENSIVE_ACTIONS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_AWAY_SPY) == 0)
		{
			fi.Value = INT_ATTR_AWAY_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_AWAY_FIREWARRIOR) == 0)
		{
			fi.Value = INT_ATTR_AWAY_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_ATTACK_PERCENTAGE) == 0)
		{
			fi.Value = INT_ATTR_ATTACK_PERCENTAGE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_AWAY_SHAMAN) == 0)
		{
			fi.Value = INT_ATTR_AWAY_SHAMAN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PEOPLE_PER_BOAT) == 0)
		{
			fi.Value = INT_ATTR_PEOPLE_PER_BOAT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PEOPLE_PER_BALLOON) == 0)
		{
			fi.Value = INT_ATTR_PEOPLE_PER_BALLOON;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_MAX_SPY_ATTACKS) == 0)
		{
			fi.Value = INT_ATTR_MAX_SPY_ATTACKS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_ENEMY_SPY_MAX_STAND) == 0)
		{
			fi.Value = INT_ATTR_ENEMY_SPY_MAX_STAND;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_MAX_ATTACKS) == 0)
		{
			fi.Value = INT_ATTR_MAX_ATTACKS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_SPY_CHECK_FREQUENCY) == 0)
		{
			fi.Value = INT_ATTR_SPY_CHECK_FREQUENCY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_RETREAT_VALUE) == 0)
		{
			fi.Value = INT_ATTR_RETREAT_VALUE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_SHAMEN_BLAST) == 0)
		{
			fi.Value = INT_ATTR_SHAMEN_BLAST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_MAX_TRAIN_AT_ONCE) == 0)
		{
			fi.Value = INT_ATTR_MAX_TRAIN_AT_ONCE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_GROUP_OPTION) == 0)
		{
			fi.Value = INT_ATTR_GROUP_OPTION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_BOAT_HUTS) == 0)
		{
			fi.Value = INT_ATTR_PREF_BOAT_HUTS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_BALLOON_HUTS) == 0)
		{
			fi.Value = INT_ATTR_PREF_BALLOON_HUTS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_BOAT_DRIVERS) == 0)
		{
			fi.Value = INT_ATTR_PREF_BOAT_DRIVERS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_PREF_BALLOON_DRIVERS) == 0)
		{
			fi.Value = INT_ATTR_PREF_BALLOON_DRIVERS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_FIGHT_STOP_DISTANCE) == 0)
		{
			fi.Value = INT_ATTR_FIGHT_STOP_DISTANCE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_SPY_DISCOVER_CHANCE) == 0)
		{
			fi.Value = INT_ATTR_SPY_DISCOVER_CHANCE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_COUNT_PREACH_DAMAGE) == 0)
		{
			fi.Value = INT_ATTR_COUNT_PREACH_DAMAGE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_USE_PREACHER_FOR_DEFENCE) == 0)
		{
			fi.Value = INT_ATTR_USE_PREACHER_FOR_DEFENCE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_RANDOM_BUILD_SIDE) == 0)
		{
			fi.Value = INT_ATTR_RANDOM_BUILD_SIDE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_BASE_UNDER_ATTACK_RETREAT) == 0)
		{
			fi.Value = INT_ATTR_BASE_UNDER_ATTACK_RETREAT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_EMPTY_AT_WAYPOINT) == 0)
		{
			fi.Value = INT_ATTR_EMPTY_AT_WAYPOINT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_DONT_USE_BOATS) == 0)
		{
			fi.Value = INT_ATTR_DONT_USE_BOATS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_DONT_GROUP_AT_DT) == 0)
		{
			fi.Value = INT_ATTR_DONT_GROUP_AT_DT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_SPELL_DELAY) == 0)
		{
			fi.Value = INT_ATTR_SPELL_DELAY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_DONT_DELETE_USELESS_BOAT_HOUSE) == 0)
		{
			fi.Value = INT_ATTR_DONT_DELETE_USELESS_BOAT_HOUSE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_BOAT_HOUSE_BROKEN) == 0)
		{
			fi.Value = INT_ATTR_BOAT_HOUSE_BROKEN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_DONT_AUTO_TRAIN_PREACHERS) == 0)
		{
			fi.Value = INT_ATTR_DONT_AUTO_TRAIN_PREACHERS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ATTR_SPARE_6) == 0)
		{
			fi.Value = INT_ATTR_SPARE_6;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_NO_SPECIFIC_SPELL) == 0)
		{
			fi.Value = INT_NO_SPECIFIC_SPELL;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BURN) == 0)
		{
			fi.Value = INT_BURN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BLAST) == 0)
		{
			fi.Value = INT_BLAST;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_LIGHTNING_BOLT) == 0)
		{
			fi.Value = INT_LIGHTNING_BOLT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_WHIRLWIND) == 0)
		{
			fi.Value = INT_WHIRLWIND;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_INSECT_PLAGUE) == 0)
		{
			fi.Value = INT_INSECT_PLAGUE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_INVISIBILITY) == 0)
		{
			fi.Value = INT_INVISIBILITY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_HYPNOTISM) == 0)
		{
			fi.Value = INT_HYPNOTISM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_FIRESTORM) == 0)
		{
			fi.Value = INT_FIRESTORM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_GHOST_ARMY) == 0)
		{
			fi.Value = INT_GHOST_ARMY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_EROSION) == 0)
		{
			fi.Value = INT_EROSION;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_SWAMP) == 0)
		{
			fi.Value = INT_SWAMP;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_LAND_BRIDGE) == 0)
		{
			fi.Value = INT_LAND_BRIDGE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_ANGEL_OF_DEATH) == 0)
		{
			fi.Value = INT_ANGEL_OF_DEATH;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_EARTHQUAKE) == 0)
		{
			fi.Value = INT_EARTHQUAKE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_FLATTEN) == 0)
		{
			fi.Value = INT_FLATTEN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_VOLCANO) == 0)
		{
			fi.Value = INT_VOLCANO;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_WRATH_OF_GOD) == 0)
		{
			fi.Value = INT_WRATH_OF_GOD;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_SHIELD) == 0)
		{
			fi.Value = INT_SHIELD;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_CONVERT) == 0)
		{
			fi.Value = INT_CONVERT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_TELEPORT) == 0)
		{
			fi.Value = INT_TELEPORT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BLOODLUST) == 0)
		{
			fi.Value = INT_BLOODLUST;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_NO_SPECIFIC_PERSON) == 0)
		{
			fi.Value = INT_NO_SPECIFIC_PERSON;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BRAVE) == 0)
		{
			fi.Value = INT_BRAVE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_WARRIOR) == 0)
		{
			fi.Value = INT_WARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_RELIGIOUS) == 0)
		{
			fi.Value = INT_RELIGIOUS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_SPY) == 0)
		{
			fi.Value = INT_SPY;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_FIREWARRIOR) == 0)
		{
			fi.Value = INT_FIREWARRIOR;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_SHAMAN) == 0)
		{
			fi.Value = INT_SHAMAN;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_NO_SPECIFIC_BUILDING) == 0)
		{
			fi.Value = INT_NO_SPECIFIC_BUILDING;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_TARGET_SHAMAN) == 0)
		{
			fi.Value = INT_TARGET_SHAMAN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_TEPEE) == 0)
		{
			fi.Value = INT_TEPEE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_HUT) == 0)
		{
			fi.Value = INT_HUT;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_FARM) == 0)
		{
			fi.Value = INT_FARM;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_DRUM_TOWER) == 0)
		{
			fi.Value = INT_DRUM_TOWER;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_TEMPLE) == 0)
		{
			fi.Value = INT_TEMPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_SPY_TRAIN) == 0)
		{
			fi.Value = INT_SPY_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_WARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_WARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_FIREWARRIOR_TRAIN) == 0)
		{
			fi.Value = INT_FIREWARRIOR_TRAIN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_WALL_PIECE) == 0)
		{
			fi.Value = INT_WALL_PIECE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_GATE) == 0)
		{
			fi.Value = INT_GATE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BOAT_HUT_1) == 0)
		{
			fi.Value = INT_BOAT_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BOAT_HUT_2) == 0)
		{
			fi.Value = INT_BOAT_HUT_2;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_AIRSHIP_HUT_1) == 0)
		{
			fi.Value = INT_AIRSHIP_HUT_1;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_AIRSHIP_HUT_2) == 0)
		{
			fi.Value = INT_AIRSHIP_HUT_2;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_CP_FREE_ENTRIES) == 0)
		{
			fi.Value = INT_CP_FREE_ENTRIES;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_RANDOM_100) == 0)
		{
			fi.Value = INT_RANDOM_100;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_NUM_SHAMEN_DEFENDERS) == 0)
		{
			fi.Value = INT_NUM_SHAMEN_DEFENDERS;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_CAMERA_ANGLE) == 0)
		{
			fi.Value = INT_CAMERA_ANGLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_CAMERA_X) == 0)
		{
			fi.Value = INT_CAMERA_X;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_CAMERA_Z) == 0)
		{
			fi.Value = INT_CAMERA_Z;
			goto _ok;
		}
		//
		if(lstrcmpi(str, SZ_INT_GAME_TURN) == 0)
		{
			fi.Value = INT_GAME_TURN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_MY_NUM_PEOPLE) == 0)
		{
			fi.Value = INT_MY_NUM_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BLUE_PEOPLE) == 0)
		{
			fi.Value = INT_BLUE_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_RED_PEOPLE) == 0)
		{
			fi.Value = INT_RED_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_YELLOW_PEOPLE) == 0)
		{
			fi.Value = INT_YELLOW_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_GREEN_PEOPLE) == 0)
		{
			fi.Value = INT_GREEN_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_MY_NUM_KILLED_BY_HUMAN) == 0)
		{
			fi.Value = INT_MY_NUM_KILLED_BY_HUMAN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_RED_KILLED_BY_HUMAN) == 0)
		{
			fi.Value = INT_RED_KILLED_BY_HUMAN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_YELLOW_KILLED_BY_HUMAN) == 0)
		{
			fi.Value = INT_YELLOW_KILLED_BY_HUMAN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_GREEN_KILLED_BY_HUMAN) == 0)
		{
			fi.Value = INT_GREEN_KILLED_BY_HUMAN;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_WILD_PEOPLE) == 0)
		{
			fi.Value = INT_WILD_PEOPLE;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_BLUE_MANA) == 0)
		{
			fi.Value = INT_BLUE_MANA;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_RED_MANA) == 0)
		{
			fi.Value = INT_RED_MANA;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_YELLOW_MANA) == 0)
		{
			fi.Value = INT_YELLOW_MANA;
			goto _ok;
		}
		if(lstrcmpi(str, SZ_INT_GREEN_MANA) == 0)
		{
			fi.Value = INT_GREEN_MANA;
			goto _ok;
		}
	}

	//

_field_error:
	ScriptLineRead(str2);
	sprintf(str3, SZ_SCRIPT_ERR_FIELD, str, script_line, str2);
	ScriptWriteSz(str3);

	return RES_ERROR;

_ok:
	for(WORD i = 0; i < script_fields; i++)
	{
		if(script_si->Fields[i].Value == fi.Value && script_si->Fields[i].Type == fi.Type)
		{
			*field = i;
			return RES_OK;
		}
	}

	if(script_fields < MAX_FIELDS)
	{
		script_si->Fields[script_fields].Value = fi.Value;
		script_si->Fields[script_fields].Type  = fi.Type;
		*field = script_fields;
		script_fields++;
		return RES_OK;
	}

	ScriptLineRead(str2);
	sprintf(str, SZ_SCRIPT_ERR_MAX_FIELDS_REACHED, script_line, str2);
	ScriptWriteSz(str);
	return RES_FATAL;
}


int ScriptCompile_BEGIN()
{
	ScriptSkipToCmd();
	ScriptWordRead(str);

	if(lstrcmpi(str, SZ_TOKEN_BEGIN) != 0)
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND_LINE, SZ_TOKEN_BEGIN, str, script_line, str2);
		ScriptWriteSz(str3);
		script_fail = true;
		return RES_FATAL;
	}

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_PARAMS_EXPECTED, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	script_begin++;
	COMPILE_ADD_CMD(TOKEN_BEGIN);

	for(;;)
	{
		ScriptSkipToCmd();
		ScriptWordRead(str);

		if(lstrcmpi(str, SZ_TOKEN_END) == 0)
		{
			if(ScriptSkipToReturn())
			{
				ScriptLineRead(str2);
				sprintf(str, SZ_SCRIPT_ERR_NO_PARAMS_EXPECTED, script_line, str2);
				ScriptWriteSz(str);
				script_fail = true;
			}

			script_begin--;
			COMPILE_ADD_CMD(TOKEN_END);
			return RES_OK;
		}

		else if(lstrcmpi(str, SZ_TOKEN_DO) == 0)
		{
			if(ScriptCompile_DO() != RES_OK) return RES_FATAL;
		}
		else if(lstrcmpi(str, SZ_TOKEN_EVERY) == 0)
		{
			if(ScriptCompile_EVERY() != RES_OK) return RES_FATAL;
		}

		else if(lstrcmpi(str, SZ_TOKEN_INCREMENT) == 0)
		{
			if(ScriptCompile_SET(1) != RES_OK) return RES_FATAL;
		}
		else if(lstrcmpi(str, SZ_TOKEN_DECREMENT) == 0)
		{
			if(ScriptCompile_SET(2) != RES_OK) return RES_FATAL;
		}
		else if(lstrcmpi(str, SZ_TOKEN_SET) == 0)
		{
			if(ScriptCompile_SET(0) != RES_OK) return RES_FATAL;
		}

		else if(lstrcmpi(str, SZ_TOKEN_MULTIPLY) == 0)
		{
			if(ScriptCompile_MULTIPLY_DIVIDE(true) != RES_OK) return RES_FATAL;
		}
		else if(lstrcmpi(str, SZ_TOKEN_DIVIDE) == 0)
		{
			if(ScriptCompile_MULTIPLY_DIVIDE(false) != RES_OK) return RES_FATAL;
		}

		else if(lstrcmpi(str, SZ_TOKEN_IF) == 0)
		{
			if(ScriptCompile_IF() != RES_OK) return RES_FATAL;
		}

		else
		{
			ScriptLineRead(str2);
			sprintf(str3, SZ_SCRIPT_ERR_UNKNOW_COMMAND_LINE, str, script_line, str2);
			ScriptWriteSz(str3);
			return RES_FATAL;
		}
	}

	return RES_OK;
}


int ScriptCompile_DO()
{
	COMPILE_ADD_CMD(TOKEN_DO);

	COMPILE_READ_PARAM();

	if(lstrcmpi(str, SZ_TOKEN_CONSTRUCT_BUILDING) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CONSTRUCT_BUILDING);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FETCH_WOOD) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FETCH_WOOD);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SHAMAN_GET_WILDS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SHAMAN_GET_WILDS);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_HOUSE_A_PERSON) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_HOUSE_A_PERSON);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SEND_GHOSTS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SEND_GHOSTS);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_BRING_NEW_PEOPLE_BACK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BRING_NEW_PEOPLE_BACK);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_TRAIN_PEOPLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRAIN_PEOPLE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_POPULATE_DRUM_TOWER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_POPULATE_DRUM_TOWER);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_DEFEND) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DEFEND);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_DEFEND_BASE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DEFEND_BASE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_PREACH) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_PREACH);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_BUILD_WALLS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BUILD_WALLS);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SABOTAGE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SABOTAGE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SPELL_OFFENSIVE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SPELL_OFFENSIVE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FIREWARRIOR_DEFEND) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FIREWARRIOR_DEFEND);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_BUILD_VEHICLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BUILD_VEHICLE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FETCH_LOST_PEOPLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FETCH_LOST_PEOPLE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FETCH_LOST_VEHICLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FETCH_LOST_VEHICLE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FETCH_FAR_VEHICLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FETCH_FAR_VEHICLE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_AUTO_ATTACK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_AUTO_ATTACK);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FLATTEN_BASE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLATTEN_BASE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_BUILD_OUTER_DEFENCES) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BUILD_OUTER_DEFENCES);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_AUTO_BUILD) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_AUTO_BUILD);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_AUTO_HOUSE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_AUTO_HOUSE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_DONT_HOUSE_SPECIALISTS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DONT_HOUSE_SPECIALISTS);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_REINCARNATION) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_REINCARNATION);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_EXTRA_WOOD_COLLECTION) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_EXTRA_WOOD_COLLECTION);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_BUCKET_USAGE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_BUCKET_USAGE);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_SPECIAL_NO_BLDG_PANEL) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_SPECIAL_NO_BLDG_PANEL);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_TURN_PUSH) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TURN_PUSH);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_ALLOW_INTERRUPT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_ALLOW_INTERRUPT);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_GIVE_UP_AND_SULK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GIVE_UP_AND_SULK);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_AUTO_MESSAGES) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_AUTO_MESSAGES);
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_ATTACK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_ATTACK);
		// team
		COMPILE_READ_TEAM();
		// num ppl
		COMPILE_READ_FIELD();
		// target
		COMPILE_READ_TARGET_TYPE();
		// attack model
		COMPILE_READ_FIELD();
		// damage
		COMPILE_READ_FIELD();
		// spell 1
		COMPILE_READ_FIELD();
		// spell 2
		COMPILE_READ_FIELD();
		// spell 3
		COMPILE_READ_FIELD();
		// attack type
		COMPILE_READ_ATTACK_TYPE();
		// bring back vehicles
		COMPILE_READ_FIELD();
		// marker 1
		COMPILE_READ_FIELD();
		// marker 2
		COMPILE_READ_FIELD();
		// marker 3
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SPELL_ATTACK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SPELL_ATTACK);
		// spell
		COMPILE_READ_FIELD();
		// marker
		COMPILE_READ_FIELD();
		// direction
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_RESET_BASE_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_RESET_BASE_MARKER);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_BASE_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_BASE_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_BASE_RADIUS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_BASE_RADIUS);
		// radius
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_COUNT_PEOPLE_IN_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_COUNT_PEOPLE_IN_MARKER);
		// team
		COMPILE_READ_TEAM_WILD();
		// marker
		COMPILE_READ_FIELD();
		// radius
		COMPILE_READ_FIELD();
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_DRUM_TOWER_POS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_DRUM_TOWER_POS);
		// x
		COMPILE_READ_FIELD();
		// z
        COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CONVERT_AT_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CONVERT_AT_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_PREACH_AT_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_PREACH_AT_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SEND_GHOST_PEOPLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SEND_GHOST_PEOPLE);
		// num
		COMPILE_READ_FIELD();
        //
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_SPELLS_CAST) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_SPELLS_CAST);
		// team
		COMPILE_READ_TEAM();
		// spell
		COMPILE_READ_FIELD();
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_NUM_ONE_OFF_SPELLS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_NUM_ONE_OFF_SPELLS);
		// team
		COMPILE_READ_TEAM();
		// spell
		COMPILE_READ_FIELD();
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_ATTACK_VARIABLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_ATTACK_VARIABLE);
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_BUILD_DRUM_TOWER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BUILD_DRUM_TOWER);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GUARD_AT_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GUARD_AT_MARKER);
		// marker
		COMPILE_READ_FIELD();
		// num braves
		COMPILE_READ_FIELD();
		// num warriors
		COMPILE_READ_FIELD();
		// num firewarriors
		COMPILE_READ_FIELD();
		// num preachers
		COMPILE_READ_FIELD();
		// guard
		COMPILE_READ_GUARD_TYPE();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GUARD_BETWEEN_MARKERS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GUARD_BETWEEN_MARKERS);
		// marker
		COMPILE_READ_FIELD();
		// marker 2
		COMPILE_READ_FIELD();
		// num braves
		COMPILE_READ_FIELD();
		// num warriors
		COMPILE_READ_FIELD();
		// num firewarriors
		COMPILE_READ_FIELD();
		// num preachers
        COMPILE_READ_FIELD();
		// guard
        COMPILE_READ_GUARD_TYPE();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SPELL_DEFENCE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SPELL_DEFENCE);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// on/off
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_HEIGHT_AT_POS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_HEIGHT_AT_POS);
		// marker
		COMPILE_READ_FIELD();
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SEND_ALL_PEOPLE_TO_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SEND_ALL_PEOPLE_TO_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_RESET_CONVERT_MARKER) == 0) // no effect
	{
		COMPILE_ADD_CMD(TOKEN_RESET_CONVERT_MARKER);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_CONVERT_MARKER) == 0) // no effect
	{
		COMPILE_ADD_CMD(TOKEN_SET_CONVERT_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MARKER_ENTRY) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MARKER_ENTRY);
		// entry
		COMPILE_READ_FIELD();
		// marker1
		COMPILE_READ_FIELD();
		// marker2
		COMPILE_READ_FIELD();
		// num braves
		COMPILE_READ_FIELD();
		// num warriors
		COMPILE_READ_FIELD();
		// num firewarriors
		COMPILE_READ_FIELD();
		// num preachers
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_MARKER_ENTRIES) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_MARKER_ENTRIES);
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CLEAR_GUARDING_FROM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CLEAR_GUARDING_FROM);
		// entry1
		COMPILE_READ_FIELD();
		// entry2
		COMPILE_READ_FIELD();
		// entry3
		COMPILE_READ_FIELD();
		// entry4
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_BUILDING_DIRECTION) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_BUILDING_DIRECTION);
		// dir
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRAIN_PEOPLE_NOW) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRAIN_PEOPLE_NOW);
		// num
		COMPILE_READ_FIELD();
		// model
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_PRAY_AT_HEAD) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_PRAY_AT_HEAD);
		// num ppl
		COMPILE_READ_FIELD();
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_PUT_PERSON_IN_DT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_PUT_PERSON_IN_DT);
		// person type
		COMPILE_READ_FIELD();
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_I_HAVE_ONE_SHOT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_I_HAVE_ONE_SHOT);
		// type
		COMPILE_READ_ONE_SHOT_TYPE();
		// model
		COMPILE_READ_FIELD();
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_BOAT_PATROL) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BOAT_PATROL);
		// num ppl
		COMPILE_READ_FIELD();
		// marker1
		COMPILE_READ_FIELD();
		// marker2
		COMPILE_READ_FIELD();
		// marker3
		COMPILE_READ_FIELD();
		// marker4
		COMPILE_READ_FIELD();
		// vehicle type
		COMPILE_READ_VEHICLE_TYPE();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DEFEND_SHAMEN) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DEFEND_SHAMEN);
		// num ppl
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SEND_SHAMEN_DEFENDERS_HOME) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SEND_SHAMEN_DEFENDERS_HOME);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_BUILDING_NEAR) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_BUILDING_NEAR);
		// building model
		COMPILE_READ_FIELD();
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// team
		COMPILE_READ_TEAM();
		// radius
		COMPILE_READ_FIELD();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_BUILD_AT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BUILD_AT);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// building model
		COMPILE_READ_FIELD();
		// settlement number
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_SPELL_ENTRY) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_SPELL_ENTRY);
		// entry
		COMPILE_READ_FIELD();
		// spell
		COMPILE_READ_FIELD();
		// mini mana
		COMPILE_READ_FIELD();
		// frequency
		COMPILE_READ_FIELD();
		// min ppl
		COMPILE_READ_FIELD();
		// base spell
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DELAY_MAIN_DRUM_TOWER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DELAY_MAIN_DRUM_TOWER);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_BUILD_MAIN_DRUM_TOWER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_BUILD_MAIN_DRUM_TOWER);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_ZOOM_TO) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_ZOOM_TO);
		// x
		COMPILE_READ_FIELD();
		// y
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DISABLE_USER_INPUTS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DISABLE_USER_INPUTS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_ENABLE_USER_INPUTS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_ENABLE_USER_INPUTS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_OPEN_DIALOG) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_OPEN_DIALOG);
		// index
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GIVE_ONE_SHOT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GIVE_ONE_SHOT);
		// spell
		COMPILE_READ_FIELD();
		// team
		COMPILE_READ_TEAM();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CLEAR_STANDING_PEOPLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CLEAR_STANDING_PEOPLE);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_ONLY_STAND_AT_MARKERS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_ONLY_STAND_AT_MARKERS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_NAV_CHECK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_NAV_CHECK);
		// team
		COMPILE_READ_TEAM();
		// type
		COMPILE_READ_TARGET_TYPE();
		// model
		COMPILE_READ_FIELD();
		// remember
		COMPILE_READ_FIELD();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TARGET_S_WARRIORS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TARGET_S_WARRIORS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DONT_TARGET_S_WARRIORS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DONT_TARGET_S_WARRIORS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TARGET_BLUE_SHAMAN) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TARGET_BLUE_SHAMAN);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DONT_TARGET_BLUE_SHAMAN) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DONT_TARGET_BLUE_SHAMAN);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TARGET_BLUE_DRUM_TOWERS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TARGET_BLUE_DRUM_TOWERS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DONT_TARGET_BLUE_DRUM_TOWERS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DONT_TARGET_BLUE_DRUM_TOWERS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_HAS_BLUE_KILLED_A_GHOST) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_HAS_BLUE_KILLED_A_GHOST);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_COUNT_GUARD_FIRES) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_COUNT_GUARD_FIRES);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// rad
		COMPILE_READ_FIELD();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_HEAD_TRIGGER_COUNT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_HEAD_TRIGGER_COUNT);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_MOVE_SHAMAN_TO_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_MOVE_SHAMAN_TO_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRACK_SHAMAN_TO_ANGLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRACK_SHAMAN_TO_ANGLE);
		// angle
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRACK_SHAMAN_EXTRA_BOLLOCKS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRACK_SHAMAN_EXTRA_BOLLOCKS);
		// angle
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_SHAMAN_AVAILABLE_FOR_ATTACK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_SHAMAN_AVAILABLE_FOR_ATTACK);
		// var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_PARTIAL_BUILDING_COUNT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_PARTIAL_BUILDING_COUNT);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SEND_BLUE_PEOPLE_TO_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SEND_BLUE_PEOPLE_TO_MARKER);
		// marker
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GIVE_MANA_TO_PLAYER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GIVE_MANA_TO_PLAYER);
		// team
		COMPILE_READ_TEAM();
		// mana
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_PLAYER_IN_WORLD_VIEW) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_PLAYER_IN_WORLD_VIEW);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DESELECT_ALL_BLUE_PEOPLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DESELECT_ALL_BLUE_PEOPLE);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLASH_BUTTON) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLASH_BUTTON);
		// id
		COMPILE_READ_FIELD();
		// on/off
		goto _on_off;
	}
	if(lstrcmpi(str, SZ_TOKEN_TURN_PANEL_ON) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TURN_PANEL_ON);
		// id
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GIVE_PLAYER_SPELL) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GIVE_PLAYER_SPELL);
		// team
		COMPILE_READ_TEAM();
		// spell
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_HAS_PLAYER_BEEN_IN_ENCYC) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_HAS_PLAYER_BEEN_IN_ENCYC);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_BLUE_SHAMAN_SELECTED) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_BLUE_SHAMAN_SELECTED);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CLEAR_SHAMAN_LEFT_CLICK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CLEAR_SHAMAN_LEFT_CLICK);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CLEAR_SHAMAN_RIGHT_CLICK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CLEAR_SHAMAN_RIGHT_CLICK);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_SHAMAN_ICON_LEFT_CLICKED) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_SHAMAN_ICON_LEFT_CLICKED);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_SHAMAN_ICON_RIGHT_CLICKED) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_SHAMAN_ICON_RIGHT_CLICKED);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
        COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRIGGER_THING) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRIGGER_THING);
		// idx
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRACK_TO_MARKER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRACK_TO_MARKER);
		// idx
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CAMERA_ROTATION) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CAMERA_ROTATION);
		// angle
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_STOP_CAMERA_ROTATION) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_STOP_CAMERA_ROTATION);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_COUNT_BLUE_SHAPES) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_COUNT_BLUE_SHAPES);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_COUNT_BLUE_IN_HOUSES) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_COUNT_BLUE_IN_HOUSES);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_HAS_HOUSE_INFO_BEEN_SHOWN) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_HAS_HOUSE_INFO_BEEN_SHOWN);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CLEAR_HOUSE_INFO_FLAG) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CLEAR_HOUSE_INFO_FLAG);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_COUNT_BLUE_WITH_BUILD_COMMAND) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_COUNT_BLUE_WITH_BUILD_COMMAND);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TARGET_PLAYER_DT_AND_S) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TARGET_PLAYER_DT_AND_S);
		// team
		COMPILE_READ_TEAM();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_REMOVE_PLAYER_THING) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_REMOVE_PLAYER_THING);
		// team
		COMPILE_READ_TEAM();
		// spell
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_WOOD_COLLECTION_RADII) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_WOOD_COLLECTION_RADII);
		// min
		COMPILE_READ_FIELD();
		// max
		COMPILE_READ_FIELD();
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_NUM_PEOPLE_CONVERTED) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_NUM_PEOPLE_CONVERTED);
		// team
		COMPILE_READ_TEAM();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
        COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_NUM_PEOPLE_BEING_PREACHED) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_NUM_PEOPLE_BEING_PREACHED);
		// team
		COMPILE_READ_TEAM();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRIGGER_LEVEL_LOST) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRIGGER_LEVEL_LOST);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_TRIGGER_LEVEL_WON) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_TRIGGER_LEVEL_WON);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_REMOVE_HEAD_AT_POS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_REMOVE_HEAD_AT_POS);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_BUCKET_COUNT_FOR_SPELL) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_BUCKET_COUNT_FOR_SPELL);
		// spell
		COMPILE_READ_FIELD();
		// mult
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CREATE_MSG_NARRATIVE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CREATE_MSG_NARRATIVE);
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CREATE_MSG_OBJECTIVE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CREATE_MSG_OBJECTIVE);
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CREATE_MSG_INFORMATION) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CREATE_MSG_INFORMATION);
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CREATE_MSG_INFORMATION_ZOOM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CREATE_MSG_INFORMATION_ZOOM);
		// idx
		COMPILE_READ_FIELD();
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_ZOOM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_ZOOM);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_TIMEOUT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_TIMEOUT);
		//
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_DELETE_ON_OK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_DELETE_ON_OK);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_RETURN_ON_OK) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_RETURN_ON_OK);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_DELETE_ON_RMB_ZOOM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_DELETE_ON_RMB_ZOOM);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_OPEN_DLG_ON_RMB_ZOOM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_OPEN_DLG_ON_RMB_ZOOM);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_CREATE_RETURN_MSG_ON_RMB_ZOOM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_CREATE_RETURN_MSG_ON_RMB_ZOOM);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_OPEN_DLG_ON_RMB_DELETE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_OPEN_DLG_ON_RMB_DELETE);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_ZOOM_ON_LMB_OPEN_DLG) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_ZOOM_ON_LMB_OPEN_DLG);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_AUTO_OPEN_DLG) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_AUTO_OPEN_DLG);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_OK_SAVE_EXIT_DLG) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_OK_SAVE_EXIT_DLG);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FIX_WILD_IN_AREA) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FIX_WILD_IN_AREA);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// rad
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CHECK_IF_PERSON_PREACHED_TO) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CHECK_IF_PERSON_PREACHED_TO);
		//
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_COUNT_ANGELS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_COUNT_ANGELS);
		// team
		COMPILE_READ_TEAM();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_NO_BLUE_REINC) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_NO_BLUE_REINC);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_SHAMAN_IN_AREA) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_SHAMAN_IN_AREA);
		// team
		COMPILE_READ_TEAM();
		// marker
		COMPILE_READ_FIELD();
		// radius
		COMPILE_READ_FIELD();
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FORCE_TOOLTIP) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FORCE_TOOLTIP);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// head / building
		COMPILE_READ_FIELD();
		// duration
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_DEFENCE_RADIUS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_DEFENCE_RADIUS);
		// rad
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_MARVELLOUS_HOUSE_DEATH) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_MARVELLOUS_HOUSE_DEATH);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CALL_TO_ARMS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CALL_TO_ARMS);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_DELETE_SMOKE_STUFF) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_DELETE_SMOKE_STUFF);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// rad
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_TIMER_GOING) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_TIMER_GOING);
		// time
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_REMOVE_TIMER) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_REMOVE_TIMER);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_HAS_TIMER_REACHED_ZERO) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_HAS_TIMER_REACHED_ZERO);
		// user var
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_START_REINC_NOW) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_START_REINC_NOW);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_CREATE_NEW) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_CREATE_NEW);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_START) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_START);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_STOP) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_STOP);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_EVENT_POS) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_EVENT_POS);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		// duration
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_EVENT_ANGLE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_EVENT_ANGLE);
		// angle
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		// duration
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_EVENT_ZOOM) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_EVENT_ZOOM);
		// zoom
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		// duration
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_EVENT_INT_POINT) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_EVENT_INT_POINT);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		// duration
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_EVENT_TOOLTIP) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_EVENT_TOOLTIP);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// code
		COMPILE_READ_FIELD();
		// start
		COMPILE_READ_FIELD();
		// duration
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_END_TARGET) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_END_TARGET);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// angle
		COMPILE_READ_FIELD();
		// zoom
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_FLYBY_SET_MESSAGE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_FLYBY_SET_MESSAGE);
		// idx
		COMPILE_READ_FIELD();
		// start
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_KILL_TEAM_IN_AREA) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_KILL_TEAM_IN_AREA);
		// x
		COMPILE_READ_FIELD();
		// z
		COMPILE_READ_FIELD();
		// rad
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_CLEAR_ALL_MSG) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_CLEAR_ALL_MSG);
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_SET_MSG_ID) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_SET_MSG_ID);
		// id
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_GET_MSG_ID) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_GET_MSG_ID);
		// id
		COMPILE_READ_VAR_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_KILL_ALL_MSG_ID) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_KILL_ALL_MSG_ID);
		// id
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}
	if(lstrcmpi(str, SZ_TOKEN_IS_PRISON_ON_LEVEL) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_IS_PRISON_ON_LEVEL);
		// var
		COMPILE_READ_FIELD();
		//
		COMPILE_CMD_END();
	}

	script_fail = true;

	ScriptLineRead(str2);
	sprintf(str3, SZ_SCRIPT_ERR_UNKNOW_COMMAND_LINE, str, script_line, str2);
	ScriptWriteSz(str3);

	ScriptSkipToReturn();

	return RES_OK;

_on_off:
	COMPILE_READ_PARAM();

	if(lstrcmpi(str, SZ_TOKEN_ON) == 0)
	{
		script_si->Codes[script_p] = TOKEN_ON;
	}
	else if(lstrcmpi(str, SZ_TOKEN_OFF) == 0)
	{
		script_si->Codes[script_p] = TOKEN_OFF;
	}
	else
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_ON_OFF_EXPECTED_LINE, str, script_line, str2);
		ScriptWriteSz(str3);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}
	if(!Script_inc_si()) return RES_FATAL;

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_MORE_PARAMS_EXPECTED, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	return RES_OK;
}


int ScriptCompile_EVERY()
{
	COMPILE_ADD_CMD(TOKEN_EVERY);

	WORD  f1, f2;
	char  *s;
	bool  positive;
	SLONG n;
	FIELDINFO fi;

	// 1st

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}
	ScriptWordRead(str);

	positive = true;
	s = str;
	n = 0;

	if(*s == '-')
	{
		positive = false;
		s++;
	}
	else if(*s == '+')
	{
		positive = true;
		s++;
	}

	while(*s)
	{
		switch(*s)
		{
		case '0': n = n * 10 + 0; break;
		case '1': n = n * 10 + 1; break;
		case '2': n = n * 10 + 2; break;
		case '3': n = n * 10 + 3; break;
		case '4': n = n * 10 + 4; break;
		case '5': n = n * 10 + 5; break;
		case '6': n = n * 10 + 6; break;
		case '7': n = n * 10 + 7; break;
		case '8': n = n * 10 + 8; break;
		case '9': n = n * 10 + 9; break;
		default:
			ScriptLineRead(str2);
			sprintf(str3, SZ_SCRIPT_ERR_PARAM_NEED_BE_CONST, str, script_line, str2);
			ScriptWriteSz(str3);
			ScriptSkipToReturn();
			script_fail = true;
			return RES_OK;
		}
		s++;
	}
	if(!positive) n *= -1;

	fi.Type = CONSTANT_VAL;
	fi.Value = n-1;

	for(WORD i = 0; i < script_fields; i++)
	{
		if(script_si->Fields[i].Value == fi.Value && script_si->Fields[i].Type == fi.Type)
		{
			f1 = i;
			goto _skip_1;
		}
	}

	if(script_fields >= MAX_FIELDS)
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MAX_FIELDS_REACHED, script_line, str2);
		ScriptWriteSz(str);
		return RES_FATAL;	
	}

	script_si->Fields[script_fields].Value = fi.Value;
	script_si->Fields[script_fields].Type  = fi.Type;
	f1 = script_fields;
	script_fields++;

_skip_1:

	//

	if((script_si->Fields[f1].Value+1 != 2) && (script_si->Fields[f1].Value+1 != 4) && (script_si->Fields[f1].Value+1 != 8) && (script_si->Fields[f1].Value+1 != 16) && (script_si->Fields[f1].Value+1 != 32) && (script_si->Fields[f1].Value+1 != 64) && (script_si->Fields[f1].Value+1 != 128) && (script_si->Fields[f1].Value+1 != 256) && (script_si->Fields[f1].Value+1 != 512) && (script_si->Fields[f1].Value+1 != 1024) && (script_si->Fields[f1].Value+1 != 2048) && (script_si->Fields[f1].Value+1 != 4096) && (script_si->Fields[f1].Value+1 != 8192))
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_PARAM_NEED_BE_POWER2, str, script_line, str2);
		ScriptWriteSz(str3);
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	// 2nd

	bool _2nd = false;

	if(ScriptSkipToParam())
	{
		ScriptWordRead(str);

		positive = true;
		s = str;
		n = 0;

		if(*s == '-')
		{
			positive = false;
			s++;
		}
		else if(*s == '+')
		{
			positive = true;
			s++;
		}

		while(*s)
		{
			switch(*s)
			{
			case '0': n = n * 10 + 0; break;
			case '1': n = n * 10 + 1; break;
			case '2': n = n * 10 + 2; break;
			case '3': n = n * 10 + 3; break;
			case '4': n = n * 10 + 4; break;
			case '5': n = n * 10 + 5; break;
			case '6': n = n * 10 + 6; break;
			case '7': n = n * 10 + 7; break;
			case '8': n = n * 10 + 8; break;
			case '9': n = n * 10 + 9; break;
			default:
				ScriptLineRead(str2);
				sprintf(str3, SZ_SCRIPT_ERR_PARAM_NEED_BE_CONST, str, script_line, str2);
				ScriptWriteSz(str3);
				ScriptSkipToReturn();
				script_fail = true;
				return RES_OK;
			}
			s++;
		}
		if(!positive) n *= -1;

		fi.Type = CONSTANT_VAL;
		fi.Value = n-1;

		for(WORD i = 0; i < script_fields; i++)
		{
			if(script_si->Fields[i].Value == fi.Value && script_si->Fields[i].Type == fi.Type)
			{
				f2 = i;
				goto _skip_2;
			}
		}

		if(script_fields >= MAX_FIELDS)
		{
			ScriptLineRead(str2);
			sprintf(str, SZ_SCRIPT_ERR_MAX_FIELDS_REACHED, script_line, str2);
			ScriptWriteSz(str);
			return RES_FATAL;	
		}

		script_si->Fields[script_fields].Value = fi.Value;
		script_si->Fields[script_fields].Type  = fi.Type;
		f2 = script_fields;
		script_fields++;

_skip_2:
		_2nd = true;
	}

	//

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_MORE_PARAMS_EXPECTED, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	COMPILE_ADD_CMD(f1);

	if(_2nd) COMPILE_ADD_CMD(f2);

	//

	if(ScriptCompile_BEGIN() != RES_OK) return RES_FATAL;

	//

	return RES_OK;
}


int ScriptCompile_SET(int n)
{
	if(n == 1)
		script_si->Codes[script_p] = TOKEN_INCREMENT;
	else if(n == 2)
		script_si->Codes[script_p] = TOKEN_DECREMENT;
	else
		script_si->Codes[script_p] = TOKEN_SET;

	if(!Script_inc_si()) return RES_FATAL;

	WORD f1, f2;

	// 1st

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f1);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	if(script_si->Fields[f1].Type == USER_VAL) goto _its_var;
	if(script_si->Fields[f1].Type == INTERNAL_VAL && script_si->Fields[f1].Value >= INT_ATTR_EXPANSION && script_si->Fields[f1].Value <= INT_ATTR_SPARE_6) goto _its_var;

	ScriptLineRead(str2);
	sprintf(str3, SZ_SCRIPT_ERR_PARAM_NEED_BE_VAR, str, script_line, str2);
	ScriptWriteSz(str3);
	ScriptSkipToReturn();
	script_fail = true;
	return RES_OK;

_its_var:

	// 2nd

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f2);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	//

	COMPILE_ADD_CMD(f1);
	COMPILE_ADD_CMD(f2);

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_MORE_PARAMS_EXPECTED, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	return RES_OK;
}


int ScriptCompile_MULTIPLY_DIVIDE(bool mul)
{
	script_si->Codes[script_p] = mul ? TOKEN_MULTIPLY : TOKEN_DIVIDE;
	if(!Script_inc_si()) return RES_FATAL;

	WORD f1, f2, f3;

	// 1st

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f1);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	if(script_si->Fields[f1].Type == USER_VAL) goto _its_var;
	if(script_si->Fields[f1].Type == INTERNAL_VAL && script_si->Fields[f1].Value >= INT_ATTR_EXPANSION && script_si->Fields[f1].Value <= INT_ATTR_SPARE_6) goto _its_var;

	ScriptLineRead(str2);
	sprintf(str3, SZ_SCRIPT_ERR_PARAM_NEED_BE_VAR, str, script_line, str2);
	ScriptWriteSz(str3);
	ScriptSkipToReturn();
	script_fail = true;
	return RES_OK;

_its_var:

	// 2nd

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f2);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	// 3th

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f3);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	//

	COMPILE_ADD_CMD(f1);
	COMPILE_ADD_CMD(f2);
	COMPILE_ADD_CMD(f3);

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_MORE_PARAMS_EXPECTED, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	return RES_OK;
}


int ScriptCompile_IF()
{
	COMPILE_ADD_CMD(TOKEN_IF);

	//

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}
	ScriptWordRead(str);

	if(lstrcmpi("(", str) != 0)
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND_LINE, "(", str, script_line, str2);
		ScriptWriteSz(str3);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	//

	WORD f1, f2, op;

_condition:

	// field 1

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f1);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	// operator

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}
	ScriptWordRead(str);

	if(lstrcmpi(str, SZ_TOKEN_GREATER_THAN) == 0)               op = TOKEN_GREATER_THAN;
	else if(lstrcmpi(str, SZ_TOKEN_LESS_THAN) == 0)             op = TOKEN_LESS_THAN;
	else if(lstrcmpi(str, SZ_TOKEN_EQUAL_TO) == 0)              op = TOKEN_EQUAL_TO;
	else if(lstrcmpi(str, SZ_TOKEN_NOT_EQUAL_TO) == 0)          op = TOKEN_NOT_EQUAL_TO;
	else if(lstrcmpi(str, SZ_TOKEN_GREATER_THAN_EQUAL_TO) == 0) op = TOKEN_GREATER_THAN_EQUAL_TO;
	else if(lstrcmpi(str, SZ_TOKEN_LESS_THAN_EQUAL_TO) == 0)    op = TOKEN_LESS_THAN_EQUAL_TO;
	else
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_OPER_EXPECTED, str, script_line, str2);
		ScriptWriteSz(str3);
		ScriptSkipToReturn();
		return RES_OK;
	}

	// field 2

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	ScriptWordRead(str);
	rs = ScriptCompile_GetField(&f2);
	if(rs == RES_FATAL)
	{
		script_fail = true;
		return RES_FATAL;
	}
	else if(rs == RES_ERROR)
	{
		ScriptSkipToReturn();
		script_fail = true;
		return RES_OK;
	}

	// logic or end

	if(!ScriptSkipToParam())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_MISSING_PARAMS, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}
	ScriptWordRead(str);

	if(lstrcmpi(str, SZ_TOKEN_AND) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_AND);

		// condition
		COMPILE_ADD_CMD(op);
		COMPILE_ADD_CMD(f1);
		COMPILE_ADD_CMD(f2);

		goto _condition;
	}
	else if(lstrcmpi(str, SZ_TOKEN_OR) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_OR);;

		// condition
		COMPILE_ADD_CMD(op);
		COMPILE_ADD_CMD(f1);
		COMPILE_ADD_CMD(f2);

		goto _condition;
	}
	else if(lstrcmpi(str, ")") != 0)
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_LOGIC_OPER_OR_END_EXEPECTED, str, script_line, str2);
		ScriptWriteSz(str3);
		script_fail = true;
		ScriptSkipToReturn();
		return RES_OK;
	}

	// condition
	COMPILE_ADD_CMD(op);
	COMPILE_ADD_CMD(f1);
	COMPILE_ADD_CMD(f2);

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_PARAMS_EXPECTED_AFTER_END, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	// begin

	if(ScriptCompile_BEGIN() != RES_OK) return RES_FATAL;

	//

	ScriptSkipToCmd();
	ScriptWordRead(str);

	// else

	if(lstrcmpi(str, SZ_TOKEN_ELSE) == 0)
	{
		COMPILE_ADD_CMD(TOKEN_ELSE);

		if(ScriptSkipToReturn())
		{
			ScriptLineRead(str2);
			sprintf(str, SZ_SCRIPT_ERR_NO_PARAMS_EXPECTED, script_line, str2);
			ScriptWriteSz(str);
			script_fail = true;
		}
		
		if(ScriptCompile_BEGIN() != RES_OK) return RES_FATAL;

		ScriptSkipToCmd();
		ScriptWordRead(str);
	}

	// endif

	if(lstrcmpi(str, SZ_TOKEN_ENDIF) != 0)
	{
		ScriptLineRead(str2);
		sprintf(str3, SZ_SCRIPT_ERR_EXPECTED_BUT_FOUND_LINE, SZ_TOKEN_ENDIF, str, script_line, str2);
		ScriptWriteSz(str3);
		script_fail = true;
		return RES_FATAL;
	}

	COMPILE_ADD_CMD(TOKEN_ENDIF);

	//

	if(ScriptSkipToReturn())
	{
		ScriptLineRead(str2);
		sprintf(str, SZ_SCRIPT_ERR_NO_PARAMS_EXPECTED, script_line, str2);
		ScriptWriteSz(str);
		script_fail = true;
	}

	//

	return RES_OK;
}
