#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3dsftk.h"

/* default input file extension */
static char3ds *FILE_EXT = {".3ds"};

static void banner( char3ds *proc_name )
{
   char3ds *cp;
	
   cp = strrchr( proc_name, '\\');
   fprintf( stdout, "Usage : %s -detail <3DS file name>\n",(cp) ? ++cp : proc_name);
   fprintf(stdout, "detail:\n%s\n%s\n%s\n",
	   "-header = only the chunk headers",
	   "-medium = most but no large arrays",
	   "-max    = everything I could find"
	   );
   
}

void main( int argc, char3ds *argv[] )
{
   file3ds *file= NULL;
   database3ds *db = NULL;
   chunklist3ds *meshlist=NULL, *matlist=NULL;
   char3ds filename[250];

   SetDumpLevel3ds(MediumDump3ds);
	
   if (argc > 1)
   {
      if ((argv[1])[0] == '-')
      {
	 if (strcmp("-header", argv[1]) == 0) SetDumpLevel3ds(TerseDump3ds);
	 if (strcmp("-medium", argv[1]) == 0) SetDumpLevel3ds(MediumDump3ds);
	 if (strcmp("-max", argv[1]) == 0) SetDumpLevel3ds(MaximumDump3ds);
	 if (argc < 3)
	 {
	    banner(argv[0]);
	    exit(0);
	 }
	 strcpy(filename,argv[2]);
      } else
      {
	 if( argc < 2 )
	 {
	    banner( argv[0] );
	    exit(0);
	 }
	 strcpy(filename,argv[1]);
      }
   } else
   {
      banner(argv[0]);
      exit(0);
   }
	
   file = OpenFile3ds(filename, "rw");
   PRINT_ERRORS_EXIT(stderr);
    
   InitDatabase3ds(&db);
   PRINT_ERRORS_EXIT(stderr);

   CreateDatabase3ds(file, db);
   PRINT_ERRORS_EXIT(stderr);

   DumpDatabase3ds(stdout, db);
   PRINT_ERRORS_EXIT(stderr);

   ReleaseDatabase3ds(&db);
   PRINT_ERRORS_EXIT(stderr);
    
   CloseFile3ds(file);
   PRINT_ERRORS_EXIT(stderr);

}

/* History:
   $Log: /Plugins/3dsftk/dump3ds/dump3ds.c $
 * 
 * 1     2/14/97 4:14p Christer
 * 
 * 1     2/14/97 4:12p Christer
 * 
 * 1     2/14/97 4:10p Christer
 * 
 * 1     2/14/97 4:06p Christer
 * Revision 1.8  1994/06/09  18:22:13  grantb
 * 1. Changed the spelling of "DataBase" to "Database"
 * 2. Took out smartall references.
 *
 * Revision 1.7  1994/06/01  18:33:15  grantb
 * 1. Replaced lib headers with single "3dsftk.h" header
 * 2. Added error tests.
 *
 * Revision 1.6  1994/05/04  09:09:26  grantb
 * Moved big old log entry to end of file.
 *
 * Revision 1.5  1994/04/28  07:25:37  grantb
 * - Added support to dump3ds for a level of detail feature.  Type dump3ds
 *   without args for a summary of the command line options.
 *
 * Revision 1.4  1993/07/26  05:00:31  grantb
 * Initialize *file and *db to NULL
 *
 * Revision 1.3  1993/07/15  13:08:42  grantb
 * Corrected capitalization errors in using the dbase3ds functions.
 *
*/

