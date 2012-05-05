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
//The User interface class - UInterface.cpp

#include "freereign.h"

UInterface::UInterface(world* par)
{
	parent = par;
	keymode = zone_type = 0;

	pickx = picky = (int)parent->getXSize()/2; 
	selecting = false;
	levelling = false;

	lines = true; //Display grid lines by default
	ground = true; //Display ground by default
	structs = true;
	lastSquare = true;
	interM = true;

	//Set up the buttons
	toolbox = new buttonbar(this,10,"graphics/toolbox.tga");
	//numButtons = 1;
 	/*buttons[0].setStart(650,440);
	buttons[0].setEnd(680,470);
	buttons[0].setFunction(&UInterface::buttonfunc);*/
	toolbox->addButton(650,440,680,470,0,&UInterface::powerLineFunc, 0.05, 0,0.13,0.11);
	toolbox->addButton(690,440,720,470,0,&UInterface::showTerrainFunc,0.255,0.0,0.3625,0.11);
	toolbox->addButton(730,440,760,470,0,&UInterface::showZoneFunc,0.4825,0.0,0.5925,0.11);
	toolbox->addButton(650, 400, 680, 430, 0, &UInterface::roadFunc,0.250, 0.18, 0.400, 0.29);

	//Terrain buttons
	toolbox->addButton(650,360,680,390,1,&UInterface::raiseTerrainFunc,0.05,0.11,0.13,0.22);
	toolbox->addButton(650,320,680,350,1,&UInterface::lowerTerrainFunc,0.255,0.11,0.3625,0.19);
	toolbox->addButton(650,280,680,310,1,&UInterface::levelTerrainFunc,0.479,0.11,0.5925,0.22);

	//Zoning buttons
	toolbox->addButton(650,360,680,390,2,&UInterface::zoneResFunc,0.6925,0.0,0.7925,0.11);
	toolbox->addButton(650,320,680,350,2,&UInterface::zoneComFunc,0.8725,0.0,0.9325,0.11);
	toolbox->addButton(650,280,680,310,2,&UInterface::zoneIndFunc,0.05,0.18,0.13,0.29);
	

	//Hide all non parent buttons
	toolbox->hideAllGroups();
}


//========================================================
//Toolbox functions
void UInterface::showTerrainFunc()
{
	toolbox->displayGroup(1);
}

void UInterface::showZoneFunc()
{
	toolbox->displayGroup(2);
}

void UInterface::raiseTerrainFunc()
{
	keymode = 1;
}

void UInterface::lowerTerrainFunc()
{
	keymode = 4;
}

void UInterface::levelTerrainFunc()
{
	keymode = 5;
}

void UInterface::powerLineFunc()
{
	keymode = 3;	
}

void UInterface::zoneResFunc()
{
	keymode = 2;
	zone_type = 1;
}

void UInterface::zoneComFunc()
{
	keymode = 2;
	zone_type = 2;
}

void UInterface::zoneIndFunc()
{
	keymode = 2;
	zone_type = 3;
}


void UInterface::roadFunc()
{
	keymode = 7;
}


void UInterface::drawButtons()
{
	//for (int i=0;i<numButtons;i++)
	//	buttons[i].drawButton();
	toolbox->drawButtons();
}

void UInterface::checkButtons(int x, int y)
{
	toolbox->checkButtons(x,y);		
}	
//===================================================================


int UInterface::getPickX()
{
	return pickx;
}

int UInterface::getPickY()
{
	return picky;
}


vertex2D UInterface::getSelector()
{
	return selector;
}

vertex2D* UInterface::getSelectL()
{
	return &selectL;
}

vertex2D* UInterface::getSelectH()
{
	return &selectH;
}


void UInterface::setSelectH(vertex2D res)
{
	selectH = res;
}


void UInterface::setSelectL(vertex2D res)
{
	selectL = res;
}

bool UInterface::isSelecting()
{
	return selecting;
}


//==================================================
void UInterface::passiveMotionFunc(int x, int y)
{
   if (x < WIDTH-TOOLBOX_WIDTH) {
	float camx=parent->getCamX();
	float camy=parent->getCamY();
	float camz=parent->getCamZ();
        int              viewport[4];
        double          modelMatrix[16];
        double          projMatrix[16];
        float           t;                              //used so we can find intersection of line and plane
        float           XZPlaneNormal[3]= {0,1,0};              //normal of the x-axis
        float           rStart[3]               = {camx, camy, camz};
        float           pStart[3]               = {0,0,0}; //Point lying on the plane
        float           pRay[3]         = {0,0,0};
        float           DiffV[3];                               //Dot product of ray and plane normal vectors
        float           Intersect[3];                      //point of intersection
        double  rRay[3];

        //================
        //Get the unprojected point
        glGetIntegerv (GL_VIEWPORT, viewport);
        glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix);
        glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);
        gluUnProject ((GLfloat)x, HEIGHT-(GLfloat)y, 1.0, modelMatrix, 
                              projMatrix, viewport, &rRay[0], &rRay[1], &rRay[2]);    

        float height = 0.0f;
        vertex2D cellAt;
	bool hit = false;
        //Now check x,y against all the different heights possible
        for (int yh = MAXUP;yh>=MAXDOWN;yh -= 1) {
            float ycalc = (float)yh * CELL_HEIGHT; //Reduces float errors
            pRay[0] = (float)rRay[0] - camx;
            pRay[1] = (float)rRay[1] - camy;
            pRay[2] = (float)rRay[2] - camz;
              
            DiffV[0]        = pStart[0] - rStart[0];
            DiffV[1]        = ycalc - rStart[1]; //Y Point on plane we're checking against
            DiffV[2]        = pStart[2] - rStart[2];
            t = (mmDotProduct (DiffV, XZPlaneNormal))/(mmDotProduct (XZPlaneNormal, pRay));

            Intersect[0] = camx + ((float)pRay[0] * t);
            Intersect[1] = camy + ((float)pRay[1] * t);
            Intersect[2] = camz + ((float)pRay[2] * t);
            //cout << "Intersect - " << Intersect[0] << ":" << Intersect[1] << ":" << Intersect[2] << endl;
            cellAt = parent->getCellPosition(Intersect[0], Intersect[2]);
            if ( parent->saneRef(cellAt) && fabs(parent->getAvHeight(cellAt.x, cellAt.y) - ycalc) < DELTA_2) {
                  height = ycalc;
                  yh = MAXDOWN-1;
		  hit = true;
            }
                     
        }
        if (cellAt.x > 0 && cellAt.y > 0 && 
	    cellAt.x < parent->getXSize() && cellAt.y < parent->getYSize()) {
             if (hit == true) {
			pickx = cellAt.x;
			picky = cellAt.y;
	     }
        }

	//cout << "Gradient: " << parent->getMap()[pickx][picky].getZGradient() << endl;
	cout << "Min height: " << parent->getMinHeight(pickx, picky)<<endl;
    }
}



//===========================================================
void UInterface::motionFunc(int x, int y)
{
  //This stuff should be moved to world...


  switch (keymode) {
	case 1: //Raise terrain
	   parent->changeTerrain(pickx, picky, 1);
	   break;
	case 3: //Power line...
	    	parent->placeStructure(pickx,picky,parent->getStructureByID(0,0),false,false);
		break;
        case 7:
		//Road
		parent->placeStructure(pickx,picky,parent->getStructureByID(1,0),false,false); 
		break;
	case 4: //Lower terrain
	   parent->changeTerrain(pickx, picky, 0);
	   break;
        case 5: //Level Terrain
	   if (levelling) {
		if (parent->getMaxHeight(pickx,picky) > level) {
			parent->changeTerrain(pickx, picky, 0);
		} else if (parent->getMinHeight(pickx,picky) < level) {
			parent->changeTerrain(pickx, picky, 1);
		}
	   }
	   break;
  }

  passiveMotionFunc(x,y);  
}



//=======================================================
void UInterface::keyboardFunc(unsigned char key, int x, int y, int console)
{
    cout << "Key: " << key << endl;

    if (console == DOWN)
    {
		if (key == con->conChar)
			if (!con->conAni)
				con->conAni = 1;

		consoleAddChar (con, (char)key);
		glutPostRedisplay ();
		return;
    }

    //TODO all these will be replaced by a toolbar later in life
    switch (key) {
	case C_CONSOLECHAR:
		con->conAni = !con->conAni;
		break;
	case 'u': //Raise terrain
		keymode = 1;
		break;
	case 'r': //Zone residential
		keymode = 2; zone_type = 1;
		break;
	case 'c': //Zone commercial
		keymode = 2; zone_type = 2;
		break;
	case 'i': //Zone industrial
		keymode = 2; zone_type = 3;
		break;
	case 'x': //Dezone
		keymode = 2; zone_type = 0;
		break;
	case 'p': //Place power line
		keymode = 3;
		break;
	case 'd': //Lower terrain
		keymode = 4;
		break;
	case 'l': //Level terrain
		keymode = 5;
		break;
	case 'k': //Blow up	
		keymode = 6;
		break;
	case 't': //Road
		keymode = 7;
		break;
	case 'g': //Grid lines
		  lines = !lines;
		  break;
	case 'f': //Ground
		  ground = !ground;
		break;

	case 's': //Structures
		  structs = !structs;
		break;

	case 'q': //Last Cell
		  lastSquare = !lastSquare;
		  break;

	case 'z': //Intermediete Runs
		  interM = !interM;
		  cout << "Turning off intermediete!" << endl;
		  break;		  

	//Movement stuff
	case '[':
		parent->setRotate(parent->getRotate()-4);
		if (parent->getRotate()<0) parent->setRotate(360);
		break;
	case ']':
		parent->setRotate(parent->getRotate()+4);
		if (parent->getRotate()>360) parent->setRotate(0);
		break;
	case '-':
		if (parent->getCamY() >= 4.0f) {
			parent->setCamY(parent->getCamY()- 0.3f);
			parent->setLookY(parent->getLookY() - 0.3f);
			parent->setRotate(parent->getRotate()); //Update positions
		}
		break;
	case '=':
		if (parent->getCamY() <= 15.0f) {
			parent->setCamY(parent->getCamY() + 0.3f);
			parent->setLookY(parent->getLookY() + 0.3f);
			parent->setRotate(parent->getRotate()); //Update positions
		}
		
		break;
	case '8': //"Forward"
		parent->setLookX( parent->getLookX() - (float)sin(parent->getRotate()*PI/180) *0.3f);
        	parent->setLookZ( parent->getLookZ() - (float)cos(parent->getRotate()*PI/180) *0.3f);
		parent->setRotate(parent->getRotate()); //Update positions
		break;
	case '2': //"Back"
		parent->setLookX( parent->getLookX() + (float)sin(parent->getRotate()*PI/180) *0.3f);
        	parent->setLookZ( parent->getLookZ() + (float)cos(parent->getRotate()*PI/180) *0.3f);
		parent->setRotate(parent->getRotate()); //Update positions
		break;

	case '4': //Left
		parent->setLookX( parent->getLookX() + (float)sin(parent->getRotate()*PI/180) *0.3f);
        	parent->setLookZ( parent->getLookZ() - (float)cos(parent->getRotate()*PI/180) *0.3f);
		parent->setRotate(parent->getRotate()); //Update positions
		break;



   }
}


//=========================================================
void UInterface::mouseFunc(int button, int state, int x, int y)
{
  switch (button) {
    case GLUT_RIGHT_BUTTON: {
	  //for (int c=0; c<parent->getXSize(); c++) {
	  //  delete[] map[c];
	  //}
	  //delete[] map;
	  //delete[] coords;
	  //delete[] power_station;
	  parent->~world();  //Doesn't seem to get called unless
			     //we do implicitly..
	 
	  exit(0);
	  break; //Yes...superfluous.
    }
    case GLUT_LEFT_BUTTON: {
     if (x < WIDTH-TOOLBOX_WIDTH) {
	 if (state == GLUT_DOWN) {
		switch (keymode) {
		   case 2:
		       if (!selecting) {
			   selector.x = pickx;
			   selector.y = picky;
		       }
		       selecting = true;
		       cout << "Start select..." <<endl;
		       break;
		   case 3:
			//Power line
		       break;
		   case 5:
			if (!levelling) {
				//Get height to level to...
				level = parent->getMaxHeight(pickx,picky);
				levelling = true;
			}
			break;
		   case 6:
			parent->destroyStructure(pickx,picky);
			break;

		   


		}

	 }
	 if (state == GLUT_UP) {
		if (selecting) {
			selecting = false;
			cout << "End select..." << endl << endl;
		}
		switch (keymode) {
			case 2: //Zoning..
				parent->doZone(selectL.x, selectL.y, selectH.x, selectH.y, zone_type);
				break;
			case 3: //Power line...
				parent->placeStructure(pickx,picky,parent->getStructureByID(0,0),false,false);
				break;
                   	case 7:
				//Road
				parent->placeStructure(pickx,picky,parent->getStructureByID(1,0),false,false); 
				break;
			case 5:
				levelling = false;
				break;
		} //Switch
	 } //If
       } else { //We're inside the toolbox
		checkButtons(x,HEIGHT-y); //Switch to GL style verticals
       }
    } //Case
    break;
  } //Switch


}


void UInterface::setzone()
{
}
