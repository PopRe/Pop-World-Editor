/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/
#pragma once
#define MAX_THINGS							66535
#define MAX_V2_THINGS						2000
#define MAX_THINGS_LOAD						100
#define	MAX_MANA_VALUE						1000000

#define LEVEL_PREFIX						".dat"
#define HEADER_PREFIX						".hdr"
#define VERSION_PREFIX						".ver"

// -=-=- obj bank -=-=-
#define SZ_OBJ_BANK_0						"bank 0"
#define SZ_OBJ_BANK_2						"bank 2"
#define SZ_OBJ_BANK_3						"bank 3"
#define SZ_OBJ_BANK_4						"bank 4"
#define SZ_OBJ_BANK_5						"bank 5"
#define SZ_OBJ_BANK_6						"bank 6"
#define SZ_OBJ_BANK_7						"bank 7"
//
#define SZ_TX_OBJ_BANK_0					"type\\o00.bmp"
#define SZ_TX_OBJ_BANK_2					"type\\o02.bmp"
#define SZ_TX_OBJ_BANK_3					"type\\o03.bmp"
#define SZ_TX_OBJ_BANK_4					"type\\o04.bmp"
#define SZ_TX_OBJ_BANK_5					"type\\o05.bmp"
#define SZ_TX_OBJ_BANK_6					"type\\o06.bmp"
#define SZ_TX_OBJ_BANK_7					"type\\o07.bmp"
//
#define SZ_MAP_0							"map 0"
#define SZ_MAP_1							"map 1"
#define SZ_MAP_2							"map 2"
#define SZ_MAP_3							"map 3"
#define SZ_MAP_4							"map 4"
#define SZ_MAP_5							"map 5"
#define SZ_MAP_6							"map 6"
#define SZ_MAP_7							"map 7"
#define SZ_MAP_8							"map 8"
#define SZ_MAP_9							"map 9"
#define SZ_MAP_A							"map A"
#define SZ_MAP_B							"map B"
#define SZ_MAP_C							"map C"
#define SZ_MAP_D							"map D"
#define SZ_MAP_E							"map E"
#define SZ_MAP_F							"map F"
#define SZ_MAP_G							"map G"
#define SZ_MAP_H							"map H"
#define SZ_MAP_I							"map I"
#define SZ_MAP_J							"map J"
#define SZ_MAP_K							"map K"
#define SZ_MAP_L							"map L"
#define SZ_MAP_M							"map M"
#define SZ_MAP_N							"map N"
#define SZ_MAP_O							"map O"
#define SZ_MAP_P							"map P"
#define SZ_MAP_Q							"map Q"
#define SZ_MAP_R							"map R"
#define SZ_MAP_S							"map S"
#define SZ_MAP_T							"map T"
#define SZ_MAP_U							"map U"
#define SZ_MAP_V							"map V"
#define SZ_MAP_W							"map W"
#define SZ_MAP_X							"map X"
#define SZ_MAP_Y							"map Y"
#define SZ_MAP_Z							"map Z"
//
#define SZ_TX_MAP_0							"type\\l0.bmp"
#define SZ_TX_MAP_1							"type\\l1.bmp"
#define SZ_TX_MAP_2							"type\\l2.bmp"
#define SZ_TX_MAP_3							"type\\l3.bmp"
#define SZ_TX_MAP_4							"type\\l4.bmp"
#define SZ_TX_MAP_5							"type\\l5.bmp"
#define SZ_TX_MAP_6							"type\\l6.bmp"
#define SZ_TX_MAP_7							"type\\l7.bmp"
#define SZ_TX_MAP_8							"type\\l8.bmp"
#define SZ_TX_MAP_9							"type\\l9.bmp"
#define SZ_TX_MAP_A							"type\\lA.bmp"
#define SZ_TX_MAP_B							"type\\lB.bmp"
#define SZ_TX_MAP_C							"type\\lC.bmp"
#define SZ_TX_MAP_D							"type\\lD.bmp"
#define SZ_TX_MAP_E							"type\\lE.bmp"
#define SZ_TX_MAP_F							"type\\lF.bmp"
#define SZ_TX_MAP_G							"type\\lG.bmp"
#define SZ_TX_MAP_H							"type\\lH.bmp"
#define SZ_TX_MAP_I							"type\\lI.bmp"
#define SZ_TX_MAP_J							"type\\lJ.bmp"
#define SZ_TX_MAP_K							"type\\lK.bmp"
#define SZ_TX_MAP_L							"type\\lL.bmp"
#define SZ_TX_MAP_M							"type\\lM.bmp"
#define SZ_TX_MAP_N							"type\\lN.bmp"
#define SZ_TX_MAP_O							"type\\lO.bmp"
#define SZ_TX_MAP_P							"type\\lP.bmp"
#define SZ_TX_MAP_Q							"type\\lQ.bmp"
#define SZ_TX_MAP_R							"type\\lR.bmp"
#define SZ_TX_MAP_S							"type\\lS.bmp"
#define SZ_TX_MAP_T							"type\\lT.bmp"
#define SZ_TX_MAP_U							"type\\lU.bmp"
#define SZ_TX_MAP_V							"type\\lV.bmp"
#define SZ_TX_MAP_W							"type\\lW.bmp"
#define SZ_TX_MAP_X							"type\\lX.bmp"
#define SZ_TX_MAP_Y							"type\\lY.bmp"
#define SZ_TX_MAP_Z							"type\\lZ.bmp"


// -=-=- textures files -=-=-
#define SZ_TX_UNKNOW						"data\\unknow.bmp"
#define SZ_TX_TRIGGER						"data\\trigger.bmp"
#define SZ_TX_DISCOVERY						"data\\discovery.bmp"
#define SZ_TX_SPELL							"data\\spell.bmp"
#define SZ_TX_EFFECT						"data\\effect.bmp"
#define SZ_TX_BUILDING_ADD_ON				"data\\addon.bmp"
#define SZ_TX_WILD							"data\\wild.bmp"
#define SZ_TX_BRAVE_BLUE					"data\\brave_blue.bmp"
#define SZ_TX_BRAVE_RED						"data\\brave_red.bmp"
#define SZ_TX_BRAVE_YELLOW					"data\\brave_yellow.bmp"
#define SZ_TX_BRAVE_GREEN					"data\\brave_green.bmp"
#define SZ_TX_SPY_BLUE						"data\\spy_blue.bmp"
#define SZ_TX_SPY_RED						"data\\spy_red.bmp"
#define SZ_TX_SPY_YELLOW					"data\\spy_yellow.bmp"
#define SZ_TX_SPY_GREEN						"data\\spy_green.bmp"
#define SZ_TX_WARRIOR_BLUE					"data\\warrior_blue.bmp"
#define SZ_TX_WARRIOR_RED					"data\\warrior_red.bmp"
#define SZ_TX_WARRIOR_YELLOW				"data\\warrior_yellow.bmp"
#define SZ_TX_WARRIOR_GREEN					"data\\warrior_green.bmp"
#define SZ_TX_PRIEST_BLUE					"data\\priest_blue.bmp"
#define SZ_TX_PRIEST_RED					"data\\priest_red.bmp"
#define SZ_TX_PRIEST_YELLOW					"data\\priest_yellow.bmp"
#define SZ_TX_PRIEST_GREEN					"data\\priest_green.bmp"
#define SZ_TX_FW_BLUE						"data\\fw_blue.bmp"
#define SZ_TX_FW_RED						"data\\fw_red.bmp"
#define SZ_TX_FW_YELLOW						"data\\fw_yellow.bmp"
#define SZ_TX_FW_GREEN						"data\\fw_green.bmp"
#define SZ_TX_SHAMAN_BLUE					"data\\shaman_blue.bmp"
#define SZ_TX_SHAMAN_RED					"data\\shaman_red.bmp"
#define SZ_TX_SHAMAN_YELLOW					"data\\shaman_yellow.bmp"
#define SZ_TX_SHAMAN_GREEN					"data\\shaman_green.bmp"
#define SZ_TX_PLANT1						"data\\plant1.bmp"
#define SZ_TX_PLANT2						"data\\plant2.bmp"
#define SZ_TX_WOOD_PILE						"data\\wood.bmp"
#define SZ_TX_ROCK							"data\\rock.bmp"
#define SZ_TX_FIRE							"data\\fire.bmp"
#define SZ_TX_LIGHT							"data\\light.bmp"
#define SZ_TX_LANDBRIDGE					"data\\landbridge.bmp"
#define SZ_TX_MARKER						"data\\marker.bmp"

// -=-=- objects models files -=-=-
#define SZ_OBJ_AOD_BLUE						"data\\aod_blue.3ds"
#define SZ_OBJ_AOD_RED						"data\\aod_red.3ds"
#define SZ_OBJ_AOD_YELLOW					"data\\aod_yellow.3ds"
#define SZ_OBJ_AOD_GREEN					"data\\aod_green.3ds"
#define SZ_OBJ_BOAT							"data\\boat.3ds"
#define SZ_OBJ_BOAT_BLUE					"data\\boat_blue.3ds"
#define SZ_OBJ_BOAT_RED						"data\\boat_red.3ds"
#define SZ_OBJ_BOAT_YELLOW					"data\\boat_yellow.3ds"
#define SZ_OBJ_BOAT_GREEN					"data\\boat_green.3ds"
#define SZ_OBJ_BALLON						"data\\ballon.3ds"
#define SZ_OBJ_BALLON_BLUE					"data\\ballon_blue.3ds"
#define SZ_OBJ_BALLON_RED					"data\\ballon_red.3ds"
#define SZ_OBJ_BALLON_YELLOW				"data\\ballon_yellow.3ds"
#define SZ_OBJ_BALLON_GREEN					"data\\ballon_green.3ds"
#define SZ_OBJ_TREE01						"data\\tree01.3ds"
#define SZ_OBJ_TREE02						"data\\tree02.3ds"
#define SZ_OBJ_TREE03						"data\\tree03.3ds"
#define SZ_OBJ_TREE04						"data\\tree04.3ds"
#define SZ_OBJ_TREE05						"data\\tree05.3ds"
#define SZ_OBJ_TREE06						"data\\tree06.3ds"
#define SZ_OBJ_TREE07						"data\\tree07.3ds"
#define SZ_OBJ_TREE08						"data\\tree08.3ds"
#define SZ_OBJ_TREE09						"data\\tree09.3ds"
#define SZ_OBJ_TREE10						"data\\tree10.3ds"
#define SZ_OBJ_TREE11						"data\\tree11.3ds"
#define SZ_OBJ_TREE12						"data\\tree12.3ds"
#define SZ_OBJ_TREE13						"data\\tree13.3ds"
#define SZ_OBJ_TREE14						"data\\tree14.3ds"
#define SZ_OBJ_KNOWLEDGE					"data\\knowledge.3ds"
#define SZ_OBJ_STONE_HEAD					"data\\stone_head.3ds"
#define SZ_OBJ_HUT1_BLUE					"data\\hut1_blue.3ds"
#define SZ_OBJ_HUT2_BLUE					"data\\hut2_blue.3ds"
#define SZ_OBJ_HUT3_BLUE					"data\\hut3_blue.3ds"
#define SZ_OBJ_HUT1_RED						"data\\hut1_red.3ds"
#define SZ_OBJ_HUT2_RED						"data\\hut2_red.3ds"
#define SZ_OBJ_HUT3_RED						"data\\hut3_red.3ds"
#define SZ_OBJ_HUT1_YELLOW					"data\\hut1_yellow.3ds"
#define SZ_OBJ_HUT2_YELLOW					"data\\hut2_yellow.3ds"
#define SZ_OBJ_HUT3_YELLOW					"data\\hut3_yellow.3ds"
#define SZ_OBJ_HUT1_GREEN					"data\\hut1_green.3ds"
#define SZ_OBJ_HUT2_GREEN					"data\\hut2_green.3ds"
#define SZ_OBJ_HUT3_GREEN					"data\\hut3_green.3ds"
#define SZ_OBJ_TOWER_BLUE					"data\\tower_blue.3ds"
#define SZ_OBJ_TOWER_RED					"data\\tower_red.3ds"
#define SZ_OBJ_TOWER_YELLOW					"data\\tower_yellow.3ds"
#define SZ_OBJ_TOWER_GREEN					"data\\tower_green.3ds"
#define SZ_OBJ_SPY_TRAIN_BLUE				"data\\spy_train_blue.3ds"
#define SZ_OBJ_SPY_TRAIN_RED				"data\\spy_train_red.3ds"
#define SZ_OBJ_SPY_TRAIN_YELLOW				"data\\spy_train_yellow.3ds"
#define SZ_OBJ_SPY_TRAIN_GREEN				"data\\spy_train_green.3ds"
#define SZ_OBJ_WARRIOR_TRAIN_BLUE			"data\\warrior_train_blue.3ds"
#define SZ_OBJ_WARRIOR_TRAIN_RED			"data\\warrior_train_red.3ds"
#define SZ_OBJ_WARRIOR_TRAIN_YELLOW			"data\\warrior_train_yellow.3ds"
#define SZ_OBJ_WARRIOR_TRAIN_GREEN			"data\\warrior_train_green.3ds"
#define SZ_OBJ_TEMPLE_BLUE					"data\\temple_blue.3ds"
#define SZ_OBJ_TEMPLE_RED					"data\\temple_red.3ds"
#define SZ_OBJ_TEMPLE_YELLOW				"data\\temple_yellow.3ds"
#define SZ_OBJ_TEMPLE_GREEN					"data\\temple_green.3ds"
#define SZ_OBJ_FW_TRAIN_BLUE				"data\\fw_train_blue.3ds"
#define SZ_OBJ_FW_TRAIN_RED					"data\\fw_train_red.3ds"
#define SZ_OBJ_FW_TRAIN_YELLOW				"data\\fw_train_yellow.3ds"
#define SZ_OBJ_FW_TRAIN_GREEN				"data\\fw_train_green.3ds"
#define SZ_OBJ_BOAT_HUT_BLUE				"data\\boat_hut_blue.3ds"
#define SZ_OBJ_BOAT_HUT_RED					"data\\boat_hut_red.3ds"
#define SZ_OBJ_BOAT_HUT_YELLOW				"data\\boat_hut_yellow.3ds"
#define SZ_OBJ_BOAT_HUT_GREEN				"data\\boat_hut_green.3ds"
#define SZ_OBJ_BALLON_HUT_BLUE				"data\\ballon_hut_blue.3ds"
#define SZ_OBJ_BALLON_HUT_RED				"data\\ballon_hut_red.3ds"
#define SZ_OBJ_BALLON_HUT_YELLOW			"data\\ballon_hut_yellow.3ds"
#define SZ_OBJ_BALLON_HUT_GREEN				"data\\ballon_hut_green.3ds"
#define SZ_OBJ_PRISON						"data\\prison.3ds"
#define SZ_OBJ_RS_PILLAR					"data\\rs_pillar.3ds"
#define SZ_OBJ_RS_PILLAR_BLUE				"data\\rs_pillar_blue.3ds"
#define SZ_OBJ_RS_PILLAR_RED				"data\\rs_pillar_red.3ds"
#define SZ_OBJ_RS_PILLAR_YELLOW				"data\\rs_pillar_yellow.3ds"
#define SZ_OBJ_RS_PILLAR_GREEN				"data\\rs_pillar_green.3ds"
#define SZ_OBJ_SPECIAL_A					"data\\special_a.3ds"
#define SZ_OBJ_SPECIAL_B					"data\\special_b.3ds"
#define SZ_OBJ_SPECIAL_C					"data\\special_c.3ds"
#define SZ_OBJ_SPECIAL_D					"data\\special_d.3ds"
#define SZ_OBJ_SPECIAL_E					"data\\special_e.3ds"
#define SZ_OBJ_SPECIAL_F					"data\\special_f.3ds"
#define SZ_OBJ_EAGLE						"data\\eagle.3ds"


// -=-=- misc strings -=-=-

#define SZ_UNKNOW							"unknow"
#define SZ_MANA								"Mana"

#define TRIBE_BLUE							0x01
#define TRIBE_RED							0x02
#define TRIBE_YELLOW						0x04
#define TRIBE_GREEN							0x08


// -=-=- things -=-=-

// - owner -
#define OWNER_NEUTRAL						-1
#define OWNER_BLUE							0
#define OWNER_RED							1
#define OWNER_YELLOW						2
#define OWNER_GREEN							3
#define OWNER_HOSTBOT						4 // Mostly reserved.
//
#define SZ_OWNER_NEUTRAL					"Neutral"
#define SZ_OWNER_BLUE						"Blue"
#define SZ_OWNER_RED						"Red"
#define SZ_OWNER_YELLOW						"Yellow"
#define SZ_OWNER_GREEN						"Green"
#define SZ_OWNER_HOSTBOT					"Hostbot"

// - types -
#define T_PERSON							1
#define T_BUILDING							2
#define T_CREATURE							3
#define T_VEHICLE							4
#define T_SCENERY							5
#define T_GENERAL							6
#define T_EFFECT							7
#define T_SHOT							    8
//#define T_SHAPE							9
//#define T_INTERNAL						10
#define T_SPELL								11
//
#define SZ_PERSON							"Person"
#define SZ_BUILDING							"Building"
#define SZ_CREATURE							"Creature"
#define SZ_VEHICLE							"Vehicle"
#define SZ_SCENERY							"Scenery"
#define SZ_GENERAL							"General"
#define SZ_EFFECT							"Effect"
#define SZ_SPELL							"Spell"
#define SZ_SHOT                             "Shot"
#define SZ_SPECIAL							"Special" // top/sub level scenery


// - person -
#define M_PERSON_WILD						1
#define M_PERSON_BRAVE						2
#define M_PERSON_WARRIOR					3
#define M_PERSON_PRIEST						4
#define M_PERSON_SPY						5
#define M_PERSON_FIREWARRIOR 				6
#define M_PERSON_SHAMAN						7
#define M_PERSON_ANGELOFDEATH				8
//
#define SZ_PERSON_WILD						"Wild"
#define SZ_PERSON_BRAVE						"Brave"
#define SZ_PERSON_WARRIOR					"Warrior"
#define SZ_PERSON_PRIEST					"Priest"
#define SZ_PERSON_SPY						"Spy"
#define SZ_PERSON_FIREWARRIOR				"FireWarrior"
#define SZ_PERSON_SHAMAN					"Shaman"
#define SZ_PERSON_ANGELOFDEATH				"Angel of Death"

// - buildings -
#define M_BUILDING_HUT1						1
#define M_BUILDING_HUT2						2
#define M_BUILDING_HUT3						3
#define M_BUILDING_TOWER					4
#define M_BUILDING_TEMPLE					5
#define M_BUILDING_SPY_TRAIN				6
#define M_BUILDING_WARRIOR_TRAIN			7
#define M_BUILDING_FIREWARRIOR_TRAIN		8
//#define M_BUILDING_RECONVERSION			9
//#define M_BUILDING_WALL_PIECE				10
//#define M_BUILDING_GATE					11
//#define M_BUILDING_CURR_OE_SLOT			12
#define M_BUILDING_BOAT_HUT_1				13
#define M_BUILDING_BOAT_HUT_2				14
#define M_BUILDING_AIRSHIP_HUT_1			15
#define M_BUILDING_AIRSHIP_HUT_2			16
//#define M_BUILDING_GUARD_POST				17
#define M_BUILDING_KNOWLEDGE				18
#define M_BUILDING_PRISON					19
//
#define SZ_BUILDING_HUT1					"Small Hut"
#define SZ_BUILDING_HUT2					"Medium Hut"
#define SZ_BUILDING_HUT3					"Large Hut"
#define SZ_BUILDING_TOWER					"Tower"
#define SZ_BUILDING_TEMPLE					"Temple"
#define SZ_BUILDING_SPY_TRAIN				"Spy Training Hut"
#define SZ_BUILDING_WARRIOR_TRAIN			"Warrior Training Hut"
#define SZ_BUILDING_FIREWARRIOR_TRAIN		"FireWarrior Training Hut"
#define SZ_BUILDING_BOAT_HUT				"Boat Hut"
#define SZ_BUILDING_AIRSHIP_HUT				"Ballon Hut"
#define SZ_BUILDING_KNOWLEDGE				"Vault of Knowledge"
#define SZ_BUILDING_PRISON					"Prison"

// - creatures -
//#define M_CREATURE_BEAR					1
//#define M_CREATURE_BUFFALO				2
//#define M_CREATURE_WOLF					3
#define M_CREATURE_EAGLE					4
//#define M_CREATURE_RABBIT					5
//#define M_CREATURE_BEAVER					6
//#define M_CREATURE_FISH					7
//
#define SZ_CREATURE_EAGLE					"Eagle"

// - vehicles -
#define M_VEHICLE_BOAT_1					1
#define M_VEHICLE_BOAT_2					2
#define M_VEHICLE_AIRSHIP_1					3
#define M_VEHICLE_AIRSHIP_2					4
//
#define SZ_VEHICLE_BOAT						"Boat"
#define SZ_VEHICLE_AIRSHIP					"Ballon"

// - scenery -
#define M_SCENERY_TREE_1					1
#define M_SCENERY_TREE_2					2
#define M_SCENERY_TREE_3					3
#define M_SCENERY_TREE_4					4
#define M_SCENERY_TREE_5					5
#define M_SCENERY_TREE_6					6
#define M_SCENERY_PLANT_1					7
#define M_SCENERY_PLANT_2					8
#define M_SCENERY_STONE_HEAD				9
#define M_SCENERY_FIRE						10
#define M_SCENERY_WOOD_PILE					11
#define M_SCENERY_RS_PILLAR					12
#define M_SCENERY_ROCK						13
//#define M_SCENERY_PORTAL					14
//#define M_SCENERY_ISLAND					15
//#define M_SCENERY_BRIDGE					16
//#define M_SCENERY_DORMANT_TREE			17
#define M_SCENERY_TOP_LEVEL_SCENERY			18
#define M_SCENERY_SUB_LEVEL_SCENERY			19
//
#define SZ_SCENERY_TREE_1					"Tree 1"
#define SZ_SCENERY_TREE_2					"Tree 2"
#define SZ_SCENERY_TREE_3					"Tree 3"
#define SZ_SCENERY_TREE_4					"Tree 4"
#define SZ_SCENERY_TREE_5					"Tree 5"
#define SZ_SCENERY_TREE_6					"Tree 6"
#define SZ_SCENERY_PLANT_1					"Plant 1"
#define SZ_SCENERY_PLANT_2					"Plant 2"
#define SZ_SCENERY_STONE_HEAD				"Stone Head"
#define SZ_SCENERY_WOOD_PILE				"Wood Pile"
#define SZ_SCENERY_RS_PILLAR				"RS Pillar"
#define SZ_SCENERY_ROCK						"Rock"
#define SZ_SCENERY_FIRE						"Fire"

// - special -
#define M_SPECIAL_A							39
#define M_SPECIAL_B							40
#define M_SPECIAL_C							41
#define M_SPECIAL_D							42
#define M_SPECIAL_E							43
#define M_SPECIAL_F							44
//
#define SZ_SPECIAL_A						"special a"
#define SZ_SPECIAL_B						"special b"
#define SZ_SPECIAL_C						"special c"
#define SZ_SPECIAL_D						"special d"
#define SZ_SPECIAL_E						"special e"
#define SZ_SPECIAL_F						"special f"

// - general -
#define M_GENERAL_LIGHT						1
#define M_GENERAL_DISCOVERY					2
//#define M_GENERAL_DEBUG_STATIC			3
//#define M_GENERAL_DEBUG_FLYING			4
#define M_GENERAL_DEBUG_FLAG				5
#define M_GENERAL_TRIGGER					6
//#define M_GENERAL_VEHICLE_CONSTRUCTION	7
//#define M_GENERAL_MAPWHO_THING			8
#define M_GENERAL_BUILDING_ADD_ON			9
//#define M_GENERAL_DISCOVERY_MARKER		10
//
#define SZ_GENERAL_DISCOVERY				"Discovery"
#define SZ_GENERAL_TRIGGER					"Trigger"
#define SZ_GENERAL_BUILDING_ADD_ON			"Building Add-on"
#define SZ_GENERAL_LIGHT					"Light"

// - effect -
#define M_EFFECT_SIMPLE_BLAST				1
#define M_EFFECT_SPRITE_CIRCLES				2
#define M_EFFECT_SMOKE						3
#define M_EFFECT_LIGHTNING_ELEM				4
#define M_EFFECT_BURN_CELL_OBSTACLES		5
#define M_EFFECT_FLATTEN_LAND				6
#define M_EFFECT_MOVE_RS_PILLAR				7 // no effect?
#define M_EFFECT_PREPARE_RS_LAND			8 // no effect?
#define M_EFFECT_SPHERE_EXPLODE_1			9
#define M_EFFECT_FIREBALL					10
#define M_EFFECT_FIRECLOUD					11
#define M_EFFECT_GHOST_ARMY					12
#define M_EFFECT_INVISIBILITY				13
#define M_EFFECT_EXPLODE_BLDG_PARTIAL		14 // no effect?
#define M_EFFECT_VOLCANO					15
#define M_EFFECT_HYPNOTISM					16
#define M_EFFECT_LIGHTNING_BOLT				17
#define M_EFFECT_SWAMP						18
#define M_EFFECT_ANGEL_OF_DEATH				19
#define M_EFFECT_WHIRLWIND					20
#define M_EFFECT_INSECT_PLAGUE				21
#define M_EFFECT_FIRESTORM					22
#define M_EFFECT_EROSION					23
#define M_EFFECT_LAND_BRIDGE				24
#define M_EFFECT_WRATH_OF_GOD				25 // no effect?
#define M_EFFECT_EARTHQUAKE					26
#define M_EFFECT_FLY_THINGUMMY				27
#define M_EFFECT_SPHERE_EXPLODE_AND_FIRE	28
#define M_EFFECT_BIG_FIRE					29
#define M_EFFECT_LIGHTNING					30
#define M_EFFECT_FLATTEN					31
#define M_EFFECT_GENERAL					32 // no effect?
#define M_EFFECT_SHAPE_SPARKLE				33 // no effect?
#define M_EFFECT_LAVA_FLOW					34
#define M_EFFECT_VOLCANO_EXPLOSIONS			35 // no effect?
#define M_EFFECT_PURIFY_LAND				36
#define M_EFFECT_UNPURIFY_LAND				37 // no effect?
#define M_EFFECT_EXPLOSION_1				38
#define M_EFFECT_EXPLOSION_2				39
#define M_EFFECT_LAVA_SQUARE				40
#define M_EFFECT_WW_ELEMENT					41
#define M_EFFECT_LIGHTNING_STRAND			42 // no effect?
#define M_EFFECT_WW_DUST					43
#define M_EFFECT_RAISE_LAND					44 // no effect?
#define M_EFFECT_LOWER_LAND					45 // no effect?
#define M_EFFECT_HILL						46
#define M_EFFECT_VALLEY						47
#define M_EFFECT_PLACE_TREE					48
#define M_EFFECT_RISE						49
#define M_EFFECT_DIP						50
#define M_EFFECT_REIN_ROCK_DEBRIS			51
#define M_EFFECT_CLEAR_MAPWHO				52 // no effect?
#define M_EFFECT_PLACE_SHAMAN				53
#define M_EFFECT_PLACE_WILD					54
#define M_EFFECT_BLDG_SMOKE					55
#define M_EFFECT_MUCH_SIMPLER_BLAST			56
#define M_EFFECT_TUMBLING_BRANCH			57
#define M_EFFECT_CONVERSION_FLASH			58 // no effect?
#define M_EFFECT_HYPNOSIS_FLASH				59 // no effect?
#define M_EFFECT_SPARKLE					60
#define M_EFFECT_SMALL_SPARKLE				61
#define M_EFFECT_EXPLOSION_3				62
#define M_EFFECT_ROCK_EXPLOSION				63
#define M_EFFECT_LAVA_GLOOP					64
#define M_EFFECT_SPLASH						65
#define M_EFFECT_SMOKE_CLOUD				66
#define M_EFFECT_SMOKE_CLOUD_CONSTANT		67
#define M_EFFECT_FIREBALL_2					68
#define M_EFFECT_GROUND_SHOCKWAVE			69
#define M_EFFECT_ORBITER					70
#define M_EFFECT_BIG_SPARKLE				71
#define M_EFFECT_METEOR						72
#define M_EFFECT_CONVERT_WILD				73
#define M_EFFECT_BLDG_SMOKE_2_FULL			74
#define M_EFFECT_BLDG_SMOKE_2_PARTIAL		75
#define M_EFFECT_BLDG_DAMAGED_SMOKE			76
#define M_EFFECT_DELETE_RS_PILLARS			77
#define M_EFFECT_SPELL_BLAST				78
#define M_EFFECT_FIRESTORM_SMOKE			79
#define M_EFFECT_PLAYER_DEAD				80 // no effect?
#define M_EFFECT_REVEAL_FOG_AREA			81
#define M_EFFECT_SHIELD						82
#define M_EFFECT_BOAT_HUT_REPAIR			83
#define M_EFFECT_REEDY_GRASS				84
#define M_EFFECT_SWAMP_MIST					85
#define M_EFFECT_ARMAGEDDON					86
#define M_EFFECT_BLOODLUST					87
#define M_EFFECT_TELEPORT					88
#define M_EFFECT_ATLANTIS_SET				89
#define M_EFFECT_ATLANTIS_INVOKE			90
#define M_EFFECT_STATUE_TO_AOD				91
#define M_EFFECT_FILL_ONE_SHOTS				92
#define M_EFFECT_FIRE_ROLL_ELEM				93 // no effect?
#define	M_EFFECT_ARMA_ARENA					94
//
#define SZ_EFFECT_SIMPLE_BLAST				"simple blast"
#define SZ_EFFECT_SPRITE_CIRCLES			"circles"
#define SZ_EFFECT_SMOKE						"smoke"
#define SZ_EFFECT_LIGHTNING_ELEM			"lightning elem"
#define SZ_EFFECT_BURN_CELL_OBSTACLES		"burn cell obstacles"
#define SZ_EFFECT_FLATTEN_LAND				"flatten land"
#define SZ_EFFECT_SPHERE_EXPLODE_1			"sphere explode 1"
#define SZ_EFFECT_FIREBALL					"fireball"
#define SZ_EFFECT_FIRECLOUD					"firecloud"
#define SZ_EFFECT_GHOST_ARMY				"ghost army"
#define SZ_EFFECT_INVISIBILITY				"invisibility"
#define SZ_EFFECT_VOLCANO					"volcano"
#define SZ_EFFECT_HYPNOTISM					"hypnotism"
#define SZ_EFFECT_LIGHTNING_BOLT			"lightning bolt"
#define SZ_EFFECT_SWAMP						"swamp"
#define SZ_EFFECT_ANGEL_OF_DEATH			"angel of death"
#define SZ_EFFECT_WHIRLWIND					"tornado"
#define SZ_EFFECT_INSECT_PLAGUE				"swarm"
#define SZ_EFFECT_FIRESTORM					"firestorm"
#define SZ_EFFECT_EROSION					"erosion"
#define SZ_EFFECT_LAND_BRIDGE				"land bridge"
#define SZ_EFFECT_EARTHQUAKE				"earthquake"
#define SZ_EFFECT_FLY_THINGUMMY				"fly thingummy"
#define SZ_EFFECT_SPHERE_EXPLODE_AND_FIRE	"sphere explode and fire"
#define SZ_EFFECT_BIG_FIRE					"big fire"
#define SZ_EFFECT_LIGHTNING					"lightning"
#define SZ_EFFECT_FLATTEN					"flatten"
#define SZ_EFFECT_LAVA_FLOW					"lava flow"
#define SZ_EFFECT_PURIFY_LAND				"purify land"
#define SZ_EFFECT_EXPLOSION_1				"explosion 1"
#define SZ_EFFECT_EXPLOSION_2				"explosion 2"
#define SZ_EFFECT_LAVA_SQUARE				"lava square"
#define SZ_EFFECT_WW_ELEMENT				"tornado element"
#define SZ_EFFECT_WW_DUST					"tornado dust"
#define SZ_EFFECT_HILL						"hill"
#define SZ_EFFECT_VALLEY					"valley"
#define SZ_EFFECT_PLACE_TREE				"place tree"
#define SZ_EFFECT_RISE						"rise"
#define SZ_EFFECT_DIP						"dip"
#define SZ_EFFECT_REIN_ROCK_DEBRIS			"rein rock debris"
#define SZ_EFFECT_PLACE_SHAMAN				"place shaman"
#define SZ_EFFECT_PLACE_WILD				"place wild"
#define SZ_EFFECT_BLDG_SMOKE				"bldg smoke"
#define SZ_EFFECT_MUCH_SIMPLER_BLAST		"much simpler blast"
#define SZ_EFFECT_TUMBLING_BRANCH			"tumbling branch"
#define SZ_EFFECT_SPARKLE					"sparkle"
#define SZ_EFFECT_SMALL_SPARKLE				"small sparkle"
#define SZ_EFFECT_EXPLOSION_3				"explosion 3"
#define SZ_EFFECT_ROCK_EXPLOSION			"rock explosion"
#define SZ_EFFECT_LAVA_GLOOP				"lava gloop"
#define SZ_EFFECT_SPLASH					"splash"
#define SZ_EFFECT_SMOKE_CLOUD				"smoke cloud"
#define SZ_EFFECT_SMOKE_CLOUD_CONSTANT		"smoke cloud constant"
#define SZ_EFFECT_FIREBALL_2				"fireball 2"
#define SZ_EFFECT_GROUND_SHOCKWAVE			"ground shockwave"
#define SZ_EFFECT_ORBITER					"orbiter"
#define SZ_EFFECT_BIG_SPARKLE				"big sparkle"
#define SZ_EFFECT_METEOR					"meteor"
#define SZ_EFFECT_CONVERT_WILD				"convert"
#define SZ_EFFECT_BLDG_SMOKE_2_FULL			"bldg smoke 2 full"
#define SZ_EFFECT_BLDG_SMOKE_2_PARTIAL		"bldg smoke 2 partial"
#define SZ_EFFECT_BLDG_DAMAGED_SMOKE		"bldg damaged smoke"
#define SZ_EFFECT_DELETE_RS_PILLARS			"delete rs pillars"
#define SZ_EFFECT_SPELL_BLAST				"spell blast"
#define SZ_EFFECT_FIRESTORM_SMOKE			"firestorm smoke"
#define SZ_EFFECT_REVEAL_FOG_AREA			"reveal fog area"
#define SZ_EFFECT_SHIELD					"magical shield"
#define SZ_EFFECT_BOAT_HUT_REPAIR			"boat hut repair"
#define SZ_EFFECT_REEDY_GRASS				"reedy grass"
#define SZ_EFFECT_SWAMP_MIST				"swamp mist"
#define SZ_EFFECT_ARMAGEDDON				"armageddon"
#define SZ_EFFECT_BLOODLUST					"bloodlust"
#define SZ_EFFECT_TELEPORT					"teleport"
#define SZ_EFFECT_ATLANTIS_SET				"atlantis set"
#define SZ_EFFECT_ATLANTIS_INVOKE			"atlantis invoke"
#define SZ_EFFECT_STATUE_TO_AOD				"statue to aod"
#define SZ_EFFECT_FILL_ONE_SHOTS			"fill one shots"
#define SZ_EFFECT_ARMA_ARENA				"arma arena"

// - shot -
#define M_SHOT_STANDARD						1
#define M_SHOT_STANDARD_2					2
#define M_SHOT_STANDARD_3					3
#define M_SHOT_FIREBALL						4
#define M_SHOT_LIGHTNING					5
#define M_SHOT_SUPER_WARRIOR				6
#define M_SHOT_VOLCANO_FIREBALL_1			7
#define M_SHOT_VOLCANO_FIREBALL_2			8

#define SZ_SHOT_STANDARD					"standard"
#define SZ_SHOT_STANDARD_2					"standard 2"
#define SZ_SHOT_STANDARD_3					"standard 3"
#define SZ_SHOT_FIREBALL					"fireball"
#define SZ_SHOT_LIGHTNING					"lightning"
#define SZ_SHOT_SUPER_WARRIOR				"super warrior"
#define SZ_SHOT_VOLCANO_FIREBALL_1			"volcano fireball 1"
#define SZ_SHOT_VOLCANO_FIREBALL_2			"volcano fireball 2"

// - internal -
/*
#define M_INTERNAL_FORMATION				1
#define M_INTERNAL_BEACON					2
#define M_INTERNAL_THING_INFO_DISPLAY		3
#define M_INTERNAL_SOUL_CONVERT				4
#define M_INTERNAL_SOUL_MAN					5
#define M_INTERNAL_MED_MAN_ATTRACT			6
#define M_INTERNAL_OBJ_FACE					7
#define M_INTERNAL_FIGHT					8
#define M_INTERNAL_PRE_FIGHT				9
#define M_INTERNAL_GUARD_CONTROL			10
#define M_INTERNAL_BRIDGE_CONTROL			11
#define M_INTERNAL_SOUL_CONVERT_2			12
#define M_INTERNAL_DT_BEACON				13
#define M_INTERNAL_PLAYER_RAISE				14
#define M_INTERNAL_PLAYER_LOWER				15
#define M_INTERNAL_GUARD_POST_DISPLAY		16
#define M_INTERNAL_PLAYER_SMOOTH			17
#define M_INTERNAL_WOOD_DISTRIB				18
#define M_INTERNAL_SINKING_BLDG				19
*/

// - spell -
#define M_SPELL_BURN						1
#define M_SPELL_BLAST						2
#define M_SPELL_LIGHTNING					3
#define M_SPELL_TORNADO						4
#define M_SPELL_SWARM						5
#define M_SPELL_INVISIBILITY				6
#define M_SPELL_HYPNOTISM					7
#define M_SPELL_FIRESTORM					8
#define M_SPELL_GHOST_ARMY					9
#define M_SPELL_ERODE						10
#define M_SPELL_SWAMP						11
#define M_SPELL_LAND_BRIDGE					12
#define M_SPELL_ANGEL_OF_DEATH				13
#define M_SPELL_EARTHQUAKE					14
#define M_SPELL_FLATTEN						15
#define M_SPELL_VOLCANO						16
#define M_SPELL_CONVERT						17
#define M_SPELL_ARMAGEDDON					18
#define M_SPELL_MAGICAL_SHIELD				19
#define M_SPELL_BLOODLUST					20
#define M_SPELL_TELEPORT					21
//#define M_SPELL_HILL						23
//#define M_SPELL_RISE						24
//#define M_SPELL_VALLEY					25
//#define M_SPELL_DIP						26
//#define M_SPELL_PLACE_TREE				27
//#define M_SPELL_CLEAR_MAPWHO				28
//#define M_SPELL_PLACE_SHAMAN				29
//#define M_SPELL_PLACE_WILD				30
//
#define SZ_SPELL_BURN						"Burn"
#define SZ_SPELL_BLAST						"Blast"
#define SZ_SPELL_LIGHTNING					"Lightning"
#define SZ_SPELL_TORNADO					"Tornado"
#define SZ_SPELL_SWARM						"Swarm"
#define SZ_SPELL_INVISIBILITY				"Invisibility"
#define SZ_SPELL_HYPNOTISM					"Hypnotism"
#define SZ_SPELL_FIRESTORM					"FireStorm"
#define SZ_SPELL_GHOST_ARMY					"Ghost Army"
#define SZ_SPELL_ERODE						"Erode"
#define SZ_SPELL_SWAMP						"Swamp"
#define SZ_SPELL_LAND_BRIDGE				"Land Bridge"
#define SZ_SPELL_ANGEL_OF_DEATH				"Angel of Death"
#define SZ_SPELL_EARTHQUAKE					"EarthQuake"
#define SZ_SPELL_FLATTEN					"Flatten"
#define SZ_SPELL_VOLCANO					"Volcano"
#define SZ_SPELL_CONVERT					"Convert"
#define SZ_SPELL_ARMAGEDDON					"Armageddon"
#define SZ_SPELL_MAGICAL_SHIELD				"Magical Shield"
#define SZ_SPELL_BLOODLUST					"Bloodlust"
#define SZ_SPELL_TELEPORT					"Teleport"


// -=-=- general availability -=-=-

#define AVAILABILITY_PERMANENT				1
#define AVAILABILITY_LEVEL					2
#define AVAILABILITY_ONCE					3

#define SZ_AVAILABILITY_PERMANENT			"Permanent"
#define SZ_AVAILABILITY_LEVEL				"Level"
#define SZ_AVAILABILITY_ONCE				"Once"


// -=-=- trigger types -=-=-

#define TRIGGER_TYPE_PROXIMITY				0
#define TRIGGER_TYPE_TIMED					1
#define TRIGGER_TYPE_PLAYER_DEATH			2
#define TRIGGER_TYPE_SHAMAN_PROXIMITY		3
#define TRIGGER_TYPE_LIBRARY				4
#define TRIGGER_TYPE_SHAMAN_AOD				5

#define SZ_TRIGGER_TYPE_PROXIMITY			"Proximity"
#define SZ_TRIGGER_TYPE_TIMED				"Timed"
#define SZ_TRIGGER_TYPE_PLAYER_DEATH		"Player Death"
#define SZ_TRIGGER_TYPE_SHAMAN_PROXIMITY	"Shaman Proximity"
#define SZ_TRIGGER_TYPE_LIBRARY				"Library"
#define SZ_TRIGGER_TYPE_SHAMAN_AOD			"Shaman AoD"


// -=-=- discovery trigger types -=-=-

#define DISCOVERY_TRIGGER_NORMAL			0
#define DISCOVERY_TRIGGER_IMMEDIATE			1


// -=-=- level flags -=-=-

#define LEVEL_FLAGS_USE_FOG				(1<<0)
#define LEVEL_FLAGS_SHAMAN_OMNI			(1<<1)
//#define LEVEL_FLAGS_FORCE640X480		(1<<2)
//#define LEVEL_FLAGS_LEVEL_EDIT		(1<<3)
#define LEVEL_FLAGS_NO_GUEST			(1<<4)
#define LEVEL_NO_REINCARNATE_TIME		(1<<5)


// -=-=- angles -=-=-

#define ANGLE_0								0x0000
#define ANGLE_0_RAD							0.0f
#define ANGLE_0_COS							1.0f
#define ANGLE_0_SIN							0.0f
#define ANGLE_45							0x0100
#define ANGLE_45_RAD						0.78539801f
#define ANGLE_45_COS						0.70710689f
#define ANGLE_45_SIN						0.70710665f
#define ANGLE_90							0x0200
#define ANGLE_90_RAD						1.5707960f
#define ANGLE_90_COS						3.1391647e-007f
#define ANGLE_90_SIN						1.0f
#define ANGLE_135							0x0300
#define ANGLE_135_RAD						2.3561940f
#define ANGLE_135_COS						-0.70710647f
#define ANGLE_135_SIN						0.70710713f
#define ANGLE_180							0x0400
#define ANGLE_180_RAD						3.1415920f
#define ANGLE_180_COS						-1.0f
#define ANGLE_180_SIN						6.2783295e-007f
#define ANGLE_225							0x0500
#define ANGLE_225_RAD						3.9269900f
#define ANGLE_225_COS						-0.70710737f
#define ANGLE_225_SIN						-0.70710623f
#define ANGLE_270							0x0600
#define ANGLE_270_RAD						4.7123880f
#define ANGLE_270_COS						-9.4174942e-007f
#define ANGLE_270_SIN						-1.0f
#define ANGLE_315							0x0700
#define ANGLE_315_RAD						5.4977860f
#define ANGLE_315_COS						0.70710599f
#define ANGLE_315_SIN						-0.70710754f


// -=-=- structs -=-=-

typedef unsigned char			UBYTE;
typedef signed char				SBYTE;
typedef unsigned short			UWORD;
typedef signed short			SWORD;
typedef unsigned long			ULONG;
typedef signed long				SLONG;


#pragma pack(push, 1)

struct PLAYERTHINGS
{
	ULONG						SpellsAvailable;
	ULONG						BuildingsAvailable;
	ULONG						BuildingsAvailableLevel;
	ULONG						BuildingsAvailableOnce;
	union {
	ULONG						SpellsAvailableLevel;
	ULONG						SpellsNotCharging;
	};
	UBYTE						SpellsAvailableOnce[32];
	UWORD						VehiclesAvailable;
	UBYTE						TrainingManaOff;
	UBYTE						Flags;
};

struct LEVELHEADERv2
{
	PLAYERTHINGS				DefaultThings;
	CHAR						Name[32];
	UBYTE						NumPlayers;
	UBYTE						ComputerPlayerIndex[3];
	UBYTE						DefaultAllies[4];
	UBYTE						LevelType;
	UBYTE						ObjectsBankNum;
	UBYTE						LevelFlags;
	UBYTE						Pad[1];
	UWORD						Markers[256];
	UWORD						StartPos;
	UWORD						StartAngle;
};

#define	MAX_LENGTH_SAVE_NAMEv2				(32)
#define	MAX_NUM_SCRIPT2						10
struct LEVELHEADERv3
{
	LEVELHEADERv2				v2;
	UBYTE						Version;								// How many objects are in the level
	ULONG						MaxAltPoints;							// How many points are in the level
	ULONG						MaxNumObjects;							// How many objects are in the level
	ULONG						MaxNumPlayers;							// How many players are in the level
	CHAR						Script2[MAX_NUM_SCRIPT2][MAX_LENGTH_SAVE_NAMEv2];
};


struct ACCESSSAVEINFO
{
	UBYTE						Model,
								Type,
								Rights;
};


struct SUNLIGHTSAVEINFO
{
	UBYTE						ShadeStart,
								ShadeRange,
								Inclination;
};


struct PLAYERSAVEINFO
{
	SWORD						StartPosX,
								StartPosY;
	SLONG						Future1,
								Future2,
								Future3;
};


struct BUILDINGSAVE
{
	SLONG						Angle;
};


struct SCENERYSAVE
{
	UBYTE						PortalStatus,
								PortalLevel,
								PortalType;
	SWORD						Angle;
	UBYTE						UserId;
	SWORD						IslandAlt;
	UBYTE						IslandNum,
								BridgeNum;
};


struct GENERALSAVE
{
	UBYTE						DiscoveryType,
								DiscoveryModel,
								AvailabilityType,
								TriggerType;
	SLONG						ManaAmt;
};


struct TRIGGERSAVE
{
	UBYTE						TriggerType,
								CellRadius,
								RandomValue;
	SBYTE						NumOccurences;
	UWORD						TriggerCount,
								ThingIdxs[10];
	SWORD						PrayTime;
	UBYTE						StartInactive,
								CreatePlayerOwned;
	SWORD						InactiveTime;
};


struct THINGSAVE
{
	UBYTE						Model,
								Type;
	SBYTE						Owner;
	WORD						PosX,
								PosZ;
	union
	{
		BUILDINGSAVE			Building;
		SCENERYSAVE				Scenery;
		GENERALSAVE				General;
		TRIGGERSAVE				Trigger;
		DWORD					Bluff[12];
	};
};

struct LEVELDATv3
{
	char						MAGIC[5];
	struct LEVELHEADERv3		Header;
	WORD						GroundHeight[128 * 128];
	BYTE						NoAccessSquares[128 * 128];
	PLAYERSAVEINFO				psi[4];
	SUNLIGHTSAVEINFO			ssi;
};

struct LEVELDATv2
{
	WORD						GroundHeight[128 * 128];
	BYTE						LandBlocks[128 * 128];
	BYTE						LandOrients[128 * 128];
	BYTE						NoAccessSquares[128 * 128];
	PLAYERSAVEINFO				psi[4];
	SUNLIGHTSAVEINFO			ssi;
	THINGSAVE					Things[MAX_V2_THINGS];
	ACCESSSAVEINFO asi[50];
};


struct LEVELVERSION
{
	SLONG						VersionNum;
	CHAR						CreatedBy[32];
	CHAR						CreatedOn[28];
	SLONG						CheckSum;
};

#pragma pack(pop)


// thing flags
#define TF_DRAW					0x00000001
#define TF_EDIT_LANDBRIDGE		0x00000002
#define TF_DRAW_LANDBRIDGE		0x00000004


struct LAND_BRIDGE_DATA
{
	float						x,
								z,
								ex,
								ey,
								ez;
};


struct THING
{
	THING						*Next,
								*Prev,
								*Links[10];
	THINGSAVE					Thing;
	float						x,
								z,
								ex,
								ey,
								ez;
	DWORD						flags;
	UWORD						Idx;
	union
	{
		LAND_BRIDGE_DATA		LandBridge;
	};
};


struct MARKER
{
	float						x,
								z,
								ex,
								ey,
								ez;
	DWORD						flags;
};
