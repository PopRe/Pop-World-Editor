/******************************************************************************
      TUNNEL.C - Example Routine For The 3DS File Library

     (C) Copyright 1992 by Autodesk, Inc.

      This program is copyrighted by Autodesk, Inc. and is  licensed
      to you under the following conditions.  You may not distribute
      or  publish the source code of this program in any form.   You
      may  incorporate this code in object form in derivative  works
      provided  such  derivative  works  are (i.) are  designed and
      intended  to  work  solely  with  Autodesk, Inc. products, and
     (ii.)  contain  Autodesk's  copyright  notice  "(C)  Copyright
      1992 by Autodesk, Inc."

      AUTODESK  PROVIDES THIS PROGRAM "AS IS" AND WITH  ALL  FAULTS.
      AUTODESK  SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF  MER-
      CHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK,  INC.
      DOES  NOT  WARRANT THAT THE OPERATION OF THE PROGRAM  WILL  BE
      UNINTERRUPTED OR ERROR FREE.

*******************************************************************************/

/* TUNNEL is a more complex example of how to use the interface library
          and data structures. Tunnel generates different geometry every
          time and shows texture mapping. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "3dsftk.h"

#include "tunnel.h"

static char3ds file_name[250];
long3ds anlength=18;
float3ds maxrate=16.0F;

static void ParseCmdLine(int argc, char *argv[]);
static void Banner();

static void ParseCmdLine(int argc, char *argv[])
{
   long3ds i;
   char3ds *cptr;
   long3ds gotname=0;

   for(i=1; i<argc; i++)
   {
      cptr = argv[i];
      if(*cptr == '-' && i+1 < argc) {
         cptr++;
         if(*cptr == 'v' || *cptr == 'V') {
            i++;
            maxrate = (float3ds)atoi(argv[i]);
         }
         else if(*cptr == 'l' || *cptr == 'L') {
                 i++;
                 anlength = atoi(argv[i]);
              }
              else if(*cptr == 'f' || *cptr == 'F') {
                      i++;
                      cptr = argv[i];
                      strcpy(file_name, cptr);
                      gotname++;
                   }
      }
   }
   if(!gotname)
      strcpy(file_name, "tunnel.3ds");
}

static void Banner()
{
   printf("\n3D Studio Tunnel Animation Generator\n\n");
   printf("\tThis program will generate a complete animation ready\n");
   printf("\tto render in 3D studio. The tunnel generated is random\n");
   printf("\tand fully 3D.\n\n");
   printf("USAGE:\tTUNNEL\n\t");
   printf("-l <number frames> - 18 minimum\n\t");
   printf("-v <maximum velocity> -(2, 4, 8, 16, 32, 64...) 32 recommended\n\t");
   printf("-f <file name> - with extension\n\n");
}

static void makeTrulyRandom()
{
  time_t t;
  struct tm *tim;
  unsigned seed;

  time(&t);
  tim = gmtime(&t);
  seed =(unsigned)(tim->tm_sec *
           (tim->tm_min + tim->tm_hour + tim->tm_mon + 5) * 
           tim->tm_yday + tim->tm_year + tim->tm_mday);
  srand(seed);
}

void main(int argc, char *argv[])
{
  database3ds *db = NULL;
  file3ds *file;

  if(argc == 1){
    Banner();
    exit(0);
  }
  
  ParseCmdLine(argc, argv);
  
  printf("3D Studio Tunnel Animation Generator\n\n");
  printf("Creating tunnel animation file %s.\n", file_name);
  
  makeTrulyRandom();
  
  ClearErrList3ds();
  
  file = OpenFile3ds(file_name, "w");
  PRINT_ERRORS_EXIT(stderr);
  
  InitDatabase3ds(&db);
  PRINT_ERRORS_EXIT(stderr);
  
  CreateNewDatabase3ds(db, MeshFile);
  PRINT_ERRORS_EXIT(stderr);
  
  CreateTunnel(db);
  PRINT_ERRORS_EXIT(stderr);
  
  WriteDatabase3ds(file, db);
  PRINT_ERRORS_EXIT(stderr);
  
#ifdef _DEBUG   
  DumpDatabase3ds(stdout, db);
  PRINT_ERRORS_EXIT(stderr);
#endif

  ReleaseDatabase3ds(&db);
  PRINT_ERRORS_EXIT(stderr);
  
  CloseAllFiles3ds(file);
  PRINT_ERRORS_EXIT(stderr);
  
  exit(0);
}


