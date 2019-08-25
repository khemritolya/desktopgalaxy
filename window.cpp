#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <cmath>

#include "window.h"
#include "galaxy.h"
#include "utility.h"

Display* display;
int screen;
Window window;
XVisualInfo vinfo;

int DesktopWindow::display_width;
int DesktopWindow::display_height;

GLXContext gl_context;

long last_time;

long first_time;
long total_sleep_time;
long frames;

int DesktopWindow::init() {
    display = XOpenDisplay(0);

    // TODO figure out if this can actually fail
    if (!display) {
        println("could not initialize X Display");
        return 0;
    }

    screen = DefaultScreen(display);

    display_width = DisplayWidth(display, screen);
    display_height = DisplayHeight(display, screen);

    println("fitting to display of size " + std::to_string(display_width) + "x" + std::to_string(display_height));

    // Create visual and window that supports transparency
    XSetWindowAttributes x_attr = {0};
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
    x_attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    window = XCreateWindow(display, DefaultRootWindow(display), 0, 0, display_width, display_height, 0, vinfo.depth,
            InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &x_attr);

    // This is where I trick X into thinking this is the background
    XSelectInput(display, window, StructureNotifyMask | ExposureMask);
    long value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_WINDOW_TYPE", False), XA_ATOM, 32, PropModeReplace,
                    (unsigned char *) &value, 1);

    XStoreName(display, window, "desktopgalaxy"); // not really relevant, as the window shouldn't exist as such

    println("created & modified window (hopefully it worked)");

    // Make X and OpenGL cooperate
    gl_context = glXCreateContext(display, &vinfo, nullptr, GL_TRUE);
    if (glXMakeCurrent(display, window, gl_context) == False) {
        println("unable to initialize GLX");
        return 0;
    }

    // TODO fiddle with these to further optimize program
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH); // #2
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // #1
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAccum(GL_MULT, 0.5f);
    glClearColor(0.0, 0.0, 0.0, 1);

    println("set up OpenGL/GLU/whatever");

    XMapWindow(display, window);
    XSync(display, False);

    return 1;
}

void DesktopWindow::quit() {
    // Print runtime statistics
    long quit_time = time();

    println("average fps was: " + std::to_string(frames * 1000.0 / (quit_time - first_time)));
    println("total sleep time: " + std::to_string(total_sleep_time) + "ms (" +
        std::to_string(100.0 * total_sleep_time / (quit_time - first_time)) + "%)");

    // Actually clean up
    print("cleaning up...");

    delete Galaxy::galaxy;

    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, gl_context);

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    println("done");
}

void paint(double brightness_falloff, double star_render_size) {
    // The numbers part
    frames++;
    last_time = time();

    // The OpenGL part
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // seems like a reasonable definition of a screen
    gluOrtho2D(0, DesktopWindow::display_width, 0, DesktopWindow::display_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(star_render_size);
    glBegin(GL_POINTS);

    // The bit where stars are drawn
    for (int i = 0; i < Galaxy::galaxy->size(); i++) {
        Galaxy::star& s = Galaxy::galaxy->at(i);

        if (brightness_falloff != 0.0) {
            glColor4f(1, 1, 1, brightness_falloff * i / Galaxy::galaxy->size() + 1 - brightness_falloff);
        }

        // Please draw me a star...
        glVertex2f(s.x + DesktopWindow::display_width / 2.0, s.y + DesktopWindow::display_height / 2.0);

        s.recompute(last_time / 1000.0); // angle is a function of time
    }
    
    glEnd();

    glXSwapBuffers(display, window);
}

int DesktopWindow::main_loop(double fps_target, double brightness_falloff, double star_render_size) {
    int sleep_time = (int)std::round(1000.0 / fps_target);
    long downtime;

    frames = 0;
    first_time = time();

    while(true) {
        // If no x events exist, render a frame + sleep
        if (XPending(display) == 0) {
            paint(brightness_falloff, star_render_size);
            downtime = sleep_time - (time() - last_time); // this is sometimes negative
            total_sleep_time += std::max(0L, downtime);
            sleep(downtime); // but don't render too many frames
        }

        // If an x event exists, render a frame
        while (XPending(display) > 0) {
            XEvent xe;
            XNextEvent(display, &xe);

            switch (xe.type) {
                case Expose:
                case ConfigureNotify:
                    paint(brightness_falloff, star_render_size);
                default:
                    break;
            }
        }
    }

    DesktopWindow::quit();

    return 0;
}