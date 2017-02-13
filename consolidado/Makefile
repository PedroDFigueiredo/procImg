all: 
	g++ -Wall -Wextra -w -fexceptions -g -I  ../PixelLab/include -I ../PixelLab/src/PixelLab -I ../PixelLab/bin -c consolidado.cpp -o consolidado.o
	g++  -w -o consolidado consolidado.o   -lGL -lGLU -lglut  -lglui ../PixelLab/lib/libpixelLab_linux.a ../PixelLab/lib/libpng12_linux64.a -lz
	./consolidado 

