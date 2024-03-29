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
//The face (polygon) class - face.h

class face {
	public:
		face(structure*);
		face() {
		}
		~face();

		void initRefs(int);
		unsigned int* getVertexRefs();
		unsigned int getNumVertices();
		GLenum getType();
		void setType(GLenum);
		void addVertexRef(unsigned int);
		void doRender();
		structure* getParent() {
		    return parent;
		}
		unsigned int getNextVert() {
		    return nextVert;
		}
	
		face &operator = (face *source) {
			// affectation au niveau des membres
			vertexRefs = source->vertexRefs;//getVertexRefs();
			numVerts = source->numVerts;//getNumVertices();
			nextVert = source->nextVert;//getNextVert();
			parent = source->parent;//getParent();
			type = source->type;//getType();
			return *this;
		}
	private:
		unsigned int* vertexRefs;
		unsigned int numVerts;
		unsigned int nextVert;
		structure* parent;
		GLenum type;
};


/*complexe &complexe::operator=(const complexe &source)
{
    m_x = source.m_x;
    m_y = source.m_y;
    return *this;
}*/
