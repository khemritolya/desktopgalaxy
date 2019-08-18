#ifndef DESKTOPGALAXY_WINDOW_H
#define DESKTOPGALAXY_WINDOW_H

#include <string>

// TODO figure out if I need to expose these, or if I can figure a way around that
extern int display_width;
extern int display_height;

// Any initialization
int init();

// Clean up & statistics
void quit();

// The main loop
int main_loop(double fps_target, double bright_falloff, double star_render_size);

#endif
