main: main.o lib/libisentlib.a
#	gcc -Wall main.o -o main lib/libisentlib.a -lm -lglut -lGL -lX11
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
	gcc -Wall main.o -o main lib/libisentlib.a -lm -framework OpenGL -framework GLUT

main.o: main.c lib/GfxLib.h lib/BmpLib.h lib/ESLib.h
	gcc -Wall -c main.c

libisentlib.a: lib/BmpLib.o lib/ErreurLib.o lib/ESLib.o lib/GfxLib.o lib/OutilsLib.o lib/SocketLib.o lib/ThreadLib.o lib/TortueLib.o lib/VectorLib.o lib/WavLib.o
	ar r lib/libisentlib.a lib/BmpLib.o lib/ErreurLib.o lib/ESLib.o lib/GfxLib.o lib/OutilsLib.o lib/SocketLib.o lib/ThreadLib.o lib/TortueLib.o lib/VectorLib.o lib/WavLib.o
	ranlib lib/libisentlib.a

BmpLib.o: lib/BmpLib.c lib/BmpLib.h lib/OutilsLib.h
	gcc -Wall -O2 -c lib/BmpLib.c

ESLib.o: lib/ESLib.c lib/ESLib.h lib/ErreurLib.h
	gcc -Wall -O2 -c lib/ESLib.c

ErreurLib.o: lib/ErreurLib.c lib/ErreurLib.h
	gcc -Wall -O2 -c lib/ErreurLib.c

GfxLib.o: lib/GfxLib.c lib/GfxLib.h lib/ESLib.h
#	gcc -Wall -O2 -c lib/GfxLib.c -I/usr/include/GL
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
	gcc -Wall -O2 -c lib/GfxLib.c -Wno-deprecated-declarations

OutilsLib.o: lib/OutilsLib.c lib/OutilsLib.h
	gcc -Wall -O2 -c lib/OutilsLib.c

SocketLib.o: lib/SocketLib.c lib/SocketLib.h
	gcc -Wall -O2 -c lib/SocketLib.c

ThreadLib.o: lib/ThreadLib.c lib/ThreadLib.h
	gcc -Wall -O2 -c lib/ThreadLib.c

TortueLib.o: lib/TortueLib.c lib/TortueLib.h lib/GfxLib.h
	gcc -Wall -O2 -c lib/TortueLib.c

VectorLib.o: lib/VectorLib.c lib/VectorLib.h
	gcc -Wall -O2 -c lib/VectorLib.c -msse3

WavLib.o: lib/WavLib.c lib/WavLib.h lib/OutilsLib.h
#	gcc -Wall -O2 -c lib/WavLib.c -Wno-unused-result
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
	gcc -Wall -O2 -c lib/WavLib.c

zip:
	tar -cvzf lib/libisentlib.tgz *.[ch] *.bmp *.pdf makefile

clean:
	rm -f *~ *.o lib/*.o

deepclean: clean
	rm -f main lib/libisentlib.a

