#define _GNU_SOURCE

#include "vlinterm.h"
#include <stdio.h>
#include <string.h>
#if defined( WIN32 ) || defined( WINDOWS )
#include <malloc.h>
#else
#include <termios.h>
#include <sys/ioctl.h>
#endif
#include <fcntl.h>

//#define DEBUG_VLINTERM

//TODO: Memoize ts->charx

static void BufferSet( struct TermStructure * ts, int start, int value, int length )
{
	int cx = ts->charx;
	int cy = ts->chary;
	uint32_t v = value | ts->current_attributes << 8 | ts->current_color << 16 | 1<<24;
	int i;
	length+=start;
	if( start < 0 ) start = 0;
	if( length >= cx*cy ) length = cx*cy;
	for( i = start; i < length; i++ )
	{
		ts->termbuffer[i] = v;
	}

	start /= cx;
	length = (length-1) / cx;
	for( i = start; i <= length; i++ )
	{
		ts->linetaint[i] = 1;
	}

	ts->tainted = 1;
}

static void BufferCopy( struct TermStructure * ts, int dest, int src, int length )
{
	int cx = ts->charx;
	int cy = ts->chary;
	int minimum = (cy-ts->historyy)*cx;
	if( src < minimum ) { 
#ifdef DEBUG_VLINTERM
		fprintf( stderr, "invalid buffer copy [0]\n" );
#endif
		return;
	} 
	if( dest < minimum ) {
#ifdef DEBUG_VLINTERM
		fprintf( stderr, "invalid buffer copy [1]\n" ); 
#endif
		return;
	} 
	if( src + length > cx*cy )
	{
#ifdef DEBUG_VLINTERM
		fprintf( stderr, "invalid buffer copy [2]\n" );
#endif
		return;
	} 
	if( dest + length > cx*cy ) {
#ifdef DEBUG_VLINTERM
		fprintf( stderr, "invalid buffer copy [3]\n" );
#endif
		return;
	} 

	if( length < 0 )
	{
#ifdef DEBUG_VLINTERM
		fprintf( stderr, "Warning! Negative len copy\n" );
#endif
		return;
	}

	//Detect if we have overlapping regions, if so we must use a temporary buffer.
	if( ( dest > src && src + length >= dest ) ||
		( src > dest && dest + length >= src ) )
	{
		char * temp = (char*)alloca(length * 4);
		memcpy( temp,  ts->termbuffer + src, length*4 );	
		memcpy( ts->termbuffer + dest, temp, length*4 );	
	}
	else
	{
		memcpy( ts->termbuffer + dest,   ts->termbuffer + src, length*4 );
	}
	int i;
	for( i = 0; i < length; i++ ) ts->termbuffer[i+dest] |= (1<<24);

	//Yugh... this is dirty, probably should rewrite these last few lines.
	length = (length-1) / cx;
	dest /= cx;
	for( i = 0; i <= length && (i+dest) < cy; i++ ) if( i+dest >= 0 ) ts->linetaint[i+dest] = 1;


	ts->tainted = 1;
}

static void UpdateTopBottom( struct TermStructure * ts )
{
	ts->top    = (ts->scroll_top<0)?0:(ts->scroll_top);
	ts->bottom = (ts->scroll_bottom<0)?ts->chary:(ts->scroll_bottom+1);
}

void ResetTerminal( struct TermStructure * ts )
{
	ts->dec_private_mode = /*(1<<12) |*/ (1<<25); //Enable cursor.
	ts->dec_mode = 0;
	ts->scrollback = 0;
	if ( ts->charx == 0 || ts->chary == 0 )
	{
		ts->charx = 80;
		ts->chary = 25;
	}
	if ( ts->historyy == 0 )
	{
		ts->historyy = 1000;
	}
	ts->curx = ts->cury = 0;
	ts->echo = 0;
	ts->escapestate = 0;

	ts->current_color = 7;
	ts->current_attributes = 0;

	ts->scroll_top = -1;
	ts->scroll_bottom = -1;

	if( !ts->termbuffer )
		ResizeScreen( ts, ts->charx, ts->chary );
	BufferSet( ts, 0, 0, ts->charx * ts->chary );
}


int FeedbackTerminal( struct TermStructure * ts, const uint8_t * data, int len )
{
#if defined(WINDOWS) || defined(WIN32)
	//XXX TODO
	return len;
#else
	return write( ts->ptspipe, data, len );
#endif
}

static void InsertBlankLines( struct TermStructure * ts, int l )
{
	if( l > ts->scroll_bottom - ts->cury ) l = ts->scroll_bottom - ts->cury;
	if( l > 0 )
	{
		int lines_need_to_move = ts->scroll_bottom - ts->cury - l;
		//printf( "PUSHING %d LINES / %d * %d\n", l, lines_need_to_move, ts->cury );
		if( lines_need_to_move > 0 ) 
			BufferCopy( ts, (l+ts->cury)*ts->charx, ts->cury*ts->charx, ts->charx * ( ts->scroll_bottom - l - ts->cury  ) );
		BufferSet( ts, ts->cury*ts->charx, 0, ts->charx * l );
	}
}

static int ScrollDown( struct TermStructure * ts, int lines )
{
	BufferCopy( ts, (ts->top+lines)*ts->charx, ts->top*ts->charx, (ts->bottom-ts->top-lines)*ts->charx );
	BufferSet( ts, (ts->top)*ts->charx, 0, ts->charx*lines );
	return 0;
}

static int ScrollUp( struct TermStructure * ts, int lines )
{
	if( ts->scroll_top > 0 || ( ts->scroll_bottom < ts->chary - 1 && ts->scroll_bottom >= 0 ) )
	{
		//Do a limited scroll
		BufferCopy( ts, ts->top*ts->charx, (ts->top+lines)*ts->charx, (ts->bottom-ts->top-lines)*ts->charx );
		BufferSet( ts, (ts->bottom-lines)*ts->charx, 0, ts->charx*lines );
	}
	else
	{
		//Scroll up into the history buffer...
		int dest = (ts->chary-ts->historyy)*ts->charx;
		BufferCopy( ts, dest, dest+lines*ts->charx, (ts->historyy-lines)*ts->charx );
		BufferSet( ts, (ts->bottom-lines)*ts->charx, 0, ts->charx*lines );
	}
	return 0;
}

static void HandleNewline( struct TermStructure * ts, int advance )
{
	if( advance && ( ts->dec_private_mode & (1<<12) ) )
	{
		InsertBlankLines( ts, 1 );
	}
	if( ts->cury >= ts->bottom )
	{
		ScrollUp( ts, 1 );
		ts->cury--;
	}
}

void EmitChar( struct TermStructure * ts, int crx )
{
#ifdef DEBUG_VLINTERM
//	fprintf( stderr, "(%d %d %c)", crx, ts->curx, crx );
#endif
	int cx = ts->charx;

	OGLockMutex( ts->screen_mutex );
	if( crx == '\r' ) { goto linefeed; }
	else if( crx == '\n' ) { goto newline; }
	else if( crx == 7 && ts->escapestate != 3 /* Make sure we're not in the OSC CSI */ ) { HandleBell( ts ); }
	else if( crx == 8 ) {
		if( ts->curx ) ts->curx--;
	}
	else if( crx == 9 ) { 	/*tabstop*/
			ts->curx = (ts->curx & (~7)) + 8;
			if( ts->curx >= cx )
				ts->curx = cx-1;
			}
	else if( crx == 0x0f || crx == 0x0e ) { /*Activate other charsets (unimplemented)*/ }
	else if( crx == 0x18 || crx == 0x1a ) ts->escapestate = 0;
	else if( crx == 0x1b ) ts->escapestate = 1;
	else if( crx == 0x9b ) goto csi_state_start;
	else if( ts->escapestate )
	{
		if( ts->escapestate == 1 )
		{
			ts->escapestate = 0;
#ifdef DEBUG_VLINTERM
			fprintf( stderr, "{E: %d[%c] (%d %d %d %d)}", crx, crx, ts->curx, ts->cury, ts->savex, ts->savey );
#endif
			switch( crx )
			{
				case 'c': ResetTerminal(ts); break;
				case 'D': goto linefeed;
				case 'E': goto newline;
				//case 'H': break; //Set tabstop
				case 'M':	//Reverse linefeed.
					ts->cury--;
					if( ts->cury < ts->top )
					{
						BufferCopy( ts, cx*(ts->top+1), cx*ts->top, (ts->bottom - ts->top - 1) * cx );
						BufferSet( ts, cx*ts->top, 0, cx );
						ts->cury = ts->top;
					} 
					break;
				//case 'Z': FeedbackTerminal( ts, "\x1b[?6c", 5 ); break;
				case '7': ts->savex = ts->curx; ts->savey = ts->cury; break;
				case '8': ts->curx = ts->savex; ts->cury = ts->savey; break;
				case '[': goto csi_state_start;
				case '=': ts->dec_keypad_mode = 1; break;
				case '>': ts->dec_keypad_mode = 2; break;
				case ']': goto csi_state_start;
				case '(': ts->escapestate = 4; break; //Start sequence defining G0 character set (Currently not implemented)
				//case ')':
				//case '%': break; //Select unicode/ISO 646, ISO 8859-1 charsets, but it doens't seem people follow this rule.
				//case '(': ts->escapestate = 5; break;
				default: 
#ifdef DEBUG_VLINTERM
					fprintf( stderr, "UNHANDLED Esape: %c %d [%d]\n", crx, ts->whichcsi, ts->csistate[0] );
#endif
					break;
			}
		}
		else if( ts->escapestate == 2 )
		{
			if( crx == ';' && ts->escapestart == ']' ) //XXX This looks WRONG
			{
				//ESC ] ### ; For an OSC command.
#ifdef DEBUG_VLINTERM
				fprintf( stderr, "];command\n" );
#endif
				ts->escapestate = 3;
				ts->osc_command_place = 0;
			}
			else if( crx == '?' && ts->whichcsi == 0 )
			{
				ts->dec_priv_csi = 1;
			}
			else if( crx >= '0' && crx <= '9' ) 			//A CSI control message, but the numbers part
			{
				if( ts->csistate[ts->whichcsi] < 0 ) ts->csistate[ts->whichcsi] = 0;
				ts->csistate[ts->whichcsi] *= 10;
				ts->csistate[ts->whichcsi] += crx - '0';
			}
			else if( ts->escapestate == 4 )
			{
				//Charset or other escape sequence (Not implemented yet)
				ts->escapestate = 0;
			}
			else
			{
				ts->escapestate = 0;
				int is_seq_default = ts->csistate[ts->whichcsi] < 0;
				if( is_seq_default ) ts->csistate[ts->whichcsi] = 1; //Default
				int cs0 = ts->csistate[0];
				int cs1 = ts->csistate[1];

#ifdef DEBUG_VLINTERM
				if( crx != ';' ) fprintf( stderr, "CRX: %d %d  %d %d %c\n", cs0, cs1, ts->curx, ts->cury, crx );
#endif
				//This is the big list of CSI escapes.
				switch( crx )
				{
				case '@': 
				{
					//Insert specified # of characters.
					int chars = cs0;
					int start = cx*ts->cury+ts->curx;
					BufferCopy( ts, start + chars, start, cx-chars-ts->curx );
					BufferSet( ts, start, 0, chars ); 
					break;
				}
				case 'F': ts->curx = 0; //Don't break!  This F does A as well.
				case 'A': ts->cury -= cs0; break;
				case 'E': ts->curx = 0;	//Don't break!  This E does B as well.
				case 'B': ts->cury += cs0; break; //CUD—Cursor Down
				case 'C': ts->curx += cs0; break; //CUF—Cursor Forward
				case 'D': ts->curx -= cs0; ts->curx = 0; break;
				case 'G': ts->curx = cs0 - 1; break;
				case 'd': ts->cury = cs0 - 1; break;
				case ';': ts->escapestate = 2; if( ts->whichcsi < MAX_CSI_PARAMS ) { ts->whichcsi++; ts->csistate[ts->whichcsi] = -1; } break;
				case 'h': 
				case 'l': //set or clear DEC or DEC private modes.
				{
					int i;
					int set = crx == 'h';
					uint32_t dec = (ts->dec_priv_csi)?ts->dec_private_mode:ts->dec_mode;

#ifdef DEBUG_VLINTERM
					fprintf( stderr, "DEC CSI[%d]: '%c' %d %d / %d[%c]\n", ts->dec_priv_csi, crx, ts->csistate[0], ts->csistate[1], ts->dec_priv_csi, ts->dec_priv_csi );
#endif
					for( i = 0; i <= ts->whichcsi; i++ ) //Tricky: We want to do this for all parameters.
					{
						int bit = ts->csistate[i];
						if( ts->dec_priv_csi ) switch( bit )
							{
							case 1049: bit = 29; break;//Scroll wheel does arrows.
							case 1000: bit = 30; break;//X11 mouse reporting.
							}
						if( bit > 31|| bit < 0 )
						{
#ifdef DEBUG_VLINTERM
							fprintf( stderr, "Error: Unknown DEC Private type %d\n", bit );
#endif
						}
						else
						{
							if( set )
								dec |= 1<<bit;
							else
								dec &= ~(1<<bit); 
						}
					}
					if( ts->dec_priv_csi )
						ts->dec_private_mode = dec;
					else
						ts->dec_mode = dec;
					break;
				}
				case 's': ts->savex = ts->curx; ts->savey = ts->cury; break;
				case 'u': ts->curx = ts->savex; ts->cury = ts->savey; break;
				case 'S': //Scroll up
					ScrollUp( ts, cs0 );
					break;
				case 'T': //Scroll down
					ScrollDown( ts, cs0 );
					break;
				case 'f':
				case 'H':	//CUP - Cursor Position
					if( cs0 <= 0 ) cs0 = 1;
					if( cs1 <= 0 ) cs1 = 1;
					ts->curx = cs1 - 1;
					ts->cury = cs0 - 1;
					break;
				case 'J':	//DECSED - Selective Erase in Display
					{
						int pos = ts->curx+ts->cury*cx;
						int end = cx * ts->chary;
						if( is_seq_default ) cs0 = 0; 
						switch( cs0 ) 
						{
							case 0:			BufferSet( ts, pos, 0, end-pos ); break;
							case 1:			BufferSet( ts, 0, 0, pos ); break;
							case 2:	case 3:	BufferSet( ts, 0, 0, end ); break;
						}
						ts->curx = 0;
					}
					break; 
				case 'K':	//DECSEL - Selective Erase in Line
					if( is_seq_default ) cs0 = 0; 
					switch( cs0 )
					{
					case 0: BufferSet( ts, ts->curx+ts->cury*cx, 0, cx-ts->curx ); break;
					case 1:	BufferSet( ts, ts->cury*cx, 0, ts->curx ); break;
					case 2:	BufferSet( ts, ts->cury*cx, 0, cx ); break;
					}
					break;
				case 'L': //Inset blank lines.
					{
						int l = cs0;
						InsertBlankLines( ts, l );
					}
					break;
				case 'M': //Delete the specified number of lines.
				{
					if( cs0 > 0 )
					{
						int lines = cs0;
						int startcopy = ts->cury;
						int stopcopy = ts->scroll_bottom - lines;
						if( startcopy < ts->scroll_top ) startcopy = ts->scroll_top;		
						if( stopcopy > startcopy )
						{
							BufferCopy( ts, startcopy*cx, (lines+startcopy)*cx, cx*(stopcopy-startcopy) );
							BufferSet( ts, stopcopy*cx, 0, cx * lines );
						}
					}
					ts->curx = 0;
					break;
				}

				case 'P': //DCH "Delete Character" "
					{
						//"As characters are deleted, the remaining characters between the cursor and right margin move to the left."
						int chars_to_del = (cs0<0)?1:cs0;
						int remain_in_line = cx - ts->curx;
						if( chars_to_del > remain_in_line ) remain_in_line = remain_in_line;
						int start = ts->cury*cx + ts->curx;
						int nr_after_shift = remain_in_line - chars_to_del-1;
						BufferCopy( ts, start, start+chars_to_del, nr_after_shift+1 );
						BufferSet( ts, start+nr_after_shift, 0, remain_in_line-nr_after_shift );
					}
					break;
				case 'X': //Erase # of characters on current line.
					if( cs0 < 1 ) cs0 = 1;
					if( cs0 > cx-ts->curx ) cs0 = cx-ts->curx;
					BufferSet( ts, ts->cury*cx + ts->curx, 0, cs0 );
					break;					
				case 'r': //DECSTBM
					ts->scroll_top = cs0-1;
					ts->scroll_bottom = cs1-1;
					ts->curx = 0;
					ts->cury = ts->scroll_top;
					UpdateTopBottom( ts );
					break;
				case 'm':
				{
					int i;
					//SGR (set attributes)
					for( i = 0; i < ts->whichcsi+1; i++ )
					{
						int k = ts->csistate[i];
						if( is_seq_default ) k = 0; 
						if( k == 0 ) { ts->current_color = 7; ts->current_attributes = 0; }
						else if( k == 1 ) { ts->current_attributes |= 1<<0; }
						else if( k == 2 ) { ts->current_attributes |= 1<<1; }
						else if( k == 4 ) { ts->current_attributes |= 1<<2; }
						else if( k == 5 ) { ts->current_attributes |= 1<<3; }
						else if( k == 7 ) { ts->current_attributes |= 1<<4; }
						else if( k == 21 ) { ts->current_attributes &= ~(1<<0); }
						else if( k == 22 ) { ts->current_attributes &= ~(1<<1); }
						else if( k == 24 ) { ts->current_attributes &= ~(1<<2); }
						else if( k == 25 ) { ts->current_attributes &= ~(1<<3); }
						else if( k == 27 ) { ts->current_attributes &= ~(1<<4); }
						else if( k >= 30 && k < 37 ) { ts->current_color = (ts->current_color&0xf0) | ( k - 30 ); }
						else if( k >= 40 && k < 47 ) { ts->current_color = (ts->current_color&0x0f) | ( ( k - 40 ) << 4 ); }
						else if( k == 38 ) { ts->current_attributes |= 1<<4; ts->current_color = (ts->current_color&0xf0) | 7; }
						else if( k == 39 ) { ts->current_attributes &= ~(1<<4); ts->current_color = (ts->current_color&0xf0) | 7; }
						else if( k == 49 ) { ts->current_color = (ts->current_color&0x0f) | (0<<4); }
					}
					break;
				}
				default:
#ifdef DEBUG_VLINTERM
					fprintf( stderr, "UNHANDLED CSI Esape: %c [%d]\n", crx, cs0 );
#endif
					break;
				}
			}
		}
		else if( ts->escapestate == 3 )
		{
			//Processing ESC ] (OSC)
			if( crx == 0x07 )
			{
				ts->osc_command[ts->osc_command_place] = 0;
				HandleOSCCommand( ts, ts->csistate[0], ts->osc_command );
				ts->escapestate = 0;
			}
			else if( ts->osc_command_place < sizeof( ts->osc_command ) - 1 )
			{
				ts->osc_command[ts->osc_command_place++] = (char)crx;
			}
		}
		else if( ts->escapestate == 5 )
		{
			ts->escapestate ++;
		}
		else if( ts->escapestate == 6 )
		{
			ts->escapestate = 0;
		}

		int top    = (ts->scroll_top<0)?0:ts->scroll_top;
		int bottom = (ts->scroll_bottom<0)?ts->chary:(ts->scroll_bottom+1);
		if( ts->cury < top ) ts->cury = top;
		if( ts->curx < 0 ) ts->curx = 0;
		if( ts->cury >= bottom ) ts->cury = bottom-1;
		if( ts->curx >= cx ) ts->curx = cx-1; //XXX DUBIOUS
	}
	else
	{
		//No escape sequence.  Just handle the character.
		//XXX TODO: Handle insert mode (DEC mode 4h)
		if( ts->curx >= cx ) { ts->curx = 0; ts->cury++; HandleNewline( ts, 0 ); }
		BufferSet( ts, ts->curx+ts->cury*cx, crx, 1 );
		ts->curx++;
#ifdef DEBUG_VLINTERM
		if( crx < 32 || crx < 0 ) fprintf( stderr, "C-X %d\n", (uint8_t)crx );
#endif
	}
	goto end;

linefeed:
	ts->curx = 0;
	HandleNewline( ts, 0 );
	goto end;

newline:
	ts->cury ++;
	if( !( ts->dec_private_mode & 0x80 ) ) //XXX: This is PROBABLY wrong, but seems to fix SL and some other things like it.
	{
		ts->curx = 0;
	}
	HandleNewline( ts, 1 );
	goto end;

csi_state_start:
	ts->escapestart = crx;
	ts->escapestate = 2;
	ts->csistate[0] = -1;
	ts->csistate[1] = -1;
	ts->dec_priv_csi = 0;
	ts->whichcsi = 0;
	goto end;
end:
	OGUnlockMutex( ts->screen_mutex );
}

void TermScroll( struct TermStructure * ts, int amount )
{
	int sb = ts->scrollback + amount;
	if( sb < 0 ) sb = 0;
	if( sb >= ts->historyy-ts->chary ) sb = ts->historyy-ts->chary-1;;
	ts->scrollback = sb;
}

void ResizeScreen( struct TermStructure * ts, int neww, int newh )
{
	if( neww < 1 || newh < 1 ) return;
	OGLockMutex( ts->screen_mutex );
	int line;
	uint32_t * oldbuffer = ts->termbuffer_raw;

	if ( ts->linetaint ) free( ts->linetaint );
	uint8_t  * linetaint = ts->linetaint = (uint8_t*)malloc( ts->historyy );
	uint32_t * newbuffer = ts->termbuffer_raw = (uint32_t*)malloc( neww * ts->historyy * 4 );
	ts->termbuffer = newbuffer + (ts->historyy-newh)*neww;
	memset( linetaint, 0, ts->historyy );
	memset( newbuffer, 0, neww * ts->historyy * 4 );
	if( oldbuffer )
	{
		int ch;
		for( line = 0; line < ts->historyy; line++ )
		{
			for( ch = 0; ch < neww; ch++ )
			{
				uint32_t och = 1<<24; //1<<24 is the 'taint' flag
				if( ch < ts->charx )
					och = oldbuffer[line*ts->charx + ch];
				newbuffer[line*neww+ch] = och | 1<<24;
			}
		}
		free( oldbuffer );
	}

	for( line = 0; line < ts->historyy; line++ )
	{
		linetaint[line] = 1;
	}

	if( ts->ptspipe )
	{
#if defined(WIN32) || defined( WINDOWS )
		//XXX TODO
#else
		struct winsize tsize;
		tsize.ws_col = neww;
		tsize.ws_row = newh;
		tsize.ws_xpixel = neww;
		tsize.ws_ypixel = newh;
		ioctl(ts->ptspipe, TIOCSWINSZ, &tsize);
#endif
	}
	ts->cury += newh-ts->chary;
	ts->charx = neww;
	ts->chary = newh;

	ts->tainted = 1;
	UpdateTopBottom( ts );

	OGUnlockMutex( ts->screen_mutex );
}

int spawn_process_with_pts( const char * execparam, char * const argv[], int * pid )
{
#if defined(WIN32) || defined( WINDOWS )
	//XXX TODO
	return -1;
#else
	int r = getpt();
	if( r <= 0 ) return -1;
	if( grantpt(r) ) return -2;
	if( unlockpt( r ) ) return -3;
	char slavepath[64];
	if(ptsname_r(r, slavepath, sizeof(slavepath)) < 0)
	{
		return -4;
	}

	int rforkv = fork();
	if (rforkv == 0)
	{
		close( r ); //Close master
		close( 0 );
		close( 1 );
		close( 2 );
		r = open( slavepath, O_RDWR | O_NOCTTY ); //Why did the previous example have the O_NOCTTY flag?
		//if (ioctl(r, TIOCSCTTY, NULL) < 0)
		//	fprintf(stderr, "ioctl TIOCSCTTY failed: %s\n", strerror(errno));
		dup2( r, 0 );
		dup2( r, 1 );
		dup2( r, 2 );
		setsid();
		setenv("TERM", "xterm", 1);
		execvp( execparam, argv );
	}
	else
	{
		//https://stackoverflow.com/questions/13849582/sending-signal-to-a-forked-process-that-calls-exec
		//setpgid(rforkv, 0);   //Why is this mutually exclusive with setsid()?
		if( rforkv < 0 )
		{
			close( r );
			return -10;
		}
		*pid = rforkv;
		return r;
	}
#endif
}


void DestroyTerminal( struct TermStructure * ts )
{
	free( ts->termbuffer_raw );
	free( ts->linetaint );
	OGDeleteMutex( ts->screen_mutex );
}

