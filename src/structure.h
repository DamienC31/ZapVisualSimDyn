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
//The structure (building/object) class - structure.h


class structure {
	public:
		structure();
		~structure();
		void doRender();
		void createList();
		void initVerts(int);
		void initFaces(int);
		void initParameters(int);
		void addVertex(float,float,float);
		void addFace(GLenum,int,unsigned int*);
		void addParameter(string,string);
		void setColor(float,float,float);
		vertex3D getVertex(int);
		
		//Useful functions
		vertex2D getSize();

		//Parameter stuff
		int getParameterByName(char*);
		char* getParamaterByName(char*);

		//Group stuff
		void setGroupID(unsigned int);
		unsigned int getGroupID();
		void setSubGroup(unsigned int);
		unsigned int getSubGroup();
		

	private:
		vertex3D* vertices;
		face* faces;
		parameter* parameters;
		
		int numVerts,nextVert;
		int numFaces,nextFace;
		int numParameters, nextParameter;
		float r,g,b;

		int xsize, ysize; //This structures size in cells

		//Identification stuff
		GLuint listID; //The display list ID
		unsigned int group_id;
		unsigned int sub_group;	
};
