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
//The cell class - cell.h

class cell {
	public:
		cell(world *);
		cell() {}
		void setOwns(bool);
		void setOwner(cell*);
		cell* getOwner();
		bool getOwns();

		//Accessor stuff
		vertex2D getPosition();
		void setPosition(int x, int y);		

		void setHeight(float height);
		float getHeight();
		void setZone(int zn); //See plan file
		int getZone();
		vertex3Dp coords[4]; //Pointers to the 4 edges of this cell
		void setStructure(structure*);
		structure* getStructure(); //Return a reference to the structure held here
		bool isFlat();

		//Cell slope stuff
		int getXGradient();
		int getZGradient();
		void setYRot(int);
		void setZRot(int);
		void recalculateNormal();
		
		int getYRot();
		int getZRot();

		//Power stuff - Local cell info
		bool hasConductivity();
		bool isConducting();
		void transmitConductivity();
		void resetConducting();


		//Normal stuff
		float nx,ny,nz; //The normal data

		cell &operator = (cell *source) {
			// affectation au niveau des membres
			position = source->position;//getVertexRefs();
			height = source->height;//getNumVertices();
			zone = source->zone;//getNextVert();
			localStruct[0] = source -> localStruct[0];
			localStruct[1] = source -> localStruct[1];
			localStruct[2] = source -> localStruct[2];
			//localStruct = source->localStruct;//getParent();
			owns = source->owns;//getType();
			owner =source->owner;
			parent =source->parent;
			yrot =source->yrot;
			zrot =source->zrot;
			flags =source->flags;
			return *this;
		}
	private:
		vertex2D position; //Our position in the map
		float height; //The average height of this cell (may be depretated)
		int zone;
		structure* localStruct[3]; //Sub/stand/superterrainian
		bool owns;  //Whether or not this cell owns the structure
		cell* owner; //The cell that owns this structure here
		
		world* parent; //The parent world

		//bool conducting; //Whether this cell currently conducts electricity.

		int yrot,zrot; //The x and y rotation factors of this cell.
		int flags; //The flags applicable to this cell.

};
