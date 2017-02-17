/*
  Name:        consolidacao.cpp
  Author:      Piroca Master
  Author:      Piroca Subordinada
  Date:        15/02/2017
  Last Update: 
  Description: Consolidação de todos os trabalho até agora 
*/

#include "pixelLab.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <stdlib.h>
#include "GL/glui.h"
#include "GL/glut.h"
#include <iostream>
#include <math.h>

using namespace std;

// Image Objects
PixelLab *img    = NULL;
PixelLab *imgMod = NULL;
PixelLab *imgMod2 = NULL;

GLUI_RadioGroup *radio;
int main_window;
int window2;
int UI_width  = 0;
int option = 0;
int brightness = 0;
int prevBrightness = 0;

char input[512];
char output[512];
/**
	MORFOLOGIA I -> FUNÇÕES E VARIAVEIS
**/
void erosao(PixelLab *imgOri, PixelLab *imgCp);
void dilatacao(PixelLab *imgOri, PixelLab *imgCp);
void erosao(PixelLab *imgOri, PixelLab *imgCp);
void dilatacao(PixelLab *imgOri, PixelLab *imgCp);
int elemEstrctSize = 3;
int **elemEstrct;

/***************************************************
*/

void idle(){
	if ( glutGetWindow() != main_window)
		glutSetWindow(main_window);
	glutPostRedisplay();
}

void computeUIWidth(){
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = img->GetWidth()-vw;
}

void reshape( int x, int y ){
  glutPostRedisplay();
}

static void display(void){
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glRasterPos2i(-1, -1); // Fix raster position
   imgMod->ViewImage();
   GLUI_Master.auto_set_viewport();

   glutSwapBuffers();
}
static void display2(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch(option){
		case 2 :{
			erosao(imgMod, imgMod2);
			imgMod2->ViewImage();
		}break;
		case 3 :{
			dilatacao(imgMod, imgMod2);
			imgMod2->ViewImage();
		}break;
	}

	//cout<<"display2 "<<elemEstrctSize<<"\n";
	glutSwapBuffers();
	glutPostRedisplay();
}
void modifyImage(char channel){
   if(img)
   {
      for(int y = 0; y < img->GetHeight(); y++)
      {
         for(int x = 0; x < img->GetWidth(); x++)
         {
            if(channel == 'A') imgMod->SetRGB( x, y, img->GetR(x,y),img->GetG(x,y),img->GetB(x,y));
            if(channel == 'R') imgMod->SetRGB( x, y, img->GetR(x,y),0,0);
            if(channel == 'G') imgMod->SetRGB( x, y, 0,img->GetG(x,y),0);
            if(channel == 'B') imgMod->SetRGB( x, y, 0,0,img->GetB(x,y));
         }
      }
   }
}

void control(int value){
	cout<<"value:"<<value<<" option:"<<option<<"\n";
	switch(value){
		case 1 :{

		}break;
		case 2 :{
			
		}break;
		case 3 :{
			
		}
		case 4 :{
			
		}break;
		case 5 :{
			
		}break;
	}
}
static void key(unsigned char key, int x, int y){
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
        break;
        case 'm':
            imgMod->Copy(img);
            modifyImage('R');
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	strcpy(input, "figs/ex1.png");
	strcpy(output, "figs/output.png");

	img    = new PixelLab(input);
	imgMod = new PixelLab();
	imgMod2 = new PixelLab();
	imgMod->Copy(img);
	imgMod2->Copy(img);

	glutInitWindowSize(img->GetWidth(),img->GetHeight());
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	main_window = glutCreateWindow("Trabalho de merda do filha da puta do Bruno! Nome de merda tbm pqp");

	glutKeyboardFunc(key);
	glutIdleFunc( idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// GLUI
	// Use the first line above to get the interface in the same window as the graphics
	// Use the second line (comment the first) to get the interface in a separeted window
	GLUI *glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );
	//GLUI *glui = GLUI_Master.create_glui( "" );

	GLUI_Master.set_glutReshapeFunc(reshape);

	GLUI_Panel *io_panel = glui->add_panel( (char *) "Input/Output" );
	GLUI_EditText *edit1, *edit2;
	edit1 = glui->add_edittext_to_panel (io_panel, (char *) "", GLUI_EDITTEXT_TEXT, input);
	edit2 = glui->add_edittext_to_panel (io_panel, (char *) "", GLUI_EDITTEXT_TEXT, output);
	edit1->set_w(185);
	edit2->set_w(185);
	glui->add_column_to_panel( io_panel, false);
	GLUI_Button *b1 = glui->add_button_to_panel( io_panel, (char*) "Load", 1, control );
	GLUI_Button *b2 = glui->add_button_to_panel( io_panel, (char*) "Save", 2, control );
	b1->set_w(50);
	b2->set_w(50);

	GLUI_Panel *channel_panel = glui->add_panel((char *) "Trabalhos" );
	glui->add_statictext_to_panel( channel_panel, (char *) "Warning: Channels change will reset image");
	radio = glui->add_radiogroup_to_panel(channel_panel, &option, 3, control);
	glui->add_radiobutton_to_group( radio, (char *) "Segmentação I" );
	glui->add_radiobutton_to_group( radio, (char *) "Segmentação II" );
	glui->add_radiobutton_to_group( radio, (char *) "Morfologia I" );
	glui->add_radiobutton_to_group( radio, (char *) "Morfologia II" );
	glui->add_radiobutton_to_group( radio, (char *) "Compressão" );
	glui->add_separator_to_panel( channel_panel );
	GLUI_Spinner* spinner;
	spinner = glui->add_spinner_to_panel( channel_panel, (char*)"Increase/Decrease brightness", GLUI_SPINNER_INT, &brightness, 4, control);

	glui->add_button((char *) "Quit", 0,(GLUI_Update_CB)exit );

	glui->set_main_gfx_window( main_window );
	GLUI_Master.set_glutIdleFunc( idle );
	computeUIWidth(); // Compute the size of the user interface
	glutReshapeWindow(imgMod->GetWidth() + UI_width, imgMod->GetHeight());


	window2 = glutCreateWindow("Segmentation Window");

	   // Display Window
	glutSetWindow (window2); // Change current window to 2
	glutDisplayFunc(display2);
	glutReshapeWindow(img->GetWidth(),img->GetHeight());
	glutPositionWindow(img->GetWidth() + 60, 30);

	glutMainLoop();

	return 1;
}

void erosao(PixelLab *imgOri, PixelLab *imgCp){
	int tempx, tempy, raio = elemEstrctSize/2;
	int matrix[elemEstrctSize][elemEstrctSize];

	for(int x = 0; x < elemEstrctSize; x++)
		for(int y = 0; y < elemEstrctSize; y++){
			matrix[x][y] = pow((x-raio ),2) + pow((y-raio),2) > pow(raio,2) ? 0 : 1;
		}

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

	}
}

void dilatacao(PixelLab *imgOri, PixelLab *imgCp){
	int tempx, tempy, raio = elemEstrctSize/2;
	int matrix[elemEstrctSize][elemEstrctSize];

	for(int x = 0; x < elemEstrctSize; x++)
		for(int y = 0; y < elemEstrctSize; y++){
			matrix[x][y] = pow((x-raio ),2) + pow((y-raio),2) > pow(raio,2) ? 0 : 1;

		}

	bool hit = false;
	int aux = 0;

	for(int x = 0; x < imgOri->GetWidth(); x++)

		for(int y = 0; y < imgOri->GetHeight(); y++){

			for (int i = 0; i < elemEstrctSize; ++i){
				for (int j = 0; j < elemEstrctSize; ++j){
					tempx = x-raio+i;
					tempy = y-raio+j;

					if(!(tempx < 0 || tempx >= imgOri->GetWidth()) && !(tempy < 0 || tempy >= imgOri->GetHeight())){
						aux = (int)imgOri->GetGrayValue(tempx, tempy) > 120 ? 255 : 0;

					if(matrix[i][j] != 0 && aux != 0){
						hit = true;
						}
					}

				}
				if(hit){
					imgCp->SetGrayValue(x, y, 1);
				}
				else
					imgCp->SetGrayValue(x, y, (int)imgOri->GetGrayValue(x,y));

				hit = false;
			}
		}
}