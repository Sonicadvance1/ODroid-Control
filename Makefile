all:
	g++ -O2 -o odroidcontrol Main.cpp Settings.cpp `pkg-config --libs --cflags gtk+-2.0 libglade-2.0` 
debug:
	g++ -g -o odroidcontrol Main.cpp Settings.cpp `pkg-config --libs --cflags gtk+-2.0 libglade-2.0` 
