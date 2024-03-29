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
//The face (polygon) class - face.cpp

#include "freereign.h"

face::face(structure* parent)
{
	face::parent = parent;
	numVerts = 0;
	vertexRefs = NULL;
}

face::~face()
{
	cout << "   Deallocating memory for face.." << endl;
	cout << "Size: " << numVerts << endl;
	if (vertexRefs != NULL) delete [] vertexRefs;
}

void face::setType(GLenum type)
{
	face::type = type;
}

GLenum face::getType()
{
	return type;
}


void face::initRefs(int num)
{
	numVerts = num;
	vertexRefs = new unsigned int[num+1];
	nextVert = 0;
}

unsigned int* face::getVertexRefs()
{
	return vertexRefs;
}

unsigned int face::getNumVertices()
{
	return numVerts;
}


void face::doRender()
{

	//Make sure we don't calculate normals for lines..
	if (numVerts > 2) {

		//Set the normal
		vertex3D v1(
			parent->getVertex(vertexRefs[0]).x,
			parent->getVertex(vertexRefs[0]).y,
			parent->getVertex(vertexRefs[0]).z);
		vertex3D v2(
			parent->getVertex(vertexRefs[1]).x,
			parent->getVertex(vertexRefs[1]).y,
			parent->getVertex(vertexRefs[1]).z);
		vertex3D v3(
			parent->getVertex(vertexRefs[2]).x,
			parent->getVertex(vertexRefs[2]).y,
			parent->getVertex(vertexRefs[2]).z); 

        	//Work out the cross products..
        	vertex3D cpx =crossProduct(v1,v2,v3);

     		//Finally get the normalised normal vector..
     		float length =sqrt(sqr(cpx.x) + sqr(cpx.y) + sqr(cpx.z));
     		cpx.x = (cpx.x / length);
     		cpx.y  = (cpx.y / length);
     		cpx.z = (cpx.z / length);
     
		//Now just set the normal..
     	 	glNormal3f(cpx.x, cpx.y, cpx.z);
	}

	for (int i=0;i<numVerts;i++) {
		glVertex3f(parent->getVertex(vertexRefs[i]).x,parent->getVertex(vertexRefs[i]).y, parent->getVertex(vertexRefs[i]).z);
	}

	//Render this face
}	

void face::addVertexRef(unsigned int ref)
{
	if (nextVert < numVerts) {
		vertexRefs[nextVert] = ref;
		nextVert++;
		return;
	} else {
		cout << "Access violation! - Number of vertices for polygon exceeded!" << endl;
		exit(0);
	}
}

