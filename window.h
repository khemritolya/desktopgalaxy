#ifndef DESKTOPGALAXY_WINDOW_H
#define DESKTOPGALAXY_WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <string>

extern int display_width;
extern int display_height;

int init();
void quit();
int main_loop(double fps_target);

#endif
