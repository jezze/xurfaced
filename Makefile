all: halo

halo:
	clang -Wall -I./include `pkg-config --cflags x11 xcomposite cairo` -c src/display.c -o src/display.o
	clang -Wall -I./include `pkg-config --cflags x11 xcomposite cairo` -c src/event.c -o src/event.o
	clang -Wall -I./include `pkg-config --cflags x11 xcomposite cairo` -c src/halo.c -o src/halo.o
	clang -Wall -I./include `pkg-config --cflags x11 xcomposite cairo` -c src/menu.c -o src/menu.o
	clang -Wall -I./include `pkg-config --cflags x11 xcomposite cairo` -c src/surface.c -o src/surface.o
	clang -Wall -I./include `pkg-config --cflags x11 xcomposite cairo` -c src/window.c -o src/window.o
	clang -Wall `pkg-config --libs x11 xcomposite cairo` src/display.o src/event.o src/halo.o src/menu.o src/surface.o src/window.o -o halo

install:
	cp ./halo /usr/bin/

clean:
	rm -f halo
	rm -f src/*.o

