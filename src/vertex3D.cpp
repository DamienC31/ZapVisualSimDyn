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
//The 3D vertex class - vertex3D.h
#include "freereign.h"

vertex3D::vertex3D()
{
	x=y=z=0.0;
}

vertex3D::vertex3D(float x,float y, float z)
{
	vertex3D::x = x;
	vertex3D::y = y;
        vertex3D::z = z;
}


/*float vertex3D::getX()
{
	return x;
}

float vertex3D::getY()
{
	return y;
}

float vertex3D::getZ()
{
	return z;
}

void vertex3D::setX(float x)
{
	vertex3D::x  = x;
}

void vertex3D::setY(float y)
{
	vertex3D::y =y;
}

void vertex3D::setZ(float z)
{
	vertex3D::z = z;
}*/

void vertex3D::setPos(float x, float y, float z)
{
	vertex3D::x  = x;
	vertex3D::y  = y;
	vertex3D::z  = z;
}


