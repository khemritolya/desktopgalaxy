#ifndef DESKTOPGALAXY_WINDOW_H
#define DESKTOPGALAXY_WINDOW_H

#include <string>

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
