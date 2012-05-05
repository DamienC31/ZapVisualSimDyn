/*
	Copyright (C) 1999
	For non-commercial use only.

  	File	: tga.h
	Date	: 05/05/1999
	Author	: Nate Miller
	Contact	: vandals1@home.com
*/

// Error Codes
// file was not found
#define TGA_FILE_NOT_FOUND			13
// color mapped image or image is not RGB uncompressed
#define TGA_BAD_IMAGE_TYPE			14
// dimension is not a power of 2
#define TGA_BAD_DIMENSION			15
// image bits is not 24 or 32
#define TGA_BAD_BITS				16
// image data could not be loaded
#define TGA_BAD_DATA				17

// id is the texture id to bind too
int loadTGA (char *name, int id);