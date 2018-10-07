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
//Junqi Yang
//221004901

using namespace std;
// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height, max_color, curveW, curveH, level;
int cxLevel = 0;
int CY[255];
unsigned char *pixmap;
unsigned char *pixrep;
float I = 0, J = 0;
int r = 0,g = 0,b=0;
int neig = 4;
int N = 4;








#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z))) 
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

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

void load(string filename){
    FILE *input_file = fopen(filename.c_str(), "r");    
    //assume we will have p6 ppm as input
    char input[100];
    fscanf(input_file, "%s", input);
    cout<<"version: "<<input<<endl;
    fscanf(input_file, "%s", input); 

    if(input[0]== '#'){
        while(input[0] == '#'){
            fgets(input, 50, input_file); 
        }
        fscanf(input_file, "%s", input);  
    }

    width = atoi(input);
 
    fscanf(input_file, "%s", input);    
    height = atoi(input);

    fscanf(input_file, "%s", input);
    max_color = atoi(input);
    
    char x = fgetc(input_file);
    glutReshapeWindow(width, height);
    pixmap = new unsigned char[width * height * 3];
    
      cout<<input<<endl;  
    for(int y = height - 1; y >= 0; y--){
      for(int x = 0; x < width; x++){
        int i = (y * width + x) * 3;
        pixmap[i++] = fgetc(input_file);
        pixmap[i++] = fgetc(input_file);
        pixmap[i] = fgetc(input_file);
      }
    }
    fclose(input_file);
}

void load_re(string filename){
    FILE *input_file = fopen(filename.c_str(), "r");    
    //assume we will have p6 ppm as input
    char input[100];
    fscanf(input_file, "%s", input);
    cout<<"version: "<<input<<endl;
    fscanf(input_file, "%s", input); 

    if(input[0]== '#'){
        while(input[0] == '#'){
            fgets(input, 50, input_file); 
        }
        fscanf(input_file, "%s", input);  
    }

    width = atoi(input);
 
    fscanf(input_file, "%s", input);    
    height = atoi(input);

    fscanf(input_file, "%s", input);
    max_color = atoi(input);
    
    char x = fgetc(input_file);
    glutReshapeWindow(width, height);
    pixrep = new unsigned char[width * height * 3];
    
      cout<<input<<endl;  
    for(int y = height - 1; y >= 0; y--){
      for(int x = 0; x < width; x++){
        int i = (y * width + x) * 3;
        pixrep[i++] = fgetc(input_file);
        pixrep[i++] = fgetc(input_file);
        pixrep[i] = fgetc(input_file);
      }
    }
    fclose(input_file);
}

void RGBtoHSV(int r, int g, int b, double &h, double &s, double &v){
  double red, green, blue; 
  double max, min, delta;
  red = r / 255.0; green = g / 255.0; blue = b / 255.0;  /* r, g, b to 0 - 1 scale */
  max = maximum(red, green, blue); 
  min = minimum(red, green, blue);
  v = max;        /* value is maximum of r, g, b */
  if(max == 0){    /* saturation and hue 0 if value is 0 */ 
    s = 0; 
    h = 0; 
  } 
  else{ 
    s = (max - min) / max;           /* saturation is color purity on scale 0 - 1 */
    delta = max - min; 
       if(delta == 0)                    /* hue doesn't matter if saturation is 0 */ 
      h = 0; 
    else{ 
      if(red == max)                  /* otherwise, determine hue on scale 0 - 360 */ 
        h = (green - blue) / delta; 
      else if(green == max) 
        h = 2.0 + (blue - red) / delta; 
      else /* (blue == max) */ 
        h = 4.0 + (red - green) / delta; 
      h = h * 60.0; 
      if(h < 0) 
        h = h + 360.0; 
    } 
  } 
} 

void HSVtoRGB(int &r, int &g, int &b, double h, double s, double v){
int i;
	float f, p, q, t;

	h /= 60;			
	i = floor( h );
	f = h - i;
    v = v*255;
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

    switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}    
} 

void replaceColor(string sourcefile, string replacefile)
{

    load(sourcefile);    
    load_re(replacefile);

    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            int r, g, b, r2, g2, b2;
            double H, S, V, H2, S2, V2;
            r = pixmap[i++];
            g = pixmap[i++];
            b = pixmap[i];

            i = (y * width + x) * 3;            
            r2 = pixrep[i++];
            g2 = pixrep[i++];
            b2 = pixrep[i];
           
            RGBtoHSV(r, g, b, H, S, V);
            RGBtoHSV(r2,g2,b2,H2,S2,V2);            
            HSVtoRGB(r, g, b, H2, S, V);

            i = (y * width + x) * 3;

            pixmap[i++] = r;
            pixmap[i++] = g;
            pixmap[i] = b;

        }
    }

}










class Vector2f{
    public:    
    float x, y;
    
    Vector2f() {}
    Vector2f( float _x, float _y ) { x=_x; y=_y; }    
    Vector2f operator+(const Vector2f &v) const  { return (Vector2f(x + v.x, y + v.y));    }
    Vector2f operator-(const Vector2f &v) const  { return (Vector2f(x - v.x, y + v.y));    }
    
    
    Vector2f operator*(float t) const  { return (Vector2f(x * t, y * t));} 
    Vector2f operator /(float t) const
    {
        float f = 1.0F / t;
        return (Vector2f(x * f, y * f));
    }
};






class Point2f: public Vector2f{
    public:
    float x, y;
    Point2f() {}
    Point2f( float _x, float _y ) { x=_x; y=_y; }
    Point2f operator+( const Point2f &pt ) const { return Point2f(x+pt.x, y+pt.y); }    
    Point2f operator-( const Point2f &pt ) const { return Point2f(x-pt.x, y-pt.y); }
    Point2f operator*(float t) const  { return (Point2f(x * t, y * t)); }
    Point2f operator/(float t) const { float f = 1.0F / t; return (Point2f(x * f, y * f));}
    
    Point2f& operator =(const Vector2f& v)
    {
        x = v.x;
        y = v.y;
        return (*this);
    }
    
    Point2f operator +(const Vector2f& v) const
    {
        return (Point2f(x + v.x, y + v.y));
    }
    
    
    
    float Dot	   ( const Point2f &pt ) const { return x*pt.x + y*pt.y; }	///< Dot product
	float operator%( const Point2f &pt ) const { return Dot(pt); }
    
};








Point2f F(Point2f p0, Point2f p1, Vector2f v0, Vector2f v1, float t)
{
    float h0 = 2.0*powf(t,3) - 3.0*powf(t,2) +1;
    float h1 = -2.0*powf(t,3) + 3.0*powf(t,2);
    float h2 = powf(t,3) - 2.0*powf(t,2) + t;
    float h3 = powf(t,3) - powf(t,2);
    Point2f Ft;
    Ft = p0*h0 + p1*h1 + v0*h2 + v1*h3;
    return Ft;
}



void getColor(int mode){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            if (mode == 0 || mode == 1) {
                pixmap[i] = CY[pixmap[i]];
            }
            i++;
            if (mode == 0 || mode == 2) {
                pixmap[i] = CY[pixmap[i]];
            }
            i++;
            if (mode == 0 || mode == 3) {
                pixmap[i] = CY[pixmap[i]];
            }
        }
    }
}

void colormap(Point2f p0, Point2f p1){
    int CX0, CX1;
    float cx0, cx1, cy0, cy1;
    p0 = p0;
    p1 = p1;   
    
    cx0 = p0.x;
    cx1 = p1.x;
    cy0 = p0.y;
    cy1 = p1.y;   
    
    CX0 = (int)(cx0 + 0.5);
    CX1 = (int)(cx1 + 0.5); 

    for (int CX = CX0; CX < CX1; CX++){
        float t = CX;
        int CYtemp = (int)(((1-t) *cy0 + t * cy1) + 0.5);
        CY[cxLevel] = CYtemp;
        cxLevel++;
    }    
}


void create_curve(Point2f p[]){
    Vector2f v[N+1];
    Point2f P0, P1;
    v[0] = p[1] - p[0];
    for (int i = 1; i<N; i++) {
        v[i] = (p[i+1] - p[i-1])/2.0;
    }
    v[N] = p[N] - p[N-1];
    P0 = F(p[0], p[1], v[0], v[1], 0);
    for (int i = 0; i< N; i++) {
        for( int T=1; T<100; T++)
        {
            float t = (float)T/(100 - 1);
            P1 = F(p[i], p[i+1], v[i], v[i+1],t);
            colormap(P0, P1);
            P0 = P1;
        }
    }
}




void curve(int mode){
    Point2f p[N+1];

    p[0] = Point2f(0, 0);
    p[1] = Point2f(0.1, 0.2);
    p[2] = Point2f(0.5, 0.8);
    p[3] = Point2f(0.7, 0.7);
    p[4] = Point2f(1, 1); 
    create_curve(p);  
    getColor(mode);
}




























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
 


// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

  //initialize the global variables
  width = 600;
  height = 600;
  

  // OpenGL Commands:
  // Once "glutMainLoop" is executed, the program loops indefinitely to all
  // glut functions.  
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100); // Where the window will display on-screen.
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Homework Zero");
  
  int option;
  cout<<"1.Curve-Red \n"
  
  
//  replaceColor("shaded.ppm", "sp.ppm");
  load("00.ppm");
  init();
  curve(1);
  save("2.ppm");
  glutReshapeFunc(windowResize);
  glutDisplayFunc(windowDisplay);
  glutMouseFunc(processMouse);
  glutMainLoop();

  return 0; //This line never gets reached. We use it because "main" is type int.
}

