#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<stack>
#include<cmath>
#include<time.h>
#include <stdlib.h>
#include "bitmap_image.hpp"
#define pi 3.14159265358979323846264338327950288419716939937
#define THRESHOLD 0.000000001

using namespace std;
ofstream ofs("stage1.txt",ofstream::out);

class Vector{
public:
    double x,y,z,w;
    Vector(double x,double y,double z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
        w=1;
    }
    Vector()
    {
        x=0;
        y=0;
        z=0;
        w=1;
    }
    double dot(Vector v)
    {
        return x*v.x+y*v.y+z*v.z;
    }
    void normalize()
    {
        double mod=sqrt(x*x+y*y+z*z);
        x=x/mod;
        y=y/mod;
        z=z/mod;
    }
    void transformPoint(double m[4][4])
    {
        double tx,ty,tz,tw;

        tx=x*m[0][0]+y*m[0][1]+z*m[0][2]+w*m[0][3];
        ty=x*m[1][0]+y*m[1][1]+z*m[1][2]+w*m[1][3];
        tz=x*m[2][0]+y*m[2][1]+z*m[2][2]+w*m[2][3];
        tw=x*m[3][0]+y*m[3][1]+z*m[3][2]+w*m[3][3];
        x=tx/tw;
        y=ty/tw;
        z=tz/tw;
        w=1;


    }
    void printVector()
    {
        double px=x,py=y,pz=z;
        if(abs(px)<THRESHOLD)
        {
            px=0;
        }
        if(abs(py)<THRESHOLD)
        {
            py=0;
        }
        if(abs(pz)<THRESHOLD)
        {
            pz=0;
        }
        ofs<<fixed<<setprecision(7)<<px<<" "<<py<<" "<<pz<<endl;
    }
    Vector operator+(Vector v)
    {
        Vector r;
        r.x=x+v.x;
        r.y=y+v.y;
        r.z=z+v.z;
        return r;
    }
    Vector operator-(Vector v)
    {
        Vector r;
        r.x=x-v.x;
        r.y=y-v.y;
        r.z=z-v.z;
        return r;
    }
    Vector operator*(double c)
    {
        Vector r;
        r.x=x*c;
        r.y=y*c;
        r.z=z*c;
        return r;

    }
    Vector operator*(Vector v)
    {
        Vector r;
        r.x=y*v.z-z*v.y;
        r.y=z*v.x-x*v.z;
        r.z=x*v.y-y*v.x;
        return r;
    }

};
class Matrix{
public:
    double mat[4][4];
    void setTranslate(double tx,double ty,double tz)
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(i!=j)
                {
                    mat[i][j]=0;
                }
                else
                {
                    mat[i][j]=1;
                }

            }
        }
        mat[0][3]=tx;
        mat[1][3]=ty;
        mat[2][3]=tz;

    }
    void setScale(double sx,double sy,double sz)
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                mat[i][j]=0;

            }
        }
        mat[0][0]=sx;
        mat[1][1]=sy;
        mat[2][2]=sz;
        mat[3][3]=1;

    }
    void setRotate(double angle,double ax,double ay,double az)
    {
        Vector a(ax,ay,az);
        a.normalize();
        Vector i(1,0,0),j(0,1,0),k(0,0,1);
        angle=angle*pi/180;
        Vector c1=(i*cos(angle))+(a*((1-cos(angle))*(i.dot(a))))+((a*i)*sin(angle));
        Vector c2=(j*cos(angle))+(a*((1-cos(angle))*(j.dot(a))))+((a*j)*sin(angle));
        Vector c3=(k*cos(angle))+(a*((1-cos(angle))*(k.dot(a))))+((a*k)*sin(angle));
        mat[0][0]=c1.x;
        mat[1][0]=c1.y;
        mat[2][0]=c1.z;
        mat[0][1]=c2.x;
        mat[1][1]=c2.y;
        mat[2][1]=c2.z;
        mat[0][2]=c3.x;
        mat[1][2]=c3.y;
        mat[2][2]=c3.z;
        for(int m=0;m<4;m++)
        {
            mat[m][3]=0;
            mat[3][m]=0;
        }
        mat[3][3]=1;
    }
    void setIdentity()
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(i!=j)
                {
                    mat[i][j]=0;
                }
                else
                {
                    mat[i][j]=1;
                }
            }
        }
    }
    void setZero()
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                 mat[i][j]=0;
            }
        }

    }
    void printMatrix()
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                cout<<mat[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    Matrix operator*(Matrix m)
    {
        Matrix r;
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                r.mat[i][j]=0;
                for(int k=0;k<4;k++)
                {
                    r.mat[i][j]+=mat[i][k]*m.mat[k][j];
                }
            }
        }
        return r;
    }
};
class Triangle{
public:
    Vector points[3];
    int r,g,b;
};
using namespace std;
main()
{
    int triangleCount=0;
    double eyeX,eyeY,eyeZ,lookX,lookY,lookZ,upX,upY,upZ,fovY,aspectRatio,near,far;
    ifstream ifs ("scene.txt", ifstream::in);
    ifs>>eyeX>>eyeY>>eyeZ>>lookX>>lookY>>lookZ>>upX>>upY>>upZ>>fovY>>aspectRatio>>near>>far;
    Matrix I;
    I.setIdentity();
    stack<Matrix> stateStack;
    stateStack.push(I);
    string command;
    while(true)
    {
        ifs>>command;
        if(command.compare("end")==0)
        {
            break;
        }
        else if(command.compare("triangle")==0)
        {
            triangleCount++;
            Vector p1,p2,p3;
            ifs>>p1.x>>p1.y>>p1.z>>p2.x>>p2.y>>p2.z>>p3.x>>p3.y>>p3.z;
            p1.transformPoint(stateStack.top().mat);
            p2.transformPoint(stateStack.top().mat);
            p3.transformPoint(stateStack.top().mat);
            p1.printVector();
            p2.printVector();
            p3.printVector();
            ofs<<endl;
        }
        else if(command.compare("translate")==0)
        {
            double tx,ty,tz;
            ifs>>tx>>ty>>tz;
            Matrix translate;
            translate.setTranslate(tx,ty,tz);
            Matrix s=stateStack.top();
            stateStack.pop();
            stateStack.push(s*translate);
        }
        else if(command.compare("scale")==0)
        {
            double sx,sy,sz;
            ifs>>sx>>sy>>sz;
            Matrix scale;
            scale.setScale(sx,sy,sz);
            Matrix s=stateStack.top();
            stateStack.pop();
            stateStack.push(s*scale);

        }
        else if(command.compare("rotate")==0)
        {
            double angle,ax,ay,az;
            ifs>>angle>>ax>>ay>>az;
            Matrix rot;
            rot.setRotate(angle,ax,ay,az);
            Matrix s=stateStack.top();
            stateStack.pop();
            stateStack.push(s*rot);
        }
        else if(command.compare("push")==0)
        {
            Matrix s=stateStack.top();
            stateStack.push(s);
        }
        else if(command.compare("pop")==0)
        {
            if(stateStack.size()>1){
                stateStack.pop();
            }
        }

    }
    ifs.close();
    ofs.close();
    Vector l(lookX-eyeX,lookY-eyeY,lookZ-eyeZ);
    l.normalize();
    Vector up(upX,upY,upZ);
    Vector r=l*up;
    r.normalize();
    Vector u=r*l;
    Matrix T,R;
    T.setTranslate(-eyeX,-eyeY,-eyeZ);
    R.mat[0][0]=r.x;
    R.mat[0][1]=r.y;
    R.mat[0][2]=r.z;
    R.mat[0][3]=0;
    R.mat[1][0]=u.x;
    R.mat[1][1]=u.y;
    R.mat[1][2]=u.z;
    R.mat[1][3]=0;
    R.mat[2][0]=-l.x;
    R.mat[2][1]=-l.y;
    R.mat[2][2]=-l.z;
    R.mat[2][3]=0;
    R.mat[3][0]=0;
    R.mat[3][1]=0;
    R.mat[3][2]=0;
    R.mat[3][3]=1;
    Matrix V=R*T;
    ifs.open("stage1.txt",ifstream::in);
    ofs.open("stage2.txt",ofstream::out);
    Vector v;
    for(int i=0;i<triangleCount;i++)
    {
        ifs>>v.x>>v.y>>v.z;
        v.transformPoint(V.mat);
        ofs<<fixed<<setprecision(7)<<v.x<<" "<<v.y<<" "<<v.z<<endl;
        ifs>>v.x>>v.y>>v.z;
        v.transformPoint(V.mat);
        ofs<<fixed<<setprecision(7)<<v.x<<" "<<v.y<<" "<<v.z<<endl;
        ifs>>v.x>>v.y>>v.z;
        v.transformPoint(V.mat);
        ofs<<fixed<<setprecision(7)<<v.x<<" "<<v.y<<" "<<v.z<<endl<<endl;
    }
    ifs.close();
    ofs.close();
    double r1,t,fovX;
    fovX = fovY*aspectRatio;
    t = near*tan((fovY/2)*pi/180);
    r1 = near*tan((fovX/2)*pi/180);
    Matrix P;
    P.setZero();
    P.mat[0][0]=near/r1;
    P.mat[1][1]=near/t;
    P.mat[2][2]=(-(far+near))/(far-near);
    P.mat[2][3]=(-(2*far*near))/(far-near);
    P.mat[3][2]=-1;
    ifs.open("stage2.txt",ifstream::in);
    ofs.open("stage3.txt",ofstream::out);
    for(int i=0;i<triangleCount;i++)
    {
        ifs>>v.x>>v.y>>v.z;
        v.transformPoint(P.mat);
        ofs<<fixed<<setprecision(7)<<v.x<<" "<<v.y<<" "<<v.z<<endl;
        ifs>>v.x>>v.y>>v.z;
        v.transformPoint(P.mat);
        ofs<<fixed<<setprecision(7)<<v.x<<" "<<v.y<<" "<<v.z<<endl;
        ifs>>v.x>>v.y>>v.z;
        v.transformPoint(P.mat);
        ofs<<fixed<<setprecision(7)<<v.x<<" "<<v.y<<" "<<v.z<<endl<<endl;
    }
    ifs.close();
    ofs.close();

    //Z-Buffer part starts
    int Screen_Width,Screen_Height;
    double lx,by,fz,rz;
    ifs.open("config.txt",ifstream::in);
    ifs>>Screen_Width>>Screen_Height>>lx>>by>>fz>>rz;
    ifs.close();
    ifs.open("stage3.txt",ifstream::in);
    Triangle triangles[triangleCount];
    for(int i=0;i<triangleCount;i++)
    {
        ifs>>triangles[i].points[0].x>>triangles[i].points[0].y>>triangles[i].points[0].z;
        ifs>>triangles[i].points[1].x>>triangles[i].points[1].y>>triangles[i].points[1].z;
        ifs>>triangles[i].points[2].x>>triangles[i].points[2].y>>triangles[i].points[2].z;
        triangles[i].r=rand()%256;
        triangles[i].g=rand()%256;
        triangles[i].b=rand()%256;
    }
    ifs.close();
    double dx,dy,Top_Y,Bottom_Y,Left_X,Right_X;
    dx=(-lx-lx)/Screen_Width;
    dy=(-by-by)/Screen_Height;
    Top_Y=-by-dy/2;
    Bottom_Y=by+dy/2;
    Right_X=-lx-dx/2;
    Left_X=lx+dx/2;
    double **z_buffer = new double*[Screen_Width];
    for(int i=0;i<Screen_Width;i++)
    {
        z_buffer[i]=new double[Screen_Height];
        for(int j=0;j<Screen_Height;j++)
        {
            z_buffer[i][j]=rz;
        }
    }
    bitmap_image image(Screen_Width,Screen_Height);
    for(int i=0;i<Screen_Width;i++)
    {
        for(int j=0;j<Screen_Height;j++)
        {
            image.set_pixel(i,j,0,0,0);
        }
    }
    for(int i=0;i<triangleCount;i++)
    {
        int firstY,secondY,thirdY;
        double top_scanline,bottom_scanline,min_y,max_y;
        min_y=triangles[i].points[0].y;
        max_y=triangles[i].points[0].y;
        if(triangles[i].points[1].y<triangles[i].points[0].y)
        {
            min_y=triangles[i].points[1].y;
            thirdY=1;
            firstY=0;
        }
        else
        {
            max_y=triangles[i].points[1].y;
            thirdY=0;
            firstY=1;
        }
        secondY=2;
        if(triangles[i].points[2].y<min_y)
        {
            min_y=triangles[i].points[2].y;
            secondY=thirdY;
            thirdY=2;
        }
        if(triangles[i].points[2].y>max_y)
        {
            max_y=triangles[i].points[2].y;
            secondY=firstY;
            firstY=2;
        }
        if(max_y<by)
        {
            continue;
        }
        if(min_y>-by)
        {
            continue;
        }
        if(max_y>-by)
        {
            max_y=Top_Y;
        }
        if(min_y<by)
        {
            min_y=Bottom_Y;
        }
        top_scanline=round((Top_Y-max_y)/dy);
        bottom_scanline=round((Top_Y-min_y)/dy);
        for(int row=top_scanline;row<=bottom_scanline;row++)
        {
            int left_intersecting_column,right_intersecting_column;
            double ys=Top_Y-(row*dy);
            double xa,xb,za,zb;
            bool flag=false;
            if(ys<=triangles[i].points[firstY].y&&ys>=triangles[i].points[secondY].y)
            {
                if(triangles[i].points[firstY].y==triangles[i].points[secondY].y)
                {
                    if(row==round((Top_Y-triangles[i].points[firstY].y)/dy))
                    {
                        flag=true;
                        if(triangles[i].points[firstY].x>triangles[i].points[secondY].x)
                        {
                            xb=triangles[i].points[firstY].x;
                            zb=triangles[i].points[firstY].z;
                            xa=triangles[i].points[secondY].x;
                            za=triangles[i].points[secondY].z;

                        }
                        else
                        {
                            left_intersecting_column=round((triangles[i].points[firstY].x-Left_X)/dx);
                            xa=triangles[i].points[firstY].x;
                            za=triangles[i].points[firstY].z;
                            right_intersecting_column=round((triangles[i].points[secondY].x-Left_X)/dx);
                            xb=triangles[i].points[secondY].x;
                            zb=triangles[i].points[secondY].z;
                        }
                    }

                }
                else
                {
                    flag=true;
                    double candidate=triangles[i].points[firstY].x-(triangles[i].points[firstY].x-triangles[i].points[secondY].x)*((triangles[i].points[firstY].y-ys)/(triangles[i].points[firstY].y-triangles[i].points[secondY].y));
                    xa=candidate;
                    xb=candidate;
                    za=triangles[i].points[firstY].z-(triangles[i].points[firstY].z-triangles[i].points[secondY].z)*((triangles[i].points[firstY].y-ys)/(triangles[i].points[firstY].y-triangles[i].points[secondY].y));
                    zb=za;
                }
            }
            if(ys<=triangles[i].points[firstY].y&&ys>=triangles[i].points[thirdY].y)
            {
                if(triangles[i].points[firstY].y==triangles[i].points[thirdY].y)
                {
                    if(row==round((Top_Y-triangles[i].points[firstY].y)/dy))
                    {
                        flag=true;
                        if(triangles[i].points[firstY].x>triangles[i].points[thirdY].x)
                        {
                            xb=triangles[i].points[firstY].x;
                            zb=triangles[i].points[firstY].z;
                            xa=triangles[i].points[thirdY].x;
                            za=triangles[i].points[thirdY].z;
                        }
                        else
                        {
                            xa=triangles[i].points[firstY].x;
                            za=triangles[i].points[firstY].z;
                            xb=triangles[i].points[thirdY].x;
                            zb=triangles[i].points[thirdY].z;
                        }
                    }

                }
                else
                {
                    double candidate=triangles[i].points[firstY].x-(triangles[i].points[firstY].x-triangles[i].points[thirdY].x)*((triangles[i].points[firstY].y-ys)/(triangles[i].points[firstY].y-triangles[i].points[thirdY].y));
                    double cz=triangles[i].points[firstY].z-(triangles[i].points[firstY].z-triangles[i].points[thirdY].z)*((triangles[i].points[firstY].y-ys)/(triangles[i].points[firstY].y-triangles[i].points[thirdY].y));
                    if(!flag)
                    {
                        xa=candidate;
                        xb=candidate;
                        za=cz;
                        zb=cz;
                        flag=true;

                    }
                    else
                    {
                        if(candidate<xa)
                        {
                            xa=candidate;
                            za=cz;
                        }
                        else if(candidate>xb)
                        {
                            xb=candidate;
                            zb=cz;
                        }

                    }


                }
            }
            if(ys<=triangles[i].points[secondY].y&&ys>=triangles[i].points[thirdY].y)
            {
                if(triangles[i].points[secondY].y==triangles[i].points[thirdY].y)
                {
                    if(row==round((Top_Y-triangles[i].points[secondY].y)/dy))
                    {
                        flag=true;
                        if(triangles[i].points[secondY].x>triangles[i].points[thirdY].x)
                        {
                            xb=triangles[i].points[secondY].x;
                            zb=triangles[i].points[secondY].z;
                            xa=triangles[i].points[thirdY].x;
                            za=triangles[i].points[thirdY].z;
                        }
                        else
                        {
                            xa=triangles[i].points[secondY].x;
                            za=triangles[i].points[secondY].z;
                            xb=triangles[i].points[thirdY].x;
                            zb=triangles[i].points[thirdY].z;
                        }
                    }

                }
                else
                {
                    double candidate=triangles[i].points[secondY].x-(triangles[i].points[secondY].x-triangles[i].points[thirdY].x)*((triangles[i].points[secondY].y-ys)/(triangles[i].points[secondY].y-triangles[i].points[thirdY].y));
                    double cz=triangles[i].points[secondY].z-(triangles[i].points[secondY].z-triangles[i].points[thirdY].z)*((triangles[i].points[secondY].y-ys)/(triangles[i].points[secondY].y-triangles[i].points[thirdY].y));
                    if(!flag)
                    {
                        flag=true;
                        xa=candidate;
                        xb=candidate;
                        za=cz;
                        zb=cz;
                    }
                    else
                    {
                        if(candidate<xa)
                        {
                            xa=candidate;
                            za=cz;
                        }
                        else if(candidate>xb)
                        {
                            xb=candidate;
                            zb=cz;
                        }

                    }


                }
            }
            if(flag)
            {
                if(xa<lx)
                {
                    za=zb-(zb-za)*((xb-Left_X)/(xb-xa));
                    xa=Left_X;
                }
                if(xb<lx)
                {
                    continue;
                }
                if(xa>-lx)
                {
                    continue;
                }
                if(xb>-lx)
                {
                    zb=zb-(zb-za)*((xb-Right_X)/(xb-xa));
                    xb=Right_X;
                }
                left_intersecting_column=round((xa-Left_X)/dx);
                right_intersecting_column=round((xb-Left_X)/dx);
                for(int column=left_intersecting_column;column<=right_intersecting_column;column++)
                {
                    double zp,xp;
                    xp=Left_X+column*dx;
                    if(xb==xa)
                    {
                        zp=za;
                    }
                    else
                    {
                        zp=zb-(zb-za)*((xb-xp)/(xb-xa));
                    }
                    if(zp<z_buffer[column][row]&&zp>fz)
                    {

                        z_buffer[column][row]=zp;
                        image.set_pixel(column,row,triangles[i].r,triangles[i].g,triangles[i].b);
                    }

                }

            }

        }


    }
    image.save_image("out.bmp");
    ofs.open("z_buffer.txt",ofstream::out);
    for(int i=0;i<Screen_Width;i++)
    {
        for(int j=0;j<Screen_Height;j++)
        {
            if(z_buffer[j][i]<rz)
            {
                ofs<<fixed<<setprecision(6)<<z_buffer[j][i]<<"\t";
            }
        }
        ofs<<endl;
    }

    for(int i = 0; i <Screen_Width; ++i)
    {
        delete [] z_buffer[i];
    }
    delete [] z_buffer;
}
