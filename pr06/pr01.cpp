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

void bilinear(float rgb[3], float x, float y){//found from the internet.
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

void _2D_transformation(float matrix[4]){
    changed = new unsigned char[width * height * 3];
    Point2f center = Point2f(width/2, height/2);
    float x, y;
    int index, new_index;
    float x_origin, y_origin, index_origin, tx, ty;
	float det = matrix[0]*matrix[3] - matrix[1]*matrix[2];
            //it is a inverse map and blinear anti-alising, we minus center because we want it rotation around center
    for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
            index = (j * width + i) * 3;
            x_origin = (matrix[3]*(i - center.x) - matrix[1]*(j - center.y))/det + center.x;
			y_origin = (-matrix[2]*(i - center.x) + matrix[0]*(j - center.y))/det + center.y;
            if (x_origin >= 0 && x_origin < width && y_origin >= 0 && y_origin < height){
                float rgb[3];
                index = (j * width + i) * 3;
                bilinear(rgb, x_origin, y_origin);
                changed[index++] = rgb[0];
                changed[index++] = rgb[1];
                changed[index]   = rgb[2];
            }
        }
    }
}

void Mirror(int h_v){
    float MirrorMatrix[4];    
	MirrorMatrix[0] = -(float)h_v;
	MirrorMatrix[1] = 0;
	MirrorMatrix[2] = 0;
	MirrorMatrix[3] = (float)h_v;
    _2D_transformation(MirrorMatrix);
}


void Rotation(float angle){
    float theta = (angle * M_PI)/180.0;
    float rotationMatrix[4]; ;
    rotationMatrix[0] = cos(theta);
    rotationMatrix[1] = -sin(theta);
    rotationMatrix[2] = sin(theta);
    rotationMatrix[3] = cos(theta);
    _2D_transformation(rotationMatrix);
    
}

void Shear(float a, float b, float c, float d){
    float ShearMatrix[4]; ;
    ShearMatrix[0] = a;
	ShearMatrix[1] = b;
	ShearMatrix[2] = c;
	ShearMatrix[3] = d;
     _2D_transformation(ShearMatrix);
}


void Scaling(float horizon_scale, float vertical_scale){
    float SscalingMatrix[4]; ;
    SscalingMatrix[0] = horizon_scale;
	SscalingMatrix[1] = 0;
	SscalingMatrix[2] = 0;
	SscalingMatrix[3] = vertical_scale;
     _2D_transformation(SscalingMatrix);
}

void _3D_transformation(float matrix[9]){
    float x, y, z;
    changed = new unsigned char[width * height * 3];
    int index;
    for(float i = 0;i < width;i++){
        for(float j = 0; j < height; j++){
            x = matrix[0] * (i) + matrix[1]*(j) + matrix[2];
            y = matrix[3] * (i) + matrix[4]*(j) + matrix[5];
            z = matrix[6] * (i) + matrix[7]*(j) + matrix[8];
            x = x/z;
            y = y/z;
            int res_x = (int)(x +0.5);
            int res_y = (int)(y +0.5);
            //it is a inverse map and blinear anti-alising
            if (res_x >= 0 && res_x < width && res_y >= 0 && res_y < height){
                index = (j * width + i) * 3;
                float rgb[3];
                bilinear(rgb, res_x, res_y);
                changed[index++] = rgb[0];
                changed[index++] = rgb[1];
                changed[index]   = rgb[2];
            }
        }
    }
}



void Translation(int x, int y){
    float transMatrix[9];
    //directly make the inverse matrix
 	transMatrix[0] = 1;
	transMatrix[1] = 0;
	transMatrix[2] = -x;
	transMatrix[3] = 0;
	transMatrix[4] = 1;
	transMatrix[5] = -y;
	transMatrix[6] = 0;
	transMatrix[7] = 0;
	transMatrix[8] = 1;   
    _3D_transformation(transMatrix);       
}




void Prespective(float a, float b){
    a =  a / ((float)width);
    b =  b / ((float)height);
    //directly make the inverse matrix
    float PrespectiveMatrix[9];
 	PrespectiveMatrix[0] = 1;
	PrespectiveMatrix[1] = 0;
	PrespectiveMatrix[2] = 0;
	PrespectiveMatrix[3] = 0;
	PrespectiveMatrix[4] = 1;
	PrespectiveMatrix[5] = 0;
	PrespectiveMatrix[6] = -a;
	PrespectiveMatrix[7] = -b;
	PrespectiveMatrix[8] = 1;   
    _3D_transformation(PrespectiveMatrix);       
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



void take_options(int option){
    float a = 0.5, b = 0.9 , c = 0.3, d = 0.7;
    float h_scale = 1.5, v_scale = 1.5;
    float angle = 45;
    int h_v = 0; 
    int move_x, move_y;
    switch(option){
            case 1:
                cout<<"input a angle(0 for default): "<<endl;
                cin>>angle;
                if(angle == 0){angle = 45;}
                Rotation(angle);
                save("Rotation.ppm");
                break;
            
            case 2:
                cout<<"Vertical Mirror(0) or Horizontal Mirror(1)"<<endl;
                cin>>h_v;
                if(h_v == 0){
                    Mirror(-1);
                    save("VerticalMirror.ppm");
                }
                else{
                    Mirror(1);  
                    save("HorizontalMirror.ppm");
                }
                break;
            
            case 3:
                cout<<"input the four number for transformation(0,0,0,0 for default)"<<endl;
                cin >> a;
                cin >> b;
                cin >> c;
                cin >> d;
                if(a == 0 && b == 0 && c == 0 && d == 0){
                    Shear(0.5,0.9,0.3,1.2);
                }
                else{
                    Shear(a,b,c,d);
                }
                save("shear.ppm");
                break;
                
            case 4:
                cout<<"input horizonal scale: "<<endl;
                cin>>h_scale;
                cout<<"input vertical scale: "<<endl;
                cin>>v_scale;
                Scaling(h_scale,v_scale);
                save("scale.ppm");
                break;
                
            case 5:
                cout<<"input x for moving toward horizonal direction(negative number for left):"<<endl;
                cin>>move_x;
                cout<<"input y for moving toward vertical direction(negative number for down):"<<endl;
                cin>>move_y;               
                Translation(move_x,move_y);
                save("transformation.ppm");
                break;
            
             case 6:                
                cout<<"input a:"<<endl;
                cin>>move_x;
                cout<<"input b:"<<endl;
                cin>>move_y;    
                Prespective(a, b);
                save("prespective.ppm");
                break;           
            
        }
}











int main(int argc, char *argv[])
{
    cout<<"select the image you want to do the transformation(ppm): "<<endl;
    string filename;
    cin>>filename;
    cout<<"what you want to do with the image"<<endl;
    cout<<"1.Rotation\n2.Mirror\n3.Shear\n4.Scale\n5.Translation\n6.Prespective"<<endl;
    int option = 1;
    cin >> option;

    
    
    load(filename);
    take_options(option);    
    
    
    
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

