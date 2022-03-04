#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include <windows.h>
#include <GL/glut.h>
#include <time.h>
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle,pausedtime;
bool showCircle[5],paused;
struct point
{
	double x,y,z;
};
point centers[5];
double xspeed[5],yspeed[5];
double speedController;
time_t starttime,endtime,startpausetime,endpausetime,startCircleTime[5],endCircletime[5];
bool insideCircle[5],shouldCollid[5];
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[200];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}



void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}
void drawBubble()
{
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(120,120,0);
    glVertex3f(120,-120,0);

    glVertex3f(120,-120,0);
    glVertex3f(-120,-120,0);

    glVertex3f(-120,-120,0);
    glVertex3f(-120,120,0);

    glVertex3f(-120,120,0);
    glVertex3f(120,120,0);
    glEnd();
    glColor3f(1,0,0);
    drawCircle(80,199);
    glColor3f(1,0.3,0.2);
    for(int i=0;i<5&&showCircle[i];i++)
    {
        glPushMatrix();
        glTranslatef(centers[i].x,centers[i].y,centers[i].z);
        drawCircle(12,50);
        glPopMatrix();

    }

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'p':
            if(!showCircle[4])
            {
                if(!paused)
                {
                    time(&startpausetime);
                }
                else
                {
                    time(&endpausetime);
                    pausedtime+=difftime(endpausetime,startpausetime);

                }
            }
            paused=!paused;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
		    if(!paused)
            {
                if(speedController<500)
                        speedController*=2;
            }

			break;
		case GLUT_KEY_UP:		// up arrow key
		    if(!paused)
            {
                if(speedController>2)
                    speedController/=2;
            }

			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawBubble();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
	if(!paused)
    {
        if(!showCircle[4])
        {
            time (&endtime);
            double dif = difftime (endtime,starttime);
            dif=dif-pausedtime;
            if(dif>1)
            {
                showCircle[0]=true;
            }
            if(dif>2)
            {
                showCircle[1]=true;
            }
            if(dif>3)
            {
                showCircle[2]=true;
            }
            if(dif>4)
            {
                showCircle[3]=true;
            }
            if(dif>5)
            {
                showCircle[4]=true;
            }

        }
        for(int i=0;i<5&&showCircle[i];i++)
        {
            if(centers[i].y>108||centers[i].y<-108)
            {
                 yspeed[i]=yspeed[i]*-1;
            }
            if(centers[i].x>108||centers[i].x<-108)
            {
                 xspeed[i]=xspeed[i]*-1;
            }

        }
        for(int i=0;i<5;i++)
        {
            double distance=sqrt(centers[i].x*centers[i].x+centers[i].y*centers[i].y);
            if(distance<=68&&!insideCircle[i])
            {

                    insideCircle[i]=true;

            }

        }
        for(int i=0;i<5;i++)
        {
            if(insideCircle[i])
            {
                bool flag=false;
                for(int j=0;j<5;j++)
                {
                    if(i==j)
                    {
                        continue;
                    }
                    double dist=sqrt((centers[i].x-centers[j].x)*(centers[i].x-centers[j].x)+(centers[i].y-centers[j].y)*(centers[i].y-centers[j].y));
                    if(dist<=24)
                    {
                        flag=true;
                        break;
                    }

                }
                if(!flag)
                {
                    shouldCollid[i]=true;
                }
            }
        }
        for(int i=0;i<5;i++)
        {
            if(insideCircle[i])
            {
                double distance=sqrt((centers[i].x+xspeed[i]/speedController)*(centers[i].x+xspeed[i]/speedController)+(centers[i].y+yspeed[i]/speedController)*(centers[i].y+yspeed[i]/speedController));
                if(distance>=68)
                {
                    point normal;
                    normal.x=centers[i].x/sqrt(centers[i].x*centers[i].x+centers[i].y*centers[i].y);
                    normal.y=centers[i].y/sqrt(centers[i].x*centers[i].x+centers[i].y*centers[i].y);
                    double dot=normal.x*xspeed[i]+normal.y*yspeed[i];
                    xspeed[i]=xspeed[i]-2*dot*normal.x;
                    yspeed[i]=yspeed[i]-2*dot*normal.y;
                }

            }
        }
        for(int i=0;i<5;i++)
        {
            if(shouldCollid[i])
                {
                    for(int j=0;j<5;j++)
                    {
                        if(i==j||!shouldCollid[j])
                        {
                            continue;
                        }
                        double distance=sqrt((centers[i].x-centers[j].x)*(centers[i].x-centers[j].x)+(centers[i].y-centers[j].y)*(centers[i].y-centers[j].y));
                        if(distance<=24)
                        {
                            point normal;
                            normal.x=(centers[i].x-centers[j].x)/distance;
                            normal.y=(centers[i].y-centers[j].y)/distance;
                            double dot=normal.x*xspeed[i]+normal.y*yspeed[i];
                            xspeed[i]=xspeed[i]-2*dot*normal.x;
                            yspeed[i]=yspeed[i]-2*dot*normal.y;

                        }
                    }
                }
        }
        for(int i=0;i<5&&showCircle[i];i++)
        {
            centers[i].x+=xspeed[i]/speedController;
            centers[i].y+=yspeed[i]/speedController;
        }


    }





}

void init(){
    //srand(time(0));
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	paused=false;
	pausedtime=0;
	speedController=100;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	for(int i=0;i<5;i++)
    {
        centers[i].x=-108;
        centers[i].y=-108;
        centers[i].z=0;
        xspeed[i]=((double) rand() / (RAND_MAX));
        yspeed[i]=((double) rand() / (RAND_MAX));
        showCircle[i]=false;
        insideCircle[i]=false;
        shouldCollid[i]=false;
        //std::cout<<xspeed[i]<<std::endl;
        //std::cout<<yspeed[i]<<std::endl;

    }
    time (&starttime);

	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
