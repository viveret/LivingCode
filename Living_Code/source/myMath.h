#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <math.h>
#include <cmath>

#include "Core.h"


struct Point2D
{
	Point2D();
	Point2D(u_int New_X, u_int New_Y);

	void Set(u_int New_X, u_int New_Y);
	u_int x, y;
};


struct Vector2
{
	float x,y;
	Vector2(){x=0.f;y=0.f;}
	Vector2(float nx,float ny){x=nx;y=ny;}
	void Clear();
	void Normalize();
	float Magnitude();
};		

struct Vector3
{
	float x,y,z;
	Vector3(){x=0.f;y=0.f;z=0.f;}
	Vector3(float nx,float ny,float nz){x=nx;y=ny;z=nz;}
	void Set(float nx,float ny,float nz){x=nx;y=ny;z=nz;}
	void Clear();
	void Normalize();
	float Magnitude();
};

float Dot_Product(Vector2 A,Vector2 B);

struct RECTID
{
    float x,y,l,w,Val;
    int ID;
};


float Normalize(float x,float y,float z,char Which);

float ObjectCollides(int ID);

class NearObj
{
    public:
    int top,bottom,right,left,front,back;
    NearObj(int atop,int abottom,int aright,int aleft,int afront,int aback)
    {
        NearObj::top=atop;
        NearObj::bottom=abottom;
        NearObj::right=aright;
        NearObj::left=aleft;
        NearObj::front=afront;
        NearObj::back=aback;
    }
};

class Box
{
    public:
    int x,w,y,h;
};

int RoundToUnit(float n);								  
void Clamp_Angle(float* Angle);//clamp between 0 and 360
float Clamp_Angle(float Angle);//clamp between 0 and 360

int log2(float n);
								 

float DistForm2d(float x1,float y1,float x2,float y2);
float DistForm2d(Vector2 A,Vector2 B);

bool Is_In_Box(float boxx,float boxy,float boxw,float boxh,float mx,float my);
bool Is_In_Box(int boxx,int boxy,int boxw,int boxh,int mx,int my);

int IntToBarPos(int h);


bool Check_Flag(int flags,int flag);


float As_Float( u_char Byte );


#endif // MATH_H_INCLUDED
