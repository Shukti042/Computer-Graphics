#include<bits/stdc++.h>
#define pi (2*acos(0.0))
#define INFINITY 99999999
#include"bitmap_image.hpp"
int recusionLevel;
bool refraction=false;
bool texture=false;
bitmap_image textureImage("C:\\Users\\Shukti\\Documents\\CSE 410\\Offline 3\\1605042\\1605042\\1605042.bmp");
class Point
{
public:
    double x,y,z;
    Point(double x,double y,double z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    Point()
    {
        x=0;
        y=0;
        z=0;
    }
    void normalize()
    {
        double mod=sqrt(x*x+y*y+z*z);
        x=x/mod;
        y=y/mod;
        z=z/mod;
    }
    double dot(Point v)
    {
        return x*v.x+y*v.y+z*v.z;
    }
    double mod()
    {
        return sqrt(x*x+y*y+z*z);
    }
    Point operator+(Point p)
    {
        Point r;
        r.x=x+p.x;
        r.y=y+p.y;
        r.z=z+p.z;
        return r;
    }
    Point operator-(Point p)
    {
        Point r;
        r.x=x-p.x;
        r.y=y-p.y;
        r.z=z-p.z;
        return r;
    }
    Point operator*(double c)
    {
        Point r;
        r.x=x*c;
        r.y=y*c;
        r.z=z*c;
        return r;

    }
    Point operator*(Point v)
    {
        Point r;
        r.x=y*v.z-z*v.y;
        r.y=z*v.x-x*v.z;
        r.z=x*v.y-y*v.x;
        return r;
    }
};
class Light{
public:
    Point light_pos;
    double color[3];
    Light(Point p,double r,double g,double b)
    {
        light_pos=p;
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }
    void draw()
    {
        glPushMatrix();
        glTranslatef(light_pos.x,light_pos.y,light_pos.z);
        double slices=80,stacks=80;
        Point points[100][100];
        int i,j;
        double h,r;
        for(i=0;i<=stacks;i++)
        {
            h=2*sin(((double)i/(double)stacks)*(pi/2));
            r=2*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        glColor3f(color[0],color[1],color[2]);
        for(i=0;i<stacks;i++)
        {
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
        glPopMatrix();

    }
};
std::vector<Light> lights;
class Ray{
public:
    Point start,direction;
    Ray(Point s,Point d)
    {
        start=s;
        direction=d;
    }
};
class Object{
public:
    Point reference_point;
    double height, width, length;
    double color[3];
    double coEfficients[4];
    int shine;
    virtual void draw()=0;
    virtual double intersect(Ray r, double *color, int level)=0;
    void setColor(double r,double g,double b){
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }
    void setCoEfficients(double ambient,double diffuse,double specular,double recursive){
        coEfficients[0]=ambient;
        coEfficients[1]=diffuse;
        coEfficients[2]=specular;
        coEfficients[3]=recursive;
    }
};
std::vector<Object*> objects;
class Sphere:public Object{
public:
    Sphere(Point center,double radius)
    {
        reference_point = center;
        length = radius;
    }
    void draw(){
        glPushMatrix();
        glTranslatef(reference_point.x,reference_point.y,reference_point.z);
        double slices=80,stacks=80;
        Point points[100][100];
        int i,j;
        double h,r;
        for(i=0;i<=stacks;i++)
        {
            h=length*sin(((double)i/(double)stacks)*(pi/2));
            r=length*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        glColor3f(color[0],color[1],color[2]);
        for(i=0;i<stacks;i++)
        {
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
        glPopMatrix();
    }
    double intersect(Ray r, double *color, int level)
    {
        double tmin;
        Point r0=reference_point-r.start;
        double tp=r0.dot(r.direction);
        double d=r0.dot(r0)-(tp*tp);
        double tPrime=sqrt((length*length)-(d));
        if(tp<0)
        {
            tmin=INFINITY;
        }

        else if(d>(length*length))
        {
            tmin=INFINITY;
        }

        else if(r0.mod()<length)
        {
            tmin=tp+tPrime;
        }
        else
        {
            tmin=tp-tPrime;
        }
        if(level==0)
        {
            return tmin;
        }
        Point intersectionPoint=r.start+r.direction*tmin;
        for(int i=0;i<3;i++)
        {
            color[i]=this->color[i]*coEfficients[0];
        }
        Point normal=intersectionPoint-reference_point;
        normal.normalize();
        for(int i=0;i<lights.size();i++)
        {
            bool inShadow=false;;
            double *dummyColor = new double[3];
            Point raydir=lights[i].light_pos-intersectionPoint;
            double distance=raydir.mod();
            raydir.normalize();
            Ray ray(intersectionPoint+(raydir*0.0000001),raydir);
            double t;
            for(int j=0;j<objects.size();j++)
            {
                t=objects[j]->intersect(ray,dummyColor,0);
                if(t<distance && t>0)
                {
                    inShadow=true;
                    break;
                }

            }
            if(!inShadow)
            {
                Point R=normal*(2*raydir.dot(normal));
                R=R-raydir;
                R.normalize();
                Point V=r.start-intersectionPoint;
                V.normalize();
                double phongValue=V.dot(R);
                double lambertValue=normal.dot(raydir);
                if(lambertValue<0)
                {
                    continue;
                }
                else{
                    for(int j=0;j<3;j++)
                    {
                        color[j]+=lights[i].color[j]*coEfficients[1]*lambertValue*(this->color[j]);
                    }
                }
                if(phongValue<0)
                {
                    continue;
                }
                else{
                    double powered=phongValue;
                    for(int pow=1;pow<shine;pow++)
                    {
                        powered=powered*phongValue;
                    }
                    for(int j=0;j<3;j++)
                    {
                        color[j]+=lights[i].color[j]*coEfficients[2]*powered*(this->color[j]);
                    }
                }

            }
            delete dummyColor;
            dummyColor=NULL;
        }
        if(level>=recusionLevel)
        {
            return tmin;
        }
        Point incidentVector=r.direction*(-1);
        Point reflectedVector=normal*(2*incidentVector.dot(normal));
        reflectedVector=reflectedVector-incidentVector;
        reflectedVector.normalize();
        Ray reflectedRay(intersectionPoint+(reflectedVector*0.0000001),reflectedVector);
        double *colorReflected = new double[3];
        double tmin2,t2;
        int nearest;
        tmin2=INFINITY;
        nearest=-1;
        for(int o=0;o<objects.size();o++)
        {
            t2=objects[o]->intersect(reflectedRay,colorReflected,0);
            if(t2<tmin2)
            {
                tmin2=t2;
                nearest=o;
            }
        }
        if(nearest>0)
        {
            tmin2=objects[nearest]->intersect(reflectedRay,colorReflected,level+1);
            color[0]+=(coEfficients[3]*colorReflected[0]);
            color[1]+=(coEfficients[3]*colorReflected[1]);
            color[2]+=(coEfficients[3]*colorReflected[2]);
        }
        delete colorReflected;
        colorReflected=NULL;
        if(refraction)
        {
            double NdotI=normal.dot(r.direction);
            double k=1-1.0*1.0*(1-NdotI*NdotI);
            if(k>0)
            {
                Point refractedVector=(r.direction*1.0)-(normal*(1.0*NdotI+sqrt(k)));
                refractedVector.normalize();
                Ray refractedRay(intersectionPoint+(refractedVector*0.0000001),refractedVector);
                double *colorReflected = new double[3];
                double tmin2,t2;
                int nearest;
                tmin2=INFINITY;
                nearest=-1;
                for(int o=0;o<objects.size();o++)
                {
                    t2=objects[o]->intersect(refractedRay,colorReflected,0);
                    if(t2<tmin2)
                    {
                        tmin2=t2;
                        nearest=o;
                    }
                }
                if(nearest>0)
                {
                    tmin2=objects[nearest]->intersect(refractedRay,colorReflected,level+1);
                    color[0]+=((coEfficients[3]/2)*colorReflected[0]);
                    color[1]+=((coEfficients[3]/2)*colorReflected[1]);
                    color[2]+=((coEfficients[3]/2)*colorReflected[2]);
                }
                delete colorReflected;
                colorReflected=NULL;
            }
        }
        return tmin;

    }
};
class Triangle:public Object{
public:
    Point vertices[3];
    Triangle(Point p1,Point p2,Point p3)
    {
        vertices[0]=p1;
        vertices[1]=p2;
        vertices[2]=p3;
    }
    void draw(){
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(vertices[0].x,vertices[0].y,vertices[0].z);
            glVertex3f(vertices[1].x,vertices[1].y,vertices[1].z);
            glVertex3f(vertices[2].x,vertices[2].y,vertices[2].z);


        }glEnd();
    }
    double intersect(Ray r, double *color, int level)
    {
        double beta,gama,tmin,A;
        A=(vertices[0].x-vertices[1].x)*((vertices[0].y-vertices[2].y)*r.direction.z-(vertices[0].z-vertices[2].z)*r.direction.y);
        A=A-(vertices[0].x-vertices[2].x)*((vertices[0].y-vertices[1].y)*r.direction.z-(vertices[0].z-vertices[1].z)*r.direction.y);
        A=A+r.direction.x*((vertices[0].y-vertices[1].y)*(vertices[0].z-vertices[2].z)-(vertices[0].z-vertices[1].z)*(vertices[0].y-vertices[2].y));
        beta=(vertices[0].x-r.start.x)*(((vertices[0].y-vertices[2].y)*r.direction.z)-(vertices[0].z-vertices[2].z)*r.direction.y);
        beta=beta-(vertices[0].x-vertices[2].x)*((vertices[0].y-r.start.y)*r.direction.z-(vertices[0].z-r.start.z)*r.direction.y);
        beta=beta+r.direction.x*((vertices[0].y-r.start.y)*(vertices[0].z-vertices[2].z)-(vertices[0].z-r.start.z)*(vertices[0].y-vertices[2].y));
        beta=beta/A;
        gama=(vertices[0].x-vertices[1].x)*((vertices[0].y-r.start.y)*r.direction.z-(vertices[0].z-r.start.z)*r.direction.y);
        gama=gama-(vertices[0].x-r.start.x)*(((vertices[0].y-vertices[1].y)*r.direction.z)-(vertices[0].z-vertices[1].z)*r.direction.y);
        gama=gama+r.direction.x*((vertices[0].y-vertices[1].y)*(vertices[0].z-r.start.z)-(vertices[0].z-vertices[1].z)*(vertices[0].y-r.start.y));
        gama=gama/A;
        tmin=(vertices[0].x-vertices[1].x)*((vertices[0].y-vertices[2].y)*(vertices[0].z-r.start.z)-(vertices[0].z-vertices[2].z)*(vertices[0].y-r.start.y));
        tmin=tmin-(vertices[0].x-vertices[2].x)*((vertices[0].y-vertices[1].y)*(vertices[0].z-r.start.z)-(vertices[0].z-vertices[1].z)*(vertices[0].y-r.start.y));
        tmin=tmin+(vertices[0].x-r.start.x)*((vertices[0].y-vertices[1].y)*(vertices[0].z-vertices[2].z)-(vertices[0].z-vertices[1].z)*(vertices[0].y-vertices[2].y));
        tmin=tmin/A;
        if(beta+gama<1&&tmin>0&&beta>0&&gama>0)
        {
            if(level==0)
            {
                return tmin;
            }
            Point intersectionPoint=r.start+r.direction*tmin;
            for(int i=0;i<3;i++)
            {
                color[i]=this->color[i]*coEfficients[0];
            }
            Point side1,side2,normal;
            side1=vertices[1]-vertices[0];
            side2=vertices[2]-vertices[0];
            normal=side1*side2;
            normal.normalize();
            if(normal.dot(r.direction)>0)
            {
                normal=normal*(-1);
            }
            for(int i=0;i<lights.size();i++)
            {
                bool inShadow=false;;
                double *dummyColor = new double[3];
                Point raydir=lights[i].light_pos-intersectionPoint;
                double distance=raydir.mod();
                raydir.normalize();
                Ray ray(intersectionPoint+(raydir*0.0000001),raydir);
                double t;
                for(int j=0;j<objects.size();j++)
                {
                    t=objects[j]->intersect(ray,dummyColor,0);
                    if(t<distance && t>0)
                    {
                        inShadow=true;
                        break;
                    }

                }
                if(!inShadow)
                {
                    double lambertValue=normal.dot(raydir);
                    if(lambertValue<0)
                    {
                        continue;
                    }
                    for(int j=0;j<3;j++)
                    {
                        color[j]+=lights[i].color[j]*coEfficients[1]*lambertValue*(this->color[j]);
                    }
                    Point R=normal*(2*raydir.dot(normal));
                    R=R-raydir;
                    R.normalize();
                    Point V=r.start-intersectionPoint;
                    V.normalize();
                    double phongValue=V.dot(R);
                    if(phongValue<0)
                    {
                        continue;
                    }
                    double powered=phongValue;
                    for(int pow=1;pow<shine;pow++)
                    {
                        powered=powered*phongValue;
                    }
                    for(int j=0;j<3;j++)
                    {
                        color[j]+=lights[i].color[j]*coEfficients[2]*powered*(this->color[j]);
                    }


                }
                delete dummyColor;
                dummyColor=NULL;
            }
            if(level>=recusionLevel)
            {
                return tmin;
            }
            Point incidentVector=r.direction*(-1);
            Point reflectedVector=normal*(2*incidentVector.dot(normal));
            reflectedVector=reflectedVector-incidentVector;
            reflectedVector.normalize();
            Ray reflectedRay(intersectionPoint+(reflectedVector*0.0000001),reflectedVector);
            double *colorReflected = new double[3];
            double tmin2,t2;
            int nearest;
            tmin2=INFINITY;
            nearest=-1;
            for(int o=0;o<objects.size();o++)
            {
                t2=objects[o]->intersect(reflectedRay,colorReflected,0);
                if(t2<tmin2)
                {
                    tmin2=t2;
                    nearest=o;
                }
            }
            if(nearest>0)
            {
                tmin2=objects[nearest]->intersect(reflectedRay,colorReflected,level+1);
                color[0]+=(coEfficients[3]*colorReflected[0]);
                color[1]+=(coEfficients[3]*colorReflected[1]);
                color[2]+=(coEfficients[3]*colorReflected[2]);
            }
            delete colorReflected;
            colorReflected=NULL;
            if(refraction)
            {
                double NdotI=normal.dot(r.direction);
                double k=1-1.0*1.0*(1-NdotI*NdotI);
                if(k>0)
                {
                    Point refractedVector=(r.direction*1.0)-(normal*(1.0*NdotI+sqrt(k)));
                    refractedVector.normalize();
                    Ray refractedRay(intersectionPoint+(refractedVector*0.0000001),refractedVector);
                    double *colorReflected = new double[3];
                    double tmin2,t2;
                    int nearest;
                    tmin2=INFINITY;
                    nearest=-1;
                    for(int o=0;o<objects.size();o++)
                    {
                        t2=objects[o]->intersect(refractedRay,colorReflected,0);
                        if(t2<tmin2)
                        {
                            tmin2=t2;
                            nearest=o;
                        }
                    }
                    if(nearest>0)
                    {
                        tmin2=objects[nearest]->intersect(refractedRay,colorReflected,level+1);
                        color[0]+=((coEfficients[3]/2)*colorReflected[0]);
                        color[1]+=((coEfficients[3]/2)*colorReflected[1]);
                        color[2]+=((coEfficients[3]/2)*colorReflected[2]);
                    }
                    delete colorReflected;
                    colorReflected=NULL;
                }
            }
            return tmin;
        }
        else
        {
            return INFINITY;
        }

    }
};
class Floor:public Object{
public:
    int tileCount;
    Floor(double floorWidth,double tileWidth)
    {
        reference_point.x=-floorWidth/2;
        reference_point.y=-floorWidth/2;
        reference_point.z=0;
        length=tileWidth;
        tileCount=(int)(floorWidth/tileWidth);

    }
    void draw()
    {
        for(int i=0;i<tileCount;i++)
        {
            for(int j=0;j<tileCount;j++)
            {
                if((i+j)%2==0)
                {
                    glColor3f(1.0, 1.0, 1.0);
                }
                else
                {
                    glColor3f(0.0, 0.0, 0.0);
                }
                glBegin(GL_QUADS);{
                    glVertex3f(reference_point.x+i*length,reference_point.y+j*length,reference_point.z);
                    glVertex3f(reference_point.x+(i+1)*length,reference_point.y+j*length,reference_point.z);
                    glVertex3f(reference_point.x+(i+1)*length,reference_point.y+(j+1)*length,reference_point.z);
                    glVertex3f(reference_point.x+i*length,reference_point.y+(j+1)*length,reference_point.z);
                }glEnd();

            }
        }
    }
    double intersect(Ray r, double *color, int level)
    {
        Point n(0,0,1);
        if(n.dot(r.direction)==0)
        {
            return INFINITY;
        }
        double t=-(n.dot(r.start))/(n.dot(r.direction));
        Point p=r.start+(r.direction*t);
        p=p-reference_point;
        int ix=p.x/length;
        int jy=p.y/length;
        if(ix<tileCount&&jy<tileCount&&ix>=0&&jy>=0&&t>0)
        {
            if((ix+jy)%2==0)
            {
                this->color[0]=1;
                this->color[1]=1;
                this->color[2]=1;
            }
            else{
                this->color[0]=0;
                this->color[1]=0;
                this->color[2]=0;
                if(texture)
                {
                    unsigned int texturei,texturej;
                    texturei=p.x-(ix*20);
                    texturej=p.y-(jy*20);
                    unsigned char r,g,b;
                    textureImage.get_pixel(texturei,texturej,r,g,b);
                    double colorscale=256;
                    this->color[0]=r/colorscale;
                    this->color[1]=g/colorscale;
                    this->color[2]=b/colorscale;


                }
            }
            if(level==0)
            {
                return t;
            }
            for(int i=0;i<3;i++)
            {
                color[i]=this->color[i]*coEfficients[0];
            }
            Point intersectionPoint=r.start+(r.direction*t);
            if(n.dot(r.direction)>0)
            {
                n=n*(-1);
            }
            for(int i=0;i<lights.size();i++)
            {
                bool inShadow=false;;
                double *dummyColor = new double[3];
                Point lightRay=lights[i].light_pos-intersectionPoint;
                double distance=lightRay.mod();
                lightRay.normalize();
                Ray ray(intersectionPoint+(lightRay*0.0000001),lightRay);
                double ttest;
                for(int j=0;j<objects.size();j++)
                {
                    ttest=objects[j]->intersect(ray,dummyColor,0);
                    if(ttest<distance && ttest>0)
                    {
                        inShadow=true;
                        break;
                    }

                }
                if(!inShadow)
                {
                    double lambertValue=n.dot(lightRay);
                    if(lambertValue<0)
                    {
                        continue;
                    }
                    else{
                        for(int j=0;j<3;j++)
                        {
                            color[j]+=lights[i].color[j]*coEfficients[1]*lambertValue*(this->color[j]);
                        }
                    }
                    Point R=n*(2*lightRay.dot(n));
                    R=R-lightRay;
                    R.normalize();
                    Point V=r.start-intersectionPoint;
                    V.normalize();
                    double phongValue=V.dot(R);
                    if(phongValue<0)
                    {
                        continue;
                    }
                    else{
                        double powered=phongValue;
                        for(int pow=1;pow<shine;pow++)
                        {
                            powered=powered*phongValue;
                        }
                        for(int j=0;j<3;j++)
                        {
                            color[j]+=lights[i].color[j]*coEfficients[2]*powered*(this->color[j]);
                        }
                    }
                }
                delete dummyColor;
                dummyColor=NULL;
            }
            if(level>=recusionLevel)
            {
                return t;
            }
            Point incidentVector=r.direction*(-1);
            Point reflectedVector=n*(2*incidentVector.dot(n));
            reflectedVector=reflectedVector-incidentVector;
            reflectedVector.normalize();
            Ray reflectedRay(intersectionPoint+(reflectedVector*0.0000001),reflectedVector);
            double *colorReflected = new double[3];
            double tmin2,t2;
            int nearest;
            tmin2=INFINITY;
            nearest=-1;
            for(int o=0;o<objects.size();o++)
            {
                t2=objects[o]->intersect(reflectedRay,colorReflected,0);
                if(t2<tmin2)
                {
                    tmin2=t2;
                    nearest=o;
                }
            }
            if(nearest>0)
            {
                tmin2=objects[nearest]->intersect(reflectedRay,colorReflected,level+1);
                color[0]+=(coEfficients[3]*colorReflected[0]);
                color[1]+=(coEfficients[3]*colorReflected[1]);
                color[2]+=(coEfficients[3]*colorReflected[2]);
            }
            delete colorReflected;
            colorReflected=NULL;
            if(refraction)
            {
                double NdotI=n.dot(r.direction);
                double k=1-1.0*1.0*(1-NdotI*NdotI);
                if(k>0)
                {
                    Point refractedVector=(r.direction*1.0)-(n*(1.0*NdotI+sqrt(k)));
                    refractedVector.normalize();
                    Ray refractedRay(intersectionPoint+(refractedVector*0.0000001),refractedVector);
                    double *colorReflected = new double[3];
                    double tmin2,t2;
                    int nearest;
                    tmin2=INFINITY;
                    nearest=-1;
                    for(int o=0;o<objects.size();o++)
                    {
                        t2=objects[o]->intersect(refractedRay,colorReflected,0);
                        if(t2<tmin2)
                        {
                            tmin2=t2;
                            nearest=o;
                        }
                    }
                    if(nearest>0)
                    {
                        tmin2=objects[nearest]->intersect(refractedRay,colorReflected,level+1);
                        color[0]+=((coEfficients[3]/2)*colorReflected[0]);
                        color[1]+=((coEfficients[3]/2)*colorReflected[1]);
                        color[2]+=((coEfficients[3]/2)*colorReflected[2]);
                    }
                    delete colorReflected;
                    colorReflected=NULL;
                }
            }
            return t;
        }
        else
        {
            return INFINITY;
        }

    }
};
class General:public Object{
public:
    double A,B,C,D,E,F,G,H,I,J;
    General(double a,double b,double c,double d,double e,double f,double g,double h,double i,double j)
    {
        A=a;
        B=b;
        C=c;
        D=d;
        E=e;
        F=f;
        G=g;
        H=h;
        I=i;
        J=j;
    }
    void draw(){
        return;
    }
    double intersect(Ray r, double *color, int level)
    {
        double a,b,c,t1,t2,determinant,tmin=INFINITY;
        a=A*r.direction.x*r.direction.x+B*r.direction.y*r.direction.y+C*r.direction.z*r.direction.z+D*r.direction.x*r.direction.y+E*r.direction.y*r.direction.z+F*r.direction.z*r.direction.x;
        b=2*(A*r.start.x*r.direction.x+B*r.start.y*r.direction.y+C*r.start.z*r.direction.z)+D*(r.start.x*r.direction.y+r.start.y*r.direction.x)+E*(r.start.y*r.direction.z+r.start.z*r.direction.y)+F*(r.start.x*r.direction.z+r.start.z*r.direction.x)+G*r.direction.x+H*r.direction.y+I*r.direction.z;
        c=A*r.start.x*r.start.x+B*r.start.y*r.start.y+C*r.start.z*r.start.z+D*r.start.x*r.start.y+E*r.start.z*r.start.y+F*r.start.x*r.start.z+G*r.start.x+H*r.start.y+I*r.start.z+J;
        determinant=b*b-4*a*c;
        if(determinant<0)
        {
            return INFINITY;
        }
        determinant=sqrt(determinant);
        t1=(-b+determinant)/(2*a);
        t2=(-b-determinant)/(2*a);
        if(t1<0&&t2<0)
        {
            return INFINITY;
        }
        if(t1<0||(t1>=t2&&t2>0))
        {
            Point p=r.start+(r.direction*t2);
            p=p-reference_point;
            bool flag=true;
            if(length!=0)
            {
                if(p.x<=0||p.x>length)
                {
                    flag=false;
                }

            }
            if(width!=0)
            {
                if(p.y<=0||p.y>width)
                {
                    flag=false;
                }
            }
            if(height!=0)
            {
                if(p.z<=0||p.z>height)
                {
                    flag=false;
                }
            }
            if(flag)
            {
                tmin=t2;
            }

        }
        if((t2<0||(t1<=t2&&t1>0))&&tmin==INFINITY)
        {
            Point p=r.start+(r.direction*t1);
            p=p-reference_point;
            bool flag=true;
            if(length!=0)
            {
                if(p.x<=0||p.x>length)
                {
                    flag=false;
                }

            }
            if(width!=0)
            {
                if(p.y<=0||p.y>width)
                {
                    flag=false;
                }
            }
            if(height!=0)
            {
                if(p.z<=0||p.z>height)
                {
                    flag=false;
                }
            }
            if(flag)
            {
                tmin=t1;
            }
        }
        if((t1>=0&&t1>t2)&&tmin==INFINITY)
        {
            Point p=r.start+(r.direction*t1);
            p=p-reference_point;
            bool flag=true;
            if(length!=0)
            {
                if(p.x<=0||p.x>length)
                {
                    flag=false;
                }

            }
            if(width!=0)
            {
                if(p.y<=0||p.y>width)
                {
                    flag=false;
                }
            }
            if(height!=0)
            {
                if(p.z<=0||p.z>height)
                {
                    flag=false;
                }
            }
            if(flag)
            {
                tmin=t1;
            }
        }
        if((t2>=0&&t1<t2)&&tmin==INFINITY)
        {
            Point p=r.start+(r.direction*t2);
            p=p-reference_point;
            bool flag=true;
            if(length!=0)
            {
                if(p.x<=0||p.x>length)
                {
                    flag=false;
                }

            }
            if(width!=0)
            {
                if(p.y<=0||p.y>width)
                {
                    flag=false;
                }
            }
            if(height!=0)
            {
                if(p.z<=0||p.z>height)
                {
                    flag=false;
                }
            }
            if(flag)
            {
                tmin=t2;
            }
        }
        if(level==0)
        {
            return tmin;
        }
        Point intersectionPoint=r.start+r.direction*tmin;
        for(int i=0;i<3;i++)
        {
            color[i]=this->color[i]*coEfficients[0];
        }
        Point normal;
        normal.x=2*A*intersectionPoint.x+D*intersectionPoint.y+E*intersectionPoint.z+G;
        normal.y=2*B*intersectionPoint.y+D*intersectionPoint.x+F*intersectionPoint.z+H;
        normal.z=2*C*intersectionPoint.z+E*intersectionPoint.x+F*intersectionPoint.y+I;
        normal.normalize();
        if(normal.dot(r.direction)>0)
        {
            normal=normal*(-1);
        }
        for(int i=0;i<lights.size();i++)
        {
            bool inShadow=false;;
            double *dummyColor = new double[3];
            Point raydir=lights[i].light_pos-intersectionPoint;
            double distance=raydir.mod();
            raydir.normalize();
            Ray ray(intersectionPoint+(raydir*0.0000001),raydir);
            double t;
            for(int j=0;j<objects.size();j++)
            {
                t=objects[j]->intersect(ray,dummyColor,0);
                if(t<distance && t>0)
                {
                    inShadow=true;
                    break;
                }

            }
            if(!inShadow)
            {
                double lambertValue=normal.dot(raydir);
                if(lambertValue<0)
                {
                    continue;
                }
                else{
                    for(int j=0;j<3;j++)
                    {
                        color[j]+=lights[i].color[j]*coEfficients[1]*lambertValue*(this->color[j]);
                    }
                }
                Point R=normal*(2*raydir.dot(normal));
                R=R-raydir;
                R.normalize();
                Point V=r.start-intersectionPoint;
                V.normalize();
                double phongValue=V.dot(R);
                if(phongValue<0)
                {
                    continue;
                }
                else{
                    double powered=phongValue;
                    for(int pow=1;pow<shine;pow++)
                    {
                        powered=powered*phongValue;
                    }
                    for(int j=0;j<3;j++)
                    {
                        color[j]+=lights[i].color[j]*coEfficients[2]*powered*(this->color[j]);
                    }
                }

            }
            delete dummyColor;
            dummyColor=NULL;
        }
        if(level>=recusionLevel)
        {
            return tmin;
        }
        Point incidentVector=r.direction*(-1);
        Point reflectedVector=normal*(2*incidentVector.dot(normal));
        reflectedVector=reflectedVector-incidentVector;
        reflectedVector.normalize();
        Ray reflectedRay(intersectionPoint+(reflectedVector*0.0000001),reflectedVector);
        double *colorReflected = new double[3];
        double tmin2;
        int nearest;
        tmin2=INFINITY;
        nearest=-1;
        for(int o=0;o<objects.size();o++)
        {
            t2=objects[o]->intersect(reflectedRay,colorReflected,0);
            if(t2<tmin2)
            {
                tmin2=t2;
                nearest=o;
            }
        }
        if(nearest>0)
        {
            tmin2=objects[nearest]->intersect(reflectedRay,colorReflected,level+1);
            color[0]+=(coEfficients[3]*colorReflected[0]);
            color[1]+=(coEfficients[3]*colorReflected[1]);
            color[2]+=(coEfficients[3]*colorReflected[2]);
        }
        delete colorReflected;
        colorReflected=NULL;
        if(refraction)
        {
            double NdotI=normal.dot(r.direction);
            double k=1-1.0*1.0*(1-NdotI*NdotI);
            if(k>0)
            {
                Point refractedVector=(r.direction*1.0)-(normal*(1.0*NdotI+sqrt(k)));
                refractedVector.normalize();
                Ray refractedRay(intersectionPoint+(refractedVector*0.0000001),refractedVector);
                double *colorReflected = new double[3];
                double tmin2,t2;
                int nearest;
                tmin2=INFINITY;
                nearest=-1;
                for(int o=0;o<objects.size();o++)
                {
                    t2=objects[o]->intersect(refractedRay,colorReflected,0);
                    if(t2<tmin2)
                    {
                        tmin2=t2;
                        nearest=o;
                    }
                }
                if(nearest>0)
                {
                    tmin2=objects[nearest]->intersect(refractedRay,colorReflected,level+1);
                    color[0]+=((coEfficients[3]/2)*colorReflected[0]);
                    color[1]+=((coEfficients[3]/2)*colorReflected[1]);
                    color[2]+=((coEfficients[3]/2)*colorReflected[2]);
                }
                delete colorReflected;
                colorReflected=NULL;
            }
        }
        return tmin;
    }

};


