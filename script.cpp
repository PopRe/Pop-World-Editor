
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "pop.h"
#include "popedt.h"
#include "script.h"



char		*script_szf,
			script_lockchar;
bool		script_fail;
int			script_begin;
DWORD		script_linep,
			script_line,
			script_size,
			script_lock,
			script_p,
			script_p2,
			script_indent;
WORD		script_code,
			script_fields,
			script_vars;
HANDLE		script_h;
SCRIPTINFO	*script_si;



void strcata(char *str1, char *str2, int align)
{
	int l = lstrlen(str1);
	int a = align - l - script_indent;

	if(a <= 0) goto _skip;

	for(int i = 0; i < a; i++, l++) str1[l] = ' ';
	str1[l] = 0;

_skip:
	strcat(str1, str2);
}


void TrimLeft(char *sz)
{
	int pos, len, i;

	len = lstrlen(sz);
	if(len == 0) return;

	pos = 0;
	for(;;pos++)
	{
		if(sz[pos] == 0) break;
		if((sz[pos] != ' ') && (sz[pos] != '\t')) break;
	}

	if(pos == 0) return;

	i = 0;
	for(;pos < len; pos++, i++) sz[i] = sz[pos];
	sz[i] = 0;
}


bool Script_inc_si()
{
	script_p++;

	if(script_p >= MAX_CODES)
	{
		ScriptWriteSz(SZ_SCRIPT_ERR_CODE_EOF);
		script_code = 0;
		script_fail = true;
		return false;
	}

	script_code = script_si->Codes[script_p];
	return true;
}


int ScriptWriteSz(char *sz)
{
	char sz2[512];

	strcpy(sz2, sz);
	strcat(sz2, "\r\n");

	int l = lstrlen(sz2);

	dwRW = 0;
	WriteFile(script_h, sz2, l, &dwRW, 0);
	if(dwRW != l) return RES_FATAL;

	return RES_OK;
}


int ScriptWriteToken(char *token)
{
	char sz[STR_SIZE*4];
	DWORD l, t;

	t = script_indent * 2;

	// for(l = 0; l < script_indent; l++) sz[l] = '\t';
	for(l = 0; l < t; l++) sz[l] = ' ';
	sz[l] = 0;

	strcat(sz, token);
	strcat(sz, "\r\n");
	l = lstrlen(sz);

	dwRW = 0;
	WriteFile(script_h, sz, l, &dwRW, 0);
	if(dwRW != l) return RES_FATAL;

	return RES_OK;
}


bool ScriptSkipToReturn()
{
	bool params = false,
		 ignore = false,
		 commentblock = false;
	DWORD commentblockline = 0;

	while(script_p2 < script_size)
	{
		if(script_szf[script_p2] == '\n')
		{
			if(commentblock)
			{
				script_p2++;
				script_linep = script_p2;
				script_line++;
				continue;
			}
			else
			{
            	return params;
			}
		}
		else if(!ignore && (script_szf[script_p2] == ' ' || script_szf[script_p2] == '\t' || script_szf[script_p2] == '\r'))
		{
			script_p2++;
			continue;
		}
		else if(!ignore && (script_szf[script_p2] == '/' && script_szf[script_p2+1] == '/'))
		{
			script_p2 += 2;
			ignore = true;
			continue;
		}
		else if(!ignore && (script_szf[script_p2] == '/' && script_szf[script_p2+1] == '*'))
		{
			commentblockline = script_line;
			commentblock = true;
			ignore = true;
			script_p2 += 2;
			continue;
		}
		else if(commentblock && (script_szf[script_p2] == '*' && script_szf[script_p2+1] == '/'))
		{
			commentblock = false;
			ignore = false;
			script_p2 += 2;
			continue;
		}

		if(!ignore) params = true;
		script_p2++;
	}

	if(commentblock)
	{
		sprintf(str, SZ_SCRIPT_ERR_MISSING_BLOCK_COMMEND_END, commentblockline);
		ScriptWriteSz(str);
	}

	return params;
}


bool ScriptSkipToCmd()
{
	bool commentblock = false;
	DWORD commentblockline = 0;

	while(script_p2 < script_size)
	{
		if(commentblock)
		{
			if(script_szf[script_p2] == '*' && script_szf[script_p2+1] == '/')
			{
				commentblock = false;
				script_p2 += 2;
				continue;
			}
			else if(script_szf[script_p2] == '\n')
			{
				script_p2++;
				script_linep = script_p2;
				script_line++;
				continue;
			}
			script_p2++;
			continue;
		}
		else if(script_szf[script_p2] == '/' && script_szf[script_p2+1] == '*')
		{
			commentblockline = script_line;
			commentblock = true;
			script_p2 += 2;
			continue;
		}
		else if(script_szf[script_p2] == '/' && script_szf[script_p2+1] == '/')
		{
			ScriptSkipToReturn();
			continue;
		}
		else if(script_szf[script_p2] == '\n')
		{
			script_p2++;
			script_linep = script_p2;
			script_line++;
			continue;
		}
		else if(script_szf[script_p2] == ' ' || script_szf[script_p2] == '\t' || script_szf[script_p2] == '\r')
		{
			script_p2++;
			continue;
		}
		return true;
	}

	if(commentblock)
	{
		sprintf(str, SZ_SCRIPT_ERR_MISSING_BLOCK_COMMEND_END, commentblockline);
		ScriptWriteSz(str);
	}

	return false;
}


bool ScriptSkipToParam()
{
	bool commentblock = false;
	DWORD commentblockline = 0;

	while(script_p2 < script_size)
	{
		if(commentblock)
		{
			if(script_szf[script_p2] == '\n')
			{
				script_p2++;
				script_linep = script_p2;
				script_line++;
				continue;
			}
			else if(script_szf[script_p2] == '*' && script_szf[script_p2+1] == '/')
			{
				script_p2 += 2;
				commentblock = false;
				continue;
			}
			script_p2++;
			continue;
		}
		else if(script_szf[script_p2] == '/' && script_szf[script_p2+1] == '*')
		{
			commentblockline = script_line;
			commentblock = true;
			script_p2 += 2;
			continue;
		}
		else if(script_szf[script_p2] == '\n')
		{
			return false;
		}
		else if(script_szf[script_p2] == ' ' || script_szf[script_p2] == '\t' || script_szf[script_p2] == '\r')
		{
			script_p2++;
			continue;
		}
		else if(script_szf[script_p2] == '/' && script_szf[script_p2+1] == '/')
		{
			ScriptSkipToReturn();
			return false;
		}
		return true;
	}

	if(commentblock)
	{
		sprintf(str, SZ_SCRIPT_ERR_MISSING_BLOCK_COMMEND_END, commentblockline);
		ScriptWriteSz(str);
	}

	return false;
}


void ScriptWordLock()
{
	if(script_p2 >= script_size) return;

	script_lock = script_p2;

	if(script_szf[script_lock] == '(' || script_szf[script_lock] == ')')
	{
		script_lock++;
		script_lockchar = script_szf[script_lock];
		script_szf[script_lock] = 0;
		return;
	}

	script_lock++;

	while(script_lock <= script_size)
	{
		if(script_szf[script_lock] == ' ' || script_szf[script_lock] == '\t' || script_szf[script_lock] == '\r' || script_szf[script_lock] == '\n' || script_szf[script_lock] == '(' || script_szf[script_lock] == ')')
		{
			script_lockchar = script_szf[script_lock];
			script_szf[script_lock] = 0;
			return;
		}
		script_lock++;
	}
}


void ScriptWordUnlock()
{
	if(script_lock > script_size) return;
	script_szf[script_lock] = script_lockchar;
}


void ScriptWordRead(char *sz)
{
	ScriptWordLock();
	strcpy(sz, &script_szf[script_p2]);
	ScriptWordUnlock();
	script_p2 = script_lock;
}


void ScriptLineLock()
{
	script_lock = script_linep;
	while(script_lock <= script_size)
	{
		if(script_szf[script_lock] == '\r' || script_szf[script_lock] == '\n')
		{
			script_lockchar = script_szf[script_lock];
			script_szf[script_lock] = 0;
			return;
		}
		script_lock++;
	}
}


void ScriptLineUnlock()
{
	if(script_lock > script_size) return;
	script_szf[script_lock] = script_lockchar;
}


void ScriptLineRead(char *sz)
{
	ScriptLineLock();
	strcpy(sz, &script_szf[script_linep]);
	TrimLeft(sz);
	ScriptLineUnlock();
}

