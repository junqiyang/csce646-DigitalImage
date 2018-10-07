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










void setCircle(){
    for(int y = 0; y < height ; y++) {
     for(int x = 0; x < width; x++) {
       int i = (y * width + x) * 3;
	   int X = x -300;
	   int Y = y -300;
       pixmap[i++] = (int)((X*X)/10 + (Y*Y)/10 +100 ) % 255; 
       pixmap[i++] = (int)((X*X)/20 + (Y*Y)/20 +200) % 255; 
       pixmap[i] =   (int)((X*X)/40 + (Y*Y)/40 +400) % 255; 
     }
   }   
}


void setSine(){
    for(int y = 0; y < height ; y++) {
     for(int x = 0; x < width; x++) {
       int i = (y * width + x) * 3; 
       pixmap[i++] = x % 255; 
       pixmap[i++] = y+x % 255; 
       pixmap[i] =   x*y % 255; 
     }
   }   
}


void setBlackandWhite(){
    int color = 1;
    int rnd = rand() % 255;
    int rnd2 = rand() % 255;
    int m_color= width/10;
    int counter1 = width;
    int counter2 = width/10;
    for(int y = 0; y < height ; y++) {
     for(int x = 0; x < width; x++) {
       int i = (y * width + x) * 3;
       if(color == 1){
           pixmap[i++] = rnd;
           pixmap[i++] = rnd2;
           pixmap[i] = 0;
           m_color--;
           counter1--;
           if(m_color == 0){
               color++;
               m_color = width/10;
           }
           if(counter1 == 0){
               counter2--;
               counter1 = width;
               if(counter2 == 0){
                    color--;
                    counter2 = width/10;
                    rnd = rand() % 255;
                    rnd2 = rand() % 255;
               }               
           }
       }
       else if(color == 2){
           pixmap[i++] = 255;
           pixmap[i++] = 255;
           pixmap[i] = 255;
           m_color--;
           counter1--;
          if(m_color == 0){
               color--;
               m_color = width/10;
           }
           if(counter1 == 0){
               counter2--;
               counter1 = width;
               if(counter2 == 0){
                    color++;
                    counter2 = width/10;
                    rnd = rand() % 255;
                    rnd2 = rand() % 255;
               }               
           }
       }
     }
   }   
}

void setColorBlock(){
    int color = 1;
    
    int max = 255;
    int min = 0;
    
    int m_color= width/18;
    int counter1 = width;
    int counter2 = width/18;
    for(int y = 0; y < height ; y++) {
     for(int x = 0; x < width; x++) {
       int i = (y * width + x) * 3;
       
       if(color == 1){
           pixmap[i++] = max;
           pixmap[i++] = min;
           pixmap[i] = min;
           m_color--;
           counter1--;
           if(m_color == 0){
               color++;
               m_color = width/18;
           }

           if(counter1 == 0){
               counter2--;
               counter1 = width;
               if(counter2 == 0){
                    color=3;
                    min = min + 12;
                    counter2 = width/18;
               }               
           }
       }
       
       else if(color == 2){
           pixmap[i++] = min;
           pixmap[i++] = max;
           pixmap[i] = min;
           m_color--;
           counter1--;
          if(m_color == 0){
               color++;
               m_color = width/18;
           }
           if(counter1 == 0){
               counter2--;
               counter1 = width;
               if(counter2 == 0){
                    color=1;
                    min = min + 12;
                    counter2 = width/18;
               }               
           }
       }
       
       else if(color == 3){
           pixmap[i++] = min;
           pixmap[i++] = min;
           pixmap[i] = max;
           m_color--;
           counter1--;
          if(m_color == 0){
               color= 1;
               m_color = width/18;
           }
           max = max - 12;
           if(counter1 == 0){
               counter2--;
               counter1 = width;
               max = max + 12*18;
               if(counter2 == 0){
                    color = 2;
                    min = min + 12;
                    counter2 = width/18;
               }               
           }
       }
     }
   }   
}

void setRainbow(){
    pixmap[0] = 255;
    pixmap[1] = 255;
    pixmap[2] = 255;
    for(int y = 1; y < height ; y++) {
         for(int x = 1; x < width; x++) {
           int i = (y * width + x) * 3;
           int last = ((y-1) * width + (x-1)) * 3;
           pixmap[i++] = (pixmap[last++] + rand()%4) % 255; 
           pixmap[i++] = (pixmap[last++] + rand()%9) % 255; 
           pixmap[i] =   (pixmap[last] + rand()%16) % 255; 
         }
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
  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
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
    cout<< "1. for reading ppm"<<endl;
    cout<< "2. for creating image and then save it as a ppm"<<endl;
    cout<<"selesct:"<<endl;
    int option = 0;
    cin>>option;
    if(option == 1){
        cout<<"which image you want to load?(XXX.ppm)"<<endl;
        string filename;
        cin>>filename;
        glutInit(&argc, argv);
        glutInitWindowPosition(100, 100); // Where the window will display on-screen.
        glutInitWindowSize(width, height);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutCreateWindow("load");
        init();
        load(filename);//read ppm
        glutReshapeFunc(windowResize);
        glutDisplayFunc(windowDisplay);
        glutMouseFunc(processMouse);   
        glutMainLoop();
    } 
    
    if(option == 2){
        width = 900;
        height = 900;
        pixmap = new unsigned char[width * height * 3];
        cout<<"try input a number from [1-5]"<<endl;
        int options = 0;
        cin>>options;
        switch(options){
            case 1:
                setBlackandWhite();
                break;
            
            case 2:
                setSine();
                break;
                
            case 3:
                setCircle();
                break;
            
            case 4:
                setRainbow();
                break;
            
            case 5:
                setColorBlock();
                break;
        }
        cout<<"give a name of your saved image(XXX.ppm)"<<endl;
        string savename;
        cin>>savename;
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
        
        
        
        
        
        
        
        
    }
    




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

