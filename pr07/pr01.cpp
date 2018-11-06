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

#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height, max_color;
unsigned char *pixmap;
unsigned char *changed;

void load(string filename){
    FILE *input_file = fopen(filename.c_str(), "r");    
    //assume we will have p6 ppm as input
    char input[100];
    fscanf(input_file, "%s", input);

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

    pixmap = new unsigned char[width * height * 3];
    changed = new unsigned char[width * height * 3];
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

void save(string filename){
    FILE *output_file = fopen(filename.c_str(), "w");
    fprintf(output_file, "P6\n");
    fprintf(output_file, "%d %d\n", width, height);
    fprintf(output_file, "255\n");   
    
    for(int y = height - 1; y >= 0; y--){
        for(int x = 0; x < width; x++){
            int i = (y * width + x) * 3;
            fputc(changed[i++], output_file);
            fputc(changed[i++], output_file);
            fputc(changed[i], output_file);
        }
      }        
    fclose(output_file);
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

void bilinear(float rgb[3], float x, float y){//found from the internet. check the bilinear interpolation wiki page
    float x1, x2, y1, y2;
    int Q11, Q12, Q21, Q22;
   
    x1 = floor(x);
    x2 = x1 + 1;
    y1 = floor(y);
    y2 = y1 + 1;
    
  	Q11 = 3 * (int(y1)* width + int(x1));
	Q12 = 3 * (int(y2)* width + int(x1));
	Q21 = 3 * (int(y1)* width + int(x2));
	Q22 = 3 * (int(y2)* width + int(x2)); 
    
  	if (x2 < width && y2 < height) {
		rgb[0] = (y2-y) / (y2-y1) * ( pixmap[Q11++] * (x2-x)/(x2-x1) + pixmap[Q21++] * (x-x1)/(x2-x1)) + (y-y1)/(y2-y1) * (pixmap[Q12++] * (x2-x)/(x2-x1) + pixmap[Q22++] * (x-x1)/(x2-x1)) + 0.5;
		rgb[1] = (y2-y) / (y2-y1) * ( pixmap[Q11++] * (x2-x)/(x2-x1) + pixmap[Q21++] * (x-x1)/(x2-x1)) + (y-y1)/(y2-y1) * (pixmap[Q12++] * (x2-x)/(x2-x1) + pixmap[Q22++] * (x-x1)/(x2-x1)) + 0.5;
		rgb[2] = (y2-y) / (y2-y1) * ( pixmap[Q11]   * (x2-x)/(x2-x1) + pixmap[Q21]   * (x-x1)/(x2-x1)) + (y-y1)/(y2-y1) * (pixmap[Q12]   * (x2-x)/(x2-x1) + pixmap[Q22]   * (x-x1)/(x2-x1)) + 0.5;
	}  
    else if(x2 == width && y2 < height){
		rgb[0] = (y2-y)/(y2-y1) * pixmap[Q11++] + (y-y1)/(y2-y1) * pixmap[Q12++] + 0.5;
		rgb[1] = (y2-y)/(y2-y1) * pixmap[Q11++] + (y-y1)/(y2-y1) * pixmap[Q12++] + 0.5;
		rgb[2] = (y2-y)/(y2-y1) * pixmap[Q11]   + (y-y1)/(y2-y1) * pixmap[Q12]   + 0.5;
    }
    else if (x2 < width && y2 == width){
 		rgb[0] = (y2-y)/(y2-y1) * pixmap[Q11++]  + (y-y1)/(y2-y1) * pixmap[Q21++] + 0.5;
		rgb[1] = (y2-y)/(y2-y1) * pixmap[Q11++]  + (y-y1)/(y2-y1) * pixmap[Q21++] + 0.5;
		rgb[2] = (y2-y)/(y2-y1) * pixmap[Q11]    + (y-y1)/(y2-y1) * pixmap[Q21]   + 0.5;        
    }
    else{
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 0;
    }
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
  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, changed);
  glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
  if(state == GLUT_UP){
    exit(0); 
  }
                // Exit on mouse click.
}
static void init(void)
{
  glClearColor(1,1,1,1); // Set background color.
}

// =============================================================================
// main() Program Entry
// =============================================================================


void getInverseBilinearWarp(float x[4],float y[4],int I,int J,int& x_res,int& y_res)
{
  //directly from the Don House book
  double a0,a1,a2,a3,b0,b1,b2,b3;
  a0=x[0];
  a1=x[3]-x[0];
  a2=x[1]-x[0];
  a3=x[2]+x[0]-x[1]-x[3];
  b0=y[0];
  b1=y[3]-y[0];
  b2=y[1]-y[0];
  b3=y[2]+y[0]-y[1]-y[3];
  
  
  double c0,c1,c2;
  c0=a1*(b0-J)+b1*(I-a0);
  c1=a3*(b0-J)+b3*(I-a0)+a1*b2-a2*b1;
  c2=a3*b2-a2*b3;
  
  double u1,v1,u2,v2,u,v;
  v1=((-1*c1)/(2*c2))+(1/(2*c2))*pow((pow(c1,2)-(4*c2*c0)),0.5);
  v2=((-1*c1)/(2*c2))-(1/(2*c2))*pow((pow(c1,2)-(4*c2*c0)),0.5);
  u1=(I-a0-a2*v1)/(a1+a3*v1);
  u2=(I-a0-a2*v2)/(a1+a3*v2);
  
  
  
  if(!(( (0<=u1 && u1<=1) && (0<=v1 && v1<=1)) || ((0<=u2 && u2<=1) && (0<=v2 && v2<=1))))
  {
    x_res=-1;
    y_res=-1;
    return;
  }
  else if((0<=u1 && u1<=1) && (0<=v1 && v1<=1))
  {
    u=u1;
    v=v1;
  }
  else if((0<=u2 && u2<=1) && (0<=v2 && v2<=1))
  {
    u=u2;
    v=v2;
  }
  else
  {
    x_res=-1;
    y_res=-1;
    return;
  }
  
  x_res = (int)((width-1)*u+0.5);
  y_res = (int)((height-1)*v+0.5);
  
  
}





void bilinearWrap(float matrix[9]){
    int x1, x2, x3, x4, y1, y2, y3, y4;
    float x[4], y[4];
    x[0] = 0;
    y[0] = 0;
    x[0] = matrix[0] * x[0] + matrix[1]* y[0] + matrix[2];
    y[0] = matrix[3] * x[0] + matrix[4]* y[0] + matrix[5];
    float z = matrix[6] * x[0] + matrix[7]* y[0] + matrix[8];
    x[0] = x[0]/z;
    y[0] = y[0]/z;    
    
    x[1] = 0;
    y[1] = height;    
    x[1] = matrix[0] * x[1] + matrix[1]* y[1] + matrix[2];
    y[1] = matrix[3] * x[1] + matrix[4]* y[1] + matrix[5];
    z = matrix[6] * x[1] + matrix[7]* y[1] + matrix[8];
    x[1] = x[1]/z;
    y[1] = y[1]/z;    
    
    x[2] = width;
    y[2] = height;
    x[2] = matrix[0] * x[2] + matrix[1]* y[2] + matrix[2];
    y[2] = matrix[3] * x[2] + matrix[4]* y[2] + matrix[5];
    z = matrix[6] * x[2] + matrix[7]* y[2] + matrix[8];
    x[2] = x[2]/z;
    y[2] = y[2]/z;     
    
    
    x[3] = width;
    y[3] = 0;
    x[3] = matrix[0] * x[3] + matrix[1]* y[3] + matrix[2];
    y[3] = matrix[3] * x[3] + matrix[4]* y[3] + matrix[5];
    z = matrix[6] * x[3] + matrix[7]* y[3] + matrix[8];
    x[3] = x[3]/z;
    y[3] = y[3]/z;     
    int index;
    
    
    int x_res;
    int y_res;
    
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            getInverseBilinearWarp(x,y,i,j,x_res,y_res);
            index = (j * width + i) * 3;
            if (x_res >= 0 && x_res < width && y_res >= 0 && y_res < height){
                float rgb[3];
                index = (j * width + i) * 3;
                bilinear(rgb, x_res, y_res);//anti alising
                changed[index++] = rgb[0];
                changed[index++] = rgb[1];
                changed[index]   = rgb[2];
            }
        }
    }
}

void Prespective(float a, float b){
    cout<<a<<" "<<b<<endl;
    a =  a / ((float)width*10);
    b =  b / ((float)height*10);
    cout<<a<<" "<<b<<endl;
    //directly make the inverse matrix
    float PrespectiveMatrix[9];
 	PrespectiveMatrix[0] = 1;
	PrespectiveMatrix[1] = 0;
	PrespectiveMatrix[2] = 0;
	PrespectiveMatrix[3] = 0;
	PrespectiveMatrix[4] = 1;
	PrespectiveMatrix[5] = 0;
	PrespectiveMatrix[6] = a;
	PrespectiveMatrix[7] = b;
	PrespectiveMatrix[8] = 1;   
    bilinearWrap(PrespectiveMatrix);       
}



void inverseSineWarp(){
    int index;
    int x_res;
    int y_res;

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            x_res = (float)i - (float)(6.00*cos((float)j/10.00));
            y_res = (float)j - (float)(6.00*sin((float)i/10.00));
            index = (j * width + i) * 3;
            if (x_res >= 0 && x_res < width && y_res >= 0 && y_res < height){
                float rgb[3];
                index = (j * width + i) * 3;
                bilinear(rgb, x_res, y_res);
                changed[index++] = rgb[0];
                changed[index++] = rgb[1];
                changed[index]   = rgb[2];
            }
        }
    }  
}



void take_options(int option){
    float a = 0.5, b = 0.9 , c = 0.3, d = 0.7;
    float h_scale = 1.5, v_scale = 1.5;
    float angle = 45;
    int h_v = 0; 
    float move_x, move_y;
    switch(option){
       
        case 1:                
            cout<<"input a:"<<endl;
            cin>>move_x;
            cout<<"input b:"<<endl;
            cin>>move_y;    
            Prespective(move_x, move_y);
            save("bilinear.ppm");
            break;  
            
        case 2:                
            inverseSineWarp();
            save("inverseSineWarp.ppm");
            break;  
    }
}







int main(int argc, char *argv[])
{
    cout<<"select the image you want to do the transformation(ppm): "<<endl;
    string filename;
    cin>>filename;
    
    int options;
    cout<<"which option:\n1.Bilinear Wrap\n2.Sine Inverse Wrap"<<endl;
    cin>>options;
    
    load(filename);
    take_options(options);
    
    
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("load");
    init();
    glutReshapeWindow(width, height);
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    glutMouseFunc(processMouse);   
    glutMainLoop();
        
        
        
        
        
        
        
        
    
    




  //initialize the global variables
/*   width = 300;
  height = 300;
  pixmap = new unsigned char[width * height * 3];  //Do you know why "3" is used?

  setPixels();


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
  glutMainLoop(); */

  return 0; //This line never gets reached. We use it because "main" is type int.
}

