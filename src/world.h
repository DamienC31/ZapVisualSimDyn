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
//The global world class - world.h

class world {
	public:
		world(int, int);
		~world();
		cell* getCell(int x, int y);
		vertex2D getCellPosition(float x, float y);
		void setZoneColor(int);
		void doZone(int,int,int,int,int);
		void setPick(int x, int y);
		float getAvHeight(int,int);
		bool checkHeight(int x, int y, float height,int);
		float getMaxHeight(int,int);
		float getMinHeight(int,int);

		//Map stuff
		int getXSize();
		int getYSize();
		cell** getMap();
		bool saneRef(vertex2D);
		bool saneRef(vertex2D*);
		void changeTerrain(int, int, int);

		//Camera stuff
		float getCamX();
		float getCamY();
		float getCamZ();
		void setRotate(int);
		int getRotate();
		void setCamY(float);
		void setLookX(float x);
		void setLookY(float x);
		void setLookZ(float z);
		float getLookX();
		float getLookY();
		float getLookZ();


		
		//Routing/Structure building functions
		structure* doRoute(structure*,int,int);
		structure* doBevel(structure *,int,int);

		//Accessors
		user getUser();
		UInterface* getUI();
		render* getRenderer();
		structure* structureRefs();
		structure* getStructureByID(unsigned int,unsigned int);
		residsim* getResidSim();

		//Game state stuff
		bool placeStructure(int,int, structure*,bool,bool);
		bool isRouteStructure(int,int,int);
		bool destroyStructure(int, int);
		void checkUpdate();
		void updateState();

		//Microsim accessors
	
	private:
		bool getStructure(char* filename);
		bool loadStructures();
		user currentUser;		


		UInterface *UI;
		cell **map;
		//cell map[MAP_HEIGHT][MAP_WIDTH];

		float* coords;
		structure *structures; //The structures (buildings, trees, whatever)
		unsigned int numStructures; //Number of structures in the world.
		unsigned int nextStructure; //The next structure to load
		//unsigned int* indices;
		int XSIZE, YSIZE;
		float cam_x, cam_y, cam_z;
		float look_x, look_y, look_z;
		int rotate; //The rotation angle in radians

		unsigned long int ticks;
		unsigned long int start_ticks;
		unsigned long int last_update;

		//int pick1, pick2;
		//int lastx, lasty;

		//The microsims
		powersim* power; //The power sub system simulator
		residsim* resid; //The residential/housing simulator

		//The renderer
		render* renderer;
};
 
