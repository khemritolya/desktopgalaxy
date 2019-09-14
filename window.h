#ifndef DESKTOPGALAXY_WINDOW_H
#define DESKTOPGALAXY_WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <cmath>

namespace DesktopWindow {
    extern int display_width;
    extern int display_height;

    // Any initialization
    int init();

    // Clean up & statistics
    void quit();

    // The main loop
    int main_loop(double fps_target, double bright_falloff, double star_render_size);
}

#endif
