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
int CY[255];
unsigned char *pixmap;
unsigned char *pixrep;
float I = 0, J = 0;
int r = 0,g = 0,b=0;
int neig = 4;
int N = 5;








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
		default:		
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






class Point2f{
    public:
    float x, y;
    Point2f() {}
    Point2f( float _x, float _y ) { x=_x; y=_y; }
    Point2f operator+( const Point2f &pt ) const { return Point2f(x+pt.x, y+pt.y); }    
    Point2f operator-( const Point2f &pt ) const { return Point2f(x-pt.x, y-pt.y); }
    Point2f operator*(float t) const  { return (Point2f(x * t, y * t)); }
    Point2f operator/(float t) const { float f = 1.0F / t; return (Point2f(x * f, y * f));}
    float Dot	   ( const Point2f &pt ) const { return x*pt.x + y*pt.y; }	///< Dot product
	float operator%( const Point2f &pt ) const { return Dot(pt); }
    
};



float F(float p0, float p1, float v0, float v1, float t)
{
    float h0 = 2.0*powf(t,3) - 3.0*powf(t,2) +1;
    float h1 = -2.0*powf(t,3) + 3.0*powf(t,2);
    float h2 = powf(t,3) - 2.0*powf(t,2) + t;
    float h3 = powf(t,3) - powf(t,2);
    float CY;
    CY = p0*h0 + p1*h1 + v0*h2 + v1*h3;
    return CY;
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


void create_curve(Point2f p[]){
    int Cx[N];
    int Cy[N];
    for(int i = 0;i<N;i++){
        Cx[i] = (int)(255*p[i].x+0.5);
        Cy[i] = (int)(255*p[i].y+0.5);        
    }
    
    int i = 0, j,k;
    for(int CX = 0;CX<256;CX++){
        if(CX > Cx[i+1]) i++;
        j = (i-1); if(j<0) j = 0;
        k = (i+2); if(k > N-1) k = N-1;
        float t = (CX - Cx[i])/(Cx[i+1]-Cx[i]);
        float a = Cy[i];
        float b = Cy[i+1];
        float c = (Cy[i+1] - Cy[j]) * (p[i+1].x - p[j].x);
        float d = (Cy[k] - Cy[i]) * (p[k].x - p[i].x);
        CY[CX] = F(a,b,c,d,t);
    }
}

void curve(int mode){
        N = 5;
        Point2f p[N];
        p[0] = Point2f(0, 0);
        p[1] = Point2f(0.3, 0.6);
        p[2] = Point2f(0.5, 0.3);
        p[3] = Point2f(0.7, 0.5);
        p[4] = Point2f(1, 1);              
        create_curve(p);  
        getColor(mode);       
}

// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

  //initialize the global variables
  width = 600;
  height = 600;
  
  int option;
  cout<<"0.Curve-AllColor\n1.Curve-Red \n2.Curve-Green\n3.Curve-Blue\n4.Replace—Hue"<<endl;
  cin>>option;
  string file;  
  if(option == 4){
        cout<<"Name of the replace Color Hue Pic: (PPM only)"<<endl;
        cin>>file;            
  }


  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100); // Where the window will display on-screen.
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);  
  glutCreateWindow("Homework Zero");
  
  switch(option){
    case 0:
        load("00.ppm");     
        init();
        curve(0);      
        save("0.ppm");
        break;    
    case 1:
        load("00.ppm");     
        init();
        curve(1);      
        save("1.ppm");
        break;
    case 2:
        load("00.ppm");
     
        init();
        curve(2);   
        save("2.ppm");
        break;
    case 3:
        load("00.ppm");
   
        init();
        curve(3);     
        save("3.ppm");
        break; 
    case 4:
        init();        
        replaceColor("00.ppm", file);
        save("6.ppm");
        break;        
  }  
  glutReshapeFunc(windowResize);
  glutDisplayFunc(windowDisplay);
  glutMouseFunc(processMouse);
  glutMainLoop();

  return 0; //This line never gets reached. We use it because "main" is type int.
}

