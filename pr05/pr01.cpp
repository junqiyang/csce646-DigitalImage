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
//Junqi Yang


int width, height, max_color;
unsigned char *pixmap;
unsigned char *control;
unsigned char *changed;
#define N 5


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
    glutReshapeWindow(width, height);
    pixmap = new unsigned char[width * height * 3];
    changed = new unsigned char[width * height * 3];
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

void load_control(string filename){
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
    glutReshapeWindow(width, height);
    control = new unsigned char[width * height * 3];

    for(int y = height - 1; y >= 0; y--){
      for(int x = 0; x < width; x++){
        int i = (y * width + x) * 3;
        control[i++] = fgetc(input_file);
        control[i++] = fgetc(input_file);
        control[i] = fgetc(input_file);
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








float getAngle(int r, int g, int center)
{
    int xDiff = r - center;
    int yDiff = g - center;
    
    float h = pow ((pow(xDiff,2) + pow(yDiff,2)),0.5);

    float sin = (float)yDiff/h;
    float cos = (float)xDiff/h;

    if (sin == 0)
    {
        if(cos == 1)
          return 0;
        else
          return M_PI;
    }
    
    if (cos == 0)  
    {
        if(sin == 1)
          return (M_PI/2);
        else
          return (1.5 * M_PI);
    }
    
    float angle = atan((double)yDiff/(double)xDiff);

    if (sin > 0)
    {
        if(cos < 0)
        angle += M_PI;
    }
    else
    {
        if(cos > 0)
          angle += (2 * M_PI);
        else if (cos < 0)
          angle += M_PI;
      }
    return angle;
}

bool in_area(int x, int y, int center, float theta){
    float XX = pow(((x-center)*cos(theta) + (y-center)*sin(theta)),2);
    float YY = pow(((x-center)*sin(theta) - (y-center)*cos(theta)),2);
    float re = XX/pow(center, 2) + YY;
    if(re <= 1) return true;
    return false;    
}

void blurFilter(float w[N][N], int x, int y){
    int center = N/2;
    
    int i = (y*width + x)*3;
    int r = control[i++];
    int g = control[i++];
    
    r = (r * (N-1))/255;
    g = (g * (N-1))/255;
    
    if(r == center && g == center){
        for(int n = 0; n < N;n++){
            for(int m = 0; m < N; m++){
                w[m][n] = 0;
            }            
        }
        w[center][center] = 1;
    }
    else{
        float theta = getAngle(r, g, center);
        for(int n = 0; n < N;n++){
            for(int m = 0; m < N; m++){
                if(in_area(m, n, center, theta)){
                    w[m][N-n-1] = 1;
                }
                else{
                    w[m][N-n-1] = 0;
                }                
            }            
        }   
    }
}

void applyblur(float w[N][N]){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 0, B = 0, G = 0, sum = 0;
            blurFilter(w, x, y);
            
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX += width;
                    if(XX>width-1)
                        XX -= width;
                    if(YY<0)
                        YY += height;
                    if(YY>height-1)
                        YY -= height;
                    
                    XX = XX%width;
                    YY = YY%height;
                                         
                    int old_index = (YY * width + XX) * 3;
                    R += w[i][j] * pixmap[old_index++];
                    G += w[i][j] * pixmap[old_index++];
                    B += w[i][j] * pixmap[old_index];
                    sum +=w[i][j];
                }
            }
            changed[index++] = R/sum;
            changed[index++] = G/sum;
            changed[index] = B/sum;
        }
    }
}



bool in_circle(int x, int y, int center, float radius){
    if(pow((x-center),2)/pow(radius,2) + pow((y-center),2)/pow(radius,2) <= 1){return true;}
    return false;    
}

void MonoFilter(float w[N][N], int x, int y){
    int center = N/2;
    
    int i = (y*width + x)*3;
    int r = control[i++];
    int g = control[i++];
    
    r = (r * (N-1))/255;
    g = (g * (N-1))/255;
    
    if(r == center && g == center){
        for(int n = 0; n < N;n++){
            for(int m = 0; m < N; m++){
                w[m][n] = 0;
            }            
        }
        w[center][center] = 1;
    }
    else{
        int xDiff = r - center;
        int yDiff = g - center;
        float radius = pow((pow(xDiff, 2)+ pow(yDiff, 2)), 0.5);
        for(int n = 0; n < N;n++){
            for(int m = 0; m < N; m++){
                if(in_circle(m, n, center, radius)){
                    w[m][N-n-1] = 1;
                }
                else{
                    w[m][N-n-1] = 0;
                }                
            }            
        }   
    }
}
    
void applyDilationFilter(float w[N][N]){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            MonoFilter(w, x, y);
            int index = (y * width + x) * 3;
            float max_k = 0;
            float R = 0, B = 0, G = 0;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {           
                    if(!w[i][j]){w[i][j] = 0.5;}
                    if(max_k < w[i][j]){max_k = w[i][j];}
                }
            }
            
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {           
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX += width;
                    if(XX>width-1)
                        XX -= width;
                    if(YY<0)
                        YY += height;
                    if(YY>height-1)
                        YY -= height;
                    
                    int old_index = (YY * width + XX) * 3;
                    float O_R = w[i][j] * pixmap[old_index++];
                    float O_G = w[i][j] * pixmap[old_index++];
                    float O_B = w[i][j] * pixmap[old_index];                    
                    if(O_R > R){R = O_R;}
                    if(O_G > G){G = O_G;}
                    if(O_B > B){B = O_B;}                   
                }
            }           
            changed[index++] = R/max_k;
            changed[index++] = G/max_k;
            changed[index] = B/max_k;

        }
    }  
}

void applyErosionFilter(float w[N][N]){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            MonoFilter(w, x, y);
            int index = (y * width + x) * 3;
            float min_k = 999;
            float R = 255, B = 255, G = 255;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {           
                    if(!w[i][j]){w[i][j] = 0.5;}
                    if(min_k > w[i][j]){min_k = w[i][j];}
                }
            }
            
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {           
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX += width;
                    if(XX>width-1)
                        XX -= width;
                    if(YY<0)
                        YY += height;
                    if(YY>height-1)
                        YY -= height;
                    
                    int old_index = (YY * width + XX) * 3;
                    float O_R = w[i][j] * pixmap[old_index++];
                    float O_G = w[i][j] * pixmap[old_index++];
                    float O_B = w[i][j] * pixmap[old_index];                    
                    if(O_R < R){R = O_R;}
                    if(O_G < G){G = O_G;}
                    if(O_B < B){B = O_B;}                   
                }
            }           
            changed[index++] = R/min_k;
            changed[index++] = G/min_k;
            changed[index] = B/min_k;

        }
    }      
}

void applyfilter(int option){
    float w[N][N];
    switch(option){
        case 1:
            applyblur(w);
            save("blur.ppm");
            break;
            
        case 2:
            applyErosionFilter(w);
            save("Erosion.ppm");
            break;
            
        case 3:
            applyDilationFilter(w);
            save("Dilation.ppm");
            break;
        
        
    }
    
    
    
    
    
   
}



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
int main(int argc, char *argv[])
{

    cout<<"which image you want to load?(XXX.ppm)"<<endl;
    string filename;
    cin>>filename;
        
    cout<<"which image you want to control?(XXX.ppm)"<<endl;
    string controlname;
    cin>>controlname;       
    
    cout<<"Current Kernel Size: "<< N <<". (Kernel size can be change in code)"<<endl;
    cout<<"Apply Filter: "<<endl;
    cout<<"1. Motion Blur \n2. Non-Stationary Erosion\n3. Non-Stationary Dilation"<<endl;
    int options;
    cin>>options;
        
   
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("load");
    init();
    load(filename);//read ppm
    load_control(controlname);
    applyfilter(options);         
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    glutMouseFunc(processMouse);   
    glutMainLoop();
        
    return 0; //This line never gets reached. We use it because "main" is type int.
}

