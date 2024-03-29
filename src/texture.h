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
//The texture class definition - "texture.h"

class texture {
  public:
     GLuint getTextureObj();
     //unsigned char* getData(ifstream *infile, int size, int imageBits);
     //unsigned char* getRGB (ifstream *infile, long size);
     //unsigned char* getRGBA(ifstream *infile, long size);
     char* getData(ifstream *infile, int size, int imageBits);
     char* getRGB (ifstream *infile, long size);
     char* getRGBA(ifstream *infile, long size);
     bool loadTexture(string name);
   
     
  private:
     
     string filename;
     GLenum texFormat;
     GLuint textureObj[1]; //The OpenGL Texture Object
   
};



