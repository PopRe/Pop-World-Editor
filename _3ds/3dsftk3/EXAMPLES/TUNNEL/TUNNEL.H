/******************************************************************************
      TUNNEL.H - Example Routine For The 3DS File Library

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

*****************************************************************************/

void fillring (float3ds *cen, float3ds *seg, double alXY, double alZ);
void loctoplite (float3ds *seg, float3ds *baf, float3ds alXY, float3ds alZ);
void AddTunnel(database3ds *db);
void CalcMotionPos();
void MoveLeft(kfspot3ds *sl);
void MoveRight (kfspot3ds *sl);
void MoveTop (kfomni3ds *ol);
void MoveDriver (kfcamera3ds *dr);
void CreateTunnel(database3ds *db);




