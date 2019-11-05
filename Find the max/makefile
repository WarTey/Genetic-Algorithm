main: main.o ga/ga.o libisentlib.a
#	gcc -Wall main.o ga/ga.o -o main libisentlib.a -lm -lglut -lGL -lX11
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
	gcc -Wall main.o ga/ga.o -o main libisentlib.a -lm -framework OpenGL -framework GLUT

main.o: main.c lib/GfxLib.h lib/ESLib.h ga/ga.h
	gcc -Wall -c main.c

ga.o: ga/ga.c ga/ga.h  lib/GfxLib.h lib/ESLib.h
	gcc -Wall -c ga/ga.c

libisentlib.a: lib/ErreurLib.o lib/ESLib.o lib/GfxLib.o
	ar r libisentlib.a lib/ErreurLib.o lib/ESLib.o lib/GfxLib.o
	ranlib libisentlib.a

ESLib.o: lib/ESLib.c lib/ESLib.h lib/ErreurLib.h
	gcc -Wall -O2 -c lib/ESLib.c

ErreurLib.o: lib/ErreurLib.c lib/ErreurLib.h
	gcc -Wall -O2 -c lib/ErreurLib.c

GfxLib.o: lib/GfxLib.c lib/GfxLib.h lib/ESLib.h
#	gcc -Wall -O2 -c lib/GfxLib.c -I/usr/include/GL
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
	gcc -Wall -O2 -c lib/GfxLib.c -Wno-deprecated-declarations

zip:
	tar -cvzf libisentlib.tgz *.[ch] *.bmp *.pdf makefile

clean:
	rm -f *~ *.o lib/*.o ga/*.o

deepclean: clean
	rm -f main libisentlib.a

