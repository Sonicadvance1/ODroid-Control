ODroid-Control
==============

This project allows you to control some minor things with your ODROID device

Main thing is controlling the clock frequency of the device.

	- Careful with overclocking and underclocking, it can hard lock the device and you'll have to reset the device
	- Overclocking of course is taking the life of your device in to your own hands, so I'm not responsible for anything you break with this.

PWM fan control for the ODROID-U2 is available but untested so I don't know for sure if it works yet.

-- Building --

To build, you must have libglade and libgtk+2.0 development libraries installed.

Via apt.

	sudo apt-get install libglade2-dev libgtk+2.0-dev

Run make in the base directory after that.

	make

-- Running --

This needs to be run as root

	sudo ./odroidcontrol

Planned features:

HDMI resolution control (720p/1080p)

Terminal based control so you don't need to load the UI to change the setting.

