/*
 *   Free Reign
 *
 *   Copyright(C) 1999 Alexander Taylor <ataylor@ihug.com.au>
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */
//The main header file - freereign.h

//Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <math.h>

//OpenGL includes
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>


//Keep with the standards...
using namespace std;

//Class headers
class user;
class cell;
class structure;
class vertex2D;
class vertex3D;
class parameter;
class microsim;
class powersim;
class residsim;
class vertex3Dp;
class map;
class world;
class UInterface;
class button;
class buttonbar;
class texture;
class render;

//The UI general def
typedef void (UInterface::*PDF)();

//Several routines..
void text(int x, int y, char* s); //A GL text drawing function
vertex3D crossProduct(vertex3D v1, vertex3D v2, vertex3D v3);
float sqr(float);


//Our local includes
#include "console.h"
#include "tga.h"
#include "user.h"
#include "texture.h"
#include "vertex3Dp.h"
#include "vertex2D.h"
#include "vertex3D.h"
#include "parameter.h"
#include "face.h"
#include "structure.h"
#include "cell.h"
#include "microsim.h"
#include "powersim.h"
#include "residsim.h"
#include "render.h"
#include "world.h"
#include "UInterface.h"
#include "button.h"
#include "buttonbar.h"

//Hard defines
#define CELL_SIZE 0.3f //Cell size in GL units
#define HEIGHT 480 //Window height
#define WIDTH 770 //Window width
#define PI 3.14153265535
#define NONAME -99 //The value returned if a parameter doesn't exist
		   //For an object

#define MAXUP 15 //Max number of cells map goes up
#define MAXDOWN -15 //MAx down
#define UPDATE 4 //Number of seconds to wait between updates
#define TOOLBOX_WIDTH 130
#define OUTOFBOUNDS -98 //Used to define a cell ref that is illegal

#define DELTA 0.2f //The max acceptance in float errors..can afford to be big :)
#define DELTA_2 0.09f //For current cell calculators
#define DELTA_3 0.05f //For height checking
#define CELL_HEIGHT 0.16f


//COnsole defines...
#define C_ID_ABOUT                              0               // ids for commands
#define C_ID_QUIT                               1
#define C_ID_VERSION                    2
#define C_ID_ECHO                               3
#define C_ID_CLEAR                              4



float mmDotProduct(float v1[3], float v2[3]);


//Flags
#define FLAG_POWERED       (0x10)
/*#define FLAG_FED           (0x20)
#define FLAG_EMPLOYED      (0x40)
#define FLAG_IS_TRANSPORT  (0x80)
#define FLAG_MB_FOOD       (0x100)
#define FLAG_MS_FOOD       (0x200)
#define FLAG_MB_JOBS       (0x400)
#define FLAG_MS_JOBS       (0x800)
#define FLAG_MB_COAL       (0x1000)
#define FLAG_MS_COAL       (0x2000)
#define FLAG_MB_ORE        (0x4000)
#define FLAG_MS_ORE        (0x8000)
#define FLAG_MB_GOODS      (0x10000)
#define FLAG_MS_GOODS      (0x20000)
#define FLAG_MB_STEEL      (0x40000)
#define FLAG_MS_STEEL      (0x80000)
#define FLAG_FIRE_COVER    (0x100000)
#define FLAG_HEALTH_COVER  (0x200000)
#define FLAG_CRICKET_COVER (0x400000)
#define FLAG_IS_RIVER      (0x800000)
#define FLAG_HAD_POWER     (0x1000000)*/


//Some more game state stuff
#define RESID_COST 10 //The cost of residential zoning
#define COMME_COST 15 //The cost of commercial zoning
#define INDUS_COST 20 //The cost of industrial zoning
#define TERRAIN_COST 100 //The cost of terraforming..  Terrible :)


