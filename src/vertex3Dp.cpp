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
//The 3D vertex (pointer) class - vertex3D.cpp

#include "freereign.h"


vertex3Dp::vertex3Dp()
{
	x = y = z = NULL;
}

void vertex3Dp::setPos(float* x, float* y, float* z)
{
	vertex3Dp::x = x;
	vertex3Dp::y = y;
	vertex3Dp::z = z;
}

/*float* vertex3Dp::getX()
{
	return x;
}

float* vertex3Dp::getY()
{
	return y;
}

float* vertex3Dp::getZ()
{
	return z;
}*/

