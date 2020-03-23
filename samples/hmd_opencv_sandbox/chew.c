#include <windows.h>
#include <stdio.h>
#include "chew.h"

#define TABLEONLY
#undef CHEWTYPEDEF
#undef CHEWTYPEDEF2
#define CHEWTYPEDEF( ret, name, ... ) name##_t name;
#define CHEWTYPEDEF2( ret, name, usename, ... ) usename##_t usename;
#include "chew.h"

#define TABLEONLY
#undef CHEWTYPEDEF
#undef CHEWTYPEDEF2
#define CHEWTYPEDEF( ret, name, ... ) #name, 
#define CHEWTYPEDEF2( ret, name, usename, ... ) #name, 
const char * symnames[] = {
	#include "chew.h"
};

#define TABLEONLY
#undef CHEWTYPEDEF
#undef CHEWTYPEDEF2
#define CHEWTYPEDEF( ret, name, ... ) (void**)&name, 
#define CHEWTYPEDEF2( ret, name, usename, ... ) (void**)&usename, 
void ** syms[] = {
	#include "chew.h"
};

//From https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
void * chewGetProcAddress(const char *name)
{
	void *p = (void *)wglGetProcAddress(name);
	if(p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1) )
	{
		static HMODULE module;
		if( !module ) module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}
	return p;
}

void chewInit() {
	int i;
	for( i = 0; i < sizeof(symnames) / sizeof(symnames[0]); i++ )
	{
		void * v = *syms[i] = (void*)chewGetProcAddress( symnames[i] );
		if( !v ) fprintf( stderr, "Warning: Can't find \"gl%s\"\n", symnames[i] );
	}
}

