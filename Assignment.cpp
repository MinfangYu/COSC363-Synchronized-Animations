#include <iostream>
#include <cmath> 
#include <fstream>
#include <climits>
#include <math.h> 
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "loadTGA.h"

#define GL_CLAMP_TO_EDGE 0x812F
using namespace std;

GLuint txId[8];   //Texture ids

float *x, *y, *z;		//vertex coordinate arrays
int *t1, *t2, *t3;		//triangles
int nvrt, ntri;			//total number of vertices and triangles
float angle= 0.9, look_x=0.0, look_y= 499.9, look_z=0.0; //Camera parameters
float eye_x=-500.0, eye_z=500.0;
float cam_hgt = 500.0;  //camera height
float ropeTheta = 20;
float ballTheta = 20;
float bulletTheta = 0;
float theta = 10;
int isRopeStop = 1;
int isBallStop = 1;
int isBulletStop = 0;
float cdr = M_PI / 180.0;
const int N = 4;  // Total number of vertices on the base curve
float vx_init[N] = { 2, 1.2, 0.9, 0.8 };
float vy_init[N] = { 0, 4, 4.2, 7};
float vz_init[N] = { 0 };
float vx[N], vy[N], vz[N];
float wx[N], wy[N], wz[N]; 
float cutoff = 10, eps = 2;   //spotlight parameters
float lpos[] = { 0.0,800.0,100.0, 1.0 };  //light's position
float sphere_x = 300;
float sphere_y = 185;
float xspeed= 6.0;
float yspeed= 9.0;
//------Function to load a texture in tga format  ------------------------
void loadTexture()				
{
	glGenTextures(8, txId); 	// Create 8 texture ids

	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("./skybox/left.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("./skybox/back.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("./skybox/right.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("./skybox/top.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture
    loadTGA("./skybox/front.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture
    loadTGA("./skybox/bottom.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[6]);  //Use this texture
    loadTGA("base.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glBindTexture(GL_TEXTURE_2D, txId[7]);  //Use this texture
    loadTGA("circle.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}
void sweepNormal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}
//----------draw a floor plane-------------------
void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -600; x <= 600; x += 50)
	{
		for(int z = -600; z <= 600; z += 50)
		{
			if(flag) glColor3f(0, 0.8, 0.8);
			else glColor3f(0.5, 0.8, 1);
			glVertex3f(x, -0.8, z);
			glVertex3f(x, -0.8, z+50);
			glVertex3f(x+50, -0.8, z+50);
			glVertex3f(x+50, -0.8, z);
			flag = !flag;
		}
	}
	glEnd();
}

void drawShelf()
{
	glPushMatrix(); 
		glTranslatef(0,600,100);          
		glScalef(450,10,10);
		glColor3f(0.55, 0.27, 0.07);
		glutSolidCube(1.0);
	glPopMatrix(); 

	glPushMatrix();                           
		glTranslatef(200,320,100);
		glScalef(10,640,10);
		glColor3f(0.55, 0.27, 0.07);
		glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix(); 
		glTranslatef(-200,320,100);
		glScalef(10,640,10);
		glColor3f(0.55, 0.27, 0.07);
		glutSolidCube(1.0);
	glPopMatrix(); 
}

void drawRope()
{
	glPushMatrix();
		glTranslatef(0, 600, 100); 
		glRotatef(ropeTheta, 0, 0, 1);      
		//glTranslatef(0, -600, -100); 
		//glTranslatef(0, 600, 100);
		glRotatef(90,1,0,0);
		glScalef(3,3,1);
		glColor3f(0.3,0.4,0.2);
		GLUquadricObj *q = gluNewQuadric();
		gluCylinder(q, 1, 1, 300, 30, 30);
    glPopMatrix();
	
	glPushMatrix(); 
		glTranslatef(0, 600, 100); 
		glRotatef(ropeTheta, 0, 0, 1);      
		glTranslatef(0, -600, -100); 
		glTranslatef(0, 270, 100);  
		glColor3f(1, 1, 0);
		glutSolidTorus(10, 30, 36, 36); 
	glPopMatrix();

}


void drawBall()
{
	glPushMatrix(); 
		glTranslatef(0, -60, 100); 
		glRotatef(ballTheta, 1, 0, 0);   
		glTranslatef(0, 60, -100); 
		glTranslatef(0, 270, 100); 
		glutSolidSphere(15, 36, 36); 

	glPopMatrix();
}

void drawCylinder1()
{
	glPushMatrix(); 
		glTranslatef(0,0,100+330*sin(20*cdr));     
		glRotatef(-90,1,0,0);     
		//glScalef(30,30,30);
		glColor3f(1, 0.3, 0.07);
		GLUquadricObj *q = gluNewQuadric();
		gluCylinder(q, 15, 15, 330*cos(20*cdr)-75, 30, 30);
	glPopMatrix(); 
}

void drawCylinder2()
{
	glPushMatrix(); 
		glTranslatef(0,0,100-330*sin(20*cdr));     
		glRotatef(-90,1,0,0);     
		//glScalef(30,30,30);
		glColor3f(1, 0.3, 0.07);
		GLUquadricObj *q = gluNewQuadric();
		gluCylinder(q, 15, 15, 330*cos(20*cdr)-75, 30, 30);
	glPopMatrix(); 
}

void drawRocket()
{
	for (int i = 0; i < N; i++)		//Initialize data everytime
	{
		vx[i] = vx_init[i];
		vy[i] = vy_init[i];
		vz[i] = vz_init[i];
	}
	//glColor3f (0.0, 0.0, 1.0);    //Used for wireframe display
	glEnable(GL_LIGHTING);
	for(int j = 0; j < 36; j++)  //36 slices in 10 deg steps 
{ 
     for(int i = 0; i < N; i++)   //N vertices along each slice 
     { 
        wx[i]= vx[i] *cos(theta * (M_PI / 180.0)) + vz[i] *sin(theta * (M_PI / 180.0));
		wy[i]= vy[i] ;
		wz[i]=  -vx[i] *sin(theta * (M_PI / 180.0)) + vz[i] *cos(theta * (M_PI / 180.0));
     } 
 
     glBegin(GL_TRIANGLE_STRIP);  //Create triangle strip using V, W 
		for (int i = 0; i < N; i++) 
	{ 
		if (i>0) {
			sweepNormal(vx[i-1],vy[i-1],vz[i-1], wx[i-1],wy[i-1],wz[i-1],vx[i],vy[i],vz[i]);
		}
		glVertex3f(vx[i], vy[i], vz[i]);  
		if (i>0) {
			sweepNormal(wx[i-1],wy[i-1],wz[i-1], wx[i],wy[i],wz[i],vx[i],vy[i],vz[i]);
		}
		glVertex3f(wx[i], wy[i],wz[i]); 
    } 
     glEnd();   
 
     //Copy W array to V for next iteration 
     for(int i = 0; i < N; i++) 
     { 
          vx[i] = wx[i]; 
          vy[i] = wy[i]; 
          vz[i] = wz[i]; 
     } 
}  
	
}

void drawBullet()
{
	glPushMatrix(); 
		glTranslatef(sphere_x, sphere_y, 100); 
		//glRotatef(20, 1, 0, 0);   
		//glTranslatef(0, -600, -100); 
		//glTranslatef(0, 270, 100); 
		glColor3f(0, 0.4, 0);
		glutSolidSphere(15, 36, 36); 

	glPopMatrix();
}
void drawBottom()
{
	glPushMatrix(); 
		glTranslatef(300,10,100);
		glScalef(100,50,100);
		glColor3f(0.55, 0.27, 0.07);
		glutSolidCube(1.0);
	glPopMatrix(); 
}

	
//--------------draw the skybox----------------------------------------------
void skybox()
{
	glEnable(GL_TEXTURE_2D);
	
	////////////////////// LEFT WALL ///////////////////////
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);
 	glBegin(GL_QUADS);
		glTexCoord2f(0.0,  0.0); 
		glVertex3f(-800, -1, 800);
		glTexCoord2f(1.0,  0.0); 
		glVertex3f(-800, -1, -800);
		glTexCoord2f(1.0, 1.0); 
		glVertex3f(-800, 800, -800);
		glTexCoord2f(0.0, 1.0);  
		glVertex3f(-800, 800, 800);
	glEnd();

	////////////////////// BACK WALL ///////////////////////

	glBindTexture(GL_TEXTURE_2D, txId[1]);
 	glBegin(GL_QUADS);
		glTexCoord2f(0.0,  0.0); 
		glVertex3f(800, -1, 800);
		glTexCoord2f(1.0,  0.0); 
		glVertex3f(-800, -1, 800);
		glTexCoord2f(1.0, 1.0); 
		glVertex3f(-800, 800, 800);
		glTexCoord2f(0.0, 1.0);  
		glVertex3f(800, 800, 800);
	glEnd();
    

	////////////////////// RIGHT WALL ///////////////////////

	glBindTexture(GL_TEXTURE_2D, txId[0]);
 	glBegin(GL_QUADS);
		glTexCoord2f(0.0,  0.0); 
		glVertex3f(800, -1, -800);
		glTexCoord2f(1.0,  0.0); 
		glVertex3f(800, -1, 800);
		glTexCoord2f(1.0, 1.0); 
		glVertex3f(800, 800, 800);
		glTexCoord2f(0.0, 1.0);  
		glVertex3f(800, 800, -800);
	glEnd();

	////////////////////// TOP WALL ///////////////////////

	glBindTexture(GL_TEXTURE_2D, txId[3]);
 	glBegin(GL_QUADS);
		glTexCoord2f(0.0,  0.0); 
		glVertex3f(-800,800, -800);
		glTexCoord2f(1.0,  0.0); 
		glVertex3f(800, 800, -800);
		glTexCoord2f(1.0, 1.0); 
		glVertex3f(800, 800, 800);
		glTexCoord2f(0.0, 1.0);  
		glVertex3f(-800, 800, 800);
	glEnd();
	
	////////////////////// FRONT WALL ///////////////////////

	glBindTexture(GL_TEXTURE_2D, txId[4]);
 	glBegin(GL_QUADS);
		glTexCoord2f(0.0,  0.0); 
		glVertex3f(-800,-1, -800);
		glTexCoord2f(1.0,  0.0); 
		glVertex3f(800, -1, -800);
		glTexCoord2f(1.0, 1.0); 
		glVertex3f(800, 800, -800);
		glTexCoord2f(0.0, 1.0);  
		glVertex3f(-800, 800, -800);
	glEnd();
	
	////////////////////// BOTTOM WALL ///////////////////////

	glBindTexture(GL_TEXTURE_2D, txId[5]);
 	glBegin(GL_QUADS);
		glTexCoord2f(0.0,  0.0); 
		glVertex3f(-800,-1, 800);
		glTexCoord2f(1.0,  0.0); 
		glVertex3f(800, -1, 800);
		glTexCoord2f(1.0, 1.0); 
		glVertex3f(800, -1, -800);
		glTexCoord2f(0.0, 1.0);  
		glVertex3f(-800, -1, -800);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void bottomTexture(){
	
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
        glVertex3f(250.0,35.1,50.0);
        glTexCoord2f(1., 0.);
		glVertex3f(350.0,35.1,50.0);
		glTexCoord2f(1., 1.);
		glVertex3f(350.0, 35.1, 150.0);
		glTexCoord2f(0., 1.);
		glVertex3f(250.0,35.1,150.0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
        glVertex3f(250,0,49.9);
        glTexCoord2f(1., 0.);
		glVertex3f(350,0,49.9);
		glTexCoord2f(1., 1.);
		glVertex3f(350, 35, 49.9);
		glTexCoord2f(0., 1.);
		glVertex3f(250,35,49.9);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
        glVertex3f(249.9,0,150);
        glTexCoord2f(1., 0.);
		glVertex3f(249.9,0,50);
		glTexCoord2f(1., 1.);
		glVertex3f(249.9, 35, 50);
		glTexCoord2f(0., 1.);
		glVertex3f(249.9,35,150);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
        glVertex3f(250,0,150.1);
        glTexCoord2f(1., 0.);
		glVertex3f(350,0,150.1);
		glTexCoord2f(1., 1.);
		glVertex3f(350, 35, 150.1);
		glTexCoord2f(0., 1.);
		glVertex3f(250,35,150.1);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
        glVertex3f(350.1,0,150);
        glTexCoord2f(1., 0.);
		glVertex3f(350.1,0,50);
		glTexCoord2f(1., 1.);
		glVertex3f(350.1, 35, 50);
		glTexCoord2f(0., 1.);
		glVertex3f(350.1,35,150);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);

}
/*
void bottom_texture(){
	
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
        glVertex3f(250.0,35.1,50.0);
        glTexCoord2f(1., 0.);
		glVertex3f(350.0,35.1,50.0);
		glTexCoord2f(1., 1.);
		glVertex3f(350.0, 35.1, 150.0);
		glTexCoord2f(0., 1.);
		glVertex3f(250.0,35.1,150.0);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);

}
*/

void circleTexture()
{
	float cutAngle = 0.;
	float  x, y, tx, ty;  
	
     glEnable(GL_TEXTURE_2D); 
     glBindTexture(GL_TEXTURE_2D, txId[7]); 
     glBegin(GL_POLYGON); 

     for (cutAngle=0.0; cutAngle<360.0; cutAngle+=2.0) 
     { 
      x = cos(cutAngle*cdr) * 15; 
      y = sin(cutAngle*cdr) * 15; 
      tx = (x/15 + 1)*0.5; 
      ty = (y/15+ 1)*0.5; 

      glTexCoord2f(tx, ty); 
      glVertex2f(x, y);   
     } 

     glEnd(); 
     glDisable(GL_TEXTURE_2D); 
}

void bulletTimer(int value)
{
	yspeed -= 1;
    sphere_x += xspeed;
    sphere_y += yspeed;

    if (sphere_y < 20){
		sphere_y = 20;
        yspeed *= -0.5;
    }
    if (sphere_x < 820) {
		glutTimerFunc(30, bulletTimer, 0);
	}

    glutPostRedisplay();
    
}
//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display() 
{
	float sldir[3];				//Spotlight's direction
	float ballpos[] = { 0.0,700.0,100.0, 1.0 }; 
   
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (eye_x, cam_hgt, eye_z, look_x, look_y, look_z, 0, 1, 0);  //camera rotation
   
	glLightfv(GL_LIGHT0, GL_POSITION, lpos); 
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, eps);
	sldir[0] = sin(ropeTheta* (M_PI / 180.0)); sldir[1] = -cos(ropeTheta* (M_PI / 180.0)); sldir[2] = 0; //spot attached to the object, with direction towards the floor
	glLightfv(GL_LIGHT1, GL_POSITION, ballpos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sldir);
	
	glRotatef(angle, 0, 0, 1); 
	
	drawFloor();
	drawBottom();
    bottomTexture();
    glPushMatrix();
		glTranslatef(0,330*cos(20*cdr)-75,100+330*sin(20*cdr));
		glRotatef(90, 1, 0, 0);   
		circleTexture();
    glPopMatrix();
    glPushMatrix();
		glTranslatef(0,330*cos(20*cdr)-75,100-330*sin(20*cdr));
		glRotatef(90, 1, 0, 0);   
		circleTexture();
    glPopMatrix();
    skybox();
    drawShelf();
    drawRope();
	float shadowMat[16] = { 800,0,0,0, 0,0,-100,-1, 0,0,800,0, 0,0,0,800 };
    glDisable(GL_LIGHTING);
    glPushMatrix();
		glMultMatrixf(shadowMat);
		glColor4f(0.2,0.2,0.2, 1.0);
		drawBall();
    glPopMatrix();
    glEnable(GL_LIGHTING);	 
    glPushMatrix();
		glColor3f(0.9, 0.4, 0.5);
        drawBall();
    glPopMatrix();
	glEnable(GL_LIGHTING);	
	glPushMatrix();
		glTranslatef(300,40,100);
		glColor3f(1, 0.3,0);
		glScalef(20,20,20);
		//glRotatef(-30, 0, 0, 1);   
		drawRocket();
	glPopMatrix();

    glPushMatrix();
    drawCylinder1();
    drawCylinder2();
    if (sqrt((330*cos(20*cdr)-330*cos(ballTheta*cdr)-15)*(330*cos(20*cdr)-330*cos(ballTheta*cdr)-15) + 
	(330*sin(20*cdr)-330*sin(ballTheta*cdr))*(330*sin(20*cdr)-330*sin(ballTheta*cdr))) <= 15) {
		xspeed = 6.0;
		yspeed = 9.0;
		sphere_x = 300;
        sphere_y = 185;
		glutTimerFunc(30, bulletTimer, 0);
		
		glutPostRedisplay();
	} 
    glPopMatrix();
	drawBullet();

	glutSwapBuffers();									
}
//------- Initialize OpenGL parameters -----------------------------------
void initialise()
{ 
	look_x = eye_x + sin(angle);
    look_z = eye_z - cos(angle);
	loadTexture();	
	float lightwhite[] = { 0.6f, 0.6f, 0.6f, 0.6f };   //light intensity
	float grey[] = { 0.2f, 0.2f, 0.2f, 1.0f };   //ambient intensity
	float black[] = { 0, 0, 0, 1 };
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightwhite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightwhite);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
	
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    
	glEnable(GL_SMOOTH);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(80.0, 1.0, 100.0, 5000.0);   //Perspective projection

}
void ropeTimer(int value)
{
    if (isRopeStop == 1) {
		ropeTheta--;
        if (ropeTheta == -20){
             isRopeStop = 0 ;
        }
    }

    if (isRopeStop == 0) {
		ropeTheta++;
        if (ropeTheta == 20){
            isRopeStop = 1;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, ropeTimer, 0);
}
void ballTimer(int value)
{
    if (isBallStop == 1) {
		ballTheta -= 1;
        if (ballTheta == -20){
            isBallStop = 0 ;
        }
    }

    if (isBallStop == 0) {
		ballTheta += 1;
		if (ballTheta == 20){
			isBallStop = 1;
		}
    }
    glutPostRedisplay();
    glutTimerFunc(30, ballTimer, 0);
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if (key == GLUT_KEY_PAGE_UP) cam_hgt-=1.0;
    if (key == GLUT_KEY_PAGE_DOWN) cam_hgt+=1.0;
    if(cam_hgt > 800) cam_hgt = 800;
	else if(cam_hgt < 0) cam_hgt = 0;

	if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
	else if(key == GLUT_KEY_DOWN)
	{  //Move backward
		eye_x -= 10*sin(angle);
		eye_z += 10*cos(angle);
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
		eye_x += 10*sin(angle);
		eye_z -= 10*cos(angle);
	}
	//if(eye_x > 800) eye_x = 800;
	//else if(eye_x < 0) eye_x = 0;
	look_x = eye_x + sin(angle);
	look_z = eye_z - cos(angle);
	glutPostRedisplay();
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (800, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Assignment1");
   initialise();
   glutTimerFunc(30, ropeTimer, 0);
   glutTimerFunc(30, ballTimer, 0);
   glutDisplayFunc(display); 
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
