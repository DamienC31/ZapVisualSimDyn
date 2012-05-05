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
//The 2D vertex class - vertex2D.cpp


#include "freereign.h"


vertex2D::vertex2D()
{
	x = y = 0;
}

vertex2D::vertex2D(int x, int y)
{
	vertex2D::x = x;
	vertex2D::y = y;
}

void vertex2D::setPos(int x, int y)
{
	vertex2D::x = x;
	vertex2D::y = y;
}

//int vertex2D::getX()
//{
//	return x;
//}

//int vertex2D::getY()
//{
//	return y;
//}

//void vertex2D::setX(int x)
//{
//	vertex2D::x = x;
//}

//void vertex2D::setY(int y)
//{
//	vertex2D::y = y;
//}


