
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "popedt.h"
#include "pop.h"
#include "engine.h"
#include "3ds.h"
#include "log.h"



#define M3DMAGIC				0x4D4D
#define MDATA					0x3D3D
#define NAMED_OBJECT			0x4000
#define N_TRI_OBJECT			0x4100
#define POINT_ARRAY				0x4110
#define FACE_ARRAY				0x4120
#define MSH_MAT_GROUP			0x4130
#define TEX_VERTS				0x4140
#define MAT_ENTRY				0xAFFF
#define MAT_NAME				0xA000
#define MAT_DIFFUSE				0xA020
#define MAT_SPECULAR			0xA030
#define MAT_TEXMAP				0xA200
#define MAT_MAPNAME				0xA300
#define COLOR_24				0x0011



#pragma pack(push, 1)


struct _3DS_MATERIAL_BYTE
{
	BYTE						r,
								g,
								b;
};


struct _3DS_UV
{
	float						u,
								v;
};


struct _3DS_FACE
{
	WORD						a,
								b,
								c,
								flag;
};


struct _3DS_POINT
{
	float						x,
								y,
								z;
};


struct CHUNK_HDR
{
	WORD						id;
	DWORD						length;
};


#pragma pack(pop)


struct CHUNK
{
	CHUNK_HDR					hdr;
	DWORD						offset;
};


struct _3DS_MAT_ENTRY
{
	_3DS_MAT_ENTRY				*Next,
								*Prev;
	char						name[16],
								texture_name[13];
	_3DS_MATERIAL_BYTE			diffuse,
								specular;
	TEXTURE						*texture;
};


struct _3DS_NAMED_OBJECT
{
	_3DS_NAMED_OBJECT			*Next,
								*Prev;
	WORD						VertsCount,
								FacesCount,
								UVCount;
	_3DS_POINT					*Verts;
	_3DS_FACE					*Faces;
	_3DS_UV						*UV;
	char						material[17];
};



#if DEBUG
void OutputDebugMsg(char *file, int line)
{
	sprintf(str, "Failed: %s:%d\n", file, line);
	OutputDebugString(str);
}
#endif


bool _3dsReadTexMap(HANDLE h, CHUNK *txchunk, _3DS_MAT_ENTRY *MatEntry)
{
	CHUNK_HDR hdr;
	DWORD dwCurrentOffset, dwLength, dwR;

	dwLength = txchunk->hdr.length - sizeof(CHUNK_HDR);
	dwCurrentOffset = txchunk->offset + sizeof(CHUNK_HDR);

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if(hdr.id == MAT_MAPNAME)
		{
			DWORD dw = hdr.length - sizeof(CHUNK_HDR) > 13 ? 13 : hdr.length - sizeof(CHUNK_HDR);

			dwR = 0;
			ReadFile(h, MatEntry->texture_name, dw, &dwR, 0);
			if(dwR != dw)
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}

			return true;
		}

		dwCurrentOffset += hdr.length;
		dwLength -= hdr.length;
	}

	return true;
}


bool _3dsReadMatColor(HANDLE h, CHUNK *matchunk, _3DS_MAT_ENTRY *MatEntry)
{
	DWORD dwCurrentOffset, dwLength, dwR;

	dwLength = matchunk->hdr.length - sizeof(CHUNK_HDR);
	dwCurrentOffset = matchunk->offset + sizeof(CHUNK_HDR);

	CHUNK_HDR hdr;
	memset(&hdr, 0, sizeof(CHUNK_HDR));

	_3DS_MATERIAL_BYTE *mb;

	if(matchunk->hdr.id == MAT_DIFFUSE)
		mb = &MatEntry->diffuse;
	else if(matchunk->hdr.id == MAT_SPECULAR)
		mb = &MatEntry->specular;
	else
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if(hdr.id == COLOR_24)
		{
			if(hdr.length != sizeof(CHUNK_HDR) + sizeof(_3DS_MATERIAL_BYTE))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}

			dwR = 0;
			ReadFile(h, mb, sizeof(_3DS_MATERIAL_BYTE), &dwR, 0);
			if(dwR != sizeof(_3DS_MATERIAL_BYTE))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}

		dwCurrentOffset += hdr.length;
		dwLength -= hdr.length;
	}

	return true;
}


bool _3dsReadMatEntry(HANDLE h, CHUNK *mtrlchunk, _3DS_MAT_ENTRY *MatEntry)
{
	CHUNK chunk;
	memset(&chunk, 0, sizeof(CHUNK));

	DWORD dwCurrentOffset, dwLength, dwR;

	dwLength = mtrlchunk->hdr.length - sizeof(CHUNK_HDR);
	dwCurrentOffset = mtrlchunk->offset + sizeof(CHUNK_HDR);

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &chunk.hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if(chunk.hdr.id == MAT_NAME)
		{
			dwR = 0;
			ReadFile(h, MatEntry->name, chunk.hdr.length - sizeof(CHUNK_HDR), &dwR, 0);
			if(dwR != chunk.hdr.length - sizeof(CHUNK_HDR))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}
		else if(chunk.hdr.id == MAT_DIFFUSE || chunk.hdr.id == MAT_SPECULAR)
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadMatColor(h, &chunk, MatEntry))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}
		else if(chunk.hdr.id == MAT_TEXMAP)
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadTexMap(h, &chunk, MatEntry))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}

		dwCurrentOffset += chunk.hdr.length;
		dwLength -= chunk.hdr.length;
	}

	return true;
}


bool _3dsReadTexVerts(HANDLE h, CHUNK *chunk, _3DS_NAMED_OBJECT *obj)
{
	SetFilePointer(h, chunk->offset + sizeof(CHUNK_HDR), 0, 0);

	DWORD dwR;

	dwR = 0;
	ReadFile(h, &obj->UVCount, sizeof(WORD), &dwR, 0);
	if(dwR != sizeof(WORD))
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	DWORD dw = sizeof(_3DS_UV) * obj->UVCount;
	obj->UV = (_3DS_UV*)malloc(dw);

	dwR = 0;
	ReadFile(h, obj->UV, dw, &dwR, 0);
	if(dwR != dw)
	{
		free(obj->UV);
		obj->UV = 0;
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	return true;
}


bool _3dsReadFaceArray(HANDLE h, CHUNK *facechunk, _3DS_NAMED_OBJECT *obj)
{
	DWORD dwR, dwCurrentOffset, dwLength;

	dwCurrentOffset = facechunk->offset + sizeof(CHUNK_HDR);
	SetFilePointer(h, dwCurrentOffset, 0, 0);

	dwR = 0;
	ReadFile(h, &obj->FacesCount, sizeof(WORD), &dwR, 0);
	if(dwR != sizeof(WORD))
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}
	if(obj->FacesCount == 0)
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	DWORD dw = sizeof(_3DS_FACE) * obj->FacesCount;
	obj->Faces = (_3DS_FACE*)malloc(dw);

	dwR = 0;
	ReadFile(h, obj->Faces, dw, &dwR, 0);
	if(dwR != dw)
	{
		free(obj->Faces);
		obj->Faces = 0;
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	dwLength = facechunk->hdr.length - sizeof(CHUNK_HDR) - sizeof(WORD) - dw;
	dwCurrentOffset += sizeof(WORD) + dw;

	CHUNK_HDR hdr;

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if(hdr.id == MSH_MAT_GROUP)
		{
			dw = hdr.length - sizeof(CHUNK_HDR) > 16 ? 16 : hdr.length - sizeof(CHUNK_HDR);

			dwR = 0;
			ReadFile(h, obj->material, dw, &dwR, 0);
			if(dwR != dw)
			{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
			obj->material[16] = 0;
		}

		dwLength -= hdr.length;
		dwCurrentOffset += hdr.length;
	};

	return true;
}


bool _3dsReadPointArray(HANDLE h, CHUNK *pointchunk, _3DS_NAMED_OBJECT *obj)
{
	SetFilePointer(h, pointchunk->offset + sizeof(CHUNK_HDR), 0, 0);

	DWORD dwR;

	dwR = 0;
	ReadFile(h, &obj->VertsCount, sizeof(WORD), &dwR, 0);
	if(dwR != sizeof(WORD))
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}
	if(obj->VertsCount == 0)
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	DWORD dw = sizeof(_3DS_POINT) * obj->VertsCount;
	obj->Verts = (_3DS_POINT*)malloc(dw);

	dwR = 0;
	ReadFile(h, obj->Verts, dw, &dwR, 0);
	if(dwR != dw)
	{
		free(obj->Verts);
		obj->Verts = 0;
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}

	return true;
}


bool _3dsReadNTriObj(HANDLE h, CHUNK *objchunk, _3DS_NAMED_OBJECT *obj)
{
	CHUNK chunk;
	memset(&chunk, 0, sizeof(CHUNK));

	DWORD dwCurrentOffset, dwLength, dwR;

	dwLength = objchunk->hdr.length - sizeof(CHUNK_HDR);
	dwCurrentOffset = objchunk->offset + sizeof(CHUNK_HDR);

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &chunk.hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if((chunk.hdr.id == POINT_ARRAY) && (!obj->Verts))
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadPointArray(h, &chunk, obj))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}
		else if((chunk.hdr.id == FACE_ARRAY) && (!obj->Faces))
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadFaceArray(h, &chunk, obj))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}
		else if((chunk.hdr.id == TEX_VERTS) && (!obj->UV))
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadTexVerts(h, &chunk, obj))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
		}

		dwCurrentOffset += chunk.hdr.length;
		dwLength -= chunk.hdr.length;
	}

	return true;
}


bool _3dsReadNamedObject(HANDLE h, CHUNK *objchunk, _3DS_NAMED_OBJECT *obj)
{
	CHUNK chunk;
	memset(&chunk, 0, sizeof(CHUNK));

	DWORD dwCurrentOffset, dwLength, dwR;
	char  name[11];
	int   len;

	SetFilePointer(h, objchunk->offset + sizeof(CHUNK_HDR), 0, 0);
	dwR = 0;
	ReadFile(h, name, 11, &dwR, 0);
	if(dwR != 11)
	{
#if DEBUG
		OutputDebugMsg(__FILE__, __LINE__);
#endif
		return false;
	}
	len = strlen(name) + 1;

	dwLength = objchunk->hdr.length - sizeof(CHUNK_HDR) - len;
	dwCurrentOffset = objchunk->offset + sizeof(CHUNK_HDR) + len;

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &chunk.hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if(chunk.hdr.id == N_TRI_OBJECT)
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadNTriObj(h, &chunk, obj))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
			return true;
		}

		dwCurrentOffset += chunk.hdr.length;
		dwLength -= chunk.hdr.length;
	}

#if DEBUG
	OutputDebugMsg(__FILE__, __LINE__);
#endif
	return false;
}


bool _3dsReadMData(HANDLE h, CHUNK *mdatachunk, OBJ3D *obj3d, char *path)
{
	bool rs;
	_3DS_NAMED_OBJECT *NamedObjs = 0,
					  *obj;
	_3DS_MAT_ENTRY	  *MatEntry = 0,
					  *mat;

	CHUNK chunk;
	memset(&chunk, 0, sizeof(CHUNK));

	DWORD dwCurrentOffset, dwLength, dwR;

	dwLength = mdatachunk->hdr.length - sizeof(CHUNK_HDR);
	dwCurrentOffset = mdatachunk->offset + sizeof(CHUNK_HDR);

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &chunk.hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR)) goto failed;

		if(chunk.hdr.id == NAMED_OBJECT)
		{
			obj = new _3DS_NAMED_OBJECT;
			memset(obj, 0, sizeof(_3DS_NAMED_OBJECT));
			LINK(NamedObjs, obj);

			chunk.offset = dwCurrentOffset;
			if(!_3dsReadNamedObject(h, &chunk, obj)) goto failed;
		}
		else if(chunk.hdr.id == MAT_ENTRY)
		{
			mat = new _3DS_MAT_ENTRY;
			memset(mat, 0, sizeof(_3DS_MAT_ENTRY));
			LINK(MatEntry, mat);

			chunk.offset = dwCurrentOffset;
			if(!_3dsReadMatEntry(h, &chunk, mat)) goto failed;
		}

		dwCurrentOffset += chunk.hdr.length;
		dwLength -= chunk.hdr.length;
	}

	mat = MatEntry;
	if(mat) do
	{
		if(mat->texture_name[0])
		{
			char file[STR_SIZE];
			sprintf(file, "%s\\%s", path, mat->texture_name);

			TEXTURE *tx = Textures;
			if(tx) do
			{
				if(tx->Name && strcmpi(file, tx->Name) == 0)
				{
					mat->texture = tx;
					break;
				}
				tx = tx-> Next;
			}
			while(tx != Textures);

			if(!mat->texture) mat->texture = EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, file, 0);
		}

		mat = mat->Next;
	}
	while(mat != MatEntry);

	obj = NamedObjs;
	if(obj) do
	{
		if(obj->Verts && obj->Faces)
		{
			OBJ3D_PART *part;
			part = new OBJ3D_PART;
			memset(part, 0, sizeof(OBJ3D_PART));
			LINK(obj3d->Parts, part);

			part->VertsCount = obj->FacesCount * 3;
			part->Verts = new D3DVERTEX[part->VertsCount];
			memset(part->Verts, 0, sizeof(D3DVERTEX) * part->VertsCount);

			WORD f, v;
			for(f = 0, v = 0; f < obj->FacesCount; f++, v += 3)
			{
				part->Verts[v + 0].x = obj->Verts[obj->Faces[f].a].x;
				part->Verts[v + 0].y = obj->Verts[obj->Faces[f].a].y;
				part->Verts[v + 0].z = obj->Verts[obj->Faces[f].a].z;

				part->Verts[v + 1].x = obj->Verts[obj->Faces[f].b].x;
				part->Verts[v + 1].y = obj->Verts[obj->Faces[f].b].y;
				part->Verts[v + 1].z = obj->Verts[obj->Faces[f].b].z;

				part->Verts[v + 2].x = obj->Verts[obj->Faces[f].c].x;
				part->Verts[v + 2].y = obj->Verts[obj->Faces[f].c].y;
				part->Verts[v + 2].z = obj->Verts[obj->Faces[f].c].z;

				D3DVECTOR vA = D3DVECTOR(obj->Verts[obj->Faces[f].b].x, obj->Verts[obj->Faces[f].b].y, obj->Verts[obj->Faces[f].b].z) - D3DVECTOR(obj->Verts[obj->Faces[f].a].x, obj->Verts[obj->Faces[f].a].y, obj->Verts[obj->Faces[f].a].z);
				D3DVECTOR vB = D3DVECTOR(obj->Verts[obj->Faces[f].c].x, obj->Verts[obj->Faces[f].c].y, obj->Verts[obj->Faces[f].c].z) - D3DVECTOR(obj->Verts[obj->Faces[f].a].x, obj->Verts[obj->Faces[f].a].y, obj->Verts[obj->Faces[f].a].z);
				D3DVECTOR n = Normalize(CrossProduct(Normalize(vA), Normalize(vB)));

				part->Verts[v + 0].nx = n.x;
				part->Verts[v + 0].ny = n.y;
				part->Verts[v + 0].nz = n.z;

				part->Verts[v + 1].nx = n.x;
				part->Verts[v + 1].ny = n.y;
				part->Verts[v + 1].nz = n.z;

				part->Verts[v + 2].nx = n.x;
				part->Verts[v + 2].ny = n.y;
				part->Verts[v + 2].nz = n.z;

				if(obj->UV && obj->UVCount >= obj->VertsCount)
				{
					part->Verts[v + 0].tu = obj->UV[obj->Faces[f].a].u;
					part->Verts[v + 0].tv = 1.0f - obj->UV[obj->Faces[f].a].v;

					part->Verts[v + 1].tu = obj->UV[obj->Faces[f].b].u;
					part->Verts[v + 1].tv = 1.0f - obj->UV[obj->Faces[f].b].v;

					part->Verts[v + 2].tu = obj->UV[obj->Faces[f].c].u;
					part->Verts[v + 2].tv = 1.0f - obj->UV[obj->Faces[f].c].v;
				}
			}

			if(obj->material[0])
			{
				mat = MatEntry;
				if(mat) do
				{
					if(strcmpi(obj->material, mat->name) == 0)
					{
						part->Mtrl.diffuse.r = (float)mat->diffuse.r / 100.0f;
						part->Mtrl.diffuse.g = (float)mat->diffuse.g / 100.0f;
						part->Mtrl.diffuse.b = (float)mat->diffuse.b / 100.0f;

						part->Mtrl.specular.r = (float)mat->specular.r / 255.0f;
						part->Mtrl.specular.g = (float)mat->specular.g / 255.0f;
						part->Mtrl.specular.b = (float)mat->specular.b / 255.0f;

						if(mat->texture) part->texture = mat->texture;

						break;
					}
					mat = mat->Next;
				}
				while(mat != MatEntry);
			}
		}
		obj = obj->Next;
	}
	while(obj != NamedObjs);

	rs = true;
	goto end;
failed:
	rs = false;
end:
	while(NamedObjs)
	{
		obj = NamedObjs;
		UNLINK(NamedObjs, obj);
		if(obj->Verts) free(obj->Verts);
		if(obj->Faces) free(obj->Faces);
		if(obj->UV) free(obj->UV);
		delete obj;
	}

	while(MatEntry)
	{
		mat = MatEntry;
		UNLINK(MatEntry, mat);
		delete mat;
	}

	return rs;
}


bool _3dsReadM3D(HANDLE h, CHUNK *m3dchunk, OBJ3D *obj, char *path)
{
	CHUNK chunk;
	memset(&chunk, 0, sizeof(CHUNK));

	DWORD dwCurrentOffset, dwLength, dwR;

	dwLength = m3dchunk->hdr.length - sizeof(CHUNK_HDR);
	dwCurrentOffset = m3dchunk->offset + sizeof(CHUNK_HDR);

	while(dwLength)
	{
		SetFilePointer(h, dwCurrentOffset, 0, 0);

		dwR = 0;
		ReadFile(h, &chunk.hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
#if DEBUG
			OutputDebugMsg(__FILE__, __LINE__);
#endif
			return false;
		}

		if(chunk.hdr.id == MDATA)
		{
			chunk.offset = dwCurrentOffset;
			if(!_3dsReadMData(h, &chunk, obj, path))
			{
#if DEBUG
				OutputDebugMsg(__FILE__, __LINE__);
#endif
				return false;
			}
			return true;
		}

		dwCurrentOffset += chunk.hdr.length;
		dwLength -= chunk.hdr.length;
	}

#if DEBUG
	OutputDebugMsg(__FILE__, __LINE__);
#endif
	return false;
}


void GetBoundBox(OBJ3D *obj)
{
	OBJ3D_PART *part = obj->Parts;

	obj->BoundBox.minX =
	obj->BoundBox.maxX = obj->Parts->Verts[0].x;
	obj->BoundBox.minY =
	obj->BoundBox.maxY = obj->Parts->Verts[0].y;
	obj->BoundBox.minZ =
	obj->BoundBox.maxZ = obj->Parts->Verts[0].z;

	do
	{
		for(int i = 0; i < part->VertsCount; i++)
		{
			obj->BoundBox.minX = min(part->Verts[i].x, obj->BoundBox.minX);
			obj->BoundBox.minY = min(part->Verts[i].y, obj->BoundBox.minY);
			obj->BoundBox.minZ = min(part->Verts[i].z, obj->BoundBox.minZ);
			obj->BoundBox.maxX = max(part->Verts[i].x, obj->BoundBox.maxX);
			obj->BoundBox.maxY = max(part->Verts[i].y, obj->BoundBox.maxY);
			obj->BoundBox.maxZ = max(part->Verts[i].z, obj->BoundBox.maxZ);
		}
		part = part->Next;
	}
	while(part != obj->Parts);

	/*
					max
		  3------2
		 /|     /|
		0------1 |
		| |    | |
		| 7----|-6
		|/     |/
		4------5
	min

	*/

	obj->BoundBox.v[0].x = obj->BoundBox.minX;
	obj->BoundBox.v[0].y = obj->BoundBox.maxY;
	obj->BoundBox.v[0].z = obj->BoundBox.minZ;

	obj->BoundBox.v[1].x = obj->BoundBox.maxX;
	obj->BoundBox.v[1].y = obj->BoundBox.maxY;
	obj->BoundBox.v[1].z = obj->BoundBox.minZ;

	obj->BoundBox.v[2].x = obj->BoundBox.maxX;
	obj->BoundBox.v[2].y = obj->BoundBox.maxY;
	obj->BoundBox.v[2].z = obj->BoundBox.maxZ;

	obj->BoundBox.v[3].x = obj->BoundBox.minX;
	obj->BoundBox.v[3].y = obj->BoundBox.maxY;
	obj->BoundBox.v[3].z = obj->BoundBox.maxZ;

	obj->BoundBox.v[4].x = obj->BoundBox.minX;
	obj->BoundBox.v[4].y = obj->BoundBox.minY;
	obj->BoundBox.v[4].z = obj->BoundBox.minZ;

	obj->BoundBox.v[5].x = obj->BoundBox.maxX;
	obj->BoundBox.v[5].y = obj->BoundBox.minY;
	obj->BoundBox.v[5].z = obj->BoundBox.minZ;

	obj->BoundBox.v[6].x = obj->BoundBox.maxX;
	obj->BoundBox.v[6].y = obj->BoundBox.minY;
	obj->BoundBox.v[6].z = obj->BoundBox.maxZ;

	obj->BoundBox.v[7].x = obj->BoundBox.minX;
	obj->BoundBox.v[7].y = obj->BoundBox.minY;
	obj->BoundBox.v[7].z = obj->BoundBox.maxZ;
}


OBJ3D* _3dsLoadObj(char *name)
{
	HANDLE h;

	h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE) goto failed;

	DWORD dwSize, dwR;

	dwSize = SetFilePointer(h, 0, 0, 2);
	SetFilePointer(h, 0, 0, 0);

	CHUNK chunk;
	memset(&chunk, 0, sizeof(CHUNK));

	dwR = 0;
	ReadFile(h, &chunk.hdr, sizeof(CHUNK_HDR), &dwR, 0);
	if(dwR != sizeof(CHUNK_HDR))
	{
		CloseHandle(h);
		goto failed;
	}

	if(chunk.hdr.id != M3DMAGIC)
	{
		CloseHandle(h);
		goto failed;
	}

	OBJ3D *obj;
	obj = new OBJ3D;
	memset(obj, 0, sizeof(OBJ3D));

	char path[STR_SIZE];
	strcpy(path, name);
	PathRemoveFileSpec(path);

	if(!_3dsReadM3D(h, &chunk, obj, path))
	{
		while(obj->Parts)
		{
			OBJ3D_PART *part = obj->Parts;
			UNLINK(obj->Parts, part);
			if(part->Verts) delete[] part->Verts;
			delete part;
		}
		delete obj;

		CloseHandle(h);

		goto failed;
	}

	CloseHandle(h);

	LINK(Objs3D, obj);

	GetBoundBox(obj);

	return obj;

failed:
	sprintf(str, SZ_3DS_LOAD_FAILED, name);
	LogWrite(str);
	return 0;
}
