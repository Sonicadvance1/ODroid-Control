all:
	g++ Main.cpp `pkg-config --libs --cflags gtk+-2.0 libglade-2.0` 
