/******************************************************************************
  TUNNEL2.C - Example Routine For The 3DS File Library
  
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "3dsftk.h"

#include "tunnel.h"

extern long3ds anlength; /* animation length set in tunnel.c */
extern float3ds maxrate; /* speed through tunnel set in tunnel.c */
float3ds *align;
float3ds *mpos;
float3ds *croll;
short3ds *tdir;
long3ds ntunseg;

float3ds hmtx[12] = {
      1.0F,     0.0F,          0.0F,
      0.0F,     0.939693F,     0.342020F,
      0.0F,    -0.342020F,     0.939693F,
  10000.0F, 10000.0F,      10000.0F, 
};

float3ds hmtx2[12] = {
  1.0F,         0.0F,          0.0F,
  0.0F,         0.939693F,    -0.342020F,
  0.0F,         0.242020F,     0.939693F,
  10000.0F, 10000.0F,      10000.0F, 
};

float3ds llite[9] = {  /* was double */
  9998.75F, 10003.0F, 9999.3F, 
  9998.75F, 10008.0F, 9999.3F, 
     0.0F,     0.0F,    0.0F,
};

float3ds rlite[9] = { /* was double */
  10001.25F, 10003.0F, 9999.3F,
  10001.25F, 10008.0F, 9999.3F, 
      0.0F,      0.0F,    0.0F,
};

/* Tunnel orientation matrix */
float3ds tmtx[12] = { 
  1.0F, 0.0F, 0.0F, 
  0.0F, 1.0F, 0.0F, 
  0.0F, 0.0F, 1.0F, 
  0.0F, 0.0F, 0.0F,
};

/* Constants for texture assignment to each tunnel segment */
textvert3ds tuv[18] = {
  {0.5625F, 0.0F},
  {0.6875F, 0.0F},
  {0.8125F, 0.0F},
  {0.9375F, 0.0F},
  {0.0F,    0.0F},
  {0.0625F, 0.0F},
  {0.1875F, 0.0F},
  {0.3125F, 0.0F},
  {0.4375F, 0.0F},
  {0.5625F, 1.0F},
  {0.6875F, 1.0F},
  {0.8125F, 1.0F},
  {0.9375F, 1.0F},
  {0.0F,    1.0F},
  {0.0625F, 1.0F},
  {0.1875F, 1.0F},
  {0.3125F, 1.0F},
  {0.4375F, 1.0F}
};

/* Face list for tunnel segment */
face3ds tface[18] = { 
  0,  1,  9,  5,
  1, 10,  9,  6,
  1,  2, 10,  5,
  2, 11, 10,  6,
  2,  3, 11,  5,
  3, 12, 11,  6,
  3,  4, 12, 13,
  4, 13, 12, 14,
  4,  5, 13,  5,
  5, 14, 13,  6,
  5,  6, 14,  5,
  6, 15, 14,  6,
  6,  7, 15,  5,
  7, 16, 15,  6,
  7,  8, 16,  5,
  8, 17, 16,  6,
  8,  0, 17,  5,
  0,  9, 17,  6};


#define drand() ((double)rand()/RAND_MAX)
#define SetPoint(p, a, b, c) (p).x = a; (p).y = b; (p).z = c;
#define SetFace(f, a, b, c, d) (f).v1 = a; (f).v2 = b; (f).v3 = c; (f).flag = d;
#define SetColor(c, x, y, z) (c).r = x; (c).g = y; (c).b = z;
#define PI 3.14159265358979323846264338327950288419716939937510F

void CreateTunnel (database3ds *db)
{
  long3ds i;
  
  meshset3ds *mesh =       NULL;
  background3ds *bgnd =    NULL;
  atmosphere3ds *atmo =    NULL;
  light3ds *slite =        NULL;
  light3ds *omni =         NULL;
  camera3ds *cam =         NULL;
  viewport3ds *view =      NULL;
  material3ds *matr =      NULL;
  kfsets3ds *kfsets =  NULL;
  kfspot3ds *kflite =      NULL;
  kfomni3ds *kfomni =      NULL;
  kfcamera3ds *kfcam =     NULL;
  
  
  align = NULL;
  
 /*-----------------------------------
  | MESH SETTINGS
  +-----------------------------------*/
  InitMeshSet3ds(&mesh);
  ON_ERROR_RETURN;
  
  mesh->ambientlight.r = 0.0F;
  mesh->ambientlight.g = 0.0F;
  mesh->ambientlight.b = 0.0F;
  mesh->masterscale = 1.0F;
  mesh->shadow.mapsize = 500;
  mesh->shadow.raybias = 3.0F;
  mesh->shadow.filter = 3.0F;
  mesh->shadow.type = UseShadowMap;
  
  PutMeshSet3ds(db, mesh);  
  ON_ERROR_RETURN;
  
  ReleaseMeshSet3ds(&mesh);
  ON_ERROR_RETURN;
  
 /*-----------------------------------
  | BACKGROUND
  +-----------------------------------*/
  InitBackground3ds(&bgnd);
  ON_ERROR_RETURN;
  
  bgnd->solid.color.r = 0.0F;
  bgnd->solid.color.g = 0.0F;
  bgnd->solid.color.b = 0.0F;
  bgnd->bgndused = UseSolidBgnd;
  PutBackground3ds(db, bgnd);
  ON_ERROR_RETURN;
  
  ReleaseBackground3ds(&bgnd);
  ON_ERROR_RETURN;
  

 /*-----------------------------------
  | ATMOSPHERE
  +-----------------------------------*/
  InitAtmosphere3ds(&atmo);
  ON_ERROR_RETURN;
  
  atmo->activeatmo = NoAtmo;
  
  PutAtmosphere3ds(db, atmo);
  ON_ERROR_RETURN;
  
  ReleaseAtmosphere3ds(&atmo);
  ON_ERROR_RETURN;
  
  
  for (i=0;i<6;i+=3) {
    llite[i] = (llite[i]-hmtx[9])*hmtx[0] + 
      (llite[i+1]-hmtx[10])*hmtx[1] + 
    (llite[i+2]-hmtx[11])*hmtx[2] + hmtx[9];
    
    llite[i+1] = (llite[i]-hmtx[9])*hmtx[3] + 
      (llite[i+1]-hmtx[10])*hmtx[4] + 
    (llite[i+2]-hmtx[11])*hmtx[5] + hmtx[10];
    
    llite[i+2] = (llite[i]-hmtx[9])*hmtx[6] + 
      (llite[i+1]-hmtx[10])*hmtx[7] + 
    (llite[i+2]-hmtx[11])*hmtx[8] + hmtx[11];
    
    rlite[i] = (rlite[i]-hmtx[9])*hmtx[0] + 
      (rlite[i+1]-hmtx[10])*hmtx[1] + 
    (rlite[i+2]-hmtx[11])*hmtx[2] + hmtx[9];
    
    rlite[i+1] = (rlite[i]-hmtx[9])*hmtx[3] + 
      (rlite[i+1]-hmtx[10])*hmtx[4] + 
    (rlite[i+2]-hmtx[11])*hmtx[5] + hmtx[10];
    
    rlite[i+2] = (rlite[i]-hmtx[9])*hmtx[6] + 
      (rlite[i+1]-hmtx[10])*hmtx[7] + 
    (rlite[i+2]-hmtx[11])*hmtx[8] + hmtx[11];
  }
  
  llite[6] = llite[0] - hmtx[9];
  llite[7] = llite[1] - hmtx[10];
  llite[8] = llite[2] - hmtx[11];
  rlite[6] = rlite[0] - hmtx[9];
  rlite[7] = rlite[1] - hmtx[10];
  rlite[8] = rlite[2] - hmtx[11];
  

 /*-----------------------------------
  | SPOTLIGHT
  +-----------------------------------*/
  InitSpotlight3ds(&slite);
  ON_ERROR_RETURN;
  
  strcpy(slite->name, "Left");
  SetPoint(slite->pos,          llite[0], llite[1], llite[2]);
  SetPoint(slite->spot->target, llite[3], llite[4], llite[5]);
  
  slite->color.r = 1.0F;
  slite->color.g = 1.0F;
  slite->color.b = 1.0F;
  
  slite->dloff = False3ds;
  slite->spot->hotspot = 40.0F;
  slite->spot->falloff = 80.0F;
  slite->spot->roll = 0.0F;
  slite->spot->shadows.bias = 0.0F;
  slite->spot->shadows.filter = 0.0F;
  slite->spot->shadows.mapsize = 0;
  slite->spot->shadows.raybias = 1.0F;
  slite->spot->cone.type = Circular;
  slite->spot->cone.show = False3ds;
  
  PutSpotlight3ds(db, slite);
  ON_ERROR_RETURN;
  
  /*--- Reuse light structure for right light */
  strcpy(slite->name, "Right");
  SetPoint(slite->pos, rlite[0], rlite[1], rlite[2]);
  SetPoint(slite->spot->target, rlite[3], rlite[4], rlite[5]);
  
  PutSpotlight3ds(db, slite);
  ON_ERROR_RETURN;
  
  ReleaseLight3ds(&slite);
  ON_ERROR_RETURN;


 /*-----------------------------------
  | OMNI LIGHT
  +-----------------------------------*/
  InitLight3ds(&omni);
  ON_ERROR_RETURN;
  
  strcpy(omni->name, "Top");
  SetPoint(omni->pos, 10000.0F, 9999.0F, 10002.5F);
  omni->color.r = 1.0F;
  omni->color.g = 0.3F;
  omni->color.b = 0.3F;
  
  PutOmnilight3ds(db, omni);
  ON_ERROR_RETURN;
  
  ReleaseLight3ds(&omni);
  ON_ERROR_RETURN;
  
  
 /*-----------------------------------
  | CAMERA
  +-----------------------------------*/
  InitCamera3ds(&cam);
  ON_ERROR_RETURN;
  
  strcpy(cam->name, "Driver");
  SetPoint(cam->position, 10000.0F, 10000.0F, 1000.7F);
  SetPoint(cam->target, 10000.0F, 10004.7F, 9998.99F);
  cam->showcone = False3ds;
  
  PutCamera3ds(db, cam);
  ON_ERROR_RETURN;
  
  ReleaseCamera3ds(&cam);
  ON_ERROR_RETURN;

 /*-----------------------------------
  | VIEWPORTS
  +-----------------------------------*/

  InitViewport3ds(&view);
  ON_ERROR_RETURN;

  view->type = CameraView3ds;
  strcpy(view->camera.name, "Driver");

  PutViewport3ds(db, view);
  ON_ERROR_RETURN;

  ReleaseViewport3ds(&view);
  ON_ERROR_RETURN;

 /*-----------------------------------
  | MATERIAL
  +-----------------------------------*/
  InitMaterial3ds(&matr);
  ON_ERROR_RETURN;
  
  strcpy (matr->name, "WALLS");
  SetColor(matr->diffuse,  0.5F, 0.0F, 0.3F);
  SetColor(matr->specular, 0.0F, 0.5F, 0.0F);
  SetColor(matr->ambient,  0.0F, 0.0F, 0.5F);
  matr->shininess = 0.25F;
  matr->shinstrength = 0.5F;
  matr->blur = 0.5F;
  matr->shading = Phong;
  matr->useblur = False3ds;
  strcpy(matr->texture.map.name, "PAT0027.TGA");
  matr->texture.map.percent = 0.80F; /*---80%---*/
  matr->texture.map.ignorealpha = False3ds;
  matr->texture.map.filter = False3ds;
  matr->texture.map.mirror = False3ds;
  matr->texture.map.negative =False3ds;
  matr->texture.map.source = RGB;
  matr->texture.map.blur = 0.07F;
  
  PutMaterial3ds(db, matr);
  ON_ERROR_RETURN;
  
  ReleaseMaterial3ds(&matr);
  ON_ERROR_RETURN;
  
  AddTunnel(db);
  ON_ERROR_RETURN;
  
  
 /*-----------------------------------
  | KEYFRAMER SECTION
  +-----------------------------------*/
  InitKfSets3ds(&kfsets);
  ON_ERROR_RETURN;
  
  kfsets->anim.length = anlength;
  kfsets->seg.end = anlength;
  PutKfSets3ds(db, kfsets);
  ON_ERROR_RETURN;
  
  ReleaseKfSets3ds(&kfsets);
  ON_ERROR_RETURN;
  
  CalcMotionPos();
  ON_ERROR_RETURN;
  
  if (align != NULL){
    free (align); align = NULL;
  }
  
  if (tdir != NULL){
    free (tdir); tdir = NULL;
  }
  

 /*-----------------------------------
  | SPOTLIGHT MOTION
  +-----------------------------------*/
  InitSpotlightMotion3ds(&kflite, anlength, 1, 1, 1, 0, anlength);
  ON_ERROR_RETURN;
  
  kflite->pkeys[0].time = 0;
  SetPoint(kflite->pos[0], llite[0], llite[1], llite[2]);
  kflite->tkeys[0].time = 0;
  SetPoint(kflite->tpos[0], llite[3], llite[4], llite[5]);
  
  kflite->hkeys[0].time = 0;
  kflite->hot[0] = 40.0F;
  kflite->fkeys[0].time = 0;
  kflite->fall[0] = 80.0F;
  kflite->ckeys[0].time = 0;
  kflite->color[0].r = 1.0F;
  kflite->color[0].g = 1.0F;
  kflite->color[0].b = 1.0F;
  strcpy (kflite->name, "Left");
  
  MoveLeft (kflite);
  
  PutSpotlightMotion3ds(db, kflite); 
  ON_ERROR_RETURN;
  
  SetPoint(kflite->pos[0],  rlite[0], rlite[1], rlite[2]);
  SetPoint(kflite->tpos[0], rlite[3], rlite[4], rlite[5]);
  strcpy (kflite->name, "Right");
  
  MoveRight (kflite);
  
  PutSpotlightMotion3ds(db, kflite); 
  ON_ERROR_RETURN;
  
  ReleaseSpotlightMotion3ds(&kflite);
  ON_ERROR_RETURN;
  
 /*-----------------------------------
  | OMNILIGHT MOTION
  +-----------------------------------*/
  InitOmnilightMotion3ds(&kfomni, anlength, anlength);
  ON_ERROR_RETURN;
  
  strcpy (kfomni->name, "Top");
  kfomni->pkeys[0].time = 0;
  SetPoint(kfomni->pos[0], 10000.0F, 9999.0F, 10002.5F)
    kfomni->ckeys[0].time = 0;
  SetColor(kfomni->color[0], 1.0F, 0.3F, 0.3F);
  
  MoveTop (kfomni);
  
  PutOmnilightMotion3ds(db, kfomni);
  ON_ERROR_RETURN;
  
  ReleaseOmnilightMotion3ds(&kfomni);
  ON_ERROR_RETURN;
  

 /*-----------------------------------
  | CAMERA MOTION
  +-----------------------------------*/
  InitCameraMotion3ds(&kfcam, anlength, 1, 1, anlength);
  ON_ERROR_RETURN;
  
  strcpy (kfcam->name, "Driver");
  kfcam->pkeys[0].time = 0;
  SetPoint(kfcam->pos[0],  10000.0F, 10000.0F, 10000.7F);
  kfcam->tkeys[0].time = 0;
  SetPoint(kfcam->tpos[0], 10000.0F, 10004.7F, 9998.99F);
  kfcam->fkeys[0].time = 0;
  kfcam->fov[0] = 60.0F;
  kfcam->rkeys[0].time = 0;
  kfcam->roll[0] = 0.0F;
  
  MoveDriver(kfcam);
  
  PutCameraMotion3ds(db, kfcam);
  ON_ERROR_RETURN;
  
  ReleaseCameraMotion3ds(&kfcam);
  ON_ERROR_RETURN;
}

void fillring (float3ds *cen, float3ds *seg, double alXY, double alZ)
{
  float3ds offZ, offY, offX, d1;
  
  offZ = (float3ds)cos ((double)alZ);
  offX = (float3ds)cos ((double)alXY);
  offY = (float3ds)sin ((double)alXY);
  
  d1      = 5.0F * (float3ds)sin (alZ);
  seg[0]  = seg[24] = cen[0] + d1 * offX;
  seg[1]  = seg[25] = cen[1] + d1 * offY;
  seg[9]  = seg[12] = seg[15] = cen[0] - d1 * offX;
  seg[10] = seg[13] = seg[16] = cen[1] - d1 * offY;
  
  d1      = 1.91F * (float3ds)sin (alZ);
  seg[3]  = seg[21] = cen[0] + d1 * offX;
  seg[4]  = seg[22] = cen[1] + d1 * offY;
  seg[6]  = seg[18] = cen[0] - d1 * offX;
  seg[7]  = seg[19] = cen[1] - d1 * offY;
  
  seg[2]  = seg[26] = cen[2] - 5.0F * offZ;
  seg[5]  = seg[23] = cen[2] - 1.91F * offZ;
  seg[8]  = seg[20] = cen[2] + 1.91F * offZ;
  seg[11] = seg[14] = seg[17] = cen[2] + 5.0F * offZ;
  
  d1   = offX;
  offX = (float3ds)fabs(offY);
  if (alXY > PI)
    offX = -offX;
  offY = (float3ds)fabs(d1);
  if (alXY < PI * 0.5 || alXY > PI * 1.5)
    offY = -offY;
  
  seg[0]  += 1.91F * offX;
  seg[1]  += 1.91F * offY;
  seg[3]  += 5.0F * offX;
  seg[4]  += 5.0F * offY;
  seg[6]  += 5.0F * offX;
  seg[7]  += 5.0F * offY;
  seg[9]  += 1.91F * offX;
  seg[10] += 1.91F * offY;
  seg[15] -= 1.91F * offX;
  seg[16] -= 1.91F * offY;
  seg[18] -= 5.0F * offX;
  seg[19] -= 5.0F * offY;
  seg[21] -= 5.0F * offX;
  seg[22] -= 5.0F * offY;
  seg[24] -= 1.91F * offX;
  seg[25] -= 1.91F * offY;
  
}

void loctoplite (float3ds *seg, float3ds *baf, float3ds alXY, float3ds alZ)
{
  float3ds offZ, offY, offX, d1;
  
  offZ   = (float3ds)sin (alZ);
  baf[2] = seg[14] + 14.0F * offZ;
  baf[5] = seg[14] + 15.0F * offZ;
  
  offX   = (float3ds)cos ((double)alXY);
  offY   = (float3ds)sin ((double)alXY);
  offZ   = (float3ds)cos ((double)alZ);
  d1     = 14.0F * offZ;
  baf[0] = seg[12] + d1 * offX;
  baf[1] = seg[13] + d1 * offY;
  
  d1     = 15.0F * offZ;
  baf[3] = seg[12] + d1 * offX;
  baf[4] = seg[13] + d1 * offY;
  
  d1     = 0.5F * offZ;
  baf[2] = baf[2] - d1;
  baf[5] = baf[5] - d1;
  
}

#define D_NONE  0
#define D_LEFT  1
#define D_RIGHT 2
#define D_DOWN  3
#define D_UP    4

void AddTunnel(database3ds *db)
{
  ulong3ds     i, j, light_counter=0;
  short3ds    cdir = D_NONE;
  float3ds    *seg;
  float3ds    *baf, offX, offY, offZ, ndir, alXY, alZ, d1;
  char3ds     name[9];
  mesh3ds     *mobj = NULL;
  light3ds    *slite = NULL;
  
  ntunseg = 
    (long3ds)ceil (((double)(anlength-18)*maxrate+maxrate*4.0)/30.0) + 5;
  
  if ((seg = (float3ds*)malloc(18*sizeof(point3ds))) == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
  
  if ((baf = (float3ds*)malloc(6*sizeof(double))) == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
  
  if ((align = (float3ds*)malloc((ntunseg+1)*3*sizeof(float3ds))) == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
  
  if ((tdir = (short3ds*)malloc((ntunseg+1)*sizeof(short3ds))) == NULL)
    ON_ERROR_RETURN;
  
  offX =   0.0F;
  alXY =   1.5708F;
  offY =  28.19F;
  offZ = -10.26F;
  alZ  =  -0.3498F;
  
  align[0] = align[1] = align[2] = 10000.0F;
  fillring (&align[0], seg, alXY, alZ);
  
 /*-------------------------------------------
  | MESH OBJECT 
  | Make scratch mesh object for tunnel pieces 
  +-------------------------------------------*/
  InitMeshObj3ds(&mobj, 0, 0,InitNoExtras3ds); 
  ON_ERROR_RETURN;
  
  mobj->nvertices = 18;
  mobj->vertexarray = (point3ds*)seg;

 /*-----------------------------
  | TEXTURE MAPPING 
  +-----------------------------*/ 
  mobj->ntextverts = 18;
  mobj->textarray = (textvert3ds *)tuv;

  mobj->usemapinfo = True3ds;  /*--- Use Texture Map */
  
  mobj->nvflags = 18;
  mobj->vflagarray = (ushort3ds *)malloc(sizeof(short3ds) * 18);
  for (i=0; i<18; i++)
    mobj->vflagarray[i] = 256;
  

  /*--- Setup matrix */
  for (i=0; i<12; i++)
    mobj->locmatrix[i] = tmtx[i];
  
  mobj->nfaces = 18;
  mobj->facearray = tface;
  
 /*---------------------------
  | MATERIAL ASSIGNMENT
  +---------------------------*/ 
  mobj->nmats = 1;
  InitMeshObjField3ds (mobj, InitMatArray3ds);
  ON_ERROR_RETURN;
  
  InitMatArrayIndex3ds (mobj, 0, mobj->nfaces);
  ON_ERROR_RETURN;
  
  strcpy(mobj->matarray[0].name, "WALLS");
  for (i=0; i<mobj->nfaces; i++) 
    mobj->matarray[0].faceindex[i] = (ushort3ds)i;
  mobj->matarray[0].nfaces = mobj->nfaces; 
 

 /*-----------------------------------
  | SPOTLIGHT
  +-----------------------------------*/
  InitSpotlight3ds(&slite);
  ON_ERROR_RETURN;
  
  SetColor(slite->color, 0.98F, 0.98F, 0.98F);
  slite->dloff = False3ds;
  slite->attenuation.on = False3ds;
  slite->spot->hotspot = 120.0F;
  slite->spot->falloff = 160.0F;
  slite->spot->hotspot = 120.0F;
  slite->spot->hotspot = 120.0F;
  slite->spot->cone.show = False3ds;
  slite->spot->shadows.type =    UseShadowMap;
  slite->spot->shadows.bias =    0.75F;
  slite->spot->shadows.filter =  2.0F;
  slite->spot->shadows.mapsize = 100;
  
  for (i = 0; i < ntunseg; i++){
    align[(i+1)*3]   = align[i*3]   + offX;
    align[(i+1)*3+1] = align[i*3+1] + offY;
    align[(i+1)*3+2] = align[i*3+2] + offZ;
    
    fillring (&align[(i+1)*3], &seg[27], alXY, alZ);
    
    /*--- Make-up object name */
    if (i<9)
      sprintf (mobj->name, "Seg000%d", i+1);
    else if (i<99)
      sprintf (mobj->name, "Seg00%d", i+1);
    else if (i<999)
      sprintf (mobj->name, "Seg0%d", i+1);
    else
      sprintf (mobj->name, "Seg%d", i+1);
    
    /*--- Put in tunnel segment */
    PutMesh3ds(db, mobj);
    ON_ERROR_RETURN;
    
    /*--- Put in tunnel light */
    if (i && !fmod ((double)i, 3.0)) {
      light_counter++;
      if (light_counter<10)
    sprintf (name, "Lite000%d", light_counter);
      else if (light_counter<100)
    sprintf (name, "Lite00%d", light_counter);
      else if (light_counter<1000)
    sprintf (name, "Lite0%d", light_counter);
      else
    sprintf (name, "Lite%d", light_counter);
      
      loctoplite (seg, baf, alXY, alZ);
      
      strcpy(slite->name, name);
      SetPoint(slite->pos,          baf[0], baf[1], baf[2]);
      SetPoint(slite->spot->target, baf[3], baf[4], baf[5]);
      PutSpotlight3ds(db, slite);
      ON_ERROR_RETURN;
    }
    
    if (cdir == D_LEFT || cdir == D_RIGHT)
      tdir[i] = cdir;
    else
      tdir[i] = D_NONE;
    
    for (j=0;j<27;j++)
      seg[j] = seg[j+27];
    
    if (i > 4 && i < ntunseg-7 && drand() <= 0.20) {
      ndir = (float3ds)drand();
      if (ndir <= 0.35)
    cdir = D_LEFT;
      else if (ndir <= 0.70)
    cdir = D_RIGHT;
      else if (ndir <= 0.85)
    cdir = D_DOWN;
      else 
    cdir = D_UP;
    }
    else if (i >= ntunseg-7)
      cdir = D_NONE;
    
    switch (cdir) {
    case D_LEFT:
      alXY -= 0.0873F;
      break;
    case D_RIGHT:
      alXY += 0.0873F;
      break;
    case D_DOWN:
      alZ -= 0.0437F;
      if (alZ < -1.4) {
    cdir = D_UP;
    alZ += 0.0873F;
      }
      break;
    case D_UP:
      alZ += 0.0437F;
      if (alZ > 1.4) {
    cdir = D_DOWN;
    alZ -= 0.0873F;
      }
      break;
    }
    if (alXY < 0.0)
      alXY = 2.0F * PI + alXY;
    else if (alXY > PI * 2.0)
      alXY -= 2.0F * PI;
    offZ = 30.0F * (float3ds)sin ((double)alZ);
    d1   = 30.0F * (float3ds)cos ((double)alZ);
    offZ = 30.0F * (float3ds)sin ((double)alZ);
    offX = d1 * (float3ds)cos ((double)alXY);
    offY = d1 * (float3ds)sin ((double)alXY);
  }
  
  /*--- don't want to try to release static memory */
  mobj->vertexarray = NULL;
  mobj->facearray =   NULL;
  mobj->textarray =   NULL;

  free(mobj->vflagarray);
  mobj->vflagarray = NULL;
  
  RelMeshObj3ds(&mobj);
  
  free(seg);    
  free(baf);    
}

void CalcMotionPos()
{
  double d1, dx, dy, dz, td, pos=0, rate=0, proll=0;
  long3ds seg=0, time;
  
  if ((mpos = (float3ds*)malloc((anlength)*6*sizeof(float3ds))) == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
  
  if ((croll = (float3ds*)malloc((anlength)*sizeof(float3ds))) == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
  
  dx = (double)(align[(seg+1)*3] - align[seg*3]);
  dy = (double)(align[(seg+1)*3+1] - align[seg*3+1]);
  dz = (double)(align[(seg+1)*3+2] - align[seg*3+2]);
  td = sqrt (dx*dx+dy*dy+dz*dz);
  
  for (time=0;time<anlength;time++) {
    pos += rate;
    if (pos > td) {
      while (pos > td) {
    pos -= td;
    seg++;
    if (seg == ntunseg) {
      printf ("Warning end of tunnel reached at time %ld\n", time);
      seg--;
      pos = td;
    }
    else {
      dx = (double)(align[(seg+1)*3] - align[seg*3]);
      dy = (double)(align[(seg+1)*3+1] - align[seg*3+1]);
      dz = (double)(align[(seg+1)*3+2] - align[seg*3+2]);
      td = sqrt (dx*dx+dy*dy+dz*dz);
    }
      }
    }
    mpos[time*6]   = align[seg*3] + (float3ds)(dx * pos / td);
    mpos[time*6+1] = align[seg*3+1] + (float3ds)(dy * pos / td);
    mpos[time*6+2] = align[seg*3+2] + (float3ds)(dz * pos / td);
    d1 = pos + 5.0;
    mpos[time*6+3] = align[seg*3] + (float3ds)(dx * d1 / td);
    mpos[time*6+4] = align[seg*3+1] + (float3ds)(dy * d1 / td);
    mpos[time*6+5] = align[seg*3+2] + (float3ds)(dz * d1 / td);
    
    if (tdir[seg] == D_NONE) {
      if (tdir[seg+1] == D_NONE) {
    if (proll > 0.0)
      proll -= 2.0;
    else if (proll < 0.0)
      proll += 2.0;
      }
      else if (tdir[seg+1] == D_LEFT)
    proll += 2.0;
      else
    proll -= 2.0;
    }
    else if (tdir[seg] == D_LEFT) {
      if (tdir[seg+1] == D_LEFT)
    proll += 2.0;
      else
    proll -= 4.0;
    }
    else {
      if (tdir[seg+1] == D_RIGHT)
    proll -= 2.0;
      else
    proll += 4.0;
    }
    if (proll > 40.0)
      proll = 40.0;
    else if (proll < -40.0)
      proll = -40.0;
    croll[time] = (float3ds)proll;
    if (time == 0)
      rate = maxrate/128;
    else if (time >= anlength-9)
      rate /= 2.0;
    else if (rate <= maxrate)
      rate *= 2.0;
    if (rate > maxrate)
      rate = maxrate;
    else if (rate < 0.0)
      rate = 0.0;
  }
}

void MoveLeft(kfspot3ds *sl)
{
  long3ds i;
  
  for (i = 1; i < anlength; i++)
    {
      sl->pkeys[i].time = sl->tkeys[i].time = i + 1;
      
      SetPoint(sl->pos[i], 
           mpos[i*6]+llite[6], 
           mpos[i*6+1]+llite[7], 
           mpos[i*6+2]+llite[8]);
      
      SetPoint(sl->tpos[i], 
           mpos[i*6+3]+llite[6], 
           mpos[i*6+4]+llite[7], 
           mpos[i*6+5]+llite[8]);
    }
}

void  MoveRight (kfspot3ds *sl)
{
  long3ds i;
  
  for (i = 1; i < anlength; i++)
    {
      sl->pkeys[i].time = sl->tkeys[i].time = i;
      SetPoint(sl->pos[i], 
           mpos[i*6]+rlite[6], 
           mpos[i*6+1]+rlite[7], 
           mpos[i*6+2]+rlite[8]);
      
      SetPoint(sl->tpos[i], 
           mpos[i*6+3]+rlite[6], 
           mpos[i*6+4]+rlite[7], 
           mpos[i*6+5]+rlite[8]);
    }
}

void MoveTop (kfomni3ds *ol)
{
  long3ds i;
  for (i=1; i < anlength; i++)
    {
      ol->pkeys[i].time = ol->ckeys[i].time = i;
      
      SetPoint(ol->pos[i], 
           mpos[i*6], 
           mpos[i*6+1]-1.0F, 
           mpos[i*6+2]+2.5F);
      
      /* make the light flash on every 3 frames */
      if ((i % 3) == 0){
    SetColor(ol->color[i], 1.0F, 0.3F, 0.3F)
      }
      else{
    SetColor(ol->color[i], 0.0F, 0.0F, 0.0F)
      }
    }
}

void MoveDriver (kfcamera3ds *dr)
{
  long3ds i;
  
  for (i = 1; i < anlength; i++)
    {
      dr->pkeys[i].time = dr->tkeys[i].time = i;
      SetPoint(dr->pos[i], 
           mpos[i*6], 
           mpos[i*6+1], 
           mpos[i*6+2]+0.6F)
    SetPoint(dr->tpos[i], 
         mpos[i*6+3], 
         mpos[i*6+4], 
         mpos[i*6+5]+0.6F);
    }
}



















