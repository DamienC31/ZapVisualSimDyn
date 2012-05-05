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
//The user interface class - UInterface.h

class UInterface {
	public:
		UInterface(world*);
		void passiveMotionFunc(int x, int y);
		void motionFunc(int,int);
		void keyboardFunc(unsigned char,int,int, int);
		void mouseFunc(int,int,int,int);

		//The Mouse cursor stuff
		int getPickX();
		int getPickY();

		//Some accessors
		vertex2D getSelector();
		vertex2D* getSelectL();
		vertex2D* getSelectH();
		bool lines;
		bool ground;
		bool structs;
		bool lastSquare;
		bool interM;
	
		void setSelectH(vertex2D);
		void setSelectL(vertex2D);
		bool isSelecting();

		//Toolbox stuff
		void drawButtons();
		void checkButtons(int,int);
		//void buttonfunc();
		
		//Callbacks
		void setzone();


		//Toolbox functions
		void showTerrainFunc();
		void showZoneFunc();

		void raiseTerrainFunc();
		void lowerTerrainFunc();
		void levelTerrainFunc();
		void powerLineFunc();

		void zoneResFunc();
		void zoneComFunc();
		void zoneIndFunc();
	
		void roadFunc();

		//For the console...
		console_t *con;

	private:
		world* parent;
		int keymode;
		int zone_type;

		int pickx, picky;
		vertex2D selector,selectL,selectH; //Start,high,low
		bool selecting; //Whether we're selecting or not

		bool levelling;
		float level;

		buttonbar *toolbox;
		//unsigned int numButtons;
};
