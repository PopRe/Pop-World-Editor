#pragma once

/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

// The build increment is expected to be overridden by the continuous build system.
#ifndef BUILD_INCREMENT
#define BUILD_INCREMENT		0
#endif

#define APPNAME					"Populous World Editor For Levelv3"

// Determine the version number constants.
#define MAJOR_VERSION		1
#define MINOR_VERSION		2
#define REVISION_NUMBER		1
#define BUILD_NUMBER		BUILD_INCREMENT

#define VER_FILE_VERSION            MAJOR_VERSION, MINOR_VERSION, REVISION_NUMBER, BUILD_NUMBER
#define VER_FILE_VERSION_STR        STRINGIZE(MAJOR_VERSION) \
                                    "," STRINGIZE(MINOR_VERSION) \
                                    "," STRINGIZE(REVISION_NUMBER) \
                                    "," STRINGIZE(BUILD_NUMBER) \

#define POPEDT_VERSION			STRINGIZE(MAJOR_VERSION) \
                                    "." STRINGIZE(MINOR_VERSION) \
                                    "." STRINGIZE(REVISION_NUMBER) \
                                    "." STRINGIZE(BUILD_NUMBER) \

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
