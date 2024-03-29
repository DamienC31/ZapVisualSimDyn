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
//The power simulator class - powersim.cpp
#include "freereign.h"

powersim::powersim(world* par)
{
	//microsim(par);
	microsim::parent = par;

}

void powersim::doSim()
{
	//TODO lookup use of iterators and use them
	cout << "Resetting conductivity..." << endl;
	/*for (int i=0;i<numStructures;i++) {
		std::cout << std::endl << "Réinitialisation de la conductivité !" << std::endl;
		vertex2D loc = structures[i];
		parent->getMap()[loc.x][loc.y].resetConducting();
	}
	cout << "Transmitting conductivity..." << endl;
	for (int i=0;i<numStructures;i++) {
		vertex2D loc = structures[i];
		parent->getMap()[loc.x][loc.y].transmitConductivity();
	}*/
	cout << "Done" << endl;
}
