#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "window.h"
#include "galaxy.h"
#include "utility.h"

/* Xlib things */
Display* display;
int screen;
Window window;
XVisualInfo vinfo;
int display_width;
int display_height;

/* opengl things */
GLXContext gl_context;

/* internal */
long last_time;

int init() {
    display = XOpenDisplay(0);

    if (!display) {
        println("Could not initialize X Display");
        return 0;
    }

    screen = DefaultScreen(display);

    display_width = DisplayWidth(display, screen);
    display_height = DisplayHeight(display, screen);

    println("Fitting to display of size " + std::to_string(display_width) + "x" + std::to_string(display_height));

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

    println("Created & Modified Window (hopefully it worked)");

    gl_context = glXCreateContext(display, &vinfo, nullptr, GL_TRUE);
    if (glXMakeCurrent(display, window, gl_context) == False) {
        println("Unable to initialize GLX");
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

    println("OpenGL/GLU/whatever set up");

    XMapWindow(display, window);
    XSync(display, False);

    generate();

    println("Created a galaxy of " + std::to_string(galaxy->size()) + " stars");

    return 1;
}

void quit() {
    print("Cleaning up...");
    delete galaxy;
    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, gl_context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    println("Done");
}

void paint() {
    last_time = time();
    double rescaled_time = last_time % 100000 / 1000.0;

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

        glColor4f(1, 1 , 1, 0.7 * i / galaxy->size() + 0.1);
        glVertex2f(s.x + display_width / 2, s.y + display_height / 2);

        s.recompute(rescaled_time);
    }
    
    glEnd();

    glXSwapBuffers(display, window);
}

int main_loop() {
    while(true) {
        if (XPending(display) == 0) {
            paint();
            sleep(33 - (time() - last_time));
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