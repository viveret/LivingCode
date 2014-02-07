#include <math.h>
#include "myMath.h"


Point2D::Point2D()
{
	this->Set(0, 0);
}

Point2D::Point2D(u_int New_X, u_int New_Y)
{
	this->Set(New_X, New_Y);
}

void Point2D::Set(u_int New_X, u_int New_Y)
{
	this->x = New_X;
	this->y = New_Y;
}



float DistForm2d(float x1,float y1,float x2,float y2)
{
    float A=x1-x2;A=A*A;
    float B=y1-y2;B=B*B;
    float C=A+B;
    return sqrt(C);
}

float Distform2d(Vector2 A,Vector2 B)
{
	return DistForm2d(A.x,A.y,B.x,B.y);
}

float DistForm3d(float x1,float y1,float z1,float x2,float y2,float z2)
{
    float A=x1-x2;A=A*A;
    float B=y1-y2;B=B*B;
    float C=z1-z2;C=C*C;
    float D=A+B+C;
    return sqrt(D);
}


bool Is_In_Box(float boxx,float boxy,float boxw,float boxh,float mx,float my)
{
     if((mx>=boxx) && (mx<=boxx+boxw) && (my>=boxy) && (my<=boxy+boxh))
     {
         return true;
     }
     else{return false;}
}

bool Is_In_Box(int boxx,int boxy,int boxw,int boxh,int mx,int my)
{
	return Is_In_Box(float(boxx),float(boxy),float(boxw),float(boxh),float(mx),float(my));
}

int IntToBarPos(int h)
{
    float Ratio=200/100;
    return (int)(h*Ratio);
}



void Vector2::Clear()
{
	this->x=0;
	this->y=0;
}

void Vector2::Normalize()
{
	float Mag=this->Magnitude();

	if( Mag != 0.f )
	{				 
		this->x/=Mag;
		this->y/=Mag;
	}
}

float Vector2::Magnitude()
{
	return DistForm2d(0,0,this->x,this->y);
}

void Vector3::Clear()
{				   
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
}

void Vector3::Normalize()
{		  
	float Mag=this->Magnitude();

	if( Mag != 0.f )
	{				 
		this->x/=Mag;
		this->y/=Mag;
		this->z/=Mag;
	}
}

float Vector3::Magnitude()
{
	return 1.f;
}

float Dot_Product(Vector2 A,Vector2 B)
{
	A.Normalize();B.Normalize();
	float nProduct=A.x*B.x + A.y*B.y;
	return nProduct;
}


float Normalize(float x,float y,float z,char Which)
{
    float Length=sqrtf(x*x+y*y+z*z);

    float Ret=0.0f;

    switch(Which)
    {
        case 'x':Ret=x/Length;break;
        case 'y':Ret=y/Length;break;
        case 'z':Ret=z/Length;break;
        default:break;
    }
    return Ret;
}

float ObjectCollides(int ID)
{
    switch(ID)
    {
        case 0:return 1.0f;break;//air
        case 1:return 0.0f;break;//dirt
        case 2:return 0.0f;break;//grass
        case 3:return 0.0f;break;//wood
        case 4:return 1.0f;break;//leaf
        case 5:return 0.0f;break;//brick
        case 6:return 0.0f;break;//concrete
        case 7:return 0.0f;break;//road NS
        case 8:return 0.0f;break;//road WE
        case 9:return 0.0f;break;//sidewalk
        case 10:return 0.0f;break;//sand
        case 11:return 0.0f;break;//wall line
        case 12:return 0.0f;break;//wall window
        case 13:return 0.0f;break;//wall
        case 14:return 0.3f;break;//water
        case 15:return 0.0f;break;//deep water
        default:return 0.0f;break;//default
    }
    return 1;
}

int RoundToUnit(float n)
{
    int going;
    if(n<0){going=-1;}
    else if(n>0){going=1;}
    else{going=0;}
    return going;
}				 

void Clamp_Angle(float* Angle)//clamp between 0 and 360
{
	while(*Angle>360)
		*Angle-=360;

	while(*Angle<0)
		*Angle+=360;
}

float Clamp_Angle(float Angle)//clamp between 0 and 360
{
	Clamp_Angle(&Angle);
	return Angle;
}

int log2(float n)
{
	return (int)(log(n)/log(2.0));
}								 


bool Check_Flag(int flags,int flag)
{
	if((flags>>log2((float)flag))&1)
		return true;

	return false;
}



float As_Float( u_char Byte )
{
	return (float)Byte/255;
}