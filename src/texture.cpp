/*
 *   glObs
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
//The implementation of the texture class - "texture.cpp"


#include "freereign.h"


GLuint texture::getTextureObj()
{
 return textureObj[0];
}


//======================================================
//Targa Loading stuff
//======================================================
//unsigned char* texture::getRGBA(ifstream *infile, long size)
char *texture::getRGBA(ifstream *infile, long size)
{
    unsigned char temp;
    int i;
    char ch;

    //static unsigned char* rgba = new unsigned char[size*4];
    static char *rgba = new char [size*4];
    long bytes_to_read = size*4;


    //for (int j=0;j<bytes_to_read;j++) {
    //    infile->get(ch);
    //    rgba[j] = ch;
    //}
    
    infile->read(rgba,bytes_to_read);

    for (i = 0; i < size * 4; i += 4 )
    {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }

    texFormat = GL_RGBA;
    return rgba;
}
//========================



//========================
//Get RGB
char* texture::getRGB (ifstream *infile, long size)
{
    //static unsigned char* rgb= new unsigned char[size*3];
    static char* rgb= new char[size*3];
    unsigned char temp;
    long bytes_to_read = size*3;
    
   
    unsigned char ch;
    //for (int j=0;j<bytes_to_read;j++) {
    //    infile->get(ch);
    //    rgb[j] = ch;
    //}
    infile->read(rgb,bytes_to_read);

    //Shift from BGR to RGB
    for (long i = 0; i < size * 3; i += 3) {
       temp = rgb[i+2];
       rgb[i+2] = rgb[i + 1];
       rgb[i + 1] = temp;
    }

    texFormat = GL_RGB;
    return  rgb;
}
//===============================


//===============================
//Determine TGA Bits
char* texture::getData(ifstream *infile, int size, int imageBits)
{
     if (imageBits == 32)
        return getRGBA (infile, size);
    else if (imageBits == 24)
        return getRGB (infile, size);	
}



//======================================
bool texture::loadTexture(string name)
{
    char *header = NULL;
    char* Data;
    int Width, Height;
    int Bits;
    long size;
   
    ifstream infile(name.c_str());    
    cout << "Loading: " << name << endl;
    header = new char[17];
    infile.read(header, 17); //Read the header

    if (header[1] != 0 || (header[2] != 2 && header[2] != 3)) {
      cout << "Bad Type!!" << endl;
      return 0;
    }

    Width = header[12] + header[13] * 256; 
    Height = header[14] + header[15] * 256;
    Bits =	header[16];
    cout << Bits << " bit image" << endl;
    cout << "Size: " << Width << ":" << Height <<endl;
    size = Width * Height;

    //Check the dimensions..
    //if (!checkSize (Width) || !checkSize (Height)) {
    //    cout << "Image size is wrong!" << endl;
    //    return 0;
    //}

    /* make sure we are loading a supported type */
    if (Bits != 32 && Bits != 24 && Bits != 8) {
      //returnError (s, TGA_BAD_BITS);
      cout << "Wrong number of imagebits!" << endl;
      return 0;
    }

    Data = getData(&infile, size, Bits);

    /* no image data */
    //if (Data == "") {
       //returnError (s, TGA_BAD_DATA);
      // cout << "no image data!!" << endl;
       //return 0;
    //}
    
    infile.close();
 
    //Now bind the texture...
    glGenTextures(1, textureObj);
    glBindTexture (GL_TEXTURE_2D, textureObj[0]);
    
    //Set a few params..
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 
   
   //Upload the stuff...
   glTexImage2D(GL_TEXTURE_2D,0,texFormat,Width,Height,0,
	texFormat,GL_UNSIGNED_BYTE, Data);

    //Free the memory we used...
    delete [] header;

    cout << "Loaded texture OK! " << endl;

    return true;
}
//================================================================
