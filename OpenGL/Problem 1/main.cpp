#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<cmath>
#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double k,theta,guntheta[4];
struct point
{
	double x,y,z;
};

struct point pos,u,r,l,ugun,rgun,lgun,ugun0,rgun0,lgun0,root;
struct point p1,p2,p3,p4;
struct point bullets[100][4];
int bulletcount=0;
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 200,0,0);
			glVertex3f(-200,0,0);

			glVertex3f(0,-200,0);
			glVertex3f(0, 200,0);

			glVertex3f(0,0, 200);
			glVertex3f(0,0,-200);
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
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
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

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
void drawMyHalfSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r,shade=1;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			   glColor3f(shade,shade,shade);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere

			}glEnd();
			shade=1-shade;
		}
	}
}
void drawMyStaticHalfSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r,shade=1;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
                //lower hemisphere
                glColor3f(shade,shade,shade);
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
			shade=1-shade;
		}
	}
}

void drawCylinder(double radius,double height,int segments)
{
    int i;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    double shade=1;
    for(i=0;i<segments;i++)
    {
        //create shading effect
        glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x,points[i].y,height);
            glVertex3f(points[i+1].x,points[i+1].y,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
        shade=1-shade;
    }
}
void drawFunnel(double innerRadius,double outerRadius,int slices,int segments)
{
    struct point points[100][100];
	int i,j;
	for(i=0;i<=segments;i++)
    {
        points[0][i].x=0;
        points[0][i].y=innerRadius-outerRadius*cos(((double)i/(double)segments)*0.5*pi);
        points[0][i].z=innerRadius-outerRadius*sin(((double)i/(double)segments)*0.5*pi);
        for(j=1;j<=slices;j++)
        {
             points[j][i].x=points[0][i].y*sin(((double)j/(double)slices)*2*pi);
             points[j][i].y=points[0][i].y*cos(((double)j/(double)slices)*2*pi);
             points[j][i].z=points[0][i].z;
        }
    }
    double shade=0;
    for(j=0;j<slices;j++)
    {
        glColor3f(shade,shade,shade);
        for(i=0;i<segments;i++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(points[j][i].x,points[j][i].y,points[j][i].z);
                glVertex3f(points[j][i+1].x,points[j][i+1].y,points[j][i+1].z);
                glVertex3f(points[j+1][i+1].x,points[j+1][i+1].y,points[j+1][i+1].z);
                glVertex3f(points[j+1][i].x,points[j+1][i].y,points[j+1][i].z);
            }
            glEnd();
        }
        shade=1-shade;
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
void drawGun()
{
    glRotatef(90,0,0,1);
    glPushMatrix();
    glRotatef((guntheta[0]*180)/pi,0,0,1);
    glPushMatrix();
        glRotatef(90,0,1,0);
        drawMyStaticHalfSphere(30,80,80);
    glPopMatrix();
    glPushMatrix();
    glRotatef((guntheta[1]*180)/pi,0,-1,0);
    glPushMatrix();
        glRotatef(90,0,1,0);
        drawMyHalfSphere(30,80,80);
    glPopMatrix();
    glTranslatef(30,0,0);
    glPushMatrix();
    glRotatef((guntheta[2]*180)/pi,0,-1,0);
    glPushMatrix();
    glRotatef((guntheta[3]*180)/pi,1,0,0);
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(10,0,0);
    glPushMatrix();
        glRotatef(90,0,1,0);
        drawMyStaticHalfSphere(10,60,60);
    glPopMatrix();
    glPushMatrix();
        glRotatef(90,0,1,0);
        drawCylinder(10,100,60);
    glPopMatrix();
    glTranslatef(120,0,0);
    glPushMatrix();
        glRotatef(-90,0,1,0);
        drawFunnel(20,10,60,60);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(p1.x,p1.y,p1.z);
    glVertex3f(p2.x,p2.y,p2.z);
    glVertex3f(p3.x,p3.y,p3.z);
    glVertex3f(p4.x,p4.y,p4.z);
    glEnd();
    glColor3f(1, 0, 0);
    for(int i=0;i<bulletcount;i++)
    {
        glBegin(GL_QUADS);
        glVertex3f(bullets[i][0].x,bullets[i][0].y,bullets[i][0].z);
        glVertex3f(bullets[i][1].x,bullets[i][1].y,bullets[i][1].z);
        glVertex3f(bullets[i][2].x,bullets[i][2].y,bullets[i][2].z);
        glVertex3f(bullets[i][3].x,bullets[i][3].y,bullets[i][3].z);
        glEnd();
    }





}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			r.x=r.x*cos(theta)+(u.y*r.z-u.z*r.y)*sin(theta);
			r.y=r.y*cos(theta)+(u.z*r.x-u.x*r.z)*sin(theta);
			r.z=r.z*cos(theta)+(u.x*r.y-u.y*r.x)*sin(theta);
			l.x=l.x*cos(theta)+(u.y*l.z-u.z*l.y)*sin(theta);
			l.y=l.y*cos(theta)+(u.z*l.x-u.x*l.z)*sin(theta);
			l.z=l.z*cos(theta)+(u.x*l.y-u.y*l.x)*sin(theta);
			break;
        case '2':
            r.x=r.x*cos(theta)-(u.y*r.z-u.z*r.y)*sin(theta);
			r.y=r.y*cos(theta)-(u.z*r.x-u.x*r.z)*sin(theta);
			r.z=r.z*cos(theta)-(u.x*r.y-u.y*r.x)*sin(theta);
			l.x=l.x*cos(theta)-(u.y*l.z-u.z*l.y)*sin(theta);
			l.y=l.y*cos(theta)-(u.z*l.x-u.x*l.z)*sin(theta);
			l.z=l.z*cos(theta)-(u.x*l.y-u.y*l.x)*sin(theta);
			break;
        case '3':
			u.x=u.x*cos(theta)+(r.y*u.z-r.z*u.y)*sin(theta);
			u.y=u.y*cos(theta)+(r.z*u.x-r.x*u.z)*sin(theta);
			u.z=u.z*cos(theta)+(r.x*u.y-r.y*u.x)*sin(theta);
			l.x=l.x*cos(theta)+(r.y*l.z-r.z*l.y)*sin(theta);
			l.y=l.y*cos(theta)+(r.z*l.x-r.x*l.z)*sin(theta);
			l.z=l.z*cos(theta)+(r.x*l.y-r.y*l.x)*sin(theta);
			break;
        case '4':
            u.x=u.x*cos(theta)-(r.y*u.z-r.z*u.y)*sin(theta);
			u.y=u.y*cos(theta)-(r.z*u.x-r.x*u.z)*sin(theta);
			u.z=u.z*cos(theta)-(r.x*u.y-r.y*u.x)*sin(theta);
			l.x=l.x*cos(theta)-(r.y*l.z-r.z*l.y)*sin(theta);
			l.y=l.y*cos(theta)-(r.z*l.x-r.x*l.z)*sin(theta);
			l.z=l.z*cos(theta)-(r.x*l.y-r.y*l.x)*sin(theta);
			break;
        case '6':
            u.x=u.x*cos(theta)+(l.y*u.z-l.z*u.y)*sin(theta);
			u.y=u.y*cos(theta)+(l.z*u.x-l.x*u.z)*sin(theta);
			u.z=u.z*cos(theta)+(l.x*u.y-l.y*u.x)*sin(theta);
			r.x=r.x*cos(theta)+(l.y*r.z-l.z*r.y)*sin(theta);
			r.y=r.y*cos(theta)+(l.z*r.x-l.x*r.z)*sin(theta);
			r.z=r.z*cos(theta)+(l.x*r.y-l.y*r.x)*sin(theta);
			break;
         case '5':
            u.x=u.x*cos(theta)-(l.y*u.z-l.z*u.y)*sin(theta);
			u.y=u.y*cos(theta)-(l.z*u.x-l.x*u.z)*sin(theta);
			u.z=u.z*cos(theta)-(l.x*u.y-l.y*u.x)*sin(theta);
			r.x=r.x*cos(theta)-(l.y*r.z-l.z*r.y)*sin(theta);
			r.y=r.y*cos(theta)-(l.z*r.x-l.x*r.z)*sin(theta);
			r.z=r.z*cos(theta)-(l.x*r.y-l.y*r.x)*sin(theta);
			break;
         case 'q':
            if(guntheta[0]<=pi/4)
            {
                guntheta[0]=guntheta[0]+theta;
            }
			break;
        case 'w':
            if(guntheta[0]>=-pi/4)
            {
                guntheta[0]=guntheta[0]-theta;
            }
			break;
         case 'e':
            if(guntheta[1]<=pi/4)
            {
                guntheta[1]=guntheta[1]+theta;

            }
            break;
         case 'r':
            if(guntheta[1]>=-pi/4)
            {
                guntheta[1]=guntheta[1]-theta;
            }
            break;
        case 'a':
            if(guntheta[2]<=pi/4)
            {
                guntheta[2]=guntheta[2]+theta;

            }
            break;
         case 's':
            if(guntheta[2]>=-pi/4)
            {
                guntheta[2]=guntheta[2]-theta;
            }
            break;
        case 'd':
            if(guntheta[3]<=pi/4)
            {
                guntheta[3]=guntheta[3]+theta;
            }
            break;
        case 'f':
            if(guntheta[3]>=-pi/4)
            {
                guntheta[3]=guntheta[3]-theta;
            }
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos.x-=k*l.x;
			pos.y-=k*l.y;
			pos.z-=k*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			pos.x+=k*l.x;
			pos.y+=k*l.y;
			pos.z+=k*l.z;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x+=k*r.x;
			pos.y+=k*r.y;
			pos.z+=k*r.z;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x-=k*r.x;
			pos.y-=k*r.y;
			pos.z-=k*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=k*u.x;
			pos.y+=k*u.y;
			pos.z+=k*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=k*u.x;
			pos.y-=k*u.y;
			pos.z-=k*u.z;
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
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
                rgun.x=rgun0.x*cos(guntheta[0])+(ugun0.y*rgun0.z-ugun0.z*rgun0.y)*sin(guntheta[0]);
                rgun.y=rgun0.y*cos(guntheta[0])+(ugun0.z*rgun0.x-ugun0.x*rgun0.z)*sin(guntheta[0]);
                rgun.z=rgun0.z*cos(guntheta[0])+(ugun0.x*rgun0.y-ugun0.y*rgun0.x)*sin(guntheta[0]);
                lgun.x=lgun0.x*cos(guntheta[0])+(ugun0.y*lgun0.z-ugun0.z*lgun0.y)*sin(guntheta[0]);
                lgun.y=lgun0.y*cos(guntheta[0])+(ugun0.z*lgun0.x-ugun0.x*lgun0.z)*sin(guntheta[0]);
                lgun.z=lgun0.z*cos(guntheta[0])+(ugun0.x*lgun0.y-ugun0.y*lgun0.x)*sin(guntheta[0]);
                ugun.x=ugun0.x;
                ugun.y=ugun0.y;
                ugun.z=ugun0.z;

                ugun.x=ugun.x*cos(guntheta[1])+(rgun.y*ugun.z-rgun.z*ugun.y)*sin(guntheta[1]);
                ugun.y=ugun.y*cos(guntheta[1])+(rgun.z*ugun.x-rgun.x*ugun.z)*sin(guntheta[1]);
                ugun.z=ugun.z*cos(guntheta[1])+(rgun.x*ugun.y-rgun.y*ugun.x)*sin(guntheta[1]);
                lgun.x=lgun.x*cos(guntheta[1])+(rgun.y*lgun.z-rgun.z*lgun.y)*sin(guntheta[1]);
                lgun.y=lgun.y*cos(guntheta[1])+(rgun.z*lgun.x-rgun.x*lgun.z)*sin(guntheta[1]);
                lgun.z=lgun.z*cos(guntheta[1])+(rgun.x*lgun.y-rgun.y*lgun.x)*sin(guntheta[1]);

                root.x=0;
                root.y=0;
                root.z=0;
                lgun.x=root.x+lgun.x*cos(guntheta[2])+(rgun.y*lgun.z-rgun.z*lgun.y)*sin(guntheta[2]);
                lgun.y=root.y+lgun.y*cos(guntheta[2])+(rgun.z*lgun.x-rgun.x*lgun.z)*sin(guntheta[2]);
                lgun.z=root.z+lgun.z*cos(guntheta[2])+(rgun.x*lgun.y-rgun.y*lgun.x)*sin(guntheta[2]);

                double tempy=(((300-lgun.x)/(lgun.x-root.x))*(lgun.y-root.y))+lgun.y;
                double tempz=(((300-lgun.x)/(lgun.x-root.x))*(lgun.z-root.z))+lgun.z;
                if(tempy<96&&tempy>-96&&tempz<96&&tempz>-96)
                {
                    bullets[bulletcount][0].x=298;
                    bullets[bulletcount][1].x=298;
                    bullets[bulletcount][2].x=298;
                    bullets[bulletcount][3].x=298;
                    bullets[bulletcount][0].y=tempy+4;
                    bullets[bulletcount][1].y=tempy+4;
                    bullets[bulletcount][2].y=tempy-4;
                    bullets[bulletcount][3].y=tempy-4;
                    bullets[bulletcount][0].z=tempz+4;
                    bullets[bulletcount][1].z=tempz-4;
                    bullets[bulletcount][2].z=tempz-4;
                    bullets[bulletcount][3].z=tempz+4;
                    bulletcount++;
                }

			}
			break;

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
	gluLookAt(pos.x,pos.y,pos.z,pos.x+l.x,pos.y+l.y,pos.z+l.z,u.x,u.y,u.z);


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

    drawGun();
    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
    pos.x=100;
    pos.y=100;
    pos.z=0;
    u.x=0;
    u.y=0;
    u.z=1;
    r.x=-1/sqrt(2);
    r.y=1/sqrt(2);
    r.z=0;
    l.x=-1/sqrt(2);
    l.y=-1/sqrt(2);
    l.z=0;
    for(int i=0;i<4;i++)
    {
        guntheta[i]=0;
    }
    lgun.x=1;
    lgun.y=0;
    lgun.z=0;
    rgun.x=0;
    rgun.y=-1;
    rgun.z=0;
    ugun.x=0;
    ugun.y=0;
    ugun.z=1;
    lgun0.x=1;
    lgun0.y=0;
    lgun0.z=0;
    rgun0.x=0;
    rgun0.y=-1;
    rgun0.z=0;
    ugun0.x=0;
    ugun0.y=0;
    ugun0.z=1;
    p1.x=300;
    p2.x=300;
    p3.x=300;
    p4.x=300;
    p1.y=100;
    p2.y=100;
    p3.y=-100;
    p4.y=-100;
    p1.z=100;
    p2.z=-100;
    p3.z=-100;
    p4.z=100;
    k=1;
    theta=0.05;
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
