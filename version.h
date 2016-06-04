#pragma once

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
                                    "." STRINGIZE(BUILD_NUMBER)
