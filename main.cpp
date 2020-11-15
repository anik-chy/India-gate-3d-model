

#include<bits/stdc++.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h> //Needed for "exit" function
#include <math.h>

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;

//Called when a key is pressed

GLuint loadTexture(Image* image) {
	GLuint textureId;

	//The first argument is the number of textures we need, and the second is an array where OpenGL will store the id's of the textures.
	glGenTextures(1, &textureId); //Make room for our texture
	// to assign the texture id to our image data.
//	We call glBindTexture(GL_TEXTURE_2D, textureId) to let OpenGL know
// that we want to work with the texture we just created. Then, we call glTexImage2D to load the image into OpenGL.
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint roadTex;
GLuint grassTex;
GLuint topTex, sideTex, nightSkyTex;
GLuint texRoof1, wall, india;

//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);*/

	//Take a look at imageloader.h. This gives us the basic idea of what loadBMP does.
	//(The actual code for loadBMP is in imageloader.cpp.) Given a filename, it returns an Image object,
	//which contains the width and height of the image, as well as the array pixels,
	//which stores the pixels' colors in the format we want.
	Image* image = loadBMP("Road.bmp");

	//Once we've got the image, we have to send it to OpenGL. We do this in a function we write called loadTexture.
	roadTex = loadTexture(image);
	//Note that we should only use images whose widths and heights are 64, 128, or 256, since computers like powers of 2.
//	Other sizes of images might not work properly.
	delete image;

	image = loadBMP("Grass.bmp");
	grassTex = loadTexture(image);
	delete image;

	image = loadBMP("side.bmp");
	sideTex = loadTexture(image);
	delete image;

	image = loadBMP("top.bmp");
	topTex = loadTexture(image);
	delete image;

	image = loadBMP("NightSky.bmp");
	nightSkyTex = loadTexture(image);
	delete image;

	image = loadBMP("texRoof1.bmp");
	texRoof1 = loadTexture(image);
	delete image;

	image = loadBMP("wall.bmp");
	wall = loadTexture(image);
	delete image;

	image = loadBMP("india.bmp");
	india = loadTexture(image);
	delete image;
}

struct point{
    float x, y, z;

    point(float _x, float _y, float _z){
        x = _x;
        y = _y;
        z = _z;
    }
};

void drawQuad(point *a, point *b, point *c, point *d)
{
    glBegin(GL_QUADS);
	glVertex3f(a->x, a->y, a->z);
	glVertex3f(b->x, b->y, b->z);
	glVertex3f(c->x, c->y, c->z);
	glVertex3f(d->x, d->y, d->z);
    glEnd();
}

void drawQuadTex(point *a, point *b, point *c, point *d, GLuint _texture, float n, float m)
{
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
	glVertex3f(a->x, a->y, a->z);
	glTexCoord2f(n, 0.0f);
	glVertex3f(b->x, b->y, b->z);
	glTexCoord2f(n, m);
	glVertex3f(c->x, c->y, c->z);
	glTexCoord2f(0.0f, m);
	glVertex3f(d->x, d->y, d->z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawCube(point *a, point *b, point *c, point *d, point *e, point *f, point *g, point *h)
{
    drawQuad(a,b,f,e);
    drawQuad(a,b,c,d);
    drawQuad(b,f,g,c);
    drawQuad(e,f,g,h);
    drawQuad(a,e,h,d);
    drawQuad(d,c,g,h);
}

void drawCubeTex(point *a, point *b, point *c, point *d, point *e, point *f, point *g, point *h, GLuint tex, float n, float m)
{
    drawQuadTex(a,b,f,e,tex,n,m);
    drawQuadTex(a,b,c,d,tex,n,m);
    drawQuadTex(b,f,g,c,tex,n,m);
    drawQuadTex(e,f,g,h,tex,n,m);
    drawQuadTex(a,e,h,d,tex,n,m);
    drawQuadTex(d,c,g,h,tex,n,m);
}

void drawCubeTex2(point *a, point *b, point *c, point *d, point *e, point *f, point *g, point *h, GLuint tex, GLuint tex2, float n, float m)
{
    drawQuadTex(a,b,f,e,tex,n,m);
    drawQuadTex(a,b,c,d,tex2,1,1);
    drawQuadTex(b,f,g,c,tex,n,m);
    drawQuadTex(e,f,g,h,tex,n,m);
    drawQuadTex(a,e,h,d,tex,n,m);
    drawQuadTex(d,c,g,h,tex,n,m);
}

void drawArc(float ang, float xx, float yy, float zz)
{
    glTranslatef(xx, yy, zz);
    glColor3f(0.43f, 0.324f, 0.215f);
    glRotatef(ang, 0.0f, 1.0f, 0.0f);
    //drawQuad(new point(-1.0f, -0.5f, 0.5f), new point(1.0f, -0.5f, 0.5f), new point(1.0f, -0.25f, 0.5f), new point(-1.0f, -0.25f, 0.5f));
    float y=-0.8f, x=-1.0f;
    for(float i=90; i>=-90; i-=1)
    {
        float irad = (i/360.0)*M_PI;
        point* a = new point(x,y,0.5f); point* b = new point(x+0.01111,y+0.01111*sin(irad),0.5);
        //point* a = new point(x,y,0.5f); point* b = new point(x+0.01111,sqrt(1-x*x),0.5);
        point* c = new point(x+0.01111,-0.25,0.5); point* d = new point(x,-0.25,0.5f);
        point* e = new point(x,y,-0.5f); point* f = new point(x+0.01111,y+0.01111*sin(irad),-0.5);
        point* g = new point(x+0.01111,-0.25,-0.5); point* h = new point(x,-0.25,-0.5f);
        drawCube(a,b,c,d,e,f,g,h);
        x = x+0.01111f; y = y+0.01111*sin(irad);
        //x = x+0.01111f; y = ysqrt(1-x*x);
    }
    glRotatef(-1*ang, 0.0f, 1.0f, 0.0f);
    xx = -1*xx; yy = -1*yy; zz = -1*zz;
    glTranslatef(xx, yy, zz);
}

void drawHalfCirc(float ang, float rad, float xx, float yy, float zz, float ymin)
{
    glTranslatef(xx, yy, zz);
    glColor3f(0.43f, 0.324f, 0.215f);
    glRotatef(ang, 0.0f, 1.0f, 0.0f);
    //drawQuad(new point(-1.0f, -0.5f, 0.5f), new point(1.0f, -0.5f, 0.5f), new point(1.0f, -0.25f, 0.5f), new point(-1.0f, -0.25f, 0.5f));

    float y=-0.8;
    for(float x=0.005-rad; x<=rad-0.005; x+=0.005)
    {
        point* a = new point(x,y,0.5f); point* b = new point(x+0.005,ymin+sqrt(1-(x*x)),0.5);
        //point* a = new point(x,y,0.4f); point* b = new point(x+0.01111,sqrt(1-x*x),0.5);
        point* c = new point(x+0.005,-0.36,0.5); point* d = new point(x,-0.36,0.5f);
        point* e = new point(x,y,-0.5f); point* f = new point(x+0.005,ymin+sqrt(1-(x*x)),-0.5);
        point* g = new point(x+0.005,-0.36,-0.5); point* h = new point(x,-0.36,-0.5f);
        drawCube(a,b,c,d,e,f,g,h);
        y = ymin+sqrt(1-(x*x));
        //x = x+0.01111f; y = ysqrt(1-x*x);
    }
    glRotatef(-1*ang, 0.0f, 1.0f, 0.0f);
    xx = -1*xx; yy = -1*yy; zz = -1*zz;
    glTranslatef(xx, yy, zz);
}

void drawFill(float ang, float xx, float yy, float zz)
{
    glTranslatef(xx, yy, zz);
    glColor3f(0.74f, 0.64f, 0.53f);
    glRotatef(ang, 0.0f, 1.0f, 0.0f);
    point* a = new point(-0.7f, -0.76f, 0.5f);
    point* b = new point(0.7f, -0.76f, 0.5f);
    point* c = new point(0.7f, 0.25f, 0.5f);
    point* d = new point(-0.7f, 0.25f, 0.5f);
    point* e = new point(-0.7f, -0.76f, -0.5f);
    point* f = new point(0.7f, -0.76f, -0.5f);
    point* g = new point(0.7f, 0.25f, -0.5f);
    point* h = new point(-0.7f, 0.25f, -0.5f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,2);

    glColor3f(0.36f, 0.22f, 0.19f);
    a = new point(-0.56f, -0.69f, 0.65f);
    b = new point(0.56f, -0.69f, 0.65f);
    c = new point(0.56f, -0.64f, 0.65f);
    d = new point(-0.56f, -0.64f, 0.65f);
    e = new point(-0.56f, -0.69f, -0.65f);
    f = new point(0.56f, -0.69f, -0.65f);
    g = new point(0.56f, -0.64f, -0.65f);
    h = new point(-0.56f, -0.64f, -0.65f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);

    glColor3f(0.74f, 0.64f, 0.53f);
    a = new point(-0.7f, -3.3f, 0.5f);
    b = new point(0.7f, -3.3f, 0.5f);
    c = new point(0.7f, -2.1f, 0.5f);
    d = new point(-0.7f, -2.1f, 0.5f);
    e = new point(-0.7f, -3.3f, -0.5f);
    f = new point(0.7f, -3.3f, -0.5f);
    g = new point(0.7f, -2.1f, -0.5f);
    h = new point(-0.7f, -2.1f, -0.5f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,2);

    glColor3f(0.54f, 0.43f, 0.38f);
    a = new point(-2.05f, -2.15f, 0.55f);
    b = new point(2.05f, -2.15f, 0.55f);
    c = new point(2.05f, -2.099f, 0.55f);
    d = new point(-2.05f, -2.099f, 0.55f);
    e = new point(-2.05f, -2.15f, -0.55f);
    f = new point(2.05f, -2.15f, -0.55f);
    g = new point(2.05f, -2.099f, -0.55f);
    h = new point(-2.05f, -2.099f, -0.55f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);

    //glColor3f(0.531f, 0.469f, 0.363f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, wall);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTranslatef(0.0f, -2.23f, 0.0f);
    glutSolidSphere(0.3f, 50, 50);
    glTranslatef(0.0f, 2.23f, 0.0f);
    glTranslatef(0.0f, -1.93f, 0.0f);
    glutSolidSphere(0.15f, 50, 50);
    glTranslatef(0.0f, 1.93f, 0.0f);
    glTranslatef(0.0f, -1.75f, 0.0f);
    glutSolidSphere(0.05f, 50, 50);
    glTranslatef(0.0f, 1.75f, 0.0f);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_2D);

    glRotatef(-1*ang, 0.0f, 1.0f, 0.0f);
    xx = -1*xx; yy = -1*yy; zz = -1*zz;
    glTranslatef(xx, yy, zz);

}

int angle=0;
int n;
int anglex = 0, angley = 0;
float xx=0, yy=0, zz=0;
float zchange = 0;
void drawScene();
void handleKeypress(unsigned char key, //The key that was pressed
					int x, int y) {    //The current mouse coordinates
	switch (key) {
		case 27: //Escape key
			exit(0); //Exit the program
			break;
        case 'r':
		    //xx=1.0; yy=1.0; zz=1.0;
		    anglex = 0; angley = 0; zchange=0;
		    glutPostRedisplay();
			break;
        case 'w':
            anglex+=2;
            anglex%=360;
            xx=1; yy=0; zz=0;
            glutPostRedisplay();
			break;
        case 's':
            anglex-=2;
            anglex=(anglex+360)%360;
            xx=1; yy=0; zz=0;
            glutPostRedisplay();
			break;
        case 'a':
            angley-=2;
            angley=(angley+360)%360;
            xx=0; yy=1; zz=0;
            glutPostRedisplay();
			break;
        case 'd':
            angley+=2;
            angley%=360;
            xx=0; yy=1; zz=0;
            glutPostRedisplay();
			break;
        case '+':
            zchange+=0.05;
            glutPostRedisplay();
			break;
        case '-':
            zchange-=0.05;
            glutPostRedisplay();
			break;
        case 'n':
            n++;
            n%=2;
            glutPostRedisplay();
            break;
	}
}

void update(int a)
{
    if(xx || yy || zz)
    {
        angle+=2;
        angle%=360;
        glutPostRedisplay();
    }
    glutTimerFunc(50, update, 0);
}

//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
				   (double)w / (double)h, //The width-to-height ratio
				   1.0,                   //The near z clipping coordinate
				   200.0);                //The far z clipping coordinate
}

void drawPillar(float ang, float x, float y, float z)
{
    glTranslatef(x, y, z);
    glRotatef(ang, 0.0f, 1.0f, 0.0f);
    glColor3f(0.74f, 0.64f, 0.53f);
	point *a = new point(-0.5f, -3.8f, 0.5f);
    point *b = new point(0.5f, -3.8f, 0.5f);
    point *c = new point(0.5f, -0.25f, 0.5f);
    point *d = new point(-0.5f, -0.25f, 0.5f);
    point *e = new point(-0.5f, -3.8f, -0.8f);
    point *f = new point(0.5f, -3.8f, -0.8f);
    point *g = new point(0.5f, -0.25f, -0.8f);
    point *h = new point(-0.5f, -0.25f, -0.8f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,10);

    glColor3f(1.0f, 1.0f, 1.0f);
    a = new point(-0.35f, -3.8f, 0.53f);
    b = new point(0.35f, -3.8f, 0.53f);
    c = new point(0.35f, -0.5f, 0.53f);
    d = new point(-0.35f, -0.5f, 0.53f);
    e = new point(-0.35f, -3.8f, 0.5f);
    f = new point(0.35f, -3.8f, 0.5f);
    g = new point(0.35f, -0.5f, 0.5f);
    h = new point(-0.35f, -0.5f, 0.5f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,8);

    glColor3f(0.54f, 0.43f, 0.38f);
    a = new point(-0.6f, -1.7f, 0.6f);
    b = new point(0.6f, -1.7f, 0.6f);
    c = new point(0.6f, -1.65f, 0.6f);
    d = new point(-0.6f, -1.65f, 0.6f);
    e = new point(-0.6f, -1.7f, -0.9f);
    f = new point(0.6f, -1.7f, -0.9f);
    g = new point(0.6f, -1.65f, -0.9f);
    h = new point(-0.6f, -1.65f, -0.9f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);

    glColor3f(0.36f, 0.22f, 0.19f);
    a = new point(-0.55f, -1.75f, 0.55f);
    b = new point(0.55f, -1.75f, 0.55f);
    c = new point(0.55f, -1.7f, 0.55f);
    d = new point(-0.55f, -1.7f, 0.55f);
    e = new point(-0.55f, -1.75f, -0.85f);
    f = new point(0.55f, -1.75f, -0.85f);
    g = new point(0.55f, -1.7f, -0.85f);
    h = new point(-0.55f, -1.7f, -0.85f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);

    glColor3f(0.36f, 0.22f, 0.19f);
    a = new point(-0.65f, -1.14f, 0.65f);
    b = new point(0.65f, -1.14f, 0.65f);
    c = new point(0.65f, -1.19f, 0.65f);
    d = new point(-0.65f, -1.19f, 0.65f);
    e = new point(-0.65f, -1.14f, -0.95f);
    f = new point(0.65f, -1.14f, -0.95f);
    g = new point(0.65f, -1.19f, -0.95f);
    h = new point(-0.65f, -1.19f, -0.95f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);

    glRotatef(-1*ang, 0.0f, 1.0f, 0.0f);
}

void drawRoof(float x, float y, float z)
{
    glTranslatef(x, y, z);
    //glColor3f(0.54f, 0.43f, 0.38f);
    glColor3f(0.74f, 0.64f, 0.53f);
    point *a = new point(-2.0f, -0.25f, 2.0f);
    point *b = new point(2.0f, -0.25f, 2.0f);
    point *c = new point(2.0f, 0.0f, 2.0f);
    point *d = new point(-2.0f, 0.0f, 2.0f);
    point *e = new point(-2.0f, -0.25f, -2.0f);
    point *f = new point(2.0f, -0.25f, -2.0f);
    point *g = new point(2.0f, 0.0f, -2.0f);
    point *h = new point(-2.0f, 0.0f, -2.0f);
    drawCubeTex(a,b,c,d,e,f,g,h,texRoof1, 1,1);
}

void drawRoof2(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.44f, 0.33f, 0.32f);

    point *a = new point(-2.05f, 0.1f, 2.05f);
    point *b = new point(2.05f, 0.1f, 2.05f);
    point *c = new point(2.05f, 0.15f, 2.05f);
    point *d = new point(-2.05f, 0.15f, 2.05f);
    point *e = new point(-2.05f, 0.1f, -2.05f);
    point *f = new point(2.05f, 0.1f, -2.05f);
    point *g = new point(2.05f, 0.15f, -2.05f);
    point *h = new point(-2.05f, 0.15f, -2.05f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,10);

}

void drawRoof3(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.36f, 0.22f, 0.19f);

    point *a = new point(-2.1f, 0.15f, 2.1f);
    point *b = new point(2.1f, 0.15f, 2.1f);
    point *c = new point(2.1f, 0.20f, 2.1f);
    point *d = new point(-2.1f, 0.20f, 2.1f);
    point *e = new point(-2.1f, 0.15f, -2.1f);
    point *f = new point(2.1f, 0.15f, -2.1f);
    point *g = new point(2.1f, 0.20f, -2.1f);
    point *h = new point(-2.1f, 0.20f, -2.1f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,10);

}

void drawRoof4(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.74f, 0.64f, 0.53f);

    point *a = new point(-2.0f, 0.20f, 2.0f);
    point *b = new point(2.0f, 0.20f, 2.0f);
    point *c = new point(2.0f, 0.65f, 2.0f);
    point *d = new point(-2.0f, 0.65f, 2.0f);
	point *e = new point(-2.0f, 0.20f, -2.0f);
    point *f = new point(2.0f, 0.20f, -2.0f);
    point *g = new point(2.0f, 0.65f, -2.0f);
    point *h = new point(-2.0f, 0.65f, -2.0f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,4,1);
}

void drawRoof5(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.92f, 0.81f, 0.7f);

    point *a = new point(-1.85f, 0.65f, 1.85f);
    point *b = new point(1.85f, 0.65f, 1.85f);
    point *c = new point(1.85f, 0.95f, 1.85f);
    point *d = new point(-1.85f, 0.95f, 1.85f);
	point *e = new point(-1.85f, 0.65f, -1.85f);
    point *f = new point(1.85f, 0.65f, -1.85f);
    point *g = new point(1.85f, 0.95f, -1.85f);
    point *h = new point(-1.85f, 0.95f, -1.85f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,3,1);
}

void drawRoof6(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.74f, 0.64f, 0.53f);

    point *a = new point(-1.7f, 0.95f, 1.7f);
    point *b = new point(1.7f, 0.95f, 1.7f);
    point *c = new point(1.7f, 1.4f, 1.7f);
    point *d = new point(-1.7f, 1.4f, 1.7f);
	point *e = new point(-1.7f, 0.95f, 0.5f);
    point *f = new point(1.7f, 0.95f, 0.5f);
    point *g = new point(1.7f, 1.4f, 0.5f);
    point *h = new point(-1.7f, 1.4f, 0.5f);
    drawCubeTex2(a,b,c,d,e,f,g,h,wall,india,1,1);


    glRotatef(180, 0.0f, 1.0f, 0.0f);
    drawCubeTex2(a,b,c,d,e,f,g,h,wall,india,1,1);

    glRotatef(-180, 0.0f, 1.0f, 0.0f);
}

void drawRoof7(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.92f, 0.81f, 0.7f);

    point *a = new point(-1.4f, 1.4f, 1.6f);
    point *b = new point(1.4f, 1.4f, 1.6f);
    point *c = new point(1.4f, 1.7f, 1.6f);
    point *d = new point(-1.4f, 1.7f, 1.6f);
	point *e = new point(-1.4f, 1.4f, 0.6f);
    point *f = new point(1.4f, 1.4f, 0.6f);
    point *g = new point(1.4f, 1.7f, 0.6f);
    point *h = new point(-1.4f, 1.7f, 0.6f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,2,1);

    glRotatef(180, 0.0f, 1.0f, 0.0f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,2,1);
    glRotatef(-180, 0.0f, 1.0f, 0.0f);
}

void drawRoof8(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.74f, 0.64f, 0.53f);

    point *a = new point(-0.8f, 1.7f, 1.6f);
    point *b = new point(0.8f, 1.7f, 1.6f);
    point *c = new point(0.8f, 1.85f, 1.6f);
    point *d = new point(-0.8f, 1.85f, 1.6f);
	point *e = new point(-0.8f, 1.7f, 0.6f);
    point *f = new point(0.8f, 1.7f, 0.6f);
    point *g = new point(0.8f, 1.85f, 0.6f);
    point *h = new point(-0.8f, 1.85f, 0.6f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);

    glRotatef(180, 0.0f, 1.0f, 0.0f);
    drawCubeTex(a,b,c,d,e,f,g,h,wall,1,1);
    glRotatef(-180, 0.0f, 1.0f, 0.0f);
}

void drawCap(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(0.48f, 0.355f, 0.238f);

    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadricObj *quad = gluNewQuadric();
    gluQuadricDrawStyle( quad, GLU_FILL); //FILL also can be line(wire)
    gluQuadricNormals( quad, GLU_SMOOTH); // For if lighting is to be used.
    gluQuadricOrientation( quad,GLU_OUTSIDE);
    gluQuadricTexture( quad, GL_TRUE);// if you want to map a texture to it.
    glColor3f(0.48f, 0.355f, 0.238f);gluDisk(quad, 0.0f, 1.0f, 50, 50);
    glColor3f(0.48f, 0.355f, 0.238f);gluCylinder(quad, 1.0f, 1.0f, 1.2f, 50, 50);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -2.0f, 0.0f);
    glTranslatef(0.0f, 1.4f, 0.0f);
    glutSolidSphere(1.0, 50, 50);
    glTranslatef(0.0f, -1.4f, 0.0f);
}

void drawRoad(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f);
    point *a = new point(-1.0f, -3.8f, 25.0f);
    point *b = new point(1.0f, -3.8f, 25.0f);
    point *c = new point(1.0f, -3.8f, -15.0f);
    point *d = new point(-1.0f, -3.8f, -15.0f);
    drawQuadTex(a,b,c,d,roadTex,1,15);
}

void drawGrass(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f);
    point *a = new point(-50.0f, -3.801f, 25.0f);
    point *b = new point(50.0f, -3.801f, 25.0f);
    point *c = new point(50.0f, -3.801f, -15.0f);
    point *d = new point(-50.0f, -3.801f, -15.0f);
    drawQuadTex(a,b,c,d,grassTex,35,35);
}

void drawSky(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f);
    point *a = new point(-50.0f, -3.80f, 25.0f);
    point *b = new point(50.0f, -3.80f, 25.0f);
    point *c = new point(50.0f, 7.80f, 25.0f);
    point *d = new point(-50.0f, 7.80f, 25.0f);
    point *e = new point(-50.0f, -3.80f, -15.0f);
    point *f = new point(50.0f, -3.80f, -15.0f);
    point *g = new point(50.0f, 7.80f, -15.0f);
    point *h = new point(-50.0f, 7.80f, -15.0f);

    if(n==0)
    {
        drawQuadTex(a,b,c,d,sideTex,50,1);
        drawQuadTex(b,f,g,c,sideTex,50,1);
        drawQuadTex(e,f,g,h,sideTex,50,1);
        drawQuadTex(a,e,h,d,sideTex,50,1);
        drawQuadTex(d,c,g,h,topTex,50,30);
    }
    else
    {
        drawQuadTex(a,b,c,d,nightSkyTex, 50, 30);
        drawQuadTex(b,f,g,c,nightSkyTex, 50, 30);
        drawQuadTex(e,f,g,h,nightSkyTex, 50, 30);
        drawQuadTex(a,e,h,d,nightSkyTex, 50, 30);
        drawQuadTex(d,c,g,h,nightSkyTex, 50, 30);
    }

    //drawQuadTex(a,b,c,d,skyTex,5,10);
    glTranslatef(-1*x, -1*y, -1*z);
}

//Draws the 3D scene
void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	//glPolygoneMode(GL_FRONT_AND_BACK, GL_LINE);

	/*float posLight0[4] = {-1.0f, 1.0f, 1.0f, 0.0f};
    float ambLight0[4] = {0.5f, 0.5f, 0.5f, 0.5f};

    glLightfv(GL_LIGHT0, GL_POSITION, posLight0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight0);*/

	glTranslatef(0.0f, 1.0f, -10.0f);

	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	glRotatef(angley, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, zchange);

	glTranslatef(0.0f, -1.0f, 10.0f);

	drawCap(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof8(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof7(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof6(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof5(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof4(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof3(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
	drawRoof2(0.0f,1.0f,-10.0f);
	glTranslatef(0.0f, -1.0f, 10.0f);
    drawRoof(0.0f,1.1f,-10.0f);
    glTranslatef(0.0f, -1.1f, 10.0f);

    drawPillar(0.0f, 1.5f,1.1f,-8.5f);
    glTranslatef(-1.5f, -1.1f, 8.5f);
    drawPillar(180.0f, 1.5f,1.1f,-11.5f);
    glTranslatef(-1.5f, -1.1f, 11.5f);
    drawPillar(0.0f, -1.5f,1.1f,-8.5f);
    glTranslatef(1.5f, -1.1f, 8.5f);
    drawPillar(180.0f, -1.5f,1.1f,-11.5f);
    glTranslatef(1.5f, -1.1f, 11.5f);

    drawHalfCirc(0.0f, 1.0f, 0.0f, 1.2f, -8.5f, -1.35f);
    drawHalfCirc(90.0, 0.7f, 1.5f, 0.2f, -10.0f, -1.45f);
    drawFill(90.0, 1.5f, 0.6f, -10.0f);
    drawHalfCirc(180.0f, 1.0f, 0.0f, 1.2f, -11.5f, -1.35f);
    drawHalfCirc(270.0, 0.7f, -1.5f, 0.2f, -10.0f, -1.45f);
    drawFill(90.0, -1.5f, 0.6f, -10.0f);

    drawRoad(0.0f, 1.1f, -20.0f);
    glTranslatef(0.0f, -1.1f, 20.0f);
    drawGrass(0.0f, 1.1f, -20.0f);
    glTranslatef(0.0f, -1.1f, 20.0f);
    drawSky(0.0f, 1.1f, -20.0f);

	glutSwapBuffers(); //Send the 3D scene to the screen
}

int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720); //Set the window size

	//Create the window
	glutCreateWindow("Basic Shapes");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	//glutTimerFunc(50, update, 0);

	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}
