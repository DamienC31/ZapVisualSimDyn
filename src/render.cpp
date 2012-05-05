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
//The renderer class

#include "freereign.h"

render::render(world* par)
{
	parent = par;
	map = parent->getMap();
	UI = parent->getUI();

	//Setup colors..
	nozoneColor = setupColor(0.6,0.4,0.3);
	residColor = setupColor(0.0,0.414,0.0);
	commColor = setupColor(0.097,0.441,0.707);
	indusColor = setupColor(0.97,0.85,0.04);	

}


///////===================================
//Some GL material stuff thrown in
GLuint render::setupColor(float r,float g, float b)
{
	GLuint temp = glGenLists(1);
	glNewList(temp, GL_COMPILE);
		GLfloat mat_ambient[] = {r,g,b,1.0};
		GLfloat mat_diffuse[] = {r,g,b,1.0};
		GLfloat mat_specular[] = {r,g,b,1.0};

		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
	glEndList();

	return temp;
}

//Set the color according to the cell's zone
void render::setZoneColor(int zone)
{
	switch (zone) {
		case 0: //No zone...
			glCallList(nozoneColor);
			break;
		case 1: //Residential
			glCallList(residColor);
			break; 
		case 2: //Commercial
			glCallList(commColor);
			//cout << commColor << endl;
			break;
		case 3: //Industrial
			glCallList(indusColor); 
			break;
	}
}


//Draw the grid lines
void render::drawLines()
{
	
	GLfloat mat_ambient[] = {0.0,0.0,0.0,1.0};
	GLfloat mat_diffuse[] = {0.0,0.0,0.0,1.0};
	GLfloat mat_specular[] = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);	

	//Draw the gridlies.....
	float constant = 0.0;
        glColor3f(0.0,0.0,0.0);
	int lastsqx = 0;
	float lastgradx=0;

	int lastsqy = 0;
	float lastgrady=0;
	float STAD = 0.03;
	glBegin(GL_LINES);
	//Draw the Grid-Lines...
	int finalx, finaly;

	//--------------------------------------
	for (int i=0;i<parent->getYSize()-1;i++) {
	    lastsqx = 0;
	    lastgradx = 0;
	    lastsqy = 0;
	    lastgrady=0;
	    int j;
	    for (j=0;j<parent->getXSize()-1;j++) {
		 //Get the gradient
		 float grad1 = *map[i][j].coords[1].y - *map[i][j].coords[0].y;
		 float grad2 = *map[j][i].coords[0].y - *map[j+1][i].coords[0].y;

		 if (grad1 == lastgradx) {
			//Do nothing...just sset lastsq to j
			//lastsq = j;
	 	 } else {
		 	lastgradx = grad1;
		 	glColor3f(0.0,0.0,0.0);
		   	  glVertex3f(*map[i][lastsqx].coords[0].x,*map[i][lastsqx].coords[0].y+STAD,*map[i][lastsqx].coords[0].z);
		   	  glVertex3f(*map[i][j].coords[0].x,*map[i][j].coords[0].y+STAD,*map[i][j].coords[0].z); 
			lastsqx = j;
	         }
		 if (grad2 == lastgrady) {
			//Do nothing...just sset lastsq to j
			//lastsq = j;
	 	 } else {
		 	lastgrady = grad2;
		 	glColor3f(0.0,0.0,0.0);
		   	  glVertex3f(*map[lastsqy][i].coords[0].x,*map[lastsqy][i].coords[0].y+STAD,*map[lastsqy][i].coords[0].z);
		   	  glVertex3f(*map[j][i].coords[0].x,*map[j][i].coords[0].y+STAD,*map[j][i].coords[0].z); 
			lastsqy = j;
	         }
	     }
	     //Draw the final line for x
	        glVertex3f(*map[i][lastsqx].coords[0].x,*map[i][lastsqx].coords[0].y+STAD,*map[i][lastsqx].coords[0].z);
	 	glVertex3f(*map[i][j].coords[0].x,*map[i][j].coords[0].y+STAD,*map[i][j].coords[0].z);
	     //Draw the final line for y
	        glVertex3f(*map[lastsqy][i].coords[0].x,*map[lastsqy][i].coords[0].y+STAD,*map[lastsqy][i].coords[0].z);
	 	glVertex3f(*map[j][i].coords[0].x,*map[j][i].coords[0].y+STAD,*map[j][i].coords[0].z); 
          }
 	  glEnd();
	
}




void render::drawGrid()
{
	
	//The selecting stuff
	//Swap the high/low points if necessary
	if (UI->isSelecting() == true) {
	        UI->getSelectL()->setPos(UI->getPickX(),UI->getPickY()); //The current position
        	UI->setSelectH(UI->getSelector()); //The start position
        	if (UI->getSelectL()->x > UI->getSelectH()->x) {
                	int temp = UI->getSelectL()->x;
                	UI->getSelectL()->x = UI->getSelectH()->x;
                	UI->getSelectH()->x = temp;
        	}
        	if (UI->getSelectL()->y > UI->getSelectH()->y) {
                	int temp = UI->getSelectL()->y;
                	UI->getSelectL()->y = UI->getSelectH()->y;
                	UI->getSelectH()->y = temp;
        	}
                
 	}
	

	glBegin(GL_TRIANGLES);

	int lastZone = 0;
	int lastChange = 0;


 	for (int i=0;i<parent->getYSize()-1;i++) {
		float lastgradx = 0;
		int lastsqx = 0;
		lastChange = 0;
		
		int j=0;
		bool hasBeenSame = false;


		for (j=0;j<parent->getXSize()-1;j++) {
		     
                        //The gradient of the squares at this point
             	        //bool flat = (map[i][j].getXGradient() == 0) && (map[i][j].getZGradient() == 0);
			float cgX = map[i][j].getXGradient();
			float cgZ = map[i][j].getZGradient();

			bool previousSame = true;;

			if (j>0) {
				previousSame =  (map[i][j-1].getXGradient() == cgX
						&&map[i][j-1].getZGradient() == cgZ &&
						 cgX != 5); // Double sloped...
			}
				
		
			if (previousSame && (map[i][j].getZone() == lastZone)) {
				if (!hasBeenSame) lastChange = j;
				hasBeenSame = true;
				continue;
			} else if ( (!previousSame  && hasBeenSame) || map[i][j].getZone() != lastZone) { //Check to make sure there is something to draw...	

	
				hasBeenSame = false;


						 //Previousflat is the key!!!!
		  if (parent->getUI()->interM) { //Make sure we aren't stuffing up
								 //and drawing across gradients :)

		   setZoneColor(map[i][lastChange].getZone());
     		   glNormal3f(map[i][lastChange].nx, map[i][lastChange].ny,map[i][lastChange].nz);
		   //==========================================================================

		   //The cell to be drawn
		   glVertex3f(*map[i][lastChange].coords[0].x,*map[i][lastChange].coords[0].y,*map[i][lastChange].coords[0].z);
		   glVertex3f(*map[i][j-1].coords[2].x,*map[i][j-1].coords[2].y,*map[i][j-1].coords[2].z);
		   glVertex3f(*map[i][j-1].coords[1].x,*map[i][j-1].coords[1].y,*map[i][j-1].coords[1].z);


		   //Tri 2
		   glVertex3f(*map[i][lastChange].coords[0].x,*map[i][lastChange].coords[0].y,*map[i][lastChange].coords[0].z);
		   glVertex3f(*map[i][lastChange].coords[3].x,*map[i][lastChange].coords[3].y,*map[i][lastChange].coords[3].z);

		   glVertex3f(*map[i][j-1].coords[2].x,*map[i][j-1].coords[2].y,*map[i][j-1].coords[2].z);
		   
		   }

		 
		   //Color change only...
		   if (map[i][j].getZone() != lastZone) {
			 lastZone = map[i][j].getZone();
			 lastChange = j;
		   }
	     }
				

			
			if (i==UI->getPickX()&&j==UI->getPickY()) {
				GLfloat mat_ambient[] = {0.484,0.512,0.494,0.6};
				GLfloat mat_diffuse[] = {0.484,0.512,0.494,0.6};
				GLfloat mat_specular[] = {0.484,0.512,0.494,0.6};

				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
				glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);

				//lastZone = -1; //Reset the lastZone counter..obvious
			} else {
				//if (lastZone != map[i][j].getZone()){
					 setZoneColor(map[i][j].getZone());

				//	 lastZone = map[i][j].getZone();
				//}
			}
	
		   if (UI->isSelecting() == true && i >= UI->getSelectL()->x && 
			i <= UI->getSelectH()->x && j >=
			UI->getSelectL()->y && j <= UI->getSelectH()->y) { 
				GLfloat mat_ambient[] = {0.42,0.92,0.51, 0.6};
				GLfloat mat_diffuse[] = {0.42,0.92,0.51,0.6};
				GLfloat mat_specular[] = {0.42,0.92,0.51,0.6};

				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
				glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);

				//lastZone =-1; //Tell it to reset the zone color..

		   }


		

     		   //Now just set the normal..
		   
     		   glNormal3f(map[i][j].nx, map[i][j].ny,map[i][j].nz);
		   //==========================================================================

		   //The cell to be drawn
		   glVertex3f(*map[i][j].coords[0].x,*map[i][j].coords[0].y,*map[i][j].coords[0].z);
		   glVertex3f(*map[i][j].coords[1].x,*map[i][j].coords[1].y,*map[i][j].coords[1].z);
		   glVertex3f(*map[i][j].coords[2].x,*map[i][j].coords[2].y,*map[i][j].coords[2].z);

		   //Tri 2
		   glVertex3f(*map[i][j].coords[0].x,*map[i][j].coords[0].y,*map[i][j].coords[0].z);
		   glVertex3f(*map[i][j].coords[2].x,*map[i][j].coords[2].y,*map[i][j].coords[2].z);
		   glVertex3f(*map[i][j].coords[3].x,*map[i][j].coords[3].y,*map[i][j].coords[3].z);
	        }
		   //Draw the last cells....
		 
	         if (parent->getUI()->lastSquare) {
     		   //Now just set the normal..
		   setZoneColor(map[i][lastChange].getZone());
     		   glNormal3f(map[i][lastChange].nx, map[i][lastChange].ny,map[i][lastChange].nz);	
	
		   //The cell to be drawn
		   glVertex3f(*map[i][lastChange].coords[0].x,*map[i][lastChange].coords[0].y,*map[i][lastChange].coords[0].z);
		   glVertex3f(*map[i][j-1].coords[2].x,*map[i][j-1].coords[2].y,*map[i][j-1].coords[2].z);
		   glVertex3f(*map[i][j-1].coords[1].x,*map[i][j-1].coords[1].y,*map[i][j-1].coords[1].z);

		   //Tri 2
		   glVertex3f(*map[i][lastChange].coords[0].x,*map[i][lastChange].coords[0].y,*map[i][lastChange].coords[0].z);
		   glVertex3f(*map[i][lastChange].coords[3].x,*map[i][lastChange].coords[3].y,*map[i][lastChange].coords[3].z);	
		   glVertex3f(*map[i][j-1].coords[2].x,*map[i][j-1].coords[2].y,*map[i][j-1].coords[2].z);
		}
	}
	glEnd();




}


void render::drawToolBox()
{
	glDisable(GL_DEPTH_TEST);	
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        // Width and height are your screen width and height.
        gluOrtho2D(0, WIDTH, 0.0f, HEIGHT);

        // Draw to a new modelview matrix.
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Draw to screen space here.
        // e.g., to draw a quad across the screen, do something
        // like this:

	/*GLfloat mat_ambient[] = {1.0,1.0,1.0,1.0};
	GLfloat mat_diffuse[] = {0.5,0.5,0.5,1.0};
	GLfloat mat_specular[] = {1.0,1.0,1.0,1.0};

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);	*/
	glColor3f(0.5,0.5,0.5);


        glBegin(GL_QUADS);
        glVertex2f(640,0);
        glVertex2f(770,0);
        glVertex2f(770,480);
        glVertex2f(640,480);
        glEnd();

	UI->drawButtons(); //Draw the toolbox
	
	glColor3f(0.0,1.0,0.2);
	glBegin(GL_LINES);
		glVertex2f(650, 395);
		glVertex2f(760,395);
	glEnd();

	int x=UI->getPickX();
	int y=UI->getPickY(); 

	/*GLfloat mat_ambient_t[] = {0.0,0.5,0.0,1.0};
	GLfloat mat_diffuse_t[] = {0.0,0.5,0.0,1.0};
	GLfloat mat_specular_t[] = {0.0,0.5,0.0,1.0};

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient_t);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse_t);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular_t);	*/
	glColor3f(0.0,0.0,1.0);

	static char s[32];
	sprintf(s, "X: %d, Y: %d", x, y);
	text(660, 5, s);

	//Now draw our current funds...
	sprintf(s, "Funds\n@%d", parent->getUser().getFunds());
	text(670, 100, s);	 

        // Pop the matrix with which you drew, and restore your original
        // matrices.
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}


void render::drawStructures()
{

	//TODO: Somehow integrate this with the main
	//grid drawing stuff - saves thousands of useless iterations
	for (int x=0;x<parent->getXSize();x++)
		for (int y=0;y<parent->getYSize();y++) {
			//Draw the structures that are here...
			bool drewStruct = false;
			if (map[x][y].getStructure() != NULL && map[x][y].getOwns()) {
				glPushMatrix();
					//Rotate
					//Translate
					glTranslatef((float)x*0.3f-(parent->getYSize()*0.3f/2),
						     parent->getMinHeight(x,y),(float)y*0.3f - (parent->getXSize()*0.3f/2));
					//cout << parent->getMinHeight(x,y) << endl;
					map[x][y].getStructure()->doRender();
				glPopMatrix();
			}
		}
	}

