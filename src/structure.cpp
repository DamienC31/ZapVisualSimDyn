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
//The structure (building) class - structure.cpp


#include "freereign.h"

structure::structure()
{
	nextVert = 0;
	numVerts = 0;
	
	nextFace = numFaces = 0;
	nextParameter = numParameters = 0;

	xsize = ysize = -1;

	group_id = sub_group = 0;

	listID = 2000; //FIXME.
	faces = NULL;
}

void structure::setGroupID(unsigned int id)
{
	group_id = id;
}

unsigned int structure::getGroupID()
{
	return group_id;
}

void structure::setSubGroup(unsigned int id)
{
	sub_group = id;
}

unsigned int structure::getSubGroup()
{
	return sub_group;
}


structure::~structure()
{
	cout << "Deallocating memory for structure..." << endl;
	cout << "Group: " << getParameterByName("GROUP_ID") << ":" << getParameterByName("SUB_GROUP") << endl;

	delete [] vertices;
	delete [] faces;
}

void structure::setColor(float r, float g, float b)
{
	structure::r = r;
	structure::g = g;
	structure::b = b;
}

void structure::createList()
{
	//TODO Some optimisation can be done with the glBegin/glEnd pairs as a
	//sort of run length encoding.  I.e. keep on the same glbegin/glend
	//pair until the type of face changes.
	listID = glGenLists(1);
	
        GLenum lastType = (GLenum)0 ;

	glNewList(listID,GL_COMPILE);
		GLfloat mat_ambient[] = {r,g,b,1.0};
		GLfloat mat_diffuse[] = {r,g,b,1.0};
		GLfloat mat_specular[] = {r,g,b,1.0};

		glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
		glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
		for (int i=0;i<numFaces;i++) {
			
			//Check if we're already on this type, 
			//to save glend/begins..
			if (lastType != -1 && lastType != faces[i].getType()) {
				glEnd();
				glBegin(faces[i].getType());
			} if (lastType == 0) glBegin(faces[i].getType());
			lastType = faces[i].getType();
			
			faces[i].doRender();
			//glEnd();
		}
	glEnd(); //the final glEnd();
	
	glEndList();
	cout << "Created structure..." << endl;
}
			

void structure::doRender()
{
	if (listID != 2000) glCallList(listID); //FIXME
}

void structure::initVerts(int num)
{
	cout << "Initialising" << num << " vertices..." << endl;
	vertices = new vertex3D[num+1];
	numVerts = num;
}

void structure::initFaces(int num)
{
	//Create the array of faces and pass the ref to this
	//faces = new face[num+1](this);

	faces = new face[num+1];
	
	//face faceTmp;
	for ( int i = 0 ; i <= num ; i++ ) {
	    //faceTmp = new face(this);
	    faces[i] = new face(this);
	}

	/*c = new C2[4];
	c[0] = new C2(); // Cette ligne te manque
	c[0].id = 2;*/

	//faces = new face(this);
	/*faces = new face[num + 1];
	for ( int i = 0 ; i <= num ; i++ ) {
	  faces[ i ] = new face(this);
	}*/
	numFaces = num;
}

void structure::initParameters(int num)
{
	//Create the array of faces and pass the ref to this
	parameters = new parameter[num+1];
	numParameters = num;
}

vertex3D structure::getVertex(int num)
{
	return vertices[num];
}


void structure::addVertex(float x, float y, float z)
{
	if (nextVert < numVerts) {
		vertices[nextVert].setPos(x,y,z);
		nextVert++;
		return;
	} else {
		cout << "Too many vertices for object - " << nextVert << " : " << numVerts<< endl;
		exit(0);
	}
}

void structure::addFace(GLenum type, int numRefs, unsigned int* refs)
{
	if (nextFace < numFaces) {
		faces[nextFace].initRefs(numRefs);
		for (int i=0;i<numRefs;i++) {
			faces[nextFace].addVertexRef(refs[i]); 
		}
		faces[nextFace].setType(type);
		nextFace++;
	} else {
		cout << "Number of vertex references for face exceeded!" << endl;
		exit(0);
	}
}


//Parameter stuff
//======================
void structure::addParameter(string name, string value)
{
	if (nextParameter < numParameters) {
		parameters[nextParameter].initParameter(name.length(),value.length());
		parameters[nextParameter].set(name.c_str(),value.c_str());

		//Check for the size parameter, and set our size
		if (name == "SIZE") {
			//Split x and y;
			int pos = value.find("x");
			xsize = atoi(value.substr(0,pos).c_str());
			ysize =atoi(value.substr(pos+1,value.length()-pos-1).c_str());
		}
		if (name == "GROUP_ID") {
			cout << "Group ID: " << value <<endl;
			setGroupID(atoi(value.c_str()));
		}
		if (name == "SUB_GROUP") {
			cout << "Sub group: " << value << endl;
			setSubGroup(atoi(value.c_str()));
		}


		nextParameter++;
	}
}


int structure::getParameterByName(char* name)
{
	cout << "Looking for name: " << name << endl;
	int value=NONAME;
	for (int i=0;i<numParameters;i++) {
		if (!strcmp(parameters[i].getParamName(),name)) {
			value = parameters[i].getParamValueInt();
			return value;
		}
	}
	return value;
}

//Return our size
vertex2D structure::getSize()
{
	vertex2D size;
	size.setPos(xsize,ysize);
	return size;
}
