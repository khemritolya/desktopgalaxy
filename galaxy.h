#ifndef DESKTOPGALAXY_GALAXY_H
#define DESKTOPGALAXY_GALAXY_H

#include <vector>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <random>
#include <algorithm>

namespace Galaxy {
    struct star {
        double angle, radius, x, y;

        star(double angle, double radius);

        void recompute(double rescaled_time);
    };

    // The galaxy to be drawn
    extern std::vector<star>* galaxy;

    // Generate a new galaxy and store it in Galaxy::galaxy
    void generate(int arm_count, int arm_length, double expansion_coefficient, int seed);
}

#endif
