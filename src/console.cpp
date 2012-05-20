/*
    UConsole Version 0.4
    Copyright 1999 by Nate 'm|d' Miller
    For non-commercial use only!

    File        -- console.c
    Author      -- Nate 'm|d' Miller
    Contact     -- vandals1@home.com
    Web         -- http://members.home.com/vandals1

    TODO
    ----
    * rethink design of console rendering, its poorly designed
    * add console buffer scrolling once things settle down
    * fix 255 char limit in consoleBufferAdd somehow

    Notes
    -----
    * All console drawing functions assume that GL_TEXTURE_2D is enabled before
      they are called.  
*/
//#include "h.h"
#include "freereign.h"
#include "tga.h"
#include "console.h"
#include <stdarg.h>

extern conf2_t tPoints[]; // this is for the point map for the font

// implement some sort of coloring system, this is temp
conf3_t white = {1.0f, 1.0f, 1.0f};
conf3_t grey = {0.7f, 0.7f, 0.7f};
conf3_t red = {1.0f, 0.0f, 0.0f};
conf3_t green = {0.0f, 1.0f, 0.0f};
conf3_t blue = {0.0f, 0.0f, 1.0f};
conf3_t purple = {0.5f, 0.0f, 0.5f};
/*
	command/variable list, see console.h for more info
	this is something that HAS to be modified to work correctly in you apps
	I have it here because it makes things simpler. 
*/

void dummy(int commandIndex, char *params )
{
	cout << "Hi!" << endl;
}



command_t table[] =
{
	{"about",		0, C_ID_ABOUT,	dummy},
	{"quit",		0, C_ID_QUIT,	dummy},
	{"version",		0, C_ID_VERSION,dummy},
	{"echo",		1, C_ID_ECHO,	dummy},
	{"clear",		0, C_ID_CLEAR,	dummy},
	{"set ani",		1, 0,			dummy},
	{"set fill",	1, 1,			dummy},
	{"commlist",	0, 0,			dummy}
};
/*
	Function -- buffCopy
	
	This function will copy chars from a source string to a destination string 
	starting from position "start" and ending at "end."  All positions assumed
	to be valid in the array.  Just a utility function.
*/
void consoleBuffCopy (char *d, char *s, int start, int end)
{
	int t = start;
	
	s += start;

	while (*s && t != end && *s != '\0')
	{
		*d = *s;
		*d ++;
		*s ++;
		t ++;
	}
	*d = '\0';
}
/*
	Function -- consoleGetCommandNum
	
	This function returns the number of commands in our command table.  Use this
	function rather than the C_COMMANDS #define in your apps.  Using this function
	gets around the "returns sizeof 0" problem.  Just a utility function.
*/
int consoleGetCommandNum (void)
{
	return C_COMMANDS;
}
/* 
	Function -- consoleInit 

	Sets up the console and all of its values.  "w" and "h" are the width and 
	height of the application window. "function" is a pointer to the
	applications redraw function. "fn" and "bn" are font file and background 
	file names respectively.  "fid" and "bid" are texture ids/  
*/
void consoleInit (console_t *p, int w, int h, void (*function) (void), char fn[], int fid, char bn[], int bid)
{
	p->winW = w;
	p->winH = h;
	p->conW = w;
	p->conH = h / 2;
	p->conAni = 0;
	p->posY = h / 2;
	p->conState = UP;
	p->conFontTex = -1;
	p->conBackTex = -1;
	p->bPos = 0;
	p->tInc = pow (C_PIXELSIZE, -1); // dont mess with this, used for texture coords
	p->maxChar = (w / C_FONTDIM);
	p->conHisPos = 0;
	p->conBufPos = 1;
	p->conChar = C_CONSOLECHAR;
	memset (p->conBuffer, '\0', sizeof (p->conBuffer));
	memset (p->conCmdHis, '\0', sizeof (p->conCmdHis));
	p->reDisplay = function;
	consoleLoadFont (p, fn, fid);
	consoleLoadBack (p, bn, bid);
}
/* 
	Function -- consoleResize 

	Called when the application window is resized.  Reconfigures the console
	for the new window dimensions.  Needs to be called from inside the
	application to make sure that the console functions properly.
*/
void consoleResize (console_t *p, int w, int h)
{
	p->winW = w;
	p->winH = h;
	p->conW = w;
	p->conH = h / 2;
	p->maxChar = (w / C_FONTDIM) - 1;
}
/* 
	Function -- consoleRenderBack

	Renders the background of the console.
*/
void consoleRenderBack (console_t *p)
{
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture (GL_TEXTURE_2D, p->conBackTex);

	glBegin (GL_QUADS);
		glTexCoord2f (0, C_BACKTILE);
		glVertex2f (0, p->winH);
		glTexCoord2f (C_BACKTILE, C_BACKTILE);
		glVertex2f (p->conW, p->winH);
		glTexCoord2f (C_BACKTILE, 0);
		glVertex2f (p->conW, p->conH);
		glTexCoord2f (0, 0);
		glVertex2f (0, p->conH);
	glEnd ();
}
/* 
	Function -- consoleDrawChar

	Draws a texture mapped polygon with the requested char at position (x, y).
*/
void consoleDrawChar (console_t *p, char c, conf_t x, conf_t y)
{
	glBegin (GL_QUADS);
		glTexCoord2f (tPoints[(int)c][0], tPoints[(int)c][1]);
		glVertex2f (x, y);

		glTexCoord2f (tPoints[(int)c][0] + p->tInc, tPoints[(int)c][1]);
		glVertex2f (x + C_FONTDIM, y);

		glTexCoord2f (tPoints[(int)c][0] + p->tInc, tPoints[(int)c][1] + p->tInc);
		glVertex2f (x + C_FONTDIM, y + C_FONTDIM);

		glTexCoord2f (tPoints[(int)c][0], tPoints[(int)c][1] + p->tInc);
		glVertex2f (x, y + C_FONTDIM);
	glEnd ();
}
/* 
	Function -- consoleDrawRawString

	Draws a string onto the console at a certain (x,y) coord.  This does not
	check for other strings and will write over everything.  It also does NO
	bounds checking.  Use with caution.
*/
void consoleDrawRawString (console_t *p, char *s, conf_t x, conf_t y, conf3_t color)
{
	int i;
	conf_t tx = x;

	glColor4f (color[0], color[1], color[2], 1.0f);
	glBindTexture (GL_TEXTURE_2D, p->conFontTex);
	
	for (i = 0; i < (int)strlen (s); i ++, tx += C_FONTDIM)
		consoleDrawChar (p, s[i], tx, y);
	
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
}
/* 
	Function -- consoleDrawString

	Draws a string onto the console at a certain level.  Why does this function 
	return an int?  It returns int so that we can know if we had to change
	levels on the console to draw a long string.  This will allow for repeated
	calls and have all output be on the correct level.  
*/
int consoleDrawString (console_t *p, int level, char *s, conf3_t color)
{
	int i, pos, t;
	int y = (int)((level * C_FONTDIM) + p->conH);
	int len = (int) strlen (s);
	int lev = level;
	conf_t x = 0.0;
	
	// makes sure we don't draw something that isn't on the screen
	if (y > (p->winH + C_FONTDIM))
		return level + 1;

	glColor4f (color[0], color[1], color[2], 1.0f);
	glBindTexture (GL_TEXTURE_2D, p->conFontTex);
	
	// draw out command line separater thingy, colorize this?
	if (level == 0)
	{
		consoleDrawChar (p, C_SEPCHAR, x, y);
		x += C_FONTDIM;
	}

	// nothing to render leave after drawing caret
	if (s[0] == '\0')
	{
		if (level == 0)
			consoleDrawChar (p, C_CARETCHAR, x, y);
	
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		return level + 1;
	}

	// figure out how many lines this string is going to span
	if (len > p->maxChar)
	{
		t = len;
		while (t > p->maxChar)
		{
			t -= p->maxChar;
			y += C_FONTDIM;
		}
		x = 0;
	}
	// draw!
	for (i = 0, pos = 0; i < len; i ++, x += C_FONTDIM, pos ++)
	{
		if (pos > p->maxChar - 1)
		{
			pos = 0;
			x = C_FONTDIM;
			y -= C_FONTDIM;
			lev ++;
		}
		consoleDrawChar (p, s[i], x, y);
	}

	// draw current cursor pos
	if (level == 0)
		consoleDrawChar (p, C_CARETCHAR, x, y);

	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	return lev + 1;
}	
/* 
	Function -- consoleRenderBuffers

	Render all of the consoles buffers, command line and non.
*/
void consoleRenderBuffers (console_t *p)
{
	int level = 0;
	int i;

	// draw the command line
	level = consoleDrawString (p, level, p->conBuffer[0], white);

	// draw the buffers
	for  (i = p->conBufPos; i < C_SCREENBUFF; i ++)
		level = consoleDrawString (p, level, p->conBuffer[i], grey);
}
/*
	Function -- consoleSetGLModes

	Either gets current gl modes or restores gl modes.  Used to prevent the
	console from changing things in the app that it shouldn't change.  
*/
void consoleSetGLModes (int state)
{
	static GLint matrixMode;
	static GLint polyMode[2];
	static GLboolean lightingOn;
	static GLboolean blendOn;
	static GLboolean depthOn;
	static GLboolean textureOn;
	static int blendSrc;
	static int blendDst;

	// grad the modes that we might need to change
	if (state == C_GET_MODES)
	{
		glGetIntegerv(GL_POLYGON_MODE, polyMode);

		if (polyMode[0] != GL_FILL)
			glPolygonMode (GL_FRONT, GL_FILL);
		if (polyMode[1] != GL_FILL)
			glPolygonMode (GL_BACK, GL_FILL);
	
		textureOn = glIsEnabled (GL_TEXTURE_2D);
		if (!textureOn)
			glEnable (GL_TEXTURE_2D);//glDisable (GL_TEXTURE_2D);

		depthOn = glIsEnabled (GL_DEPTH_TEST);

		if (depthOn)
			glDisable (GL_DEPTH_TEST);

		lightingOn= glIsEnabled (GL_LIGHTING);        
	
		if (lightingOn) 
			glDisable(GL_LIGHTING);

		glGetIntegerv(GL_MATRIX_MODE, &matrixMode); 
	
		// i don't know if this is correct
		blendOn= glIsEnabled (GL_BLEND);        
		if (!blendOn)
		{
			glEnable (GL_BLEND);
			glGetIntegerv (GL_BLEND_SRC, &blendSrc);
			glGetIntegerv (GL_BLEND_DST, &blendDst);
		}
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else if (state == C_RESTORE_MODES)
	{
			// put everything back where it was before
		if (polyMode[0] != GL_FILL)
			glPolygonMode (GL_FRONT, (GLenum)polyMode[0]);
		if (polyMode[1] != GL_FILL)
			glPolygonMode (GL_BACK, (GLenum)polyMode[1]);
	
		if (lightingOn)
			glEnable(GL_LIGHTING);
	
		if (!blendOn)
			glDisable (GL_BLEND);

		if (depthOn)
			glEnable (GL_DEPTH_TEST);

		if (!textureOn)
			glDisable (GL_TEXTURE_2D);//glEnable (GL_TEXTURE_2D);
		
		if (!blendOn)
			glDisable (GL_BLEND);
		else glBlendFunc ((GLenum)blendSrc, (GLenum)blendDst);	

		glMatrixMode ((GLenum)matrixMode);
	}
}
/* 
	Function -- consoleDraw

	Main console drawing system.  This should not modify any of the programs
	existing OpenGL settings.  All states should be returned to their original
	values at exit.  
*/
void consoleDraw (console_t *p)
{
	// if we are up and not animating get outta here
	if (p->conState == UP && !p->conAni)
		return;

	consoleSetGLModes (C_GET_MODES);

	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

	glOrtho (0, p->winW, 0, p->winH, -1, 1);
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	// there has to be a better way to do this
	glTranslatef (0, p->posY,0);
	
	consoleRenderBack (p);
	consoleRenderBuffers (p);
	consoleDrawRawString (p, C_APP, p->conW - (strlen (C_APP) * C_FONTDIM), p->winH - C_FONTDIM, green);
	// if user is scrolling into console buffer let them know
	if (p->conBufPos != C_HOME)
		consoleDrawRawString (p, "^", p->conW - C_FONTDIM, p->conH, white);

	glPopMatrix ();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	consoleSetGLModes (C_RESTORE_MODES);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
}
/* 
	Function -- consoleAnimate

	Animates the console, either up or down
*/
void consoleAnimate (console_t * p)
{
	if (p->conState == UP)
	{
		p->posY -= C_VEL;
		if (p->posY < 0)
		{
			p->posY = 0;
			p->conState = DOWN;
			p->conAni = 0;
		}
	} else if (p->conState == DOWN)
	{
		p->posY += C_VEL;
		if (p->posY > p->conH)
		{
			p->posY = p->conH;
			p->conState = UP;
			p->conAni = 0;
		}
	}
}
/* 
	Function -- consoleLoadFont

	Loads up the consoles font texture.  This is where changes would be made
	for your own custom image loading code.  Texture MUST contain an alpha
	channel since blending is used for the font.  
*/
void consoleLoadFont (console_t *p, char *name, int texId)
{
	p->conFontTex = texId;
	loadTGA (name, texId);
}
/* 
	Function -- consoleLoadBack.
	
	This is where changes would be made for your own image loading code
*/
void consoleLoadBack (console_t *p, char *name, int texId)
{
	p->conBackTex = texId;
	loadTGA (name, texId);
}
/* 
	Function -- consoleAddChar

	Adds a character to the current command line buffer.  
*/
void consoleAddChar (console_t *p, char c)
{
	// don't bother trying to add chars if the console is UP
	if (p->conState == UP)
		return;

	switch (c)
	{
		case '`':
			return;
		break;
		case '\r':
			consoleCommand (p);
			p->bPos = 0;
			p->conHisPos = 0;
		break;
		case '\b':
			p->bPos --;
			p->conBuffer[0][p->bPos] = '\0';
			if (p->bPos < 0)
				p->bPos = 0;	
		break;
		default :
			// something odd is going on here
			p->conBuffer[0][p->bPos] = c;
			p->bPos ++;
			if (p->bPos > C_BUFFSIZE - 2)
				p->bPos = C_BUFFSIZE - 2;
 		break;
	}
}
/* 
	Function -- consoleParseCommand

	This function is called when a user hits enter.  It then proceedes to 
	add the command to history, parse the command for variables, clear the
	command line and redraw the console.  All command line data is gone after
	a call to this function.
*/
void consoleCommand (console_t *p)
{
	if (p->conBuffer[0][0] == '\0')
		return;

	consoleHistoryAdd (p, p->conBuffer[0]);

	consoleCommandParse (p, p->conBuffer[0]);

	memset (p->conBuffer[0], '\0', sizeof (p->conBuffer[0]));

	p->reDisplay ();
}
/* 
	Function -- consoleCommandParse

	Looks through the command/variable table for a match to the command just
	typed and if a match is found call the apropriate parsing function.
*/
void consoleCommandParse (console_t *p, char *s)
{
	int i;
	char params[255];

	for (i = 0; i < C_COMMANDS; i ++)
	{
		if (!strncmp (table[i].comm, s, strlen (s)))
		{
			if (table[i].params)
			{
				if (s[strlen (table[i].comm)] == ' ')
				{
					consoleBuffCopy (params, s, 	strlen (table[i].comm) + 1, strlen (s));
					table[i].function (table[i].id, params); // pass param to the function
				} else table[i].function (table[i].id, NULL); // no param provided 
			} else table[i].function (table[i].id, NULL); // doesn't need a param 
			return;
		}
	}
	consoleBufferAdd (p, "Invalid Command!");
}
/* 
	Function -- consoleBufferAdd

	Add a string to the console buffer.  Never touches the command line.  If
	data is londer than C_BUFFSIZE the extra characters will be cut off.  If
	data is longer than 255 then it breaks :(
*/

void consoleBufferAdd (console_t *p, char *s, ...)
{
	int i = C_SCREENBUFF - 1;
	char temp[255];
	va_list	msg;

	va_start(msg, s);
	vsprintf(temp, s, msg);
	va_end(msg);

	for (; i > 1; i --)
		strncpy (p->conBuffer[i], p->conBuffer[i - 1], C_BUFFSIZE - 1);
	
	strncpy (p->conBuffer[1], temp, C_BUFFSIZE - 1);

	p->reDisplay ();
}
/* 
	Function -- consoleClearBuffers

	Clears all of the consoles buffers, history is NOT cleared.
*/
void consoleClearBuffers (console_t *p)
{
	memset (p->conBuffer, '\0', sizeof (p->conBuffer));
	p->conBufPos = C_HOME;
}
/* 
	Function -- consoleHistoryRecall

	Recalls a recent command for the consoles history.
*/
void consoleHistoryRecall (console_t *p)
{
	if (p->conHisPos >= C_HISBUFF)
		p->conHisPos = 0;

	strncpy (p->conBuffer[0], p->conCmdHis[p->conHisPos],C_BUFFSIZE - 1);

	p->bPos = strlen (p->conCmdHis[p->conHisPos]);
	p->conHisPos ++;
	
	if (p->conCmdHis[p->conHisPos][0] == '\0')
		p->conHisPos = 0;

	p->reDisplay ();
}
/* 
	Function -- consoleHistoryAdd

	Add a command into the history. 
*/
void consoleHistoryAdd (console_t *p, char *s)
{
	int i = C_HISBUFF - 1;

	for (; i > 0; i --)
		strncpy (p->conCmdHis[i], p->conCmdHis[i - 1], C_BUFFSIZE - 1);
	
	strncpy (p->conCmdHis[0], s, C_BUFFSIZE - 1);
}
/*
	Function -- consoleCommandLineClear
	
	Clears all data from the command line buffer
*/
void consoleCommandLineClear (console_t *p)
{
	memset (p->conBuffer[0], '\0', sizeof (p->conBuffer[0]));
	p->bPos = 0;
	p->reDisplay ();
}
/*
	Function -- consoleBufferMove
	
	Moves the console buffer up or down, allows you to scroll through the buffer.
*/
void consoleBufferMove (console_t *p, int dir)
{
	switch (dir)
	{
		case UP:
			p->conBufPos ++;
			if (p->conBufPos > C_SCREENBUFF - 1)
				p->conBufPos = C_SCREENBUFF - 1;
		break;
		case DOWN:
			p->conBufPos --;
			if (p->conBufPos < C_HOME)
				p->conBufPos = C_HOME;
		break;
		case HOME:
			p->conBufPos = C_HOME;
		break;
	} 
	p->reDisplay ();
}
