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
int display_width;
int display_height;

GLXContext gl_context;

long last_time;

long first_time;
long frames;

int init() {
    display = XOpenDisplay(0);

    if (!display) {
        println("could not initialize X Display");
        return 0;
    }

    screen = DefaultScreen(display);

    display_width = DisplayWidth(display, screen);
    display_height = DisplayHeight(display, screen);

    println("fitting to display of size " + std::to_string(display_width) + "x" + std::to_string(display_height));

    XSetWindowAttributes x_attr = {0};
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
    x_attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    window = XCreateWindow(display, DefaultRootWindow(display), 0, 0, display_width, display_height, 0, vinfo.depth,
            InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &x_attr);

    XSelectInput(display, window, StructureNotifyMask | ExposureMask);
    long value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_WINDOW_TYPE", False), XA_ATOM, 32, PropModeReplace,
                    (unsigned char *) &value, 1);

    XStoreName(display, window, "desktopgalaxy");

    println("created & modified window (hopefully it worked)");

    gl_context = glXCreateContext(display, &vinfo, nullptr, GL_TRUE);
    if (glXMakeCurrent(display, window, gl_context) == False) {
        println("unable to initialize GLX");
        return 0;
    }

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAccum(GL_MULT, 0.5f);
    glClearColor(0.0, 0.0, 0.0, 1);

    println("set up OpenGL/GLU/whatever");

    XMapWindow(display, window);
    XSync(display, False);

    return 1;
}

void quit() {
    println("average fps was: " + std::to_string(frames * 1000.0 / (time() - first_time)));
    print("cleaning up...");
    delete galaxy;
    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, gl_context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    println("done");
}

void paint() {
    frames++;
    last_time = time();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, display_width, 0, display_height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(3.0f);
    glBegin(GL_POINTS);


    for (int i = 0; i < galaxy->size(); i++) {
        star& s = galaxy->at(i);

        glColor4f(1, 1 , 1, 0.5 * i / galaxy->size() + 0.3);
        glVertex2f(s.x + display_width / 2, s.y + display_height / 2);

        s.recompute(last_time / 1000.0);
    }
    
    glEnd();

    glXSwapBuffers(display, window);
}

int main_loop(double fps_target) {
    int sleep_time = (int)std::round(1000.0 / fps_target);

    frames = 0;
    first_time = time();

    while(true) {
        if (XPending(display) == 0) {
            paint();
            sleep(sleep_time - (time() - last_time));
        }

        while (XPending(display) > 0) {
            XEvent xe;
            XNextEvent(display, &xe);
            switch (xe.type) {
                case Expose:
                case ConfigureNotify:
                    paint();
                default:
                    break;
            }
        }
    }

    quit();

    return 0;
}