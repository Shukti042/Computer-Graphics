#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
#include"classes.hpp"
double nearDis=1,farDis=1000;
double viewAngle;
double theta,k;
double imageDimension,windowDimension=500;
Point pos,u,r,l;

void drawAxes()
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
void capture()
{
    bitmap_image image(imageDimension,imageDimension);
    for(int i=0;i<imageDimension;i++)
    {
        for(int j=0;j<imageDimension;j++)
        {
            image.set_pixel(i,j,0,0,0);
        }
    }
    double planeDistance,du,dv,t,tmin=INFINITY;
    int nearest;
    Point topleft;
    planeDistance=(windowDimension/2)/(tan((viewAngle*pi)/360));
    topleft=pos+(l*planeDistance)-(r*(windowDimension/2))+(u*(windowDimension/2));
    du = windowDimension/imageDimension;
    dv = windowDimension/imageDimension;
    topleft = topleft + (r*(0.5*du)) - (u*(0.5*dv));
    for(int i=0;i<imageDimension;i++)
    {
        for(int j=0;j<imageDimension;j++)
        {
            Point curPixel=topleft + (r*(i*du)) - (u*(j*dv));
            Point raydir=curPixel-pos;
            raydir.normalize();
            Ray ray(pos,raydir);
            double *color = new double[3];
            tmin=INFINITY;
            nearest=-1;
            for(int o=0;o<objects.size();o++)
            {
                t=objects[o]->intersect(ray,color,0);
                if(t<tmin&&t<farDis)
                {
                    tmin=t;
                    nearest=o;
                }
            }
            if(nearest>=0)
            {
                tmin=objects[nearest]->intersect(ray,color,1);
                if(tmin>nearDis)
                {
                    for(int c=0;c<3;c++)
                    {
                        if(color[c]>1)
                        {
                            color[c]=1;
                        }
                        if(color[c]<0)
                        {
                            color[c]=0;
                        }
                    }
                    image.set_pixel(i,j,(int)(color[0]*255),(int)(color[1]*255),(int)(color[2]*255));
                }
                else
                {
                    image.set_pixel(i,j,0,0,0);
                }
            }
            delete color;
            color=NULL;

        }

    }


    image.save_image("C:\\Users\\Shukti\\Documents\\CSE 410\\Offline 3\\1605042\\1605042\\out.bmp");
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
         case '0':
            capture();
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
    for(int i=0;i<objects.size();i++)
    {
        objects[i]->draw();
    }
    for(int i=0;i<lights.size();i++)
    {
        lights[i].draw();
    }
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
	Point p1(50,30,0),p2(70,60,0),p3(50,45,50),rayDir(2,1,-1),rayStart(0,0,0);
	Triangle T(p1,p2,p3);
	Ray ray(rayStart,rayDir);
	double *c=new double[3];
	T.intersect(ray,c,0);
	viewAngle=80;
	nearDis=1;
	farDis=1000;
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
    theta=0.05;
    k=5;
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
	gluPerspective(viewAngle,	1,	nearDis,	farDis);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void loadData()
{
    std::ifstream ifs ("C:\\Users\\Shukti\\Documents\\CSE 410\\Offline 3\\1605042\\1605042\\scene.txt", std::ifstream::in);
    ifs>>recusionLevel>>imageDimension;
    int objectCount;
    ifs>>objectCount;
    for(int i=0;i<objectCount;i++)
    {
        std::string objectType;
        ifs>>objectType;
        if(objectType.compare("sphere")==0)
        {
            Point p;
            double radius;
            ifs>>p.x>>p.y>>p.z;
            ifs>>radius;
            Object *s=new Sphere(p,radius);
            double r,g,b,ambient, diffuse, specular,reflection;
            int shine;
            ifs>>r>>g>>b>>ambient>>diffuse>>specular>>reflection>>shine;
            s->setColor(r,g,b);
            s->setCoEfficients(ambient, diffuse, specular, reflection);
            s->shine=shine;
            objects.push_back(s);
        }
        if(objectType.compare("triangle")==0)
        {
            Point p1,p2,p3;
            ifs>>p1.x>>p1.y>>p1.z>>p2.x>>p2.y>>p2.z>>p3.x>>p3.y>>p3.z;
            Object *s=new Triangle(p1,p2,p3);
            double r,g,b,ambient, diffuse, specular, reflection;
            int shine;
            ifs>>r>>g>>b>>ambient>>diffuse>>specular>>reflection>>shine;
            s->setColor(r,g,b);
            s->setCoEfficients(ambient, diffuse, specular, reflection);
            s->shine=shine;
            objects.push_back(s);
        }
        if(objectType.compare("general")==0)
        {
            double a,b,c,d,e,f,g,h,i,j;
            ifs>>a>>b>>c>>d>>e>>f>>g>>h>>i>>j;
            Object *s=new General(a,b,c,d,e,f,g,h,i,j);
            ifs>>s->reference_point.x>>s->reference_point.y>>s->reference_point.z>>s->length>>s->width>>s->height;
            double r,ambient, diffuse, specular, reflection;
            int shine;
            ifs>>r>>g>>b>>ambient>>diffuse>>specular>>reflection>>shine;
            s->setColor(r,g,b);
            s->setCoEfficients(ambient, diffuse, specular, reflection);
            s->shine=shine;
            objects.push_back(s);
        }

    }
    Object *f=new Floor(1000,20);
    f->coEfficients[0]=0.4;
    f->coEfficients[1]=0.2;
    f->coEfficients[2]=0.2;
    f->coEfficients[3]=0.2;
    f->shine=5;
    objects.push_back(f);
    int lightCount;
    ifs>>lightCount;
    for(int i=0;i<lightCount;i++)
    {
        Point p;
        double r,g,b;
        ifs>>p.x>>p.y>>p.z>>r>>g>>b;
        Light l(p,r,g,b);
        lights.push_back(l);
    }
    ifs.close();
}


int main(int argc, char **argv){
    loadData();
	glutInit(&argc,argv);
	glutInitWindowSize(windowDimension, windowDimension);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Offline 3");

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
