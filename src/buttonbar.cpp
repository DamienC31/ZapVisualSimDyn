/*
 *   Free Reign
 *
 *   Copyright(C) 2000 Alexander Taylor <ataylor@ihug.com.au>
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
//The toolbar class - buttonbar.cpp

#include "freereign.h"

buttonbar::buttonbar(UInterface *parent,int numButtons, char *texFilename)
{
	barTexture.loadTexture(texFilename);
	buttonbar::parent = parent;
	buttonbar::numButtons = numButtons;
	//buttons = new button[numButtons](parent,this);
	
	buttons = new button[numButtons];
	
	for ( int i = 0 ; i <= numButtons - 1 ; i++ ) {
		buttons[i] = new button(parent,this);
	}

	nextButton = 0;
}


void buttonbar::displayGroup(int groupID)
{
	for (int i=0;i<numButtons;i++) {
		if (buttons[i].getGroup() == groupID) buttons[i].setVisible();
	}
}

void buttonbar::hideAllGroups()
{
	for (int i=0;i<numButtons;i++) {
		//Hide if not universal group
		if (buttons[i].getGroup() != 0) buttons[i].setInvisible();
	}
}


void buttonbar::drawButtons()
{
	for (int i=0;i<numButtons;i++) {
		if (buttons[i].getVisible() || buttons[i].getGroup() == 0) buttons[i].drawButton();
	}
}

void buttonbar::addButton(int x1, int y1, int x2, int y2, int group, PDF function, float tx1, float ty1,float tx2,float ty2)
{
 	buttons[nextButton].setStart(x1,y1);
	buttons[nextButton].setEnd(x2,y2);
	buttons[nextButton].setFunction(function);
	buttons[nextButton].setTexCoords(tx1,ty1,tx2,ty2);
	buttons[nextButton].setGroup(group);
	nextButton++;
}

void buttonbar::checkButtons(int x, int y)
{
   for (int i=0;i<numButtons;i++) {
	//Check to see if the x,y values are inside any of the 
	//Buttons boundaries
	if (x > buttons[i].getStart().x && x < buttons[i].getEnd().x
	  &&y > buttons[i].getStart().y && y < buttons[i].getEnd().y
	  && buttons[i].getVisible()) {
		//Run this buttons callback
		if (buttons[i].getGroup() == 0) hideAllGroups();
		buttons[i].doFunction();
	}
   }
}

GLuint buttonbar::getTextureRef()
{
	return barTexture.getTextureObj();
}

