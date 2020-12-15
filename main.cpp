#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include<time.h>

#include<math.h>
#include<string>

#define PI 3.1416
#define ESCAPE 27

using namespace std;

void rock();
void display1();
void display2();
void display3();
void ship();
void water();



struct point {
    GLfloat x,y,z;
};

void floatingWater();
int factorial(int);
void computeNcR(int, int*);
void computeBezier(float, point *, int, point *, int *);
void bezier(point *, int, int);

void text(int, int, string, int);
void myMouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void menu();
void initialiseValues();


struct timespec jmp,jmp2;

GLfloat a,b,c,d,e,f,g,h,x,i;
bool main_menu;
bool start;







float theta;



void initialiseValues() {
    a=0;    b=0;    c=0;    d=0;    e=0;    f=0;    
    g=500;  h=600;  
    x=0;    i=0;
    main_menu = true;
    start = false;
    theta = 0;
}




void update(int value)
{
	if (start) a+=1;
	glutPostRedisplay();
	glutTimerFunc(10,update,0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (main_menu) {
        menu();
    }

    else if (x<-100) {
        printf("The ship sunk completely");
        initialiseValues();
    }

    else {

        if (a<950) display1();

	    if(a>950) {
            if(b>180) b+=0.5;
            else b+=1.5;
		    display2();
	    }

	    if(b==180) {
            b+=1.5;
		    jmp.tv_sec = 0;
		    jmp.tv_nsec = 50000000L;
		    nanosleep(&jmp , &jmp2);
	    }

	    if(b>180) {
		    c+=0.75;
		    display3();
	    }

    }

	glFlush();
	glutSwapBuffers();
}


void display1()
{
	glPushMatrix();
	glTranslated(a,75,0.0);
	ship();
	glPopMatrix();
	floatingWater();
}

void display2()
{

	glClear(GL_COLOR_BUFFER_BIT);
	rock();
	glPushMatrix();
	glTranslated(b,75,0.0);
	ship();
	glPopMatrix();
//	water();
	floatingWater();

}

void display3()
{
	glClear(GL_COLOR_BUFFER_BIT);
	x-=0.1;
	rock();
	glPushMatrix();
	glTranslated(b,100+x*5,0.0);
	glRotated(-10,0,0,1);
	ship();
	glPopMatrix();
//	water();
	floatingWater();


}














void text(int x, int y, string s, int font) {
	int i=0;
	glColor3f(0.0,0.0,0.8);
	glRasterPos2f(x,y);
	for(i=0;i<s.length();i++) {
		if(font==1)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,s[i]);
		else if(font==2)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
		else if(font==3) {
			glColor3f(1.0,0.0,0.0);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
		}
	}
}






int factorial(int n) {
    if(n<=1) return 1;
    return n*factorial(n-1);
}




void computeNcR(int n, int *hold_ncr_values) {
    int r;
    for(r=0; r<=n; r++) hold_ncr_values[r] = factorial(n) / (factorial(n-r) * factorial(r));
}




void computeBezier(float t, point *actual_bezier_point, int n_main, point *control_points_array, int *hold_ncr_values) {
    int i, n = n_main-1;
    float bernstein_polynomial;
    actual_bezier_point -> x = 0;
    actual_bezier_point -> y = 0;
    actual_bezier_point -> z = 0;
    for(i=0; i<n_main; i++) {
        bernstein_polynomial = hold_ncr_values[i] * pow(t,i) * pow(1-t,n-i);
        actual_bezier_point -> x += bernstein_polynomial * control_points_array[i].x;
        actual_bezier_point -> y += bernstein_polynomial * control_points_array[i].y;
        actual_bezier_point -> z += bernstein_polynomial * control_points_array[i].z;
    }
}




void bezier(point *control_points_array, int number_of_control_points, int number_of_bezier_points) {
    point actual_bezier_point;
    float t;
    int *hold_ncr_values, i;
    hold_ncr_values = new int[number_of_control_points];
    computeNcR(number_of_control_points-1, hold_ncr_values);

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<=number_of_bezier_points; i++) {
        t = float(i)/float(number_of_bezier_points);
        computeBezier(t, &actual_bezier_point, number_of_control_points, control_points_array, hold_ncr_values);
        glVertex2f(actual_bezier_point.x, actual_bezier_point.y);
    }
    glEnd();
    delete[] hold_ncr_values;
}




void floatingWater() {
    int number_of_control_points=4, number_of_bezier_points=20;
    float water_height = 125;
    point control_points_array[number_of_control_points] = 
                {{-250,water_height,0},{250,water_height+25,0},{750,water_height-25,0},{1250,water_height,0}};
    control_points_array[1].x += 50*sin(theta*PI/180.0);
    control_points_array[1].y += 25*sin(theta*PI/180.0);
    control_points_array[2].x -= 50*sin((theta+30)*PI/180.0);
    control_points_array[2].y -= 50*sin((theta+30)*PI/180.0);
    control_points_array[3].x -= 25*sin((theta-30)*PI/180.0);
    control_points_array[3].y += sin((theta-30)*PI/180.0);
    theta += 2;
    glPushMatrix();
    glPointSize(5);
    glEnable(GL_BLEND);
    glBlendFunc(1, 0.5);
    glColor3f(0.196078,0.6,0.8);
    for(int i=0; i<200; i++) {
        glTranslated(0,-0.8,0);
        bezier(control_points_array, number_of_control_points, number_of_bezier_points);
    }
    glPopMatrix();
}




void keyboard(unsigned char key, int x, int y) {
	
	switch(key) {

		case ESCAPE: exit(1);
		
		case ' ':
			if (main_menu) main_menu = false;
			break;

		default:
        	printf("You pressed: %c", key);
	}
}

void myMouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP) {
        if (main_menu) main_menu = false;
        else if (!start) start = true;
	}

	glutPostRedisplay();
}



void menu() {
	glClearColor(1.0,1.0,0.6,1.0);
	text(290,700,"TRAFFIC LIGHT SIMULATOR",1);
	text(390,660,"Using OpenGL",1);
	text(430,620,"Made By:",2);
	text(300,590,"Athish Venkatesh       Mahendar Singh Rathod",1);
	text(300,560," 18GAEI6010               18GAEM9042",2);
    text(400,540,"INSTRUCTIONS:",2);
	text(100,500,"Left click on your mouse to start",2);
	text(100,460,"Press ESC to exit at any time",2);
//  text(100,420,"W -- Yellow Light",2);
//	text(100,380,"R -- Green Light",2);
//	text(560,500,"For Right Traffic Light",2);
//	text(600,460,"A -- Red Light",2);
//	text(600,420,"S -- Yellow Light",2);
//	text(600,380,"D -- Green Light",2);
	text(200,100,"PRESS SPACEBAR TO CONTINUE",3);
	glutPostRedisplay();
	//glutSwapBuffers();
}
















void water()
{
    glEnable(GL_BLEND);
    glBlendFunc(1, 0.5);
	glColor3f(0.196078,0.6,0.8);
	glBegin(GL_POLYGON);
		glVertex2f(0,0);
		glVertex2f(1000,0);
		glVertex2f(1000,100);
		glVertex2f(0,100);
	glEnd();
}

void ship()
{


	glScaled(24,24,0);

	glColor3f(0.329412,0.329412,0.329412); //base
	glBegin(GL_POLYGON);
		glVertex2f(0.5,5.0);
		glVertex2f(3,1);
		glVertex2f(22,1);
		glVertex2f(24.0,5.0);
	glEnd();

	glColor3f(1.0,1.0,1.0); //ring
	glBegin(GL_POLYGON);
		glVertex2f(1.35,3.5);
		glVertex2f(1.6,3.2);
		glVertex2f(23.2,3.2);
		glVertex2f(23.35,3.5);
	glEnd();

	glColor3f(0.329412,0.329412,0.329412); //base
	glBegin(GL_POLYGON);
		glVertex2f(21.0,5.0);
		glVertex2f(21.0,6.0);
		glVertex2f(24.5,6.0);
		glVertex2f(24.0,5.0);
	glEnd();

	glColor3f(0.74902,0.847059,0.847059); //top-mid
	glBegin(GL_POLYGON);
		glVertex2f(2.0,5.0);
		glVertex2f(2.0,12.0);
		glVertex2f(15.0,12.0);
		glVertex2f(19.5,5.0);
	glEnd();

	glColor3f(0.137255,0.137255,0.556863); //ring
	glBegin(GL_POLYGON);
		glVertex2f(2.0,8.2);
		glVertex2f(2.0,8.8);
		glVertex2f(17.1,8.8);
		glVertex2f(17.5,8.2);
	glEnd();

	glColor3f(0.90,0.91,0.98); //window
	glBegin(GL_POLYGON);
		glVertex2f(3.0,6.0);
		glVertex2f(3.0,7.2);
		glVertex2f(4.0,7.2);
		glVertex2f(4.0,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(4.7,6.0);
		glVertex2f(4.7,7.2);
		glVertex2f(5.7,7.2);
		glVertex2f(5.7,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(8.1,6.0);
		glVertex2f(8.1,7.2);
		glVertex2f(9.1,7.2);
		glVertex2f(9.1,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(9.8,6.0);
		glVertex2f(9.8,7.2);
		glVertex2f(10.8,7.2);
		glVertex2f(10.8,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(6.4,6.0);
		glVertex2f(6.4,7.2);
		glVertex2f(7.4,7.2);
		glVertex2f(7.4,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(11.5,6.0);
		glVertex2f(11.5,7.2);
		glVertex2f(12.5,7.2);
		glVertex2f(12.5,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(13.3,6.0);
		glVertex2f(13.3,7.2);
		glVertex2f(14.3,7.2);
		glVertex2f(14.3,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(15.1,6.0);
		glVertex2f(15.1,7.2);
		glVertex2f(16.1,7.2);
		glVertex2f(16.1,6.0);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(16.9,6.0);
		glVertex2f(16.9,7.2);
		glVertex2f(17.9,7.2);
		glVertex2f(17.9,6.0);
	glEnd();




	glColor3f(0.90,0.91,0.98); //window
	glBegin(GL_POLYGON);
		glVertex2f(3.0,9.8);
		glVertex2f(3.0,11.0);
		glVertex2f(4.0,11.0);
		glVertex2f(4.0,9.8);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(4.7,9.8);
		glVertex2f(4.7,11.0);
		glVertex2f(5.7,11.0);
		glVertex2f(5.7,9.8);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(8.1,9.8);
		glVertex2f(8.1,11.0);
		glVertex2f(9.1,11.0);
		glVertex2f(9.1,9.8);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(9.8,9.8);
		glVertex2f(9.8,11.0);
		glVertex2f(10.8,11.0);
		glVertex2f(10.8,9.8);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(6.4,9.8);
		glVertex2f(6.4,11.0);
		glVertex2f(7.4,11.0);
		glVertex2f(7.4,9.8);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(11.5,9.8);
		glVertex2f(11.5,11.0);
		glVertex2f(12.5,11.0);
		glVertex2f(12.5,9.8);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(13.3,9.8);
		glVertex2f(13.3,11.0);
		glVertex2f(14.3,11.0);
		glVertex2f(14.3,9.8);
	glEnd();

	glColor3f(0.329412,0.329412,0.329412); //top-cover
	glBegin(GL_POLYGON);
		glVertex2f(1.5,12.0);
		glVertex2f(1.5,12.5);
		glVertex2f(16.0,12.5);
		glVertex2f(16.0,12.0);
	glEnd();

	glColor3f(0.0,0.0,0.0); //chim
	glBegin(GL_POLYGON);
		glVertex2f(2.5,12.5);
		glVertex2f(2.5,16.0);
		glVertex2f(5.0,16.0);
		glVertex2f(5.0,12.5);
	glEnd();

	glColor3f(1.0,0.25,0.0); //ring
	glBegin(GL_POLYGON);
		glVertex2f(2.5,12.5);
		glVertex2f(2.5,13.5);
		glVertex2f(5.0,13.5);
		glVertex2f(5.0,12.5);
	glEnd();

	glColor3f(1.0,0.25,0.0); //ring
	glBegin(GL_POLYGON);
		glVertex2f(2.5,14.5);
		glVertex2f(2.5,15.5);
		glVertex2f(5.0,15.5);
		glVertex2f(5.0,14.5);
	glEnd();

	glColor3f(0.0,0.0,0.0); //chim
	glBegin(GL_POLYGON);
		glVertex2f(6.0,12.5);
		glVertex2f(6.0,16.0);
		glVertex2f(8.5,16.0);
		glVertex2f(8.5,12.5);
	glEnd();

	glColor3f(1.0,0.25,0.0); //ring
	glBegin(GL_POLYGON);
		glVertex2f(6.0,14.5);
		glVertex2f(6.0,15.5);
		glVertex2f(8.5,15.5);
		glVertex2f(8.5,14.5);
	glEnd();

	glColor3f(1.0,0.25,0.0); //ring
	glBegin(GL_POLYGON);
		glVertex2f(6.0,12.5);
		glVertex2f(6.0,13.5);
		glVertex2f(8.5,13.5);
		glVertex2f(8.5,12.5);
	glEnd();
}

void rock()
{
	glPushMatrix();
	glTranslated(600,50,0.0);
	glScaled(10,10,0);
	glColor3f(0.36,0.25,0.20);

	if(c>0)
	{
		glPushMatrix();
		glTranslated(0,x,0);
		glPushMatrix();
		glTranslated(7,2,0.0);
		glRotated(-x,0,0,1);
		glTranslated(-7,-2,0.0);

		glBegin(GL_POLYGON);
		glVertex2f(8.1,1);
		glVertex2f(7.89,1.23);
		glVertex2f(11.26,18);
		glVertex2f(13.23,21.33);
		glVertex2f(14.23,2.53);
		glEnd();

		glPopMatrix();
		glPushMatrix();

		glTranslated(12,3,0.0);
		glRotated(x,0,0,1);
		glTranslated(-12,-3,0.0);

		glBegin(GL_POLYGON);
		glVertex2f(11.1,3.5);
		glVertex2f(13.2,18.7);
		glVertex2f(16.4,18.5);
		glVertex2f(18.12,3.65);
		glVertex2f(19.21,1.25);
		glEnd();

		glPopMatrix();
		glPopMatrix();
	}
	else
	{
		glBegin(GL_POLYGON);
		glVertex2f(8.1,1);
		glVertex2f(7.89,1.23);
		glVertex2f(11.26,18);
		glVertex2f(13.23,21.33);
		glVertex2f(14.23,2.53);
		glEnd();
			glBegin(GL_POLYGON);
			glVertex2f(11.1,3.5);
			glVertex2f(13.2,18.7);
			glVertex2f(16.4,18.5);
			glVertex2f(18.12,3.65);
			glVertex2f(19.21,1.25);
			glEnd();
	}


	glPopMatrix();

}



void myinit()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glColor3f(1.0,0.0,0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,999.0,0.0,799.0);
}

int main(int argc, char* argv[])

{
	int chs;
    initialiseValues();
	printf("Project Developed By Athish Venkatesh and Mahendar Singh Rathod\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024.0,768.0);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Sinking Ship");
	glutDisplayFunc(display);
	myinit();
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutTimerFunc(250,update,0);
	glutMainLoop();
	return 0;
}
