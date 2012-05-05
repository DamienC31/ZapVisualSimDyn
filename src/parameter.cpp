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
//The parameter class - parameter.cpp

#include "freereign.h"

parameter::parameter()
{
}

parameter::~parameter()
{
	delete [] paramName;
	delete [] paramValue;
}

void parameter::initParameter(int nameNum,int valueNum)
{
	//Init the strings
	paramName = new char[nameNum];
	paramValue = new char[valueNum];
}

void parameter::set(const char* name, const char* value)
{
	strcpy(paramName, name);
	strcpy(paramValue, value);
}


int parameter::getParamValueInt()
{
	//TODO://If it's specifically initialised as an int, we need
	//To store it as one, saves time, good optimisation :)
	return atoi(paramValue);
}

char* parameter::getParamValue()
{
	return paramValue;
}

char* parameter::getParamName()
{
	return paramName;
}

int parameter::getParamType()
{
	return paramType;
}

