/*
  Name:        test04.Modify.Loaded.Image.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Date:        04/07/2009
  Last Update: 07/09/2016
  Description: Load an image from file and modify its data.
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "GL/glut.h"
#include <stdlib.h>

// Image Object
PixelLab *img = NULL;

void idle()
{
   glutPostRedisplay();
}

static void display(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   img->ViewImage();
   glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
        break;
    }
    glutPostRedisplay();
}

void modifyImage()
{
   pixel **m;
   // Allocate Matrix
   img->AllocatePixelMatrix(&m, img->GetHeight(), img->GetWidth() );
   // Get Image Data
   img->GetDataAsMatrix(m);

   uByte aux;
   if(img)
   {
      for(int y = 0; y < img->GetHeight(); y++)
         for(int x = 0; x < img->GetWidth(); x++)
         {
            aux = m[y][x].R;
            m[y][x].R = m[y][x].G;
            m[y][x].G = m[y][x].B;
            m[y][x].B = aux;
         }
   }
   // Update PixelLab Object
   img->SetDataAsMatrix(m);
   // Deallocate Matrix
   img->DeallocatePixelMatrix(&m, img->GetHeight(), img->GetWidth() );
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   img = new PixelLab();
   img->Read("../figs/woman.png");

   // Modifica os canais RGB da imagem
   modifyImage();

   glutInitWindowSize(img->GetWidth(),img->GetHeight());
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutCreateWindow("Convert Data to Pixel");

   glutKeyboardFunc(key);
   glutIdleFunc( idle);
   glutDisplayFunc(display);

   glutMainLoop();

   return 1;
}
