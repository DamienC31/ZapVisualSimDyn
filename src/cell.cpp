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
//The cell class - cell.cpp

#include "freereign.h"

//The constructor
cell::cell(world* parent)
{
	for (int i=0;i<3;i++) localStruct[i] = NULL;
	owns = false;
	zone = 0;
	cell::parent = parent;

	flags = 0;


}

//Calculate the normal...
void cell::recalculateNormal()
{
	//=========================================================================
	//Set the normal
	vertex3D v1(*coords[0].x,*coords[0].y,*coords[0].z);
	vertex3D v2(*coords[1].x,*coords[1].y,*coords[1].z);
  	
	vertex3D v3(*coords[2].x,*coords[2].y,*coords[2].z);
        //Work out the cross products..
        vertex3D cpx =crossProduct(v1,v2,v3);

     	//Finally get the normalised normal vector..
     	float length =sqrt(sqr(cpx.x) + sqr(cpx.y) + sqr(cpx.z));
     	nx = cpx.x / length;
     	ny = cpx.y / length;
     	nz = cpx.z / length;
 
	   
}

//Toggle whether or not this cell owns the building it points to.
void cell::setOwns(bool owns)
{
	cell::owns = owns;
}

void cell::setOwner(cell* owner)
{
	cell::owner = owner;
}

//Return ownership bit
bool cell::getOwns()
{
	return owns;
}

//Return our ownder
cell* cell::getOwner()
{
	return owner;
}

//Set the position in the map
void cell::setPosition(int x, int y)
{
	position.setPos(x,y);

	//We know at this state that we have our coord info..
	recalculateNormal(); //On init, calc the initial normal..
}

vertex2D cell::getPosition()
{
	return position;
}


//Get the zone of this cell
int cell::getZone()
{
	return zone;
}


//Set the zone of this cell
void cell::setZone(int zn)
{
	zone = zn;
}

//Set the structure at this location
void cell::setStructure(structure* str)
{
	localStruct[1] = str; //On ground
}

//Return a reference to the structure held here
structure* cell::getStructure()
{
	return localStruct[1];
}


//TODO: make it so you can build on angled tiles
//(OK, easier said than done
bool cell::isFlat()
{
	//cout << "0: " << *coords[0].getY() << endl << "2: " << *coords[2].getY() << endl;
	if (*coords[0].y == *coords[2].y &&
	    *coords[1].y == *coords[3].y)  return true;
	return false;
}



//Some electricity tests
//======================
bool cell::hasConductivity()
{
	int i;

	//First check if we have a structure here or not...
	if (localStruct[1] == NULL) return false;
	
	if ( (i=localStruct[1]->getParameterByName("CONDUCTIVE")) == 1 ) return true;
	return false;
}

bool cell::isConducting()
{
	//Whether or not this cell is already conducting for this update
	return (flags & FLAG_POWERED);
	//return false;
}	


void cell::transmitConductivity()
{
	//If this is called, we can assume that this
	//Cell is already conductive.
	if (!isConducting()) flags ^= FLAG_POWERED;
	//Now check the 4 squares around...
	vertex2D cells[5];
	cells[0].setPos(position.x-1, position.y);
	cells[1].setPos(position.x+1, position.y);
	cells[2].setPos(position.x, position.y+1);
	cells[3].setPos(position.x, position.y-1);
	

	for (int i=0;i<4;i++)
		if (parent->getMap()[cells[i].x][cells[i].y].hasConductivity() && 
			!parent->getMap()[cells[i].x][cells[i].y].isConducting() ) 
				parent->getMap()[cells[i].x][cells[i].y].transmitConductivity();
}

void cell::resetConducting()
{
	//cout << position.getX() << ":" << position.getY() << endl;
	//return;


	if (isConducting()) flags ^= FLAG_POWERED;
	for (int i=position.x-1;i<=position.x+1;i++)
		for (int j=position.y-1;j<=position.y+1;j++)
			if (parent->getMap()[i][j].isConducting() ) 
				parent->getMap()[i][j].resetConducting();
}
	




//Slope stuff
int cell::getXGradient()
{
	float x1 = ( (*coords[1].y - *coords[0].y) / CELL_SIZE);
		//cout << "X1: "  << x1<< endl;
	float x2 =   ( (*coords[2].y - *coords[3].y) / CELL_SIZE);
		//cout << "X2: " << x2 << endl;
	if ( fabs(x1 - x2) < DELTA ) {
		if (x1 > 0)
			return 1;
		else if (x1 < 0) //Make sure they're not equal still...
			return -1;
		else 
			return 0;
	} else
		return 5;  //doubly sloped in this dir...
}


int cell::getZGradient()
{
	float z1 =  ( (*coords[2].y - *coords[1].y) / CELL_SIZE);
		//cout << "z1: " << z1 << endl;
	float z2 =  ( (*coords[3].y - *coords[0].y) / CELL_SIZE);
		//cout << "z2: " << z2 << endl;
	
	if (fabs(z1-z2) < DELTA) {
		if (z1 > 0)
			return 1;
		else if (z1 < 0)	
			return -1;
		else 		
			return 0;
 
	} else
		return 5; //Doubly sloped in this dir...
}


