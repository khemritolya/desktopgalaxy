#ifndef DESKTOPGALAXY_GALAXY_H
#define DESKTOPGALAXY_GALAXY_H

#include <vector>

struct star {
    double angle, radius, x, y;

    star(double angle, double radius);

    void recompute(double rescaled_time);
};

extern std::vector<star>* galaxy;

void generate();

#endif
