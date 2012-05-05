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
//The global world class - world.cpp



#include "freereign.h"



world::world(int xsize, int ysize)
{
	XSIZE = xsize;
	YSIZE = ysize;

	//pick1 = pick2  = 0;
	//selecting = false;
		

	//Set up the camera/viewing location
	cam_y = 10.f;
	look_x = -4.0f;
	look_y = 0.0;
	look_z = -6.0f;
	setRotate(40); //X,Z set by setrotate
	rotate = 40;


	nextStructure = 0;
	//Now allocate memory	
	/** ERROR **/
	//map = new cell*[XSIZE+2](this);
	
	map = new cell*[XSIZE+2];
	for ( int i = 0 ; i <= XSIZE + 1 ; i++ ) {
		map[i] = new cell[YSIZE+2];
	}

	for ( int i = 0 ; i <= XSIZE + 1 ; i++ ) {
		for ( int j = 0 ; i <= YSIZE + 1 ; i++ ) {
		  map[i][j] = new cell(this);
		}
	}

	 /*grid = new int*[wprime];
  for (int i = 0 ; i < wprime ; i++)
      grid[i] = new int[hprime];
	
	map = new cell*[XSIZE+2][YSIZE+2];
	
	for ( int i = 0 ; i <= XSIZE + 1 ; i++ ) {
		for ( int j = 0 ; i <= YSIZE + 1 ; i++ ) {
		  map[i][j] = new cell(this);
		}
	}*/

        coords = new float[(XSIZE+1)*(YSIZE+1)*3*4+12];  //XSIZE*YSIZE*3 for x,y,z*4 for quads

	//Set up the cell coordinates
	unsigned long count=0;
	for (int i=0;i<YSIZE+1;i++) 
        	for (int j=0;j<XSIZE;j++) {
	                coords[count] = (i*CELL_SIZE) - (XSIZE*CELL_SIZE/2);
	                coords[count+1] = 0;
	                coords[count+2] = (j*CELL_SIZE) - (YSIZE*CELL_SIZE/2);
	                count += 3;
        }

	//...Now assign them to the map
	count = 0;
	for (int i=0;i<YSIZE+1;i++)
        	for (int j=0;j<XSIZE+1;j++) {
	                //indices[count] = XSIZE*3*(i) + 0+(j*3);
	     	      float* tmpx = &coords[XSIZE*3*(i) + 0+(j*3)];
	                float* tmpy = &coords[XSIZE*3*(i) + 1+(j*3)];
	                float* tmpz = &coords[XSIZE*3*(i) + 2+(j*3)];
			map[i][j].coords[0].setPos(tmpx,tmpy,tmpz);

	                //indices[count+1] = XSIZE*3*(i) + 3+(j*3);
	                tmpx = &coords[XSIZE*3*(i) + 3+(j*3)];
	                tmpy = &coords[XSIZE*3*(i) + 4+(j*3)];
	                tmpz = &coords[XSIZE*3*(i) + 5+(j*3)];
			map[i][j].coords[1].setPos(tmpx,tmpy,tmpz);

	                //indices[count+2] = XSIZE*3*(i+1) + 3+(j*3);
	                tmpx = &coords[XSIZE*3*(i+1) + 3+(j*3)];
	                tmpy = &coords[XSIZE*3*(i+1) + 4+(j*3)];
	                tmpz = &coords[XSIZE*3*(i+1) + 5+(j*3)];
			map[i][j].coords[2].setPos(tmpx,tmpy,tmpz);
			
			//indices[count+3] = XSIZE*3*(i+1) + (0+(j*3));
	                tmpx = &coords[XSIZE*3*(i+1) + (0+(j*3))];
	                tmpy = &coords[XSIZE*3*(i+1) + (1+(j*3))];
	                tmpz = &coords[XSIZE*3*(i+1) + (2+(j*3))];
			map[i][j].coords[3].setPos(tmpx,tmpy,tmpz);
	                count +=4;      

			map[i][j].setPosition(i,j);
        	}

	//Some final stuff
	//lastx=lasty=0;
	//selecting = false;

	//Make the user interface
	UI = new UInterface(this);
	bool success = loadStructures(); //Load the models

	//Init the microsims;
	power = new powersim(this);
	resid = new residsim(this);


	//Stick a few in for fun....	
	bool canset = placeStructure(30,15,&structures[0],false,true);
	canset = placeStructure(5,5,&structures[1],false,true);

	//Set up the timing/gamestate system
	time_t starter;
	starter = time(NULL);
	last_update = 0;
	cout << "Starting timer at: " << starter << endl;


	//Init the renderer
	//cout << "Starting renderer.." << endl;
	renderer = new render(this);
	//cout << "Started renderer..." << endl;

}

world::~world()
{
	cout << "Deallocating memory from map..." << endl;
	for (int c=0; c<XSIZE; c++) {
            delete[] map[c];
        }
        delete[] map;
        delete[] coords;
	delete[] structures;
	delete power;
	delete resid;
	delete renderer;
	delete UI;
	
}


//===========================
//Check if it's time to update the game state
void world::checkUpdate()
{
	ticks = time(NULL);
	if (ticks - last_update >= UPDATE) {
		updateState();
		last_update = ticks;
	}
}
	


//Game state functionality
//------------------------------------------------
void world::updateState()
{
	//cout << "Updating state..." << endl;
	power->doSim();
	resid->doSim();
}



UInterface* world::getUI()
{
	return UI;
}


render* world::getRenderer()
{
	return renderer;
}



cell** world::getMap()
{
	return map;
}





//Zone an area on the map...
void world::doZone(int x1, int y1, int x2, int y2, int zone_type)
{
	int size=abs(x2-x1)*abs(y2-y1);
	//Calculate cost
	int cost = 0;
	switch (zone_type) {
		case 1:  cost = RESID_COST*size; break;
		case 2:  cost = COMME_COST*size; break;
		case 3:  cost = INDUS_COST*size; break;
	}

	for (int i=y1;i<=y2;i++)
		for (int j=x1;j<=x2;j++) {
			map[j][i].setZone(zone_type);
			resid->addSquare(j,i);
		}
		
	//Now decrement it..
	currentUser.decrementFunds(cost);

}



//===============================================
//A few height/cell calculator functions
vertex2D world::getCellPosition(float x, float z)
{
	const float delta = 0.05f; //For the rounding stuff

	//First get the cell at (x,y)
	float lowx = *map[0][0].coords[0].x;
	float lowz = *map[0][0].coords[0].z;
	
	int xcell = (int)( ((fabs(lowx) + x) / CELL_SIZE) + delta);
	int zcell = (int)( ((fabs(lowz) + z) / CELL_SIZE) + delta);
	vertex2D ret;
	ret.setPos(xcell,zcell);
	return ret;
}

float world::getAvHeight(int x, int y)
{
        //Now get average height...
        //float avheight = 0.0f;
	float avHeight = *map[x][y].coords[0].y;
        if ( (x >= 0 && x <= XSIZE) && (y >= 0 && y <= YSIZE) ) {
		for (int i=0;i<4;i++)
			if (*map[x][y].coords[i].y < avHeight) avHeight = *map[x][y].coords[i].y;
        }
        return avHeight;
}


//Returns true if height is insane (i.e. we can't change the height
//because it would screw up the world.
bool world::checkHeight(int x, int y, float height, int upordown)
{
  //Check
  //x-1,y-1      x,y-1  x+1,y-1
  //x-1,y        x,y    x+1,y
  //x-1,y+1      x,y+1  x+1,y+1

  bool insane = false;
  for (int j=x-1;j<=x+1;j++) 
	for (int k=y-1;k<=y+1;k++)
		for (int i=0;i<4;i++)
			if (k>=0 && j>= 0) 
				if ( ((*map[k][j].coords[i].y+CELL_HEIGHT < height-DELTA_3 && upordown==1) 
				|| (*map[k][j].coords[i].y-CELL_HEIGHT > height+DELTA_3 && upordown ==0)))
					insane = true;

  if (insane) cout << "Insane ref..." << endl;
  return insane;
	
}


bool world::saneRef(vertex2D ref)
{
	if (ref.x >= 0 && ref.y >= 0 && ref.x < XSIZE && ref.y < YSIZE) return true;
	return false;
}

bool world::saneRef(vertex2D* ref)
{
	return saneRef(*ref);
}


//=====================================
//A few accessor functions
user world::getUser()
{
	return currentUser;
}



float world::getCamX()
{
	return cam_x;
}

float world::getCamY()
{
	return cam_y;
}

float world::getCamZ()
{
	return cam_z;
}

void world::setCamY(float y)
{
	cam_y = y;
}

void world::setLookX(float x)
{
	look_x = x;
}

void world::setLookY(float y)
{
	look_y = y;
}

void world::setLookZ(float z)
{
	look_z = z;
}

float world::getLookX()
{
	return look_x;
}

float world::getLookZ()
{
	return look_z;
}

float world::getLookY()
{
	return look_y;
}


int world::getRotate()
{
	return rotate;
}


void world::setRotate(int rotate)
{
	world::rotate = rotate;

	cam_x = look_x + (sin(rotate*PI/180) * 10.0);
	cam_z = look_z + (cos(rotate*PI/180) * 10.0);
}


int world::getXSize()
{
	return XSIZE;
}

int world::getYSize()
{
	return YSIZE;
}

structure* world::structureRefs()
{
	return structures;
}


//Microsim
residsim* world::getResidSim()
{
	return resid;
}



//=========================


//Raise or lower the terrain
//0=down,1=up
void world::changeTerrain(int x, int y, int upOrDown)
{
   int pickx = x;
   int picky = y;
   float height= *map[pickx][picky].coords[0].y;
   for (int i=0;i<4;i++) {
	if ( (upOrDown == 1 && *map[pickx][picky].coords[i].y < height) ||
	     (upOrDown == 0 && *map[pickx][picky].coords[i].y > height)) {
		height = *map[pickx][picky].coords[i].y;
	}
   }
  
   //If invalid height, make it so (so I like star trek :)
   //This needs cleanup..
   //This stuff is way unstable, make it an option...
   /*if ( (upOrDown==1 && checkHeight(picky,pickx,height+CELL_HEIGHT,1))) {
	  //cout << "One or more is invalid..." << endl;
	  for (int j=picky-1;j<=picky+1;j++) 
	     for (int k=pickx-1;k<=pickx+1;k++)
		for (int i=0;i<4;i++)
	           if ((*map[k][j].coords[i].y+CELL_HEIGHT < height+CELL_HEIGHT-DELTA_3)) {
			//cout << "Invalid terrain...changing..." << endl;
			if (saneRef(new vertex2D(k,j))) changeTerrain(k,j, 1);
		   }
   }*/



   if ( (upOrDown==1 && !checkHeight(picky, pickx, height+CELL_HEIGHT,1)) ||
	(upOrDown==0 && !checkHeight(picky, pickx, height-CELL_HEIGHT,0)) ) {
      float mod=0.0;
      switch (upOrDown) {
	case 0: mod = -CELL_HEIGHT; break;
	case 1: mod = CELL_HEIGHT; break;
      }
      for (int i=0;i<4;i++) {
		*map[pickx][picky].coords[i].y=height+mod;
      }

      //Now destroy buildings/structures that were on the changed cells
      //TODO: this doesn't always work.  Check to see if a cell changed
      //before we destroy some poor guy's hard work.
      for (int i=pickx-1;i<=pickx+1;i++)
          for (int j=picky-1;j<=picky+1;j++) {
		destroyStructure(i,j);
		map[i][j].recalculateNormal(); //Changed squares need new normals...
	  }


      //Now bill 'em
      currentUser.decrementFunds(TERRAIN_COST);
		
   }
}

float world::getMaxHeight(int x, int y)
{
	float height = *map[x][y].coords[0].y;
	for (int i=0;i<4;i++) {
		if (*map[x][y].coords[i].y > height) 
			height = *map[x][y].coords[i].y;
	}
	return height;
}

float world::getMinHeight(int x, int y)
{
	float height = *map[x][y].coords[0].y;
	for (int i=1;i<4;i++) {
		if (*map[x][y].coords[i].y < height) height = *map[x][y].coords[i].y;
	}
	return height;
}



void world::setPick(int x, int y)
{
	//pick1 = x;
	//pick2 = y;
}






//===================================================================
bool world::loadStructures()
{
	numStructures = 32;
	nextStructure = 0;
	structures = new structure[numStructures];
	bool success;

	//TODO: make a stash index file, get rid of this mess
	success = getStructure("models/coal.st1");          //1
	success = getStructure("models/power_line0.st1");   //2
	success = getStructure("models/power_line1.st1");   //3
	success = getStructure("models/power_line2.st1");   //4
	success = getStructure("models/power_line3.st1");   //5
	success = getStructure("models/power_line4.st1");   //6
	success = getStructure("models/power_line5.st1");   //7
	success = getStructure("models/power_line6.st1");   //8
	success = getStructure("models/power_line7.st1");   //9
	success = getStructure("models/power_line8.st1");   //10
	success = getStructure("models/power_line9.st1");   //11
	success = getStructure("models/power_line10.st1");  //12
	success = getStructure("models/power_line11.st1");  //13
	success = getStructure("models/power_line12.st1");  //14
	success = getStructure("models/power_line13.st1");  //15
	success = getStructure("models/power_line14.st1");  //16
	success = getStructure("models/house.st1");         //17
	success = getStructure("models/road_0.st1");        //18
	success = getStructure("models/road_1.st1");        //19
	success = getStructure("models/road_2.st1");	    //20
	success = getStructure("models/road_3.st1");	    //21
	success = getStructure("models/road_4.st1");	    //22
	success = getStructure("models/road_5.st1");        //23
	success = getStructure("models/road_6.st1");        //24
	success = getStructure("models/road_7.st1");        //25
	success = getStructure("models/road_8.st1");        //26
	success = getStructure("models/road_9.st1");        //27
	success = getStructure("models/road_10.st1");       //28
	success = getStructure("models/road_11.st1");       //29
	success = getStructure("models/road_12.st1");       //30
	success = getStructure("models/road_13.st1");       //31
	success = getStructure("models/road_14.st1");       //32



}


//===========================================
//Structure/Building loaders
//Eventually, this will all be put in a "stash" (get it? wad - pak - stash
// ..anyways) file...all models and info on them will be stored in it.
//It is just this way so I can get on with programming the game models
//Without having to write a loader for dxf models (I'm using blender, it
//onyl exports dxfs...
bool world::getStructure(char *filename)
{
	ifstream from(filename);
	cout << "Reading from " << filename << endl;
	string line;
	//Get the solid color of object
	//Man this is kludgy.  Will be glad to get rid of this
	float r,g,b;
	from >>r;from>>g;from>>b;
	structures[nextStructure].setColor(r,g,b);

	while (!from.eof()) {
		from >> line;
		if (line == "VERTEX") {
			cout << "Getting: " << line << endl;
			int numverts;
			from >> numverts;
			cout << "    Number of vertices: " << numverts << endl;
			float x,y,z;
			structures[nextStructure].initVerts(numverts);
			cout << "Initialised verts..." << endl;
			for (int i =0;i<numverts;i++) {
				from >> x; from >> y; from >> z;
			 	structures[nextStructure].addVertex(x,y,z);
				cout << "    Vert " << i << ": (" << x << "," << y << "," << z << ")" << endl;
			}		
		}
		if (line == "FACE") {
			cout << "Getting faces.." << endl;
			int numFaces;
			from >> numFaces;
			cout << "    Number of faces: " << numFaces << endl;
			structures[nextStructure].initFaces(numFaces);
			for (int i=0;i<numFaces;i++) {
				int numRefs;
				from >> numRefs; //Get the number of vert refs for this face
				cout << "      Number of refs for face " << i << ": " << numRefs << endl;
				cout << "        They are: ";
				unsigned int* refs = new unsigned int[numRefs+1];
				int type;
				from >> type;
				for (int j=0;j<numRefs;j++) {
					from >> refs[j];
					cout << refs[j] << " ";
				}
				cout << endl;
				//Convert our types to GLenums
				GLenum typeEnum;
				switch (type) {
					case 2: typeEnum = GL_LINES; break;
					case 3: typeEnum = GL_TRIANGLES; break;
					case 4: typeEnum = GL_QUADS; break;
				}
				structures[nextStructure].addFace(typeEnum, numRefs,refs);
				//Finished getting the face
			}
		}
		if (line == "PARAMETERS") {
			cout << "Getting parameters..." << endl;
			int numParameters;
			from >> numParameters;
			cout << "    Number of parameters..." << numParameters << endl;
			structures[nextStructure].initParameters(numParameters);
		 	for (int i=0;i<numParameters;i++) {
				string name,value;
				from >> name; from >> value;
				cout << "     " << name << " : " << value << endl;
				structures[nextStructure].addParameter(name,value);	
			
			}
			cout << "Exiting parameters.." << endl;
		}
		line = "";
	}
	from.close();

	//Now create this structure's display list
	structures[nextStructure].createList();
	nextStructure++;

	//Loaded structure...
	cout << "Done Loading structure" << endl;
	return true;
}





//===================================================================
//Place a building
//Override means replace a structure with the new one.
bool world::placeStructure(int x, int y, structure* placStruc,bool override,bool noRoute)
{
	structure *applyStruc;
	applyStruc = placStruc; //Initially assume we are placing this structure
	vertex2D size = placStruc->getSize();


	//Check if there's already something here if override is false
	//i.e. build regardless
	if (override == false) {
		for (int i=x;i<x+size.x;i++)
			for (int j=y;j<y+size.y;j++)
				if (map[i][j].getStructure() != NULL) return false;
	}


	if (! map[x][y].isFlat()) {
	  if (map[x][y].getZGradient() != 0.0 && map[x][y].getXGradient() != 0.0) {
		//Whoa...double sloped tile..
		cout << "Placstruc returned false..." << endl;
		return false;
	  } else  {
		applyStruc = doBevel(placStruc,x,y); //Else get the appropriate bevelled tile

		//If it's a double sloped tile, return false...
		if (applyStruc == NULL) return false;

	  }
	} else {
		//Only process routing information if this building is part of a route
		//i.e. a road, power line, highway etc..
		if (placStruc->getParameterByName("ISROUTE") == 1 && !noRoute) {
			cout << "Doing route..." << endl;
			applyStruc = doRoute(placStruc,x,y);
		}
	}



	//There's nothing here, so proceed
	//Now set all the squares to owner/etc...
	for (int i=0;i<size.x;i++)
		for (int j=0;j<size.y;j++) {
			map[x+i][y+j].setOwner(&map[x][y]);
			map[x+i][y+j].setOwns(false);
			map[x+i][y+j].setStructure(applyStruc);
		}
	
	
	//Set the main square to the structure	
	map[x][y].setStructure(applyStruc);
	map[x][y].setOwns(true);
	map[x][y].setOwner(NULL);

	//Decrement their funds by the relevant amount...
	//If no route is set,we're just adjusting, therefore give them a freebie
	if (placStruc->getParameterByName("ISROUTE") == 1 && !noRoute)
		currentUser.decrementFunds(getStructureByID(placStruc->getParameterByName("GROUP_ID"),0)
				->getParameterByName("UNIT_COST"));
	else if (!noRoute)
		currentUser.decrementFunds(placStruc->getParameterByName("UNIT_COST"));

	//Now fix our neighbours (maybe...)
	if (placStruc->getParameterByName("ISROUTE") && !noRoute) {
		if (isRouteStructure(x-1,y,applyStruc->getParameterByName("GROUP_ID"))) 
			placeStructure(x-1,y,doRoute(placStruc,x-1,y), true, true);
		if (isRouteStructure(x+1,y,applyStruc->getParameterByName("GROUP_ID"))) 
			placeStructure(x+1,y,doRoute(placStruc,x+1,y), true, true);	
		if (isRouteStructure(x,y+1,applyStruc->getParameterByName("GROUP_ID"))) 
			placeStructure(x,y+1,doRoute(placStruc,x,y+1), true, true);
		if (isRouteStructure(x,y-1,applyStruc->getParameterByName("GROUP_ID"))) 
			placeStructure(x,y-1,doRoute(placStruc,x,y-1), true, true);
	}


	//Now see if it's going to a microsim....
	if (placStruc->getParameterByName("MICROSIM") != NONAME) {
		switch (placStruc->getParameterByName("MICROSIM")) {
			case 1: power->addStructure(x,y);
				break;
		}
	}		


	return true; //TODO: return something useful - like whether we can
		     //place here or not
}


bool world::isRouteStructure(int x, int y, int ID)
{
	if (map[x][y].getStructure() != NULL && map[x][y].getStructure()->getGroupID() == ID) return true;
  	
	return false; //No match
}

structure* world::doRoute(structure* placStruc, int x,int y)
{
	structure* applyStruc= placStruc;


       //Routing stuff - i.e. roads,power lines,highways etc....
        //TODO needs redesigning to be more generic
	//====================================================================
	
	//Check left and right if there is a structure in this group
	//If so, use the left/right model


	//First of all, decide what kind of layout we have
	//-------
	//| | | |
	//|-|-|-|
	//| |X|N| <--- x+1
	//|-|-|-|
	//| |N| |
        //-------

	//   ^
	//   |
	//  y + 1

	//Case 1: x+1 --Straight horiz                   
	//Case 2: y+1 -Straight vertical                 
	//Case 3: x-1 --Straight horiz                   
	//Case 4: y-1 --Straight vertical                
	//Case 5: x-1, x+1 --Straight Horiz             
	//Case 6: y-1, y+1 --Straight Vertical          
	//Case 7: x+1, y+1 -Corner                      
	//Case 8: x-1, y-1 -Corner                      
	//Case 9: x+1, y-1 -Corner                      
	//Case 10: x-1, y+1 - Corner			
	//Case 11: x-1, y-1, y+1 - TJunction             
	//Case 12: x+1, y-1, y+1 - TJunction
	//Case 13: x-1, x+1, y+1 - TJunction
	//Case 14: x-1, x+1, y-1 - TJunction
	//Case 15: x-1, x+1, y-1, y+1 - Crossroads


	//Set up an array representing the current state of affairs...
	bool rt[3][3];

	for (int i=0;i<3;i++) for (int j=0;j<3;j++) rt[i][j] = false;

	for (int i=y-1;i<=y+1;i++) 
	   for (int j=x-1;j<=x+1;j++) {
		rt[i-y+1][j-x+1] = isRouteStructure(j,i,placStruc->getGroupID());
	   }

	//The route caser..
	int routeCase = 0;

	cout << "Where we are..." << endl;
	cout << " ---\n";
	for (int i=0;i<3;i++) {
	  cout << "|";
	  for (int j=0;j<3;j++) {
		if (rt[i][j] ==true) 
			cout << "*"; 
		else 
			cout << " ";
	  }
	  cout << "|" << endl;
	}

	//FIXME THis is a bit of a kludge
	rt[1][1] = false;


	//Case 1
	if (             !rt[0][1] &&
	    !rt[1][0]                 && rt[1][2] &&
	                 !rt[2][1]) {
		routeCase = 1;

		//Now return what we're doing...
		cout << "Doing new route..." << endl;
		return 	getStructureByID(placStruc->getGroupID(),getStructureByID(placStruc->getGroupID(),0)->getParameterByName("RIGHT"));
		


	} else if (         rt[0][1]&&
	       !rt[1][0] &&            !rt[1][2] &&
	                    !rt[2][1]) {
		routeCase = 2;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(),getStructureByID(placStruc->getGroupID(),0)->getParameterByName("UPDOWN"));

	} else if (         !rt[0][1] &&
	       rt[1][0] &&               !rt[1][2] &&
	                    !rt[2][1]) {
		routeCase = 3;
		cout << "Doing new route..." << endl;
		return 	getStructureByID(placStruc->getGroupID(),getStructureByID(placStruc->getGroupID(),0)->getParameterByName("RIGHT"));

	} else if (         !rt[0][1] &&
	       !rt[1][0] &&              !rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 4;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(),getStructureByID(placStruc->getGroupID(),0)->getParameterByName("UPDOWN"));

	} else if (         !rt[0][1] &&
	       rt[1][0] &&              rt[1][2] &&
	                    !rt[2][1]) {
		routeCase = 5;
		cout << "Doing new route..." << endl;
		return 	getStructureByID(placStruc->getGroupID(),getStructureByID(placStruc->getGroupID(),0)->getParameterByName("RIGHT"));

	} else if (         rt[0][1] &&
	       !rt[1][0] &&             !rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 6;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(),getStructureByID(placStruc->getGroupID(),0)->getParameterByName("UPDOWN"));

	} else if (         !rt[0][1] &&
	       !rt[1][0] &&              rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 7;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("UP_RIGHT_CORNER"));

	
	} else if (         rt[0][1] &&
	       rt[1][0] &&               !rt[1][2] &&
	                    !rt[2][1]) {
		routeCase = 8;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("LEFT_CORNER"));

	} else if (         rt[0][1] &&
	    !rt[1][0] &&                rt[1][2] &&
	                    !rt[2][1]) {
		routeCase = 9;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("RIGHT_CORNER"));

	} else if (         !rt[0][1] &&
	        rt[1][0] &&              !rt[1][2] &&
	                    rt[2][1]) {
		routeCase  = 10;
		cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("UP_LEFT_CORNER"));



	} else if (         rt[0][1]&&
	        rt[1][0] &&              !rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 11;
	cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("UP_T"));

	} else if (         rt[0][1]&&
	        !rt[1][0] &&              rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 12;
	cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("RIGHT_T"));

	} else if (         !rt[0][1]&&
	        rt[1][0] &&              rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 13;
	cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("DOWN_T"));

	} else if (         rt[0][1]&&
	        rt[1][0] &&              rt[1][2] &&
	                    !rt[2][1]) {
		routeCase = 14;
	cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("LEFT_T"));

	} else if (         rt[0][1]&&
	        rt[1][0] &&              rt[1][2] &&
	                    rt[2][1]) {
		routeCase = 14;
	cout << "Doing new route..." << endl;
		return getStructureByID(placStruc->getGroupID(), getStructureByID(placStruc->getGroupID(),0)->getParameterByName("CROSS"));

	}

	//If none of the cases fit, just return what we started with and cross
	//our fingers
	return placStruc;

	//Complete route system requires 11 different models - 4 T's, 4 corners, 2 straights and a cross
} 


structure* world::doBevel(structure* placStruc, int x, int y)
{
  	structure* applyStruc = placStruc;
	//This overrides the above - sloped cells are already aligned in the
	//right direction for routes!!!
	//Check if there are tiles defined for this group if the cell we

	cout << "Doing bevel..." << endl;
	
	
	if ( ((abs(map[x][y].getXGradient()) != 0) || (abs(map[x][y].getZGradient()) != 0)) &&
	     //Check that the structure is not a route...
	     placStruc->getParameterByName("ISROUTE") == 0) {
		cout << "We're on a slope..no tiles here...yet... FIXME" <<endl;
		return NULL;
	}


	//are allocating to is on a slope
	cout << "X gradient: " << map[x][y].getXGradient() << endl; 
	cout << "Z gradient: " << map[x][y].getZGradient() << endl;
	

	if ( map[x][y].getXGradient() != 0 ) {
	   switch (map[x][y].getXGradient()) {
		case 1: //X facing up slope	
 			//cout << "GRa 1" << endl;
			if (placStruc->getParameterByName("SLOPE_UP") != NONAME) {
			    return getStructureByID(placStruc->getGroupID(),placStruc->getParameterByName("SLOPE_UP"));
			}
			break;
		case -1: //X facing down slope
			//cout << "GRa -1" << endl;
			if (placStruc->getParameterByName("SLOPE_DOWN") != NONAME) {
	  		   return getStructureByID(placStruc->getGroupID(),placStruc->getParameterByName("SLOPE_DOWN"));
			   cout << "Down";	
			}
			break;
	   }
	}
	cout << "Nothing on X, check Z..." << endl;		

	switch (map[x][y].getZGradient()) {
		case 1: //Z facing up slope
			if (placStruc->getParameterByName("SLOPE_UPR") != NONAME) {
			      return getStructureByID(placStruc->getGroupID(),placStruc->getParameterByName("SLOPE_UPR"));
			}
			break;
		case -1: //Z facing down slope
			if (placStruc->getParameterByName("SLOPE_UPL") != NONAME) {
			    return getStructureByID(placStruc->getGroupID(),placStruc->getParameterByName("SLOPE_UPL"));
			}
			break;

	}
	return NULL;
}
//==============================================================
//==============================================================




//Universal destroy function
bool world::destroyStructure(int x, int y)
{
	//Leave if there's no structure here
	if (map[x][y].getStructure() == NULL) return false;

	//Now get the owner of this structure
	cell* owner= NULL;
	if (map[x][y].getOwns() == true) {
		owner = &map[x][y];
	} else {
		owner = map[x][y].getOwner();
		cout << "Not owner" << endl;
	}
	
	//Get the size and start position
	vertex2D size = owner->getStructure()->getSize();
	vertex2D start = owner->getPosition();
	cout << "Destroying a building of size - " << size.x << ":" << size.y << endl;
	cout << "Owner at: "<< start.x << ":" << start.y;



	int ID = map[x][y].getStructure()->getParameterByName("GROUP_ID");
	structure* placStruc = getStructureByID(ID,0);	
	bool isRoute = (map[x][y].getStructure()->getParameterByName("ISROUTE") == 1);

	//Now reset these squares...
	for (int i=start.x;i<start.x+size.x;i++)
		for (int j=start.y;j<start.y+size.y;j++) {
			map[i][j].setStructure(NULL);
			map[i][j].setOwns(false);
			map[i][j].setOwner(NULL);
		}

	//Now fix out friends...
	if (isRoute) {

		if (isRouteStructure(x-1,y,ID)) 
			placeStructure(x-1,y,doRoute(placStruc,x-1,y), true, true);
		if (isRouteStructure(x+1,y,ID)) 
			placeStructure(x+1,y,doRoute(placStruc,x+1,y), true, true);	
		if (isRouteStructure(x,y+1,ID))
			placeStructure(x,y+1,doRoute(placStruc,x,y+1), true, true);
		if (isRouteStructure(x,y-1,ID))
			placeStructure(x,y-1,doRoute(placStruc,x,y-1), true, true);
	}	
	
	
	
}


structure* world::getStructureByID(unsigned int group_id, unsigned int sub_group)
{
	cout << "Looking for " << group_id << " : " << sub_group << endl;
	for (int i=0;i<numStructures;i++) {
		if (structures[i].getParameterByName("GROUP_ID") == group_id &&
		    structures[i].getParameterByName("SUB_GROUP") == sub_group) {
			cout << "Returning a structure" << endl;
			return &structures[i];
		}
	}
	return NULL;
}
