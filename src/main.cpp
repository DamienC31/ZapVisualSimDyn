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
//The main file - main.cpp


#include "freereign.h"
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>


//For the console...
extern command_t table[];
console_t con;



//Function headers
void display();
GLuint setupColor(float,float,float);
void motion(int x, int y);
void passivemotion(int,int);
void keyboardFunc(unsigned char, int,int);
void specialKeyboardFunc(int key, int x, int y);
void mouseFunc(int,int,int,int);
void idleFunc();

int frames=1;
float frame_time = 0;
world *simulator;
//void* fred = &world::drawGrid;



//============================================================
//Get the elapsed time since last call..
float
timedelta(void)
{
    static long begin = 0;
    static long finish, difference;
    static struct tms tb;

    finish = times(&tb);

    difference = finish - begin;
    begin = finish;

    return (float)difference/(float)1000;  /* CLK_TCK=1000 */
}


/* text: draws a string of text with an 18 point helvetica bitmap font
   at position (x,y) in window space (bottom left corner is (0,0). */
void
text(int x, int y, char* s) 
{
    int lines;
    char* p;

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WIDTH, 
	    0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3ub(0, 0, 0);
    glRasterPos2i(x+1, y-1);
    for(p = s, lines = 0; *p; p++) {
	if (*p == '\n') {
	    lines++;
	    glRasterPos2i(x+1, y-1-(lines*12));
	}
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
    }
    glColor3ub(128, 0, 255);
    glRasterPos2i(x, y);
    for(p = s, lines = 0; *p; p++) {
	if (*p == '\n') {
	    lines++;
	    glRasterPos2i(x, y-(lines*12));
	}
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}


void do_frames()
{
      float rot_rad, rate;
        static char s[32];
	frames++;
	if (frames > 7) {
	    sprintf(s, "%g fps", (float)7/frame_time);
	    frame_time = 0;
	    frames = 0;
	}
	text(5, 5, s);

}
//============================================================


void reshape (int w, int h)
{
  //Prevent a crash...
  if (h == 0) h = 1;
  consoleResize (&con, w, h);

  glViewport(0,0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 50.0);
  glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

  	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  	glutInitWindowSize(WIDTH, HEIGHT);
  	glutInitWindowPosition(128,128);
  	glutCreateWindow("Free Reign");

	std::cout << "\nFenêtre initialisée..." << std::endl;
	


	//Our GL init stuff
	glEnable(GL_DEPTH_TEST);
        glClearColor (0.0, 0.15, 0.4, 1.0);
	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	simulator = new world(60,60);




	
	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0);


	glutReshapeFunc(reshape);		
	glutDisplayFunc(display);
	glutPassiveMotionFunc(passivemotion);
	glutMotionFunc(motion);
	glutMouseFunc(mouseFunc);
	glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialKeyboardFunc);
	glutIdleFunc(idleFunc);

	
	simulator->getUI()->con = &con;
	consoleInit (&con, WIDTH, HEIGHT, display, "graphics/font.tga", 13, "graphics/back.tga", 14);


	glutMainLoop();
	delete simulator;
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 	glLoadIdentity();
	
	gluLookAt(simulator->getCamX(),simulator->getCamY(),simulator->getCamZ(),simulator->getLookX(),simulator->getLookY(),simulator->getLookZ(),   0.0,1.0,0.0);

	GLfloat light0_position[] = {-15.0, 20.0, -15.0, 1.0};
	GLfloat light_ambient[] = {0.5,0.5,0.5,0.5};
	GLfloat light_diffuse[] = {1.0,0.5,0.6,1.0};
	

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);



	if (simulator->getUI()->ground) simulator->getRenderer()->drawGrid();
	if (simulator->getUI()->lines)  simulator->getRenderer()->drawLines();
	if (simulator->getUI()->structs)simulator->getRenderer()->drawStructures();
	
	simulator->getRenderer()->drawToolBox();

	//Check if it's time to update all our game state stuff..
	simulator->checkUpdate();

	do_frames();

	//Draw the console...
	consoleDraw (&con);


	glutSwapBuffers();
}

void passivemotion(int x, int y)
{
	simulator->getUI()->passiveMotionFunc(x,y);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	simulator->getUI()->motionFunc(x,y);
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	simulator->getUI()->keyboardFunc(key,x,y,con.conState);
	glutPostRedisplay();
}

void specialKeyboardFunc(int key, int x, int y) {
    simulator->getUI()->specialKeyboardFunc(key,x,y,con.conState);
	glutPostRedisplay();
}

void mouseFunc(int button,int state,int x, int y)
{
	simulator->getUI()->mouseFunc(button,state,x,y);
	glutPostRedisplay();
}

void idleFunc()
{
        static float dt;

        dt = timedelta();
        frame_time += dt;

	if (con.conAni)
	{
		consoleAnimate (&con);
		glutPostRedisplay ();
	}


	glutPostRedisplay();
}

