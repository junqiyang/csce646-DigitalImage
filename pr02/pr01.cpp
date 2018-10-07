// =============================================================================
// VIZA654/CSCE646 at Texas A&M University
// Homework 0
// Created by Anton Agana based from Ariel Chisholm's template
// 05.23.2011
//
// This file is supplied with an associated makefile. Put both files in the same
// directory, navigate to that directory from the Linux shell, and type 'make'.
// This will create a program called 'pr01' that you can run by entering
// 'homework0' as a command in the shell.
//
// If you are new to programming in Linux, there is an
// excellent introduction to makefile structure and the gcc compiler here:
//
// http://www.cs.txstate.edu/labs/tutorials/tut_docs/Linux_Prog_Environment.pdf
//
// =============================================================================
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<sstream>
#include<algorithm>
#include<string>
#include<limits.h>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>


using namespace std;
// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height;
unsigned char *pixmap;
float I = 0, J = 0;
int r = 0,g = 0,b=0;
int neig = 4;

// =============================================================================
// setPixels()
//
// This function stores the RGB values of each pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void setPixels()
{
   for(int y = 0; y < height ; y++) {
     for(int x = 0; x < width; x++) {
       int i = (y * width + x) * 3; 
       pixmap[i++] = 255;
       pixmap[i++] = 0xFF; //Do you know what "0xFF" represents? Google it!
       pixmap[i] = 0x00; //Learn to use the "0x" notation to your advantage.
     }
   }
}

void save(string filename){
    FILE *output_file = fopen(filename.c_str(), "w");
    fprintf(output_file, "P6\n");
    fprintf(output_file, "%d %d\n", width, height);
    fprintf(output_file, "255\n");   
    
    for(int y = height - 1; y >= 0; y--){
        for(int x = 0; x < width; x++){
            int i = (y * width + x) * 3;
            fputc(pixmap[i++], output_file);
            fputc(pixmap[i++], output_file);
            fputc(pixmap[i], output_file);
        }
      }        
    fclose(output_file);
}

// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================




static void windowResize(int w, int h)
{   
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,(w/2),0,(h/2),0,1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity() ;
}
static void windowDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos2i(0,0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
  glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
  if(state == GLUT_UP)
  exit(0);               // Exit on mouse click.
}
static void init(void)
{
  glClearColor(1,1,1,1); // Set background color.
}






class Point2f{
    public:
    float x, y;
    Point2f() {}
    Point2f( float _x, float _y ) { x=_x; y=_y; }
    Point2f operator+( const Point2f &pt ) const { return Point2f(x+pt.x, y+pt.y); }    
    Point2f operator-( const Point2f &pt ) const { return Point2f(x-pt.x, y-pt.y); }
    float Dot	   ( const Point2f &pt ) const { return x*pt.x + y*pt.y; }	///< Dot product
	float operator%( const Point2f &pt ) const { return Dot(pt); }
};

class Point3f{
    public:
    float x, y, z;
    Point3f() {}
    Point3f( float _x, float _y, float _z ) { x=_x; y=_y; z=_z;}
    Point3f operator+( const Point3f &pt ) const { return Point3f(x+pt.x, y+pt.y, z+pt.z); }
    Point3f operator-( const Point3f &pt ) const { return Point3f(x-pt.x, y-pt.y, z-pt.z); }
    float	Dot		 ( const Point3f &pt ) const { return x*pt.x + y*pt.y + z*pt.z; }	///< Dot product
	float	operator%( const Point3f &pt ) const { return Dot(pt); }	
 };
 
double distanceBetweenPoints(Point3f A,Point3f B)
{return sqrt((A-B)%(A-B));}

double distanceBetweenPoints(Point2f A,Point2f B)
{return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y));}
 


bool Smaller(Point2f a, Point2f b, Point2f p){
    return ((b.x - a.x)*(p.y - a.y) - (b.y - a.y)*(p.x - a.x)) > 0;
}

bool onedge(Point2f a, Point2f b, Point2f p){
    return ((b.x - a.x)*(p.y - a.y) - (b.y - a.y)*(p.x - a.x)) < 5;
}

class Star{
	public: 
        Point2f A,B,C,D,E;
        Star(){};
        Star(Point2f a, Point2f b, Point2f c, Point2f d,Point2f e){A=a,B=b,C=c,D=d, E=e;};
        bool isInsideStar(Point2f p)
        {
            if( Smaller(A,C,p) + !Smaller(A,D,p) + Smaller(B,D,p) + !Smaller(B,E,p) + !Smaller(E,C,p) >= 4){return true;}
            return false;
        }
        
        void draw(){
            for(int y = 0; y < height ; y++) {
                for(int x = 0; x< width; x++) {
                    int i = (y * width + x) * 3;            
                    double rx = (float)((float)(rand()%100)/(100.0));
                    double ry = (float)((float)(rand()%100)/(100.0));
                    r = 0; b = 0; g = 0;
                    for (int p = 0; p < neig; p++) {
                        for (int q = 0; q < neig; q++)	{
                            I=x+(((float)p/neig)+((rx)/neig));
                            J=y+(((float)q/neig)+((ry)/neig));
				      	if(isInsideStar(Point2f(I,J))){ r += 255; b += 0;g += 0;}
				      	else {r += 255; b += 0;g += 255;}
				    }
				}
            r=r/neig*neig;b/=neig*neig;g/=neig*neig;
			pixmap[i++] = r;
			pixmap[i++] = g; 
			pixmap[i] = b; 
                }
            }            
        }
        
        
};

class Convex{
    public:
        Point2f A,B,C,D;
        Convex(){};
    	Convex(Point2f a, Point2f b, Point2f c, Point2f d){A=a,B=b,C=c,D=d;};
        
     	bool isInsideConvex(Point2f p)
        {
            if( !Smaller(A,C,p) && !Smaller(C,D,p) && Smaller(B,D,p) && Smaller(A,B,p) ){return true;}
            return false;
        }
        
        void draw(){
            
            for(int y = 0; y < height ; y++) {
                for(int x = 0; x< width; x++) {
                    int i = (y * width + x) * 3;            
                    double rx = (float)((float)(rand()%100)/(100.0));
                    double ry = (float)((float)(rand()%100)/(100.0));
                    r = 0; b = 0; g = 0;
                    for (int p = 0; p < neig; p++) {
                        for (int q = 0; q < neig; q++)	{
                            I=x+(((float)p/neig)+((rx)/neig));
                            J=y+(((float)q/neig)+((ry)/neig));
				      	if(isInsideConvex(Point2f(I,J))){
                            r+=60; b+=120;g+= 250;
                        }
				      	else {
                            r += 90; b += 120;g+=60;
                        }
				    }
				}
            r=r/neig*neig;b/=neig*neig;g/=neig*neig;
			pixmap[i++] = r;
			pixmap[i++] = g; 
			pixmap[i] = b; 
                }
            }            
        }
        
};

class Blobby{
    public:
    vector<Point2f> point;
    double R, L;
    Blobby(){
            point.push_back(Point2f(300,300));
			point.push_back(Point2f(300,320));
			point.push_back(Point2f(360,360));
			point.push_back(Point2f(340,340));
			point.push_back(Point2f(280,280));
			point.push_back(Point2f(250,320));
			point.push_back(Point2f(200,300));
			point.push_back(Point2f(250,280));
 			point.push_back(Point2f(232,250));
			point.push_back(Point2f(280,270));
			point.push_back(Point2f(250,300));        
			point.push_back(Point2f(340,250));        
			point.push_back(Point2f(340,240));                    
			R = 50, L = 0.15;    
    }
    bool isInsideBlob(Point2f p)
	{
		return F(p)>0;
	}
	double F(Point2f p)
	{
		double dist = 0, r=0;
		for(int i = 0;i<point.size();i++)
		{
			r = sqrt((point[i]-p)%(point[i]-p));
			if(r < R)
				dist += (1-3*(r/R)*(r/R) +3*pow((r/R),4) - pow(r/R,6)); 
			else
				dist+=0;
		}
		return dist - L;
	}
    
    void draw(){
            
            for(int y = 0; y < height ; y++) {
                for(int x = 0; x< width; x++) {
                    int i = (y * width + x) * 3;            
                    double rx = (float)((float)(rand()%100)/(100.0));
                    double ry = (float)((float)(rand()%100)/(100.0));
                    r = 0; b = 0; g = 0;
                    for (int p = 0; p < neig; p++) {
                        for (int q = 0; q < neig; q++)	{
                            I=x+(((float)p/neig)+((rx)/neig));
                            J=y+(((float)q/neig)+((ry)/neig));
				      	if(isInsideBlob(Point2f(I,J))){
                            r+=0; b+= 60;g+=255;
                        }
				      	else {
                            r += 0; b += 0;g+=0;
                        }
				    }
				}
            r=r/neig*neig;b/=neig*neig;g/=neig*neig;
			pixmap[i++] = r;
			pixmap[i++] = g; 
			pixmap[i] = b; 
                }
            }            
        }
};

class Functions{
    public:    
    bool functionSine(Point2f p)
    {
        return (p.y > 300 + 100*sin(p.x/30));
    }
    
    void draw(){
        for(int y = 0; y < height ; y++) {
                for(int x = 0; x< width; x++) {
                    int i = (y * width + x) * 3;            
                    double rx = (float)((float)(rand()%100)/(100.0));
                    double ry = (float)((float)(rand()%100)/(100.0));
                    r = 0; b = 0; g = 0;
                    for (int p = 0; p < neig; p++) {
                        for (int q = 0; q < neig; q++)	{
                            I=x+(((float)p/neig)+((rx)/neig));
                            J=y+(((float)q/neig)+((ry)/neig));
				      	if(functionSine( Point2f(I,J) )){
                            r+=0; b+= 0;g+=0;
                        }
				      	else {
                            r += 0; b +=255;g+=0;
                        }
				    }
				}
            r=r/neig*neig;b/=neig*neig;g/=neig*neig;
			pixmap[i++] = r;
			pixmap[i++] = g; 
			pixmap[i] = b; 
                }
            }
    }
};

class Circle{
	public:
	double radius;
    Point2f center,specularPoint;
	Point3f specularColor, color;
    Circle(){}
	Circle(Point2f c, double rad,Point3f col)
	{
		radius= rad;
		center=c;
		color=col;
	}

	void setSpecularPoint(Point2f p,Point3f q= Point3f(1,1,1))
	{
		specularPoint = p;
		specularColor = q;
	}	
	bool isPointInside(Point2f Pe)
	{
		double c=(center-Pe)%(center-Pe)-(radius*radius);		
		if(c>0)	{return false;}	//Point is inside the circle 
		else {return true;}	
	}	
    
    void draw(){
        for(int y = 0; y < height ; y++) {
			for(int x = 0; x< width; x++) {
				int i = (y * width + x) * 3; 
				double rndx =(float)((float)(rand()%100)/(100.0));
				double rndy=(float)((float)(rand()%100)/(100.0));
				r = 0; b = 0;g=0;
				for (int p = 0; p < neig; p++)
				{
					for (int q = 0; q < neig; q++)
					{
						I=x+(((float)p/neig)+((rndx)/neig));
						J=y+(((float)q/neig)+((rndy)/neig));
						 Point2f pointP(I,J);
				      	if( isPointInside(pointP) )
				      	{
						// shade the circle
						double d = distanceBetweenPoints(center, specularPoint)+radius;
						double t = (distanceBetweenPoints(pointP,specularPoint))/(d);	
				      	r+=255*(specularColor.x*(1-t) + t*color.x);
						b+=255*(specularColor.y*(1-t) + t*color.y);
						g+=255*(specularColor.z*(1-t) + t*color.z);
					}
				      	else
				      	{
                            Point3f bgColor1(1,0,0),bgColor2(0,1,0);
                            double g = distanceBetweenPoints(Point2f(0,0), center)+radius;
                            double h = (distanceBetweenPoints(pointP,center))/(g);
				      		r += (int)(255*((1-h)*bgColor1.x + h*bgColor2.x)) % 255;
                            b += (int)(255*((1-h)*bgColor1.y + h*bgColor2.y)) % 255;
                            g += (int)(255*((1-h)*bgColor1.z + h*bgColor2.z)) % 255;
				      	}
					}
				}
				r=r/(neig*neig);b/=(neig*neig);g/=(neig*neig);
				pixmap[i++] = r;
				pixmap[i++] = g; 
				pixmap[i] = b;     
			}
		}
        
        
    }
};

class Sonic_simulation{
    public:
        Point2f A, B, C, I_A, I_B, I_C;
        Sonic_simulation(){};
        Sonic_simulation(Point2f _A, Point2f _B, int inner){
            A = _A;
            B = _B;
            C = Point2f((B.x-A.x)/2+A.x,(sqrt(3))*(B.x-A.x)/2+B.y);
            I_A = Point2f(A.x+inner, A.y + inner/(sqrt(3)));
            I_B = Point2f(B.x-inner, B.y + inner/(sqrt(3)));
            I_C = Point2f((I_B.x-I_A.x)/2+I_A.x,(sqrt(3))*(I_B.x-I_A.x)/2+I_B.y);
        }
        
        bool inside_out_T(Point2f p){
            if( Smaller(A,B,p) && !Smaller(A,C,p) && Smaller(B,C,p) ){return true;}
            return false;            
        }      

        bool on_edge_T(Point2f p){
            if( onedge(A,B,p) && !onedge(A,C,p) && onedge(B,C,p) ){return true;}
            return false;            
        }      
        
        bool inside_in_T(Point2f p){
            if( Smaller(I_A,I_B,p) && !Smaller(I_A,I_C,p) && Smaller(I_B,I_C,p) ){return true;}
            return false;
        }
        
        void draw(){
            for(int y = 0; y < height ; y++) {
                for(int x = 0; x< width; x++) {
                    int i = (y * width + x) * 3;            
                    double rx = (float)((float)(rand()%100)/(100.0));
                    double ry = (float)((float)(rand()%100)/(100.0));
                    r = 0; b = 0; g = 0;
                    for (int p = 0; p < neig; p++) {
                        for (int q = 0; q < neig; q++)	{
                            I=x+(((float)p/neig)+((rx)/neig));
                            J=y+(((float)q/neig)+((ry)/neig));
                            
                            Point3f pointP(I,J,0);                           
				      	if(inside_out_T(Point2f(I,J))){
                            if(inside_in_T(Point2f(I,J))){
                                r += 255;b += 255; g += 255;
                            }
                            else{
                                double t = I/width, ty = J/height;
                                Point3f bgColor2(0,1,0);
                                double g = distanceBetweenPoints(C, Point2f(0,0));
                                double h = distanceBetweenPoints(Point2f(x,y),     Point2f(0,0)) / g;
                                r += (int)(73)%255;    
                                b += (int)(210-(210*h))%255;
                                g += (int)(216)%255;
                            }
                        }
				      	else{
                            double t = I/width, ty = J/height;
                            Point3f bgColor2(0,1,0);
                            double g = distanceBetweenPoints(Point2f(600,600), Point2f(0,0));
                            double h = distanceBetweenPoints(Point2f(x,y),     Point2f(0,0)) / g;
                            r += (int)(255)%255;    
                            b += (int)((255-255*h))%255;
                            g += (int)(255)%255; 
                        }
				    }
				}
            r=r/neig*neig;b/=neig*neig;g/=neig*neig;
			pixmap[i++] = r;
			pixmap[i++] = g; 
			pixmap[i] = b; 
                }
            }    
        }
    
    
    
};





























// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

  //initialize the global variables
	width = 600;
	height = 600;
    cout<<"1. Star \n2.Convex\n3.Blobby\n4.Function\n5.Shaded\n6.special "<<endl;
    int input = 0;
    cin>>input;
    pixmap = new unsigned char[width * height * 3];    
    switch(input){
        case 1:
        {
            Point2f A(160,120), B(440,120),C(500,400),D(300,560),E(100,400);
            Star star = Star(A,B,C,D,E);
            star.draw();
            save("star.ppm");
            break;
        }    
        case 2:
        {
            Point2f A(200,300), B(500,200),C(300,400),D(500,500);
            Convex convex = Convex(A,B,C,D);
            convex.draw();
            save("convex.ppm");
            break;            
        }
        case 3:
        {
            Blobby blobby = Blobby();
            blobby.draw();
            save("blobby.ppm");
            break;            
        }
        case 4:
        {
            Functions functions = Functions();
            functions.draw();
            save("function.ppm");
            break;            
        }
        case 5:
        {
            Circle circle(Point2f(300,300), 100, Point3f(1,1,2));
            circle.setSpecularPoint(circle.center + Point2f(-20,0));
            circle.draw();
            save("shaded.ppm");
            break;            
        }
        case 6:
        {
            Point2f A(170, 220), B(430, 220);
            Sonic_simulation s(A, B, 70);
            s.draw();
            save("sp.ppm");
            break;            
        }
        
    }
      




  // OpenGL Commands:
  // Once "glutMainLoop" is executed, the program loops indefinitely to all
  // glut functions.  
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100); // Where the window will display on-screen.
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Homework Zero");
  init();
  glutReshapeFunc(windowResize);
  glutDisplayFunc(windowDisplay);
  glutMouseFunc(processMouse);
  glutMainLoop();

  return 0; //This line never gets reached. We use it because "main" is type int.
}

