/*
  Name:        trab01.cpp
  Author:      Pedro Diniz Figueiredo
  Date:        20/09/2016
  Last Update: 21/09/2016
  Description: Global Operations.
*/

#include "pixelLab.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

// Image Objects
PixelLab *imgCopy = NULL, *imgOriginal = NULL, *imgCopy2 = NULL;
int window1 = 0;
int window2 = 0;
int filtro = 1;

void erosao(PixelLab *imgOri, PixelLab *imgCp);
void dilatacao(PixelLab *imgOri, PixelLab *imgCp);
void subtracao(PixelLab *a, PixelLab *b);

int elemEstrctSize = 3;

int **elemEstrct;
char *input_img;

void idle()
{
   glutPostRedisplay();
}

static void display1(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   imgOriginal->ViewImage();
   glutSwapBuffers();
   //glutPostRedisplay();
}
static void display2(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   
    erosao(imgOriginal, imgCopy);
         
    //imgCopy->ViewImage();
     // imgCopy2->Copy(imgCopy);
    subtracao(imgCopy2, imgCopy);
    imgCopy2->ViewImage();
   

   //cout<<"display2 "<<elemEstrctSize<<"\n";
   glutSwapBuffers();
   glutPostRedisplay();
}
void erosao(PixelLab *imgOri, PixelLab *imgCp){
   int tempx, tempy, raio = elemEstrctSize/2;
   int matrix[elemEstrctSize][elemEstrctSize];

int matrix2[8][8] =
{
{0,0,0,0,0,0,0,0},
{0,0,0,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{0,1,1,1,1,0,0,0},
{0,1,1,1,0,0,0,0},
{0,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
};
int matrix3[8][8];
 
   for(int x = 0; x < elemEstrctSize; x++)
      for(int y = 0; y < elemEstrctSize; y++){
         //matrix[x][y] = pow((x-raio ),2) + pow((y-raio),2) > pow(raio,2) ? 0 : 1;
         matrix[x][y]=1;

      }


  /*  for(int x = 0; x < elemEstrctSize; x++){
        for(int y = 0; y < elemEstrctSize; y++){
            cout<<matrix[x][y]<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";*/


   bool fit = true;
   int aux = 0;
   
   for(int x = 0; x < imgOri->GetWidth(); x++)
      for(int y = 0; y < imgOri->GetHeight(); y++){



         for (int i = 0; i < elemEstrctSize; ++i){
            for (int j = 0; j < elemEstrctSize; ++j){

               tempx = x-raio+i;
               tempy = y-raio+j;
                 
               if(!(tempx < 0 || tempx >= imgOri->GetWidth()) && !(tempy < 0 || tempy >= imgOri->GetHeight())){
                  aux = (int)imgOri->GetGrayValue(tempx, tempy) != 0 ? 255 : 0;
                  if(matrix[i][j] != 0 && aux == 0){
                     fit = false;
                  }
               }
            }
         }

         if(fit){

            imgCp->SetGrayValue(x, y, (int)imgOri->GetGrayValue(x,y));
         }
         else
            imgCp->SetGrayValue(x, y, 0);

          fit = true;

        // imgCp->SetGrayValue(x, y, (int)imgOri->GetGrayValue(x,y)/2 > 70 ? 255 : 0);
      }
}

void dilatacao(PixelLab *imgOri, PixelLab *imgCp){
   int tempx, tempy, raio = elemEstrctSize/2;
   int matrix[elemEstrctSize][elemEstrctSize];

    for(int x = 0; x < elemEstrctSize; x++)
      for(int y = 0; y < elemEstrctSize; y++){
         matrix[x][y] = pow((x-raio ),2) + pow((y-raio),2) > pow(raio,2) ? 0 : 1;

      }

/*   for(int x = 0; x < elemEstrctSize; x++){
      for(int y = 0; y < elemEstrctSize; y++){
         cout<<matrix[x][y]<<" ";
      }
      cout<<"\n";
   }*/

   bool hit = false;
   int aux = 0;
   
	for(int x = 0; x < imgOri->GetWidth(); x++)

		for(int y = 0; y < imgOri->GetHeight(); y++){

		for (int i = 0; i < elemEstrctSize; ++i){
			for (int j = 0; j < elemEstrctSize; ++j){
				tempx = x-raio+i;
				tempy = y-raio+j;

				if(!(tempx < 0 || tempx >= imgOri->GetWidth()) && !(tempy < 0 || tempy >= imgOri->GetHeight())){
					aux = (int)imgOri->GetGrayValue(tempx, tempy) != 0 ? 255 : 0;

					if(matrix[i][j] != 0 && aux != 0){
						hit = true;
					}
				}

			}
			if(hit){
				imgCp->SetGrayValue(x, y, 255);
			}
				//imgCp->SetGrayValue(x, y, (int)imgOri->GetGrayValue(x,y));

			hit = false;

			// imgCp->SetGrayValue(x, y, (int)imgOri->GetGrayValue(x,y)/2 > 70 ? 255 : 0);
		}
	}
}

//B(a)
void subtracao(PixelLab *a, PixelLab *b){

    for(int x = 0; x < a->GetWidth(); x++) 
        for(int y = 0; y < a->GetHeight(); y++){
            
            if((int)b->GetGrayValue(x,y) > 0){
                a->SetGrayValue(x, y, 0);
            }
        }

}

// Keyboard
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
      case '+' :
         elemEstrctSize += elemEstrctSize >= 17 ? 0 : 2;
         break;
      case '-' :
         elemEstrctSize -= elemEstrctSize <= 1 ? 0 : 2;
         break;
      case 'q':
         exit(0);
      break;
    }

   cout<<"key:"<<key<<" size:"<<elemEstrctSize<<"\n";

    glutPostRedisplay();
}

// Special Keys callback
void specialKeys(int key, int x, int y)
{
    switch(key)
   {
      case GLUT_KEY_LEFT:
      break;
      case GLUT_KEY_RIGHT:
      break;
      case GLUT_KEY_UP:
      break;
      case GLUT_KEY_DOWN:
      break;
   }
   
   glutPostRedisplay();
}

void checkMouseCilckArea(int x, int y){

}
// Mouse callback - Capture mouse click in the brightness window
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON)
      if(state == GLUT_DOWN){
      }
}

// Motion callback - Capture mouse motion when left button is clicked
void motion(int x, int y )
{
   checkMouseCilckArea(x,y);
   //cout<<x<<" "<<y<<"\n";
}

void init()
{
   imgOriginal = new PixelLab();
   //imgOriginal->Read("ex1.png");
   imgOriginal->Read(input_img);
   //imgOriginal->Read("../PixelLab/figs/split.example2.png");

   imgCopy = new PixelLab();
   imgCopy->Copy(imgOriginal);

   imgCopy2 = new PixelLab();
   imgCopy2->Copy(imgOriginal);

   //printf("Move the dot by pressing the directional keys or by the mouse.\nChange the dot to be moved by pressing 0 to 3.\n");
}

int main(int argc, char *argv[])
{
   input_img = argv[1];

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);   
   // Init image objects
   init();

   // Create two windows
   window2 = glutCreateWindow("Segmentation Window");
   window1 = glutCreateWindow("Morphology Window");

   ///////////////////////////////////////////////////////////
   // Control Window
   //lutSetWindow (window1);   // Change current window to 1 (all callbacks will be set to this window)
  // glutDisplayFunc(display1);
   glutPositionWindow(100, 30);
   glutReshapeWindow(256, 256);
   //glutMouseFunc( mouse );
   //glutMotionFunc( motion );
   glutSpecialFunc(specialKeys);
   glutKeyboardFunc(key);

   glutSetWindow (window1); // Change current window to 2
   glutDisplayFunc(display1);
   glutReshapeWindow(imgOriginal->GetWidth(),imgOriginal->GetHeight());
   glutPositionWindow(30, 30);
   
   ///////////////////////////////////////////////////////////
   // Display Window
   glutSetWindow (window2); // Change current window to 2
   glutDisplayFunc(display2);
   glutReshapeWindow(imgOriginal->GetWidth(),imgOriginal->GetHeight());
   glutPositionWindow(imgOriginal->GetWidth() + 60, 30);

   glutMainLoop();

   return 1;
}
int rgbToInt(int r, int g, int b){
   return r + (g*256) + (b*256*256);
}
