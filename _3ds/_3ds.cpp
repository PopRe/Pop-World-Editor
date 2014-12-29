
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#define D3D_OVERLOADS
#include "d3d.h"



#define LINK(list, add) { if(list) { add->Prev = list->Prev; add->Next = list; list->Prev->Next = add; list->Prev = add; } else { list = add; add->Next = add->Prev = add; } }
#define UNLINK(list, del) { del->Next->Prev = del->Prev; del->Prev->Next = del->Next; if(list == del) { list = del->Next; if(list == del) list = 0; } del->Next = 0; del->Prev = 0; }


#define M3DMAGIC				0x4D4D
#define MDATA					0x3D3D
#define NAMED_OBJECT			0x4000
#define N_TRI_OBJECT			0x4100
#define POINT_ARRAY				0x4110
#define FACE_ARRAY				0x4120
#define	MSH_MAT_GROUP			0x4130
#define	TEX_VERTS				0x4140
#define MAT_ENTRY				0xAFFF
#define MAT_NAME				0xA000
#define MAT_DIFFUSE				0xA020
#define MAT_SPECULAR			0xA030
#define MAT_TEXMAP				0xA200
#define MAT_MAPNAME				0xA300
#define COLOR_24				0x0011



#pragma pack(push, 1)

struct _3DS_UV
{
	float						u,
								v;
};

struct _3DS_POINT
{
	float						x,
								y,
								z;
};

struct _3DS_FACE
{
	WORD						a,
								b,
								c,
								flag;
};

struct CHUNK_HDR
{
	WORD						id;
	DWORD						length;
};

#pragma pack(pop)


struct CHUNK
{
	CHUNK						*Next,
								*Prev,
								*Child;
	CHUNK_HDR					hdr;
	DWORD						offset,
								data_size;
	void						*data;
};


WORD dwVertFixCount = 0;



void ErrorMsg(char *msg)
{
	MessageBox(0, msg, "3ds", MB_ICONHAND);
}


DWORD _3dsCalcChunkSize(CHUNK *root)
{
	DWORD dwSize = sizeof(CHUNK_HDR);

	if(root->data) dwSize += root->data_size;

	CHUNK *chunk = root->Child;
	if(chunk) do
	{
		dwSize += _3dsCalcChunkSize(chunk);
		chunk = chunk->Next;
	}
	while(chunk != root->Child);

	root->hdr.length = dwSize;

	return dwSize;
}


void _3dsDestroyChunk(CHUNK *root)
{
	if(root->data)
	{
		free(root->data);
		root->data = 0;
	}

	while(root->Child)
	{
		CHUNK *child = root->Child;
		UNLINK(root->Child, child);
		_3dsDestroyChunk(child);
		delete child;
	}
}


void* _3dsLoadData(HANDLE h, DWORD offset, DWORD length)
{
	SetFilePointer(h, offset, 0, 0);

	void *data = malloc(length);

	DWORD dwR = 0;
	ReadFile(h, data, length, &dwR, 0);
	if(dwR != length)
	{
		free(data);
		ErrorMsg("read error");
		return 0;
	}

	return data;
}


bool _3dsLoadSubChunks(HANDLE h, CHUNK *root, DWORD offset, DWORD length)
{
	while(length)
	{
		SetFilePointer(h, offset, 0, 0);

		CHUNK *chunk;
		chunk = new CHUNK;
		memset(chunk, 0, sizeof(CHUNK));
		LINK(root->Child, chunk);
		chunk->offset = offset;

		DWORD dwR = 0;
		ReadFile(h, &chunk->hdr, sizeof(CHUNK_HDR), &dwR, 0);
		if(dwR != sizeof(CHUNK_HDR))
		{
			ErrorMsg("read error");
			return false;
		}

		length -= chunk->hdr.length;
		offset += chunk->hdr.length;

		switch(chunk->hdr.id)
		{
		// just read data
		case POINT_ARRAY:
		case MSH_MAT_GROUP:
		case MAT_NAME:
		case COLOR_24:
		case MAT_MAPNAME:
		case TEX_VERTS:
			chunk->data_size = chunk->hdr.length - sizeof(CHUNK_HDR);
			chunk->data = _3dsLoadData(h, chunk->offset + sizeof(CHUNK_HDR), chunk->data_size);
			if(!chunk->data) return false;
			break;

		// just read chunks
		case MDATA:
		case N_TRI_OBJECT:
		case MAT_ENTRY:
		case MAT_DIFFUSE:
		case MAT_SPECULAR:
		case MAT_TEXMAP:
			if(!_3dsLoadSubChunks(h, chunk, chunk->offset + sizeof(CHUNK_HDR), chunk->hdr.length - sizeof(CHUNK_HDR))) return false;
			break;

		case NAMED_OBJECT:
			{
				char str[11];
				dwR = 0;
				ReadFile(h, str, 11, &dwR, 0);
				if(dwR != 11)
				{
					ErrorMsg("read error");
					return false;
				}

				chunk->data_size = strlen(str) + 1;
				chunk->data = malloc(chunk->data_size);
				memcpy(chunk->data, str, chunk->data_size);

				if(!_3dsLoadSubChunks(h, chunk, chunk->offset + sizeof(CHUNK_HDR) + chunk->data_size, chunk->hdr.length - sizeof(CHUNK_HDR) - chunk->data_size)) return false;
			}
			break;

		case FACE_ARRAY:
			{
				WORD w;
				dwR = 0;
				ReadFile(h, &w, sizeof(WORD), &dwR, 0);
				if(dwR != sizeof(WORD))
				{
					ErrorMsg("read error");
					return false;
				}

				chunk->data_size = sizeof(WORD) + w * sizeof(_3DS_FACE);
				chunk->data = _3dsLoadData(h, chunk->offset + sizeof(CHUNK_HDR), chunk->data_size);

				if(!_3dsLoadSubChunks(h, chunk, chunk->offset + sizeof(CHUNK_HDR) + chunk->data_size, chunk->hdr.length - sizeof(CHUNK_HDR) - chunk->data_size)) return false;
			}
			break;

		default: // unknow
			UNLINK(root->Child, chunk);
			delete chunk;
		}
	}

	return true;
}


CHUNK* _3dsReadFile(char *name)
{
	HANDLE h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		ErrorMsg("cannot open file");
		return 0;
	}

	CHUNK *root;
	root = new CHUNK;
	memset(root, 0, sizeof(CHUNK));

	DWORD dwR;

	dwR = 0;
	ReadFile(h, &root->hdr, sizeof(CHUNK_HDR), &dwR, 0);
	if(dwR != sizeof(CHUNK_HDR))
	{
		CloseHandle(h);
		delete root;
		ErrorMsg("read error");
		return 0;
	}

	if(root->hdr.id != M3DMAGIC)
	{
		CloseHandle(h);
		delete root;
		ErrorMsg("invalid magic");
		return 0;
	}

	if(!_3dsLoadSubChunks(h, root, sizeof(CHUNK_HDR), root->hdr.length - sizeof(CHUNK_HDR)))
	{
		CloseHandle(h);
		_3dsDestroyChunk(root);
		delete root;
		return false;
	}

	CloseHandle(h);

	return root;
}


bool _3dsWriteChunk(HANDLE h, CHUNK *root)
{
	DWORD dwW;

	dwW = 0;
	WriteFile(h, &root->hdr, sizeof(CHUNK_HDR), &dwW, 0);
	if(dwW != sizeof(CHUNK_HDR))
	{
		ErrorMsg("write error");
		return false;
	}

	dwW = 0;
	WriteFile(h, root->data, root->data_size, &dwW, 0);
	if(dwW != root->data_size)
	{
		ErrorMsg("write error");
		return false;
	}

	CHUNK *chunk = root->Child;
	if(chunk) do
	{
		if(!_3dsWriteChunk(h, chunk)) return false;
		chunk = chunk->Next;
	}
	while(chunk != root->Child);

	return true;
}


bool _3dsWriteFile(char *name, CHUNK *root)
{
	HANDLE h = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		ErrorMsg("cannot save file");
		return false;
	}

	_3dsCalcChunkSize(root);

	if(!_3dsWriteChunk(h, root))
	{
		CloseHandle(h);
		return false;
	}

	CloseHandle(h);

	return true;
}


void read_n_tri_object(CHUNK *chunk)
{
	CHUNK
		*c,
		*c_point,
		*c_face,
		*c_tex;

	c = chunk->Child;
	do
	{
		if(POINT_ARRAY == c->hdr.id) c_point = c;
		else if(FACE_ARRAY == c->hdr.id) c_face = c;
		else if(TEX_VERTS == c->hdr.id) c_tex = c;

		c = c->Next;
	}
	while(c != chunk->Child);

	if(!c_point->data) return;
	WORD point_count = *(WORD*)c_point->data;
	_3DS_POINT *points = (_3DS_POINT*)((BYTE*)c_point->data + sizeof(WORD));

#if 1
	/************************************/
	for(int a = 0; a < point_count; a++)
	{
		//points[a].x *= 1.2f;
		points[a].y += 2.5f;
		//points[a].z *= 1.2f;
	}
	/************************************/
#endif

	if(!c_face->data) return;
	WORD face_count = *(WORD*)c_face->data;
	_3DS_FACE *faces = (_3DS_FACE*)((BYTE*)c_face->data + sizeof(WORD));

	WORD w = sizeof(_3DS_FACE) * face_count;

	_3DS_FACE *new_faces;
	new_faces = (_3DS_FACE*)malloc(w);
	memset(new_faces, 0, w);

	WORD i = 0, f;
	for(f = 0; f < face_count; f++)
	{
		D3DVECTOR vA = D3DVECTOR(points[faces[f].b].x, points[faces[f].b].y, points[faces[f].b].z) - D3DVECTOR(points[faces[f].a].x, points[faces[f].a].y, points[faces[f].a].z);
		D3DVECTOR vB = D3DVECTOR(points[faces[f].c].x, points[faces[f].c].y, points[faces[f].c].z) - D3DVECTOR(points[faces[f].a].x, points[faces[f].a].y, points[faces[f].a].z);
		D3DVECTOR n = Normalize(CrossProduct(Normalize(vA), Normalize(vB)));

		if((n.x <= 1.0f) && (n.x >= -1.0f) && (n.y <= 1.0f) && (n.y >= -1.0f) && (n.z <= 1.0f) && (n.z >= -1.0f))
		{
			new_faces[i].a = faces[f].a;
			new_faces[i].b = faces[f].b;
			new_faces[i].c = faces[f].c;
			new_faces[i].flag = faces[f].flag;
			i++;
		}
		else
			dwVertFixCount++;
	}

	free(c_face->data);
	c_face->data = 0;
	c_face->data_size = 0;

	w = sizeof(WORD) + sizeof(_3DS_FACE) * i;
	c_face->data = malloc(w);
	c_face->data_size = w;

	w -= sizeof(WORD);
	*(WORD*)c_face->data = i;
	memcpy((BYTE*)c_face->data + sizeof(WORD), new_faces, w);

	free(new_faces);

	if(c_face->Child)
	{
		CHUNK *chunk0 = c_face->Child;
		do
		{
			if(chunk0->data)
			{
				char sz[17];
				strcpy(sz, (char*)chunk0->data);

				free(chunk0->data);
				chunk0->data = 0;
				chunk0->data_size = 0;

				f = lstrlen(sz) + 1;
				chunk0->data_size = f + sizeof(WORD) + sizeof(WORD) * i;
				chunk0->data = malloc(chunk0->data_size);
				strcpy((char*)chunk0->data, sz);

				WORD *pw = (WORD*)((BYTE*)chunk0->data + f);
				*pw++ = i;

				for(f = 0; f < i; f++) *pw++ = f;
			}
			chunk0 = chunk0->Next;
		}
		while(chunk0 != c_face->Child);
	}
}


void read_named_object(CHUNK *chunk)
{
	CHUNK *c = chunk->Child;
	do
	{
		if(N_TRI_OBJECT == c->hdr.id) read_n_tri_object(c);
		c = c->Next;
	}
	while(c != chunk->Child);
}


void read_mdata(CHUNK *chunk)
{
	CHUNK *c = chunk->Child;
	do
	{
		if((c->Child) && (NAMED_OBJECT == c->hdr.id)) read_named_object(c);
		c = c->Next;
	}
	while(c != chunk->Child);
}


void read_m3dmagic(CHUNK *chunk)
{
	CHUNK *c = chunk->Child;
	do
	{
		if((c->Child) && (MDATA == c->hdr.id)) read_mdata(c);
		c = c->Next;
	}
	while(c != chunk->Child);
}


int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int)
{
#if 1
	WIN32_FIND_DATA w32fd;
	HANDLE h = FindFirstFile("*.3ds", &w32fd);

	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			CHUNK *chunk;

			chunk = _3dsReadFile(w32fd.cFileName);
			if(!chunk) continue;

			dwVertFixCount = 0;

			if((chunk->Child) && (M3DMAGIC == chunk->hdr.id)) read_m3dmagic(chunk);

			if(_3dsWriteFile(w32fd.cFileName, chunk))
			{
				char sz[256];
				sprintf(sz, "done\n\nName: %s\nFixed Verts: %d", w32fd.cFileName, dwVertFixCount);
				MessageBox(0, sz, "", MB_ICONASTERISK);
			}

			_3dsDestroyChunk(chunk);
			delete chunk;
		}
		while(FindNextFile(h, &w32fd));

		FindClose(h);
	}
#else
	CHUNK *chunk;

	chunk = _3dsReadFile("src.3ds");
	if(!chunk) return -1;

    if((chunk->Child) && (M3DMAGIC == chunk->hdr.id)) read_m3dmagic(chunk);

	if(_3dsWriteFile("dst.3ds", chunk))
	{
		char sz[256];
		sprintf(sz, "done\n\nFixed Verts: %d", dwVertFixCount);
		MessageBox(0, sz, "", MB_ICONASTERISK);
	}

	_3dsDestroyChunk(chunk);
	delete chunk;
#endif

	return 0;
}
