/*
    UConsole Version 0.4
    Copyright 1999 by Nate 'm|d' Miller
    For non-commercial use only!

	File        -- console.h
	Author      -- Nate 'm|d' Miller
	Contact     -- vandals1@home.com
	Web         -- http://members.home.com/vandals1
*/
#define C_APP                   "UConsole Demo 0.4"
#define C_VERSION               0.4
#define C_VEL                   5.0             // velocity to animate the console with
#define C_FONTDIM               14              // size of the font in pixels, 14 or 16 looks best
#define C_PIXELSIZE             16              // size of pixels that a character takes up
#define C_BUFFSIZE              51              // size of the buffers for text
#define C_HISBUFF               10              // number of items in history
#define C_SCREENBUFF            21              // number of lines in screen buffer, 1 line is taken up by command line
#define C_BACKTILE              2               // number of times to tile background image
#define C_CARETCHAR	            '_'             // caret character
#define C_SEPCHAR               ']'             // separator char
#define C_CONSOLECHAR           '`'             // char to bring down console, can be int, use with care!
#define C_GET_MODES	            1               // used in consoleSetGLModes, gets all active modes
#define C_RESTORE_MODES	        2               // used in consoleSetGLModes, sets modes back
#define C_HOME                  1               // console buffer "home" position
#define C_COMMANDS              sizeof (table) / sizeof (table[0])// number of commands/vars in list

#ifndef __CONSOLE__
#define __CONSOLE__
typedef float conf_t;
typedef float conf2_t[2];
typedef conf_t conf3_t[3];

// Used for conState and buffer scrolling in the console_t structure
enum {
	UP = 0, 
	DOWN,
	HOME, 
};
/*
	console_t Details
	-----------------
	winW		- Application window height
	winH		- Application window height
	conW		- Width of the console
	conH		- Height of the console
	posY		- Vertical position of the console, used for animating
	conAni		- Used for animation, 1 = animating, 0 = not animating
	conState	- State of the console, UP or DOWN
	conFontTex	- OpenGL texture id for the console font texture
	conBackTex	- OpenGL texture id for the console background texture
	bPos		- Command line buffer position
	maxChar		- max characters to draw per row
	conHisPos	- Position in the console history
	conBufPos	- Position to start rendering the console from
	conChar		- Character that will bring down the console
	tInc		- Increment for texture coordinates
	conCmdHis	- Buffer for command history
	conBuffer	- Buffers for the console
	reDisplay	- Function pointer to a redraw function, a void type is needed 
				  for the paramates list, can be changed
*/
typedef struct 
{
	int winW;
	int winH;
	conf_t conW;
	conf_t conH;
	conf_t posY;
	int conAni;	
	int conState;
	int conFontTex;
	int conBackTex;
	int bPos;
	int maxChar;
	int conHisPos;
	int conBufPos;
	int conChar;
	conf_t tInc;
	char conCmdHis[C_HISBUFF][C_BUFFSIZE];
	char conBuffer[C_SCREENBUFF][C_BUFFSIZE];
	void (*reDisplay) (void);
} console_t;

/*
	command_t Details
	-----------------
	comm		- Nate of command or variable
	params		- Does this command or variable take paramaters?
	id			- Optional id, used when one function parses many commands or variables
	function	- Function to parse command, commandIndex is optional.  
*/
typedef struct
{
	char comm[25];
	int params;
	int id;
	void (*function)(int commandIndex, char *params); 
} command_t;

#endif

//  User calls
void consoleInit (console_t *p, int w, int h, void (*function) (void), char *fn, int fid, char *bn, int bid);
void consoleBufferAdd (console_t *p, char *s, ...);
void consoleClearBuffers (console_t *p);
void consoleHistoryRecall (console_t *p);
void consoleDraw (console_t *p);
void consoleAddChar (console_t *p, char c);
void consoleCommandLineClear (console_t *p);
void consoleResize (console_t *p, int w, int h);
void consoleAnimate (console_t * p);
void consoleDrawRawString (console_t *p, char *s, conf_t x, conf_t y, conf3_t color);
void consoleBufferMove (console_t *p, int dir);
int consoleGetCommandNum (void);
// Internal console calls
void genericParse (int commandIndex, char *params);
void consoleLoadBack (console_t *p, char *name, int texId);
void consoleLoadFont (console_t *p, char *name, int texId);
void consoleDrawChar (console_t *p, char c, conf_t x, conf_t y);
int consoleDrawString (console_t *p, int level, char *s, conf3_t color);
void consoleRenderBuffers (console_t *p);
void consoleCommand (console_t *p);
void consoleHistoryAdd (console_t *c, char *s);
void consoleCommandParse (console_t *p, char *s);
void consoleBuffCopy (char *d, char *s, int start, int end);
void consoleSetGLModes (int state);
