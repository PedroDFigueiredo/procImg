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

GLUI_RadioGroup *radio;
int main_window;
int window2;
int UI_width  = 0;
int option = 0;

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
   UI_width = 512-vw;
}

void reshape( int x, int y ){
  glutPostRedisplay();
}

static void display(void){
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //glutSwapBuffers();
}

void control(int value){
	switch(value){
		case 1 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_curvas'");
			break;
		}
		case 2 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_spacial_filter'");
			break;	
		}
		case 3 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_spacial_filter2'");
			break;
		}
		case 4 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_morfologia'");
			break;	
		}
		case 5 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_morfologiaII'");
			break;	
		}
		case 6 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_segmentacaoI'");
			break;	
		}
		case 7 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_segmentacaoII'");
			break;	
		}

		case 8 :{
			system("gnome-terminal -x sh -c 'make -f Makefile_compress'");
			break;	
		}
	}
}

static void key(unsigned char key, int x, int y){
    switch (key) {
        case 27 :
        case 'q':
            exit(0);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	
	glutInitWindowSize(512,512);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	main_window = glutCreateWindow("Fotoshop");

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

	GLUI_Panel *io_panel = glui->add_panel( (char *) "Functions" );
	glui->add_column_to_panel( io_panel, false);
	GLUI_Button *b1 = glui->add_button_to_panel( io_panel, (char*) "Curvas de nível", 1, control);
	GLUI_Button *b2 = glui->add_button_to_panel( io_panel, (char*) "Filtros Espaciais 1", 2, control);
	GLUI_Button *b3 = glui->add_button_to_panel( io_panel, (char*) "Filtros Espaciais 2", 3, control);
	GLUI_Button *b4 = glui->add_button_to_panel( io_panel, (char*) "Morfologia 1", 4, control);
	GLUI_Button *b5 = glui->add_button_to_panel( io_panel, (char*) "Morfologia 2", 5, control);
	GLUI_Button *b6 = glui->add_button_to_panel( io_panel, (char*) "Segmentação 1", 6, control);
	GLUI_Button *b7 = glui->add_button_to_panel( io_panel, (char*) "Segmentação 2", 7, control);
	GLUI_Button *b8 = glui->add_button_to_panel( io_panel, (char*) "Compressão", 8, control);
	
	b1->set_w(50);
	b2->set_w(50);
	b3->set_w(50);
	b4->set_w(50);
	b5->set_w(50);
	b6->set_w(50);
	b7->set_w(50);

	glui->add_button((char *) "Quit", 0,(GLUI_Update_CB)exit );

	//glui->set_main_gfx_window( main_window );
	GLUI_Master.set_glutIdleFunc( idle );
	computeUIWidth(); // Compute the size of the user interface
	glutReshapeWindow(UI_width, 250);

	glui->set_main_gfx_window(main_window);

	glutMainLoop();

	return 1;
}