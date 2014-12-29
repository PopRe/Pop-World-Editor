/* Program exercises the Get* and Put* functions by copying a file a structure
   at a time */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "3dsftk.h"

/* default file extension */
static char3ds *FILE_EXT = {".3ds"};

static void banner( char3ds *proc_name )
{
   char3ds *cp;
	
   cp = strrchr( proc_name, '\\');
   fprintf( stdout, "Usage : %s -option <input file name> <output file name>\n",(cp) ? ++cp : proc_name);
   fprintf(stdout, "options:\n%s\n%s\n%s\n%s\n%s\n", 
	   "-mesh   = Duplicate mesh objects",
	   "-spot   = Duplicate spotlights",
	   "-omni   = Duplicate omni lights",
	   "-camera = Duplicate cameras",
	   "-mat    = Duplicate materials"
	   );
   
   
}

char3ds *DefaultAppname = "Purple Tiger's Duplicate Objects Test Case";
      material3ds *mat = NULL;

void main( int argc, char3ds *argv[] )
{
   file3ds *ifile = NULL, *ofile = NULL;
   database3ds *idb = NULL, *odb = NULL;
   char3ds iname[250], oname[250];
   chunktag3ds dupwhat = NULL_CHUNK;
   

   if (argc > 1)
   {
      if ((argv[1])[0] == '-')
      {
	 if (strcmp("-mesh", argv[1]) == 0) dupwhat = N_TRI_OBJECT;
	 if (strcmp("-spot", argv[1]) == 0) dupwhat = DL_SPOTLIGHT;
	 if (strcmp("-omni", argv[1]) == 0) dupwhat = N_DIRECT_LIGHT;
	 if (strcmp("-camera", argv[1]) == 0) dupwhat = N_CAMERA;
	 if (strcmp("-mat", argv[1]) == 0) dupwhat = MAT_ENTRY;

	 if (argc < 4)
	 {
	    banner(argv[0]);
	    exit(0);
	 }
	 strcpy(iname,argv[2]);
	 strcpy(oname,argv[3]);
      } else
      {
	 if( argc < 3 )
	 {
	    banner( argv[0] );
	    exit(0);
	 }
	 strcpy(iname,argv[1]);
	 strcpy(oname,argv[2]);
      }
   } else
   {
      banner(argv[0]);
      exit(0);
   }
   
   ifile = OpenFile3ds(iname, "rb");
   PRINT_ERRORS_EXIT(stderr);
   
   ofile = OpenFile3ds(oname, "wb");
   PRINT_ERRORS_EXIT(stderr);
   
   /* Initializes the database structure */
   InitDatabase3ds(&idb);
   PRINT_ERRORS_EXIT(stderr);

   InitDatabase3ds(&odb);
   PRINT_ERRORS_EXIT(stderr);
   
   /* Generate the database with the input 3DS file */
   CreateDatabase3ds(ifile, idb);
   PRINT_ERRORS_EXIT(stderr);
   
   CreateNewDatabase3ds(odb, GetDatabaseType3ds(idb));
   PRINT_ERRORS_EXIT(stderr);
   
   if ((dupwhat == MDATA) || (dupwhat == NULL_CHUNK))
   {
      atmosphere3ds *atmo = NULL;
      meshset3ds *mset = NULL;
      background3ds *bgnd = NULL;

      InitAtmosphere3ds(&atmo);
      PRINT_ERRORS_EXIT(stderr);
      
      GetAtmosphere3ds(idb, &atmo);
      PRINT_ERRORS_EXIT(stderr);
      
      PutAtmosphere3ds(odb, atmo);
      PRINT_ERRORS_EXIT(stderr);
      
      ReleaseAtmosphere3ds(&atmo);
      PRINT_ERRORS_EXIT(stderr);

      InitMeshSet3ds(&mset);
      PRINT_ERRORS_EXIT(stderr);
      
      GetMeshSet3ds(idb, &mset);
      PRINT_ERRORS_EXIT(stderr);
      
      PutMeshSet3ds(odb, mset);
      PRINT_ERRORS_EXIT(stderr);
      
      ReleaseMeshSet3ds(&mset);
      PRINT_ERRORS_EXIT(stderr);

      InitBackground3ds(&bgnd);
      PRINT_ERRORS_EXIT(stderr);
      
      GetBackground3ds(idb, &bgnd);
      PRINT_ERRORS_EXIT(stderr);
      
      PutBackground3ds(odb, bgnd);
      PRINT_ERRORS_EXIT(stderr);
      
      ReleaseBackground3ds(&bgnd);
      PRINT_ERRORS_EXIT(stderr);
      
   }
   
   if ((dupwhat == VIEWPORT_LAYOUT) || (dupwhat == NULL_CHUNK))
   {
      viewport3ds *vp;

      GetViewport3ds(idb, &vp);
      PRINT_ERRORS_EXIT(stderr);

      PutViewport3ds(odb, vp);
      PRINT_ERRORS_EXIT(stderr);

      ReleaseViewport3ds(&vp);
      PRINT_ERRORS_EXIT(stderr);
   }
   
   if ((dupwhat == N_TRI_OBJECT) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *meshlist = NULL;
      mesh3ds *mesh = NULL;
      ulong3ds i;

      /* Generate a list of names to test the Name searching */
      GetMeshNameList3ds(idb, &meshlist);
      PRINT_ERRORS_EXIT(stderr);
      

      for (i=0; i < meshlist->count; i++)
      {
	 GetMeshByName3ds(idb, meshlist->list[i].name, &mesh);
	 PRINT_ERRORS_EXIT(stderr);
	 
	 PutMesh3ds(odb, mesh);
	 PRINT_ERRORS_EXIT(stderr);
	 
	 RelMeshObj3ds (&mesh);
	 PRINT_ERRORS_EXIT(stderr);
      }
      
      ReleaseNameList3ds(&meshlist);
      PRINT_ERRORS_EXIT(stderr);
      
   }

   if ((dupwhat == N_CAMERA) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *camlist = NULL;
      camera3ds *cam = NULL;
      ulong3ds i;

      GetCameraNameList3ds(idb, &camlist);
      PRINT_ERRORS_EXIT(stderr);
      

      for (i=0; i < camlist->count; i++)
      {
	 GetCameraByName3ds(idb, camlist->list[i].name, &cam);
	 PRINT_ERRORS_EXIT(stderr);
	 
	 PutCamera3ds(odb, cam);
	 PRINT_ERRORS_EXIT(stderr);
	 
	 ReleaseCamera3ds(&cam);
	 PRINT_ERRORS_EXIT(stderr);
      }

      ReleaseNameList3ds(&camlist);
      PRINT_ERRORS_EXIT(stderr);
      
   }

   if ((dupwhat == MAT_ENTRY) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *matlist = NULL;
      ulong3ds i;

      mat = NULL;
      
      GetMaterialNameList3ds(idb, &matlist);
      PRINT_ERRORS_EXIT(stderr);
      
      for (i = 0; i < matlist->count; i++)
      {
	 GetMaterialByName3ds(idb, matlist->list[i].name, &mat);
	 PRINT_ERRORS_EXIT(stderr);
	 
	 PutMaterial3ds(odb, mat);
	 PRINT_ERRORS_EXIT(stderr);
	 
	 ReleaseMaterial3ds(&mat);
	 PRINT_ERRORS_EXIT(stderr);
      }

      ReleaseNameList3ds(&matlist);
      PRINT_ERRORS_EXIT(stderr);
   }

   if ((dupwhat == N_DIRECT_LIGHT) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *omnilist = NULL;
      light3ds *omni = NULL;
      ulong3ds i;

      GetOmnilightNameList3ds(idb, &omnilist);
      PRINT_ERRORS_EXIT(stderr);

      for (i = 0; i < omnilist->count; i++)
      {
	 GetOmnilightByName3ds(idb, omnilist->list[i].name, &omni);
	 PRINT_ERRORS_EXIT(stderr);

	 PutOmnilight3ds(odb, omni);
	 PRINT_ERRORS_EXIT(stderr);

	 ReleaseLight3ds(&omni);
	 PRINT_ERRORS_EXIT(stderr);
      }

      ReleaseNameList3ds(&omnilist);
      PRINT_ERRORS_EXIT(stderr);
   }

   if ((dupwhat == DL_SPOTLIGHT) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *spotlist = NULL;
      light3ds *spot = NULL;
      ulong3ds i;

      GetSpotlightNameList3ds(idb, &spotlist);
      PRINT_ERRORS_EXIT(stderr);

      for (i = 0; i < spotlist->count; i++)
      {
	 GetSpotlightByName3ds(idb, spotlist->list[i].name, &spot);
	 PRINT_ERRORS_EXIT(stderr);

	 PutSpotlight3ds(odb, spot);
	 PRINT_ERRORS_EXIT(stderr);

	 ReleaseLight3ds(&spot);
	 PRINT_ERRORS_EXIT(stderr);
      }

      ReleaseNameList3ds(&spotlist);
      PRINT_ERRORS_EXIT(stderr);
   }

   if ((dupwhat == KFHDR) || (dupwhat == NULL_CHUNK))
   {
      kfsets3ds *key = NULL;

      GetKfSets3ds(idb, &key);
      PRINT_ERRORS_EXIT(stderr);

      PutKfSets3ds(odb, key);
      PRINT_ERRORS_EXIT(stderr);

      ReleaseKfSets3ds(&key);
   }

   if ((dupwhat == CAMERA_NODE_TAG) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *camnodelist = NULL;
      kfcamera3ds *cam = NULL;
      ulong3ds i;

      GetCameraNodeNameList3ds(idb, &camnodelist);
      PRINT_ERRORS_EXIT(stderr);

      for (i = 0; i < camnodelist->count; i++)
      {
	 GetCameraMotionByName3ds(idb, camnodelist->list[i].name, &cam);
	 PRINT_ERRORS_EXIT(stderr);

	 PutCameraMotion3ds(odb, cam);
	 PRINT_ERRORS_EXIT(stderr);

	 ReleaseCameraMotion3ds(&cam);
      }
   }

   if ((dupwhat == OBJECT_NODE_TAG) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *objlist = NULL;
      kfmesh3ds *mesh = NULL;
      ulong3ds i;

      GetObjectNodeNameList3ds(idb, &objlist);
      PRINT_ERRORS_EXIT(stderr);

      for (i = 0; i < objlist->count; i++)
      {
	 GetObjectMotionByName3ds(idb, objlist->list[i].name, &mesh);
	 PRINT_ERRORS_EXIT(stderr);

	 PutObjectMotion3ds(odb, mesh);
	 PRINT_ERRORS_EXIT(stderr);

	 ReleaseObjectMotion3ds(&mesh);
      }

   }

   if ((dupwhat == SPOTLIGHT_NODE_TAG) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *spotlist = NULL;
      kfspot3ds *spot = NULL;
      ulong3ds i;

      GetSpotlightNodeNameList3ds(idb, &spotlist);
      PRINT_ERRORS_EXIT(stderr);

      for (i = 0; i < spotlist->count; i++)
      {
	 GetSpotlightMotionByName3ds(idb, spotlist->list[i].name, &spot);
	 PRINT_ERRORS_EXIT(stderr);

	 PutSpotlightMotion3ds(idb, spot);
	 PRINT_ERRORS_EXIT(stderr);

	 ReleaseSpotlightMotion3ds(&spot);
      }
   }

   if ((dupwhat == AMBIENT_NODE_TAG) || (dupwhat == NULL_CHUNK))
   {
      kfambient3ds *amb;

      GetAmbientLightMotion3ds(idb, &amb);
      PRINT_ERRORS_EXIT(stderr);

      PutAmbientLightMotion3ds(odb, amb);
      PRINT_ERRORS_EXIT(stderr);
   }

   if ((dupwhat == LIGHT_NODE_TAG) || (dupwhat == NULL_CHUNK))
   {
      namelist3ds *lightlist = NULL;
      kfomni3ds *omni = NULL;
      ulong3ds i;

      GetOmnilightNodeNameList3ds(idb, &lightlist);
      PRINT_ERRORS_EXIT(stderr);

      for (i = 0; i < lightlist->count; i++)
      {
	 GetOmnilightMotionByName3ds(idb, lightlist->list[i].name, &omni);
	 PRINT_ERRORS_EXIT(stderr);

	 PutOmnilightMotion3ds(odb, omni);
	 PRINT_ERRORS_EXIT(stderr);

	 ReleaseOmnilightMotion3ds(&omni);
      }
   }

   WriteDatabase3ds(ofile, odb);
   PRINT_ERRORS_EXIT(stderr);
   
   ReleaseDatabase3ds(&idb);
   PRINT_ERRORS_EXIT(stderr);
   
   ReleaseDatabase3ds(&odb);
   PRINT_ERRORS_EXIT(stderr);
    
   CloseAllFiles3ds();
   PRINT_ERRORS_EXIT(stderr);
   
   
}

/* History:
   $Log: /Plugins/3dsftk/duptest/duptest.c $
 * 
 * 1     2/14/97 4:14p Christer
 * 
 * 1     2/14/97 4:10p Christer
 * 
 * 1     2/14/97 4:06p Christer
 * Revision 1.6  1994/06/13  08:27:09  grantb
 * Added more objects to the test.
 *
 * Revision 1.5  1994/06/11  07:52:29  grantb
 * Added tests for more things.
 *
 * Revision 1.4  1994/06/09  18:28:38  grantb
 * 1. Changed the spelling of "DataBase" to "Database"
 * 2. Took out smartall references.
 * 3. Added profuse error checks.
 *
 * Revision 1.3  1994/06/01  18:39:24  grantb
 * 1. Changed to work with 3dsftk.h
 *
*/

