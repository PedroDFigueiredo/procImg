
g++ -Wall -Wextra -w -fexceptions -g -I  ../../include -I ../PixelLab -I ../../bin -c /home/pedro/desenvolvimento/cg/processamentoimagens/PixelLab/src/Examples/test10.1.Glui.Example.cpp -o Example.o

g++  -w -o Example Example.o -lGL -lGLU -lglut  -lglui ../../lib/libpixelLab_linux.a ../../lib/libpng12_linux64.a -lz

./Example