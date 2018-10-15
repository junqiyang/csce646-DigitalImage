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
#define N 3


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

void blurFilter(int type, float w[N][N]){    
    switch (type) {
        case 0:
            //uniform
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    w[i][j] = 1.0/(float)(N*N);
                }
            }
            break;

        case 1:
        {
            //random
            float temp[N][N];
            float sum = 0;
            srand ((unsigned int)time(NULL));
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    temp[i][j] = (float)rand()/RAND_MAX;
                    sum += temp[i][j];
                }
            }
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    w[i][j] = temp[i][j]/sum;
                }
            }
            break;
        }
        default:
            break;
    }
}

void applyblur(float w[N][N]){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 0, B = 0, G = 0;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX=0;
                    if(XX>width-1)
                        XX=width-1;
                    if(YY<0)
                        YY=0;
                    if(YY>height-1)
                        YY=height-1;
                     
                    int old_index = (YY * width + XX) * 3;
                    R += w[i][j] * pixmap[old_index++];
                    G += w[i][j] * pixmap[old_index++];
                    B += w[i][j] * pixmap[old_index];
                }
            }
            changed[index++] = fmax(0, fmin(R, 255));
            changed[index++] = fmax(0, fmin(G, 255));
            changed[index] = fmax(0, fmin(B, 255));
        }
    }
}

void EmbossFilter(float w[N][N], float theta, float d){
    Point2f pc((float)N/2, (float)N/2);
    Point2f n0(sinf(M_PI/2.0+theta), cosf(M_PI/2.0+theta));
    float sum = 0;
    
    for (int i = 0; i <= N-1; i++) {
        for (int j = 0; j <= N-1; j++) {
            Point2f p(i,j);
            float F = n0 % (p-pc);
            if (fabsf(F) <= d) {
                w[i][j] = 0;
            }else if (F < d) {
                w[i][j] = 1;
            }else {
                w[i][j] = -1;
            }
            sum += w[i][j];
        }
    }

    if (sum != 0) {
        int number = 0;
        for (int i = 0; i <= N-1; i++) {
            for (int j = 0; j <= N-1; j++) {
                if (sum > 0) {
                    if (w[i][j] > 0) {
                        number++;
                    }
                }else {
                    if (w[i][j] < 0) {
                        number++;
                    }
                }
            }
        }
        
        for (int i = 0; i <= N-1; i++) {
            for (int j = 0; j <= N-1; j++) {
                if (sum > 0) {
                    if (w[i][j] > 0) {
                        w[i][j] = w[i][j] - (float)(sum/number);
                    }
                }else {
                    if (w[i][j] < 0) {
                        w[i][j] = w[i][j] - (float)(sum/number);
                    }
                }
            }
        }
    }
}

void applyEmbossFilter(float w[N][N]){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 0, B = 0, G = 0;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX=0;
                    if(XX>width-1)
                        XX=width-1;
                    if(YY<0)
                        YY=0;
                    if(YY>height-1)
                        YY=height-1;
                     
                    int old_index = (YY * width + XX) * 3;
                    R += w[i][j] * pixmap[old_index++];
                    G += w[i][j] * pixmap[old_index++];
                    B += w[i][j] * pixmap[old_index];
                }
            }
            changed[index++] = (R+255*3)/6;
            changed[index++] = (G+255*3)/6;
            changed[index] = (B+255*3)/6;
        }
    }  
}

void applyoutlineFilter(){
    float w[N][N] = {	{-1,-1,-1},
                        {-1, 8,-1},
                        {-1,-1,-1}  }; 
    
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 0, B = 0, G = 0;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX=0;
                    if(XX>width-1)
                        XX=width-1;
                    if(YY<0)
                        YY=0;
                    if(YY>height-1)
                        YY=height-1;
                     
                    int old_index = (YY * width + XX) * 3;
                    R += w[i][j] * pixmap[old_index++];
                    G += w[i][j] * pixmap[old_index++];
                    B += w[i][j] * pixmap[old_index];
                }
            }
            changed[index++] = fmax(0, fmin(R, 255));
            changed[index++] = fmax(0, fmin(G, 255));
            changed[index] = fmax(0, fmin(B, 255));
        }
    }  
}

void applysharpenFilter(){
    float w[N][N] = {	{0,-1,0},
                        {-1,5,-1},
                        {0,-1,0}  }; 
    
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 0, B = 0, G = 0;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX=0;
                    if(XX>width-1)
                        XX=width-1;
                    if(YY<0)
                        YY=0;
                    if(YY>height-1)
                        YY=height-1;
                     
                    int old_index = (YY * width + XX) * 3;
                    R += w[i][j] * pixmap[old_index++];
                    G += w[i][j] * pixmap[old_index++];
                    B += w[i][j] * pixmap[old_index];
                }
            }
            changed[index++] = fmax(0, fmin(R, 255));
            changed[index++] = fmax(0, fmin(G, 255));
            changed[index] = fmax(0, fmin(B, 255));
        }
    }  
}

void applyDilationFilter(){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 0, B = 0, G = 0;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX=0;
                    if(XX>width-1)
                        XX=width-1;
                    if(YY<0)
                        YY=0;
                    if(YY>height-1)
                        YY=height-1;
                     
                    int old_index = (YY * width + XX) * 3;
                    float O_R = pixmap[old_index++];
                    float O_G = pixmap[old_index++];
                    float O_B = pixmap[old_index];
                    if(O_R > R){R = O_R;}
                    if(O_G > G){G = O_G;}
                    if(O_B > B){B = O_B;}
                }
            }
            changed[index++] = R;
            changed[index++] = G;
            changed[index] = B;
        }
    }      
}

void applyErosionFilter(){
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float R = 255, B = 255, G = 255;
            for (int i = 0; i <= N-1; i++) {
                for (int j = 0; j <= N-1; j++) {
                    int XX = x + i -(N-1)/2;
                    int YY = y + j -(N-1)/2;
                    if(XX<0)
                        XX=0;
                    if(XX>width-1)
                        XX=width-1;
                    if(YY<0)
                        YY=0;
                    if(YY>height-1)
                        YY=height-1;
                     
                    int old_index = (YY * width + XX) * 3;
                    float O_R = pixmap[old_index++];
                    float O_G = pixmap[old_index++];
                    float O_B = pixmap[old_index];
                    if(O_R < R){R = O_R;}
                    if(O_G < G){G = O_G;}
                    if(O_B < B){B = O_B;}
                }
            }
            changed[index++] = R;
            changed[index++] = G;
            changed[index] = B;
        }
    }      
}

void applyfilter(int option){
    float w[N][N];
    switch(option){
        case 1:
            blurFilter(0,w);
            applyblur(w);
            save("blur_uni.ppm");
            break;
            
        case 2:
            blurFilter(1,w);
            applyblur(w);
            save("blur_random.ppm");
            break;        

        case 3:
            EmbossFilter(w, M_PI/2, 0.4);
            applyEmbossFilter(w);
            save("emboss90degree.ppm");
            break;
            
        case 4:
            applyoutlineFilter();
            save("outline.ppm");
            break;
        
        case 5:
            applysharpenFilter();
            save("sharpen.ppm");
            break;
            
        case 6:
            applyErosionFilter();
            save("Erosion.ppm");
            break;
            
        case 7:
            applyDilationFilter();
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
        
        cout<<"Apply Filter: "<<endl;
        cout<<"1.Blur(uniform) \n 2.Blur(Random) \n 3.Emboss \n 4.Outline \n 5.Sharpen \n 6.Erosion \n 7.Dilation \n"<<endl;
        int options;
        cin>>options;
        
        
        glutInit(&argc, argv);
        glutInitWindowPosition(100, 100); // Where the window will display on-screen.
        glutInitWindowSize(width, height);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutCreateWindow("load");
        init();
        load(filename);//read ppm
        applyfilter(options);
        
        glutReshapeFunc(windowResize);
        glutDisplayFunc(windowDisplay);
        glutMouseFunc(processMouse);   
        glutMainLoop();

        
        
        
        
        string savename = "test.ppm";
        save(savename);//save ppm
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

