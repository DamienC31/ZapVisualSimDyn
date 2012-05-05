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
//The residence simulator class - residsim.cpp
#include "freereign.h"

residsim::residsim(world* par)
{
	//microsim(par);
	microsim::parent = par;

	//No tiles are currently in the simulator..
	numTiles = 0;
}


void residsim::addSquare(int x, int y)
{
	vertex2D inVert(x,y);
	zoned.insert(zoned.end(), inVert);
	numTiles++;
}	

void residsim::doSim()
{
	//HAcked code, just to test :)
	for (int i=0; i<numTiles;i++) {
		cell *tempCell = &parent->getMap()[zoned[i].x][zoned[i].y];
		int randNum =1+(int) (5.0*rand()/(RAND_MAX+1.0));
		if (tempCell->getStructure() == NULL && checkPower(tempCell) && 
		    checkRoad(tempCell) && randNum == 2) {
			parent->placeStructure(zoned[i].x,zoned[i].y,parent->getStructureByID(10,0), false,true);
		}
	}
}


//Check if the surrounding cells to this one have power...
bool residsim::checkPower(cell* checkCell)
{
	bool powered = false;

	int x =	checkCell->getPosition().x;
	int y = checkCell->getPosition().y;

	for (int i=x-1;i<=x+1;i++)
		for (int j=y-1;j<=y+1;j++)
			if (parent->getMap()[i][j].isConducting()) return true;

	//If we're still here...	
	return false;
}

//Check to see if the surrounding cells to this one have a road..
//This should be extended to make sure the road actually *goes*
//somewhere, i.e. along the route, goes within 1 cell of a
//industrial area, or a commercial area, bonus points for
//facilities like parks.
//This could get quite computationally expensive, but we only
//have to do it once....or do we?
//Still, today's computers should be able to keep up with it...
bool residsim::checkRoad(cell* checkCell)
{
	bool road = false;

	int x = checkCell->getPosition().x;
	int y = checkCell->getPosition().y;

	for (int i=x-1;i<=x+1;i++)
		for (int j=y-1;j<=y+1;j++)
			//Check if there is road here..
			if (parent->getMap()[i][j].getStructure() != NULL &&
			    parent->getMap()[i][j].getStructure()->getParameterByName("GROUP_ID") == 1   ) return true;
		
	
	return false;	
}

