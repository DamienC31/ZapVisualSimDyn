/*
	Copyright (C) 1999
	For non-commercial use only.

  	File	: tga.c
	Date	: 05/05/1999
	Author	: Nate Miller
	Contact	: vandals1@home.com
*/
//#include "h.h"
#include "freereign.h"
#include "tga.h"

GLenum texFormat;

// make sure the image is ok without scaling
int checkSize (int x)
{
	if (x == 2	 || x == 4 || 
		x == 8	 || x == 16 || 
		x == 32  || x == 64 ||
		x == 128 || x == 256 || x == 512)
		return 1;
	else return 0;
}
// reads in RGBA data and returns the same, 32 bit image
unsigned char *getRGBA (FILE *s, int size)
{
	unsigned char *rgba;
	unsigned char temp;
	int bread;
	int i;

	rgba = new unsigned char[size * 4]; // allocate memory for image

	//  no memory allocated?
	if (rgba == NULL)
		return 0;

	bread = fread (rgba, sizeof (unsigned char), size * 4, s); 
	
	// make sure that all of the data is where it should be
	if (bread != size * 4)
	{
		free (rgba);
		return 0;
	}
	for (i = 0; i < size * 4; i += 4 )
	{
		temp = rgba[i];
		rgba[i] = rgba[i + 2];
		rgba[i + 2] = temp;
	}

	texFormat = GL_RGBA;
	return rgba;
}
// reads in RGB data and returns RGB, 24bit image
unsigned char *getRGB (FILE *s, int size)
{
	unsigned char *rgb;
	unsigned char temp;
	int bread;
	int i;

	rgb = new unsigned char[size * 3]; // allocate memory for image
	
	//  no memory allocated?
	if (rgb == NULL)
		return 0;

	bread = fread (rgb, sizeof (unsigned char), size * 3, s); // read in the image data

	// make sure that all of the data is where it should be
	if (bread != size * 3)
	{
		// Bytes read != Bytes requested
		free (rgb);
		return 0;
	}

	for (i = 0; i < size * 3; i += 3)
	{
		temp = rgb[i];
		rgb[i] = rgb[i + 2];
		rgb[i + 2] = temp;
	}
	
	texFormat = GL_RGB;

	return rgb;
}
// get the image data
unsigned char *getData (FILE *s, int sz, int iBits)
{
	if (iBits == 32)
		return getRGBA (s, sz);
	else if (iBits == 24)
		return getRGB (s, sz);	
}
// loads a 24bit or 32 bit targa file and uploads it to memory
// id is the texture id to bind too
int loadTGA (char *name, int id)
{
	unsigned char type[4];
	unsigned char info[7];
	unsigned char *imageData = NULL;
	int imageWidth, imageHeight;
	int imageBits, size;
	FILE *s;
	
	// Could not open the file!
	if (!(s = fopen (name, "r+bt")))
		return TGA_FILE_NOT_FOUND;

	fread (&type, sizeof (char), 3, s); // read in colormap info and image type, byte 0 ignored
	fseek (s, 12, SEEK_SET);			// seek past the header and useless info
	fread (&info, sizeof (char), 6, s);

	// make sure that the colormap is 0 and type is 2, unmapped RGB
	if (type[1] != 0 || type[2] != 2)
		return TGA_BAD_IMAGE_TYPE;

	imageWidth = info[0] + info[1] * 256; // image width
	imageHeight = info[2] + info[3] * 256; // image height
	imageBits =	info[4]; // pixel depth

	size = imageWidth * imageHeight; // image size

	// Dimension imageWidth x imageHeight is not a valid dimension!!
	if (!checkSize (imageWidth) || !checkSize (imageHeight))
		return TGA_BAD_DIMENSION;

	// Image bits != Supported bits 
	if (imageBits != 32 && imageBits != 24)
		return TGA_BAD_BITS;

	// get the image data
	imageData = getData (s, size, imageBits);
	
	fclose (s);

	// something went wrong
	if (imageData == NULL)
		return TGA_BAD_DATA;

	// OpenGL texture variables, change for various apps
	glBindTexture (GL_TEXTURE_2D, id);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D (GL_TEXTURE_2D, 0, texFormat, imageWidth, imageHeight, 0, texFormat, GL_UNSIGNED_BYTE, imageData);

	// release our image data since it has already been uploaded
	free (imageData);

	return 1;
}

