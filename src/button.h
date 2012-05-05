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
//The toolbar button class - button.h

class button {
	public:
		button(UInterface*, buttonbar*);
		button() {}
		void setFunction(PDF);
		void doFunction();

		//Positioning stuff
		void setStart(int,int);
		void setEnd(int,int);
		void setTexCoords(float,float,float,float);
		void setGroup(int);
		int getGroup();
		void drawButton();

		vertex2D getStart();
		vertex2D getEnd();

		void setVisible();
		void setInvisible();
		bool getVisible();
		
		button &operator = (button *source) {
			// affectation au niveau des membres
			start = source->start;//getVertexRefs();
			end = source->end;
			visible = source->visible;
			intparent = source->intparent;
			barparent = source->barparent;
			return *this;
		}
		
	private:
	     void (UInterface::*callFunction)();
	     vertex2D start;
	     vertex2D end;
	     bool visible;
	     UInterface* intparent;
	     buttonbar* barparent;

	     int group;

	     float x1,x2,y1,y2; //Texture coordinates
};

