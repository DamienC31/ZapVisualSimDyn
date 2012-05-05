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
//The toolbar button class - button.cpp
#include "freereign.h"

button::button(UInterface* parent, buttonbar* barparent)
{
	button::intparent = parent;
	button::barparent = barparent;
	visible = true;
	group = 0;
}

void button::setFunction(PDF func)
{
	callFunction = func;
}

void button::doFunction()
{
	(intparent->*callFunction)();
}	


void button::setStart(int x, int y)
{
	start.setPos(x,y);
}

void button::setEnd(int x, int y)
{
	end.setPos(x,y);
}


void button::setTexCoords(float tx1,float ty1,float tx2,float ty2)
{
	x1 = tx1; //0.0
	y1 = ty1; //0.0
	x2 = tx2; //0.2	
	y2 = ty2; //0.2
}

void button::drawButton()
{
	//TODO texturing stuff for nice looking buttons
	//glColor3f(0.5,0.5,0.5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,barparent->getTextureRef());
	glBegin(GL_QUADS);
		glTexCoord2f(x1,y2);
		glVertex2f(start.x,start.y);
		glTexCoord2f(x2,y2);
		glVertex2f(end.x,start.y);
		glTexCoord2f(x2,y1);
		glVertex2f(end.x,end.y);
		glTexCoord2f(x1,y1);
		glVertex2f(start.x,end.y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.5,0.5,0.5);
	glPolygonMode(GL_FRONT,GL_LINE);
	glBegin(GL_QUADS);
		glVertex2f(start.x,start.y);
		glVertex2f(end.x,start.y);
		glVertex2f(end.x,end.y);
		glVertex2f(start.x,end.y);
	glEnd();
	glPolygonMode(GL_FRONT,GL_FILL);
}

vertex2D button::getStart()
{
	return start;
}

vertex2D button::getEnd()
{
	return end;
}


int button::getGroup()
{
	return group;
}

void button::setGroup(int group)
{	
	button::group = group;
}


void button::setVisible()
{
	visible = true;
}

void button::setInvisible()
{
	visible = false;
}

bool button::getVisible()
{
	return visible;
}
