all: halo

halo:
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/limb/llist.c -o src/limb/llist.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/client.c -o src/client.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/display.c -o src/display.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/event.c -o src/event.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/halo.c -o src/halo.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/menu.c -o src/menu.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/surface.c -o src/surface.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/window.c -o src/window.o
	gcc -Wall -lm -lpthread `pkg-config --libs x11 xrender xcomposite cairo` src/limb/llist.o src/client.o src/display.o src/event.o src/halo.o src/menu.o src/surface.o src/window.o -o halo

install:
	cp ./halo /usr/bin/
	mkdir -p /usr/share/halo
	cp -r ./share/* /usr/share/halo

clean:
	rm -f halo
	rm -f src/*.o
	rm -f src/limb/*.o

