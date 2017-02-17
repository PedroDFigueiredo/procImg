#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <cmath>

using namespace std;

int window1 = 0;
int window2 = 0;

int op = 0;

PixelLab *img1 = NULL, *img2 = NULL;

int windth, height, UI_width;

int option_baixa = 1;
int option_alta = 1;

void displayImage1(void);
void displayImage2(void);
void mean(int);
void gaus(int);
void sobel(int);
void prewitt(int);
void keyboardPress(unsigned char, int, int);
void mouse(int, int, int, int);

int mat5x5[5][5] = {
	{1, 4, 7, 4, 1},
	{4, 16, 26, 16, 4},
	{7, 26, 41, 26, 7},
	{4, 16, 26, 16, 4},
	{1, 4, 7, 4, 1}
};

int mat7x7[7][7] = {
	{0, 0, 0, 5, 0, 0, 0},
	{0, 5, 18, 32, 18, 5, 0},
	{0, 18, 64, 100, 64, 18, 0},
	{5, 32, 100, 100, 100, 32, 5},
	{0, 18, 64, 100, 64, 18, 0},
	{0, 5, 18, 32, 18, 5, 0},
	{0, 0, 0, 5, 0, 0, 0}
};

int mat3x3[3][3] = {
	{1, 2, 1},
	{2, 4, 2},
	{1, 2, 1}
};

int matHoriPre[3][3] = {
	{-1, -1, -1},
	{0, 0, 0},
	{1, 1, 1}
};

int matVertPre[3][3] = {
	{-1, 0, 1},
	{-1, 0, 1},
	{-1, 0, 1}
};

int matHoriSob[3][3] = {
	{-1, -2, -1},
	{0, 0, 0},
	{1, 2, 1}
};

int matVertSob[3][3] = {
	{-1, 0, 1},
	{-2, 0, 2},
	{-1, 0, 1}
};

void init (void) {
	img1 = new PixelLab();
	img1->Read("figs/lena.png");

  img2 = new PixelLab();
	img2->Read("figs/lena.png");
}

void computeUIWidth() {
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = vw;
}

void idle() {
   if (glutGetWindow() != window1)
      glutSetWindow(window1);
   glutPostRedisplay();
}

void reshape( int x, int y ) {
  glutPostRedisplay();
}

void controlBaixa(int value) {
}

void controlAlta(int value) {

}

// Main Function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();

	// Create two windows
	window1 = glutCreateWindow("Image");
  window2 = glutCreateWindow("Resultado");

  //Janela da imagem
	glutSetWindow (window1);
	glutDisplayFunc(displayImage1);
  glutKeyboardFunc(keyboardPress);
	glutPositionWindow(300, 356);

	GLUI *glui = GLUI_Master.create_glui_subwindow(window1, GLUI_SUBWINDOW_RIGHT);

  GLUI_Master.set_glutReshapeFunc(reshape);

  GLUI_Panel *passa_baixa_panel = glui->add_panel( (char *) "Passa-Baixa" );
  glui->add_column_to_panel(passa_baixa_panel, false);

  GLUI_Button *b1 = glui->add_button_to_panel(passa_baixa_panel, (char*) "Media", option_baixa, mean);
  GLUI_Button *b2 = glui->add_button_to_panel(passa_baixa_panel, (char*) "Gaussiana", option_baixa, gaus);

  GLUI_Panel *passa_alta_panel = glui->add_panel( (char *) "Passa-Alta" );
  glui->add_column_to_panel(passa_alta_panel, false);

	GLUI_Button *b3 = glui->add_button_to_panel(passa_alta_panel, (char*) "Sobel", 2, sobel);
  GLUI_Button *b4 = glui->add_button_to_panel(passa_alta_panel, (char*) "Prewitt", 2, prewitt);
  b2->set_w(50);
	b3->set_w(50);
  b4->set_w(50);

  GLUI_Panel *channel_panel = glui->add_panel((char *) "Passa-baixa" );

  GLUI_RadioGroup *radio_baixa = glui->add_radiogroup_to_panel(channel_panel, &option_baixa, 0, controlBaixa);
  glui->add_radiobutton_to_group(radio_baixa, (char *) "3x3");
  glui->add_radiobutton_to_group(radio_baixa, (char *) "5x5");
  glui->add_radiobutton_to_group(radio_baixa, (char *) "7x7");

  GLUI_Panel *alta_panel = glui->add_panel((char *) "Passa-alta" );

  GLUI_RadioGroup *radio_alta = glui->add_radiogroup_to_panel(alta_panel, &option_alta, 0, controlAlta);
  glui->add_radiobutton_to_group(radio_alta, (char *) "vertical" );
  glui->add_radiobutton_to_group(radio_alta, (char *) "horizontal" );

  glui->set_main_gfx_window(window1);
  GLUI_Master.set_glutIdleFunc(idle);
  computeUIWidth(); // Compute the size of the user interface
  glutReshapeWindow(img1->GetWidth() + UI_width, img1->GetHeight());

  //Janela da filtro aplicado
	glutSetWindow (window2); //Change current window to 2
	glutDisplayFunc(displayImage2);
	glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
	glutPositionWindow(100, 356);

	glutMainLoop();

	return 0;
}

void mean(int mask) {
  uByte r, g, b;
	mask = option_baixa + 1;
	for(int x = mask; x < img1->GetWidth() - mask; x++) {
		for(int y = mask; y < img1->GetHeight() - mask; y++) {
      //Percorre a submatriz
			int _r = 0, _g = 0, _b = 0;
      for (int i = x-mask; i <= x+mask; i++) {
        for (int j = y-mask; j <= y+mask; j++) {
          img1->GetRGB(i, j, r, g, b);
          _r += r; _g += g; _b += b;
        }
      }
			float aux = pow(mask * 2 + 1, 2);
      _r /= aux; _g /= aux; _b /= aux;
      img2->SetRGB(x, y, (uByte)_r, (uByte)_g, (uByte)_b);
    }
  }
}

void gaus(int mask) {
	uByte r, g, b;
	mask = option_baixa + 1;

	for(int x = mask; x < img1->GetWidth() - mask; x++) {
		for(int y = mask; y < img1->GetHeight() - mask; y++) {
      //Percorre a submatriz
			int _r = 0, _g = 0, _b = 0;
			int m = 0, n = 0, sum = 0;
      for (int i = x-mask; i < x+mask; i++) {
        for (int j = y-mask; j < y+mask; j++) {
          int k = 0;
					img1->GetRGB(i, j, r, g, b);
					if (mask == 1) k = mat3x3[m][n];
					if (mask == 2) k = mat5x5[m][n];
					if (mask == 3) k = mat7x7[m][n];
          _r += r * k; _g += g * k; _b += b * k;
					sum += k;
					n++;
        }
				m++;
      }
			_r /= sum; _g /= sum; _b /= sum;
      img2->SetRGB(x, y, (uByte)_r, (uByte)_g, (uByte)_b);
    }
  }
}

void sobel(int mask) {
	uByte r, g, b;

	for(int x = mask; x < img1->GetWidth() - mask; x++) {
		for(int y = mask; y < img1->GetHeight() - mask; y++) {
      //Percorre a submatriz
			int _r = 0, _g = 0, _b = 0;
			int m = 0, n = 0;
      for (int i = x-mask; i < x+mask; i++) {
        for (int j = y-mask; j < y+mask; j++) {
          int k = 0;
					img1->GetRGB(i, j, r, g, b);
					if (option_alta == 0) k = matVertSob[m][n];
					if (option_alta == 1) k = matHoriSob[m][n];
					_r += r * k; _g += g * k; _b += b * k;
					n++;
        }
				m++;
      }
      img2->SetRGB(x, y, (uByte)_r, (uByte)_g, (uByte)_b);
    }
  }
}

void prewitt(int mask) {
	uByte r, g, b;

	for(int x = mask; x < img1->GetWidth() - mask; x++) {
		for(int y = mask; y < img1->GetHeight() - mask; y++) {
      //Percorre a submatriz
			int _r = 0, _g = 0, _b = 0;
			int m = 0, n = 0, sum = 0;
      for (int i = x-mask; i < x+mask; i++) {
        for (int j = y-mask; j < y+mask; j++) {
          int k = 0;
					img1->GetRGB(i, j, r, g, b);
					if (option_alta == 0) k = matVertPre[m][n];
					if (option_alta == 1) k = matHoriPre[m][n];
					_r += r * k; _g += g * k; _b += b * k;
					n++;
        }
				m++;
      }
      img2->SetRGB(x, y, (uByte)_r, (uByte)_g, (uByte)_b);
    }
  }
}

void displayImage1(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	glutSwapBuffers();
  glutPostRedisplay();
}

void displayImage2(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	glutSwapBuffers();
  glutPostRedisplay();
}

// Keyboard callback
void keyboardPress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
			break;
		default:
			printf("\nUma tecla não mapeada foi pressionada");
			break;
	}
}
