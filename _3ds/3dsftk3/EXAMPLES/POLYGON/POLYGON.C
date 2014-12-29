/******************************************************************************
      POLYGON.C - Example Routine For The 3DS File Library

      (C) Copyright 1992 by Autodesk, Inc.

      This program is copyrighted by Autodesk, Inc. and is  licensed
      to you under the following conditions.  You may not distribute
      or  publish the source code of this program in any form.   You
      may  incorporate this code in object form in derivative  works
      provided  such  derivative  works  are  (i.) are  designed and 
      intended  to  work  solely  with  Autodesk, Inc. products, and
      (ii.)  contain  Autodesk's  copyright  notice  "(C)  Copyright  
      1992 by Autodesk, Inc."

      AUTODESK  PROVIDES THIS PROGRAM "AS IS" AND WITH  ALL  FAULTS.
      AUTODESK  SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF  MER-
      CHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK,  INC.
      DOES  NOT  WARRANT THAT THE OPERATION OF THE PROGRAM  WILL  BE
      UNINTERRUPTED OR ERROR FREE.

*******************************************************************************/

/* POLYGON is a very simple example of how to use the interface library
           and data structures */


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "3dsftk.h"

/* Some macros to make initialization easier */
#define SetPoint(p, a, b, c) (p).x = a; (p).y = b; (p).z = c;
#define SetFace(f, a, b, c, d) (f).v1 = a; (f).v2 = b; (f).v3 = c; (f).flag = d;
#define FileName3ds "polygon.3ds"

void CreatePolygon();

void main ()
{
   file3ds *file;
   database3ds *db = NULL;
   meshset3ds *mset = NULL;

   printf ("\n3D Polygon Animation Generator\n\n");
   printf ("Creating polygon animation file %s\n", FileName3ds);

   /* Clear error list, not necessary but safe */
   ClearErrList3ds();
   
   file = OpenFile3ds(FileName3ds, "w");
   PRINT_ERRORS_EXIT(stderr);

   InitDatabase3ds(&db);
   PRINT_ERRORS_EXIT(stderr);
    
   CreateNewDatabase3ds(db, MeshFile);
   PRINT_ERRORS_EXIT(stderr);

   CreatePolygon(db);
   PRINT_ERRORS_EXIT(stderr);

   WriteDatabase3ds(file, db);
   PRINT_ERRORS_EXIT(stderr);

   CloseAllFiles3ds(file);
   PRINT_ERRORS_EXIT(stderr);

   ReleaseDatabase3ds(&db);
   PRINT_ERRORS_EXIT(stderr);

   exit(0);
}

void CreatePolygon(database3ds *db)
{
   int i;
   material3ds *matr     = NULL;
   mesh3ds *mobj         = NULL;
   light3ds *slite       = NULL;
   kfspot3ds *kflite     = NULL;
   camera3ds *cam        = NULL;
   kfcamera3ds *kfcam    = NULL;
   kfmesh3ds *kobj       = NULL;
   meshset3ds *mesh      = NULL;
   background3ds *bgnd   = NULL;   
   atmosphere3ds *atmo   = NULL;
   kfambient3ds *amlite  = NULL; 
   kfsets3ds *kfsets     = NULL;
   viewport3ds *view     = NULL;

   /*--- MESH SETTINGS */
   InitMeshSet3ds(&mesh);
   ON_ERROR_RETURN;
   
   mesh->ambientlight.r = mesh->ambientlight.g = mesh->ambientlight.b = 0.3F;
   PutMeshSet3ds(db, mesh);  
   ON_ERROR_RETURN;
   ReleaseMeshSet3ds(&mesh);
   ON_ERROR_RETURN;
   
   /*--- BACKGROUND */
   InitBackground3ds(&bgnd);
   ON_ERROR_RETURN;

   /*--- Set-up Gradient */
   bgnd->vgradient.top.r = 0.5F;
   bgnd->vgradient.top.g = 0.0F;
   bgnd->vgradient.top.b = 0.0F;

   bgnd->vgradient.mid.r = 1.0F;
   bgnd->vgradient.mid.g = 0.0F;
   bgnd->vgradient.mid.b = 0.5F;

   bgnd->vgradient.bottom.r = 0.0F;
   bgnd->vgradient.bottom.g = 0.0F;
   bgnd->vgradient.bottom.b = 0.0F;
   bgnd->vgradient.gradpercent = 0.5F;

   bgnd->bgndused = UseVGradientBgnd;
   PutBackground3ds(db, bgnd);
   ON_ERROR_RETURN;
   ReleaseBackground3ds(&bgnd);
   ON_ERROR_RETURN;

   /*--- ATMOSPHERE */
   InitAtmosphere3ds(&atmo);
   ON_ERROR_RETURN;

   atmo->fog.nearplane = 0.0F;
   atmo->fog.neardensity = 0.0F;
   atmo->fog.farplane = 100.0F;
   atmo->fog.fardensity = 100.0F;
   atmo->fog.fogcolor.r = 0.8F;
   atmo->fog.fogcolor.g = 0.0F;
   atmo->fog.fogcolor.b = 0.0F;
   atmo->fog.fogbgnd = True3ds;
   atmo->activeatmo = UseFog;
   PutAtmosphere3ds(db, atmo);
   ON_ERROR_RETURN;
   ReleaseAtmosphere3ds(&atmo);
   ON_ERROR_RETURN;

   
   /*--- CAMERA */
   InitCamera3ds(&cam);
   ON_ERROR_RETURN;

   strcpy(cam->name, "Camera1");
   cam->position.x = 65.0F;
   cam->position.y = -3440.0F;
   cam->position.z = 185.0F;
   cam->target.x = 25.0F;
   cam->target.y = 300.0F;
   cam->target.z = 3000.0F;
   cam->ranges.cam_near = (float3ds)4000.0;
   cam->ranges.cam_far  = (float3ds)9000.0;
   cam->roll = 0.0F;
   cam->fov = 60.0F;
   cam->showcone = True3ds;

   PutCamera3ds(db, cam);
   ON_ERROR_RETURN;
   ReleaseCamera3ds(&cam);
   ON_ERROR_RETURN;

   /*--- SPOTLIGHT */
   InitSpotlight3ds(&slite);
   ON_ERROR_RETURN;

   strcpy(slite->name, "Spot1");
   slite->pos.x = 1000.0F;
   slite->pos.y = -4500.0F;
   slite->pos.z = 3000.0F;
   slite->color.r = 0.1F;
   slite->color.g = 0.2F;
   slite->color.b = 0.3F;

   slite->dloff = False3ds;
   slite->attenuation.on = False3ds;
   slite->attenuation.inner = 10.0F;
   slite->attenuation.outer = 100.0F;

   slite->spot->target.x = 0.0F;
   slite->spot->target.y = 0.0F;
   slite->spot->target.z = 0.0F;
   slite->spot->hotspot = 44.0F;
   slite->spot->falloff = 45.0F;
   slite->spot->roll = 0.0F;
   slite->spot->aspect = 1.0F;
   slite->spot->shadows.cast = False3ds;
   slite->spot->shadows.type = UseShadowMap;
   slite->spot->shadows.local = False3ds;
   slite->spot->shadows.bias = 1.0F;
   slite->spot->shadows.filter = 3.0F;
   slite->spot->shadows.mapsize = 512;
   slite->spot->shadows.raybias = 1.0F;
   slite->spot->cone.type = Circular;
   slite->spot->cone.show = True3ds;
   slite->spot->cone.overshoot = False3ds;
   slite->spot->projector.use = False3ds;
   slite->spot->projector.bitmap = NULL;
   
   PutSpotlight3ds(db, slite);
   ON_ERROR_RETURN;
   ReleaseLight3ds(&slite);
   ON_ERROR_RETURN;

   /*--- MATERIAL */
   InitMaterial3ds(&matr);
   ON_ERROR_RETURN;

   strcpy(matr->name, "RedSides");

   matr->ambient.r = 0.0F;
   matr->ambient.g = 0.0F;
   matr->ambient.b = 0.0F;

   matr->diffuse.r = 1.0F;
   matr->diffuse.g = 0.0F;
   matr->diffuse.b = 0.0F;

   matr->specular.r = 1.0F;
   matr->specular.g = 0.7F;
   matr->specular.b = 0.65F;

   matr->shininess = 0.0F;
   matr->shinstrength = 0.0F;
   matr->blur = 0.2F;
   matr->transparency = 0.0F;
   matr->transfalloff = 0.0F;
   matr->selfillumpct = 0.0F;
   matr->wiresize = 1.0F;
   matr->shading = Phong;
   matr->useblur = True3ds;

   PutMaterial3ds(db, matr);
   ON_ERROR_RETURN;
   ReleaseMaterial3ds(&matr);
   ON_ERROR_RETURN;

   /*--- MESH OBJECT: Backdrop */
   InitMeshObj3ds(&mobj, 4, 2, InitNoExtras3ds);
   ON_ERROR_RETURN;

   strcpy(mobj->name, "Bdrop");

   SetPoint(mobj->vertexarray[0], -4500.0F, 2000.0F, -4500.0F)
   SetPoint(mobj->vertexarray[1],  4500.0F, 2000.0F, -4500.0F)
   SetPoint(mobj->vertexarray[2],  4500.0F, 3000.0F,  4500.0F)
   SetPoint(mobj->vertexarray[3], -4500.0F, 3000.0F,  4500.0F)

   SetFace(mobj->facearray[0], 0, 1, 2, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[1], 2, 3, 0, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);

   FillMatrix3ds(mobj);
   PutMesh3ds(db, mobj);
   ON_ERROR_RETURN;
   RelMeshObj3ds(&mobj);
   ON_ERROR_RETURN;

   /* Always remember to release the memory used by a mesh3ds pointer prior
      to using the pointer again so that the values of the last mesh will
      not be carried over. */

   /*--- MESH OBJECT: Poly1 */
   InitMeshObj3ds(&mobj, 12, 8, InitNoExtras3ds);
   ON_ERROR_RETURN;

   strcpy (mobj->name, "Poly1");

   SetPoint(mobj->vertexarray[0],  -46.62F,   -2.31F, -103.57F);
   SetPoint(mobj->vertexarray[1],  -46.75F,   -2.18F, -103.57F);
   SetPoint(mobj->vertexarray[2],  -46.89F,   -2.31F, -103.57F);
   SetPoint(mobj->vertexarray[3],  -46.75F,   -2.45F, -103.57F);
   SetPoint(mobj->vertexarray[4],   95.49F,   -2.55F,   42.42F);
   SetPoint(mobj->vertexarray[5],  -46.52F,  139.88F,   42.42F);
   SetPoint(mobj->vertexarray[6], -188.96F,   -2.08F,   42.42F);
   SetPoint(mobj->vertexarray[7], - 46.98F, -144.52F,   42.42F);
   SetPoint(mobj->vertexarray[8], - 46.62F,   -2.31F,  188.42F);
   SetPoint(mobj->vertexarray[9], - 46.75F,   -2.18F,  188.42F);
   SetPoint(mobj->vertexarray[10], -46.89F,   -2.31F,  188.42F);
   SetPoint(mobj->vertexarray[11], -46.75F,   -2.45F,  188.42F);

   SetFace(mobj->facearray[0], 0, 5,  4, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[1], 1, 6,  5, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[2], 2, 7,  6, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[3], 3, 4,  7, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[4], 4, 5,  9, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[5], 5, 6, 10, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[6], 6, 7, 11, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);
   SetFace(mobj->facearray[7], 7, 4,  8, 
       FaceABVisable3ds|FaceBCVisable3ds|FaceCAVisable3ds);

   mobj->nmats = 1;
   InitMeshObjField3ds (mobj, InitMatArray3ds);
   ON_ERROR_RETURN;

   InitMatArrayIndex3ds (mobj, 0, 7);
   ON_ERROR_RETURN;

   strcpy(mobj->matarray[0].name, "RedSides");
   for (i=0; i<7; i++) mobj->matarray[0].faceindex[i] = i;
   mobj->matarray[0].nfaces = 7;


   FillMatrix3ds(mobj);
   PutMesh3ds(db, mobj);
   ON_ERROR_RETURN;
   /* release later */

   /*--- KEYFRAMER SECTION */

   /*---Any object used in keyframer section must be defined already */

   /*--- SPOTLIGHT */
   InitSpotlightMotion3ds(&kflite, 3, 2, 1, 1, 1, 2);
   ON_ERROR_RETURN;

   /*--- POSITION */
   strcpy (kflite->name, "Spot1");
   kflite->pkeys[0].time = 0; 
   SetPoint(kflite->pos[0], 1000.0F, -4500.0F,  3000.0F);
   kflite->pkeys[1].time = 10;
   SetPoint(kflite->pos[1], 1000.0F,     0.0F,  3000.0F);
   kflite->pkeys[2].time = 15;
   SetPoint(kflite->pos[2], 1000.0F, -4500.0F, -1000.0F);

   /*--- COLOR */
   kflite->ckeys[0].time = 0;
   kflite->color[0].r = 0.1F;
   kflite->color[0].g = 0.2F;
   kflite->color[0].b = 0.3F;
   kflite->ckeys[1].time = 30;
   kflite->color[1].r = 0.5F;
   kflite->color[1].g = 0.4F;
   kflite->color[1].b = 0.3F;
   /*--- HOTSPOT */
   kflite->hkeys[0].time = 0;
   kflite->hot[0] = 44.0F;
   /*--- FALLOFF */
   kflite->fkeys[0].time = 0;
   kflite->fall[0] = 45.0F;
   /*--- ROLL */
   kflite->rkeys[0].time = 0;
   kflite->roll[0] = 0.0F;

   kflite->flags1 = 20; 
   kflite->flags2 = 0; 

   kflite->tkeys[0].time = 0;
   SetPoint(kflite->tpos[0], 0.0F,  0.0F, 0.0F)
   kflite->tkeys[1].time = 30;
   SetPoint(kflite->tpos[1], 0.0F, 0.0F, 3000.0F)

   PutSpotlightMotion3ds(db, kflite); 
   ON_ERROR_RETURN;
   ReleaseSpotlightMotion3ds(&kflite);
   ON_ERROR_RETURN;

   /*--- CAMERA MOTION */
   InitCameraMotion3ds(&kfcam, 1, 5, 1, 5);
   ON_ERROR_RETURN;

   strcpy (kfcam->name, "Camera1");
   kfcam->ntflag = TrackLoops3ds;
   kfcam->flags2 = 1;

   kfcam->pkeys[0].time = 0;
   SetPoint(kfcam->pos[0], 65.0F, -3440.0F, 185.0F)

   kfcam->tkeys[0].time = 0;
   SetPoint(kfcam->tpos[0],  0.0F,   0.0F,   0.0F)
   kfcam->tkeys[1].time = 5;
   SetPoint(kfcam->tpos[1], 25.0F,   0.0F, 300.0F)
   kfcam->tkeys[2].time = 10;
   SetPoint(kfcam->tpos[2], 25.0F, -30.0F, 300.0F)
   kfcam->tkeys[3].time = 15;
   SetPoint(kfcam->tpos[3], 25.0F,   0.0F, 300.0F)
   kfcam->tkeys[4].time = 20;
   SetPoint(kfcam->tpos[4], 25.0F,  30.0F, 300.0F)

   kfcam->rkeys[0].time = 0;
   kfcam->roll[0] = 0.0F;

   kfcam->nfflag = TrackRepeats3ds;
   kfcam->fkeys[0].time = 0;
   kfcam->fov[0] = 20.0F;
   kfcam->fkeys[1].time = 5;
   kfcam->fov[1] = 30.0F;
   kfcam->fkeys[2].time = 10;
   kfcam->fov[2] = 20.0F;
   kfcam->fkeys[3].time = 15;
   kfcam->fov[3] = 33.5F;
   kfcam->fkeys[4].time = 20;
   kfcam->fov[4] = 20.0F;

   PutCameraMotion3ds(db, kfcam);   /* this is the last villan */
   ON_ERROR_RETURN;
   ReleaseCameraMotion3ds(&kfcam);
   ON_ERROR_RETURN;

   /*--- OBJECT MOTION POLY1 */
   InitObjectMotion3ds(&kobj, 6, 2, 1, 0, 0);
   ON_ERROR_RETURN;

   strcpy (kobj->name, "Poly1");

   /* Set the pivot point to the mesh's center */
   SetPoint(kobj->pivot, 0.0F, 0.0F, 0.0F);

   SetBoundBox3ds (mobj, kobj);
   ON_ERROR_RETURN;

   kobj->pkeys[0].time = 0;
   kobj->pos[0].x = (kobj->boundmax.x - kobj->boundmax.x) / 2.0F;
   kobj->pos[0].y = (kobj->boundmax.y - kobj->boundmax.y) / 2.0F;
   kobj->pos[0].z = (kobj->boundmax.z - kobj->boundmax.z) / 2.0F;

   kobj->pkeys[1].time = 8;
   SetPoint(kobj->pos[1], -421.5F, -1392.0F,  30.5F)
   kobj->pkeys[2].time = 15;
   SetPoint(kobj->pos[2],  403.0F, -1639.0F,  25.0F)
   kobj->pkeys[3].time = 24;
   SetPoint(kobj->pos[3], 1145.5F,  -484.0F,  71.5F)
   kobj->pkeys[4].time = 31;
   SetPoint(kobj->pos[4],  870.5F,   224.0F, 610.5F)
   kobj->pkeys[5].time = 40;
   SetPoint(kobj->pos[5],   10.5F,    30.0F,  57.0F)

   kobj->nrflag = TrackLoops3ds;
   kobj->nsflag = TrackSingle3ds;

   kobj->rkeys[0].time = 0;
   kobj->rot[0].angle = 0.0F;
   kobj->rot[0].x =  0.0F;
   kobj->rot[0].y =  0.0F;
   kobj->rot[0].z = -1.0F;

   kobj->rkeys[1].time = 20;
   kobj->rot[1].angle = 3.1415F;
   kobj->rot[1].x =  0.0F;
   kobj->rot[1].y =  0.0F;
   kobj->rot[1].z = -1.0F;


   PutObjectMotion3ds(db, kobj);
   ON_ERROR_RETURN;
   ReleaseObjectMotion3ds (&kobj);
   ON_ERROR_RETURN;

   RelMeshObj3ds(&mobj);

   /*--- SET KEYFRAMER AMBIENT LIGHT */
   InitAmbientLightMotion3ds(&amlite, 1);
   ON_ERROR_RETURN;
   amlite->flags1 = (ushort3ds)10;
   amlite->ckeys[0].time = 15;
   amlite->ckeys[0].rflags = (ushort3ds)40;
   amlite->color[0].r = amlite->color[0].g = amlite->color[0].b = 0.5F;
   PutAmbientLightMotion3ds(db, amlite);
   ON_ERROR_RETURN;
   ReleaseAmbientLightMotion3ds(&amlite);
   ON_ERROR_RETURN;

   /*--- KEYFRAMER ANIMATION DATA */
   InitKfSets3ds(&kfsets);
   ON_ERROR_RETURN;
   kfsets->anim.length = 60;
   kfsets->anim.curframe = 0;
   kfsets->seg.use = False3ds;

   PutKfSets3ds(db, kfsets);
   ON_ERROR_RETURN;
   ReleaseKfSets3ds(&kfsets);
   ON_ERROR_RETURN;

   /*--- Viewport setup */
   InitViewport3ds(&view);
   ON_ERROR_RETURN;

   view->type = CameraView3ds;
   strcpy(view->camera.name, "Camera1");

   PutViewport3ds(db, view); 
   ON_ERROR_RETURN;
      
   ReleaseViewport3ds(&view);
   ON_ERROR_RETURN;

}
