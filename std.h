#pragma once

/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#define STR_SIZE  256

#define _WIN32_WINDOWS 0x0500
#pragma comment(lib, "comctl32.lib")

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _DEBUG
#include <assert.h>
#endif
#include "resource.h"
#include "version.h"

#pragma comment(lib, "shlwapi.lib")
#include <shlwapi.h>



#define LINK(list, add) { if(list) { add->Prev = list->Prev; add->Next = list; list->Prev->Next = add; list->Prev = add; } else { list = add; add->Next = add->Prev = add; } }
#define UNLINK(list, del) { del->Next->Prev = del->Prev; del->Prev->Next = del->Next; if(list == del) { list = del->Next; if(list == del) list = 0; } del->Next = 0; del->Prev = 0; }



// build flags

#define USE_FOG							1
#define USE_NEW_VIEW					1
#define SPHERICAL_WORLD					1

#ifdef _DEBUG
  #define DEBUG							1
  #define DRAW_OBJECTS_LINES			0
  #define DRAW_MINIMAP_LINES			0
  #define DRAW_OBJECTS_NORMALS			0
  #define DRAW_SPRITE_BOUNDBOXES		1
#else // _DEBUG
  #define DEBUG							0
  #define DRAW_OBJECTS_LINES			0
  #define DRAW_MINIMAP_LINES			0
  #define DRAW_OBJECTS_NORMALS			0
  #define DRAW_SPRITE_BOUNDBOXES		0
#endif // _DEBUG
