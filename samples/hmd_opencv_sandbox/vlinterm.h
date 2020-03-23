#ifndef _VLINTERM_H
#define _VLINTERM_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "os_generic.h"


#define MAX_CSI_PARAMS 8

struct TermStructure
{
	int ptspipe;

	uint8_t * linetaint;	//One for each line, indicating that particular line is tainted.

	uint32_t * termbuffer;
	uint32_t * termbuffer_raw;
	// text  <<lsB
	// attrib
	// color
	// taint <<msB  (Could also contain more attirbutes if needed)
	int current_color;
	int current_attributes;

	int scrollback; //Not actually used by vlinterm, you can use it for yourself.
	int historyy;
	int savex, savey;
	int curx, cury;
	int charx, chary;
	int echo;

	int csistate[MAX_CSI_PARAMS];
	int whichcsi;
	int escapestart; // [ or ]
	int escapestate;
	int dec_priv_csi;

	int dec_mode; // ECMA-48 Mode Switches (Need to implement 4 (insert mode) as well as Auto-follow LF, VT, FF with CR (TODO)
	int dec_private_mode;

	int dec_keypad_mode;

	int osc_command_place;
	char osc_command[128];

	int pid;

	int scroll_top;
	int scroll_bottom;

	//Must update these from charx, chary.
	int top;
	int bottom; //XXX TODO: make sure scroll_top and scroll_bottom shouldn't be this elsewhere in code.

	og_mutex_t screen_mutex;
	uint8_t tainted;

	void * user;

};

void EmitChar( struct TermStructure * ts, int crx );
void ResetTerminal( struct TermStructure * ts );
int FeedbackTerminal( struct TermStructure * ts, const uint8_t * data, int len );
void ResizeScreen( struct TermStructure * ts, int neww, int newh );
void TermScroll( struct TermStructure * ts, int amount );
void DestroyTerminal( struct TermStructure * ts );

//Use this in conjunction with setting the terminal pid setting.
int spawn_process_with_pts( const char * execparam, char * const argv[], int * pid );

//You must implement this.
void HandleOSCCommand( struct TermStructure * ts, int parameter, const char * value );
void HandleBell( struct TermStructure * ts );


#ifdef __cplusplus
};
#endif
#endif

