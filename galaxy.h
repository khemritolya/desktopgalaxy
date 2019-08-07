#ifndef DESKTOPGALAXY_GALAXY_H
#define DESKTOPGALAXY_GALAXY_H

#include <vector>

#define PI 3.14159265

struct star {
    double angle, radius, x, y;

    star(double angle, double radius);

    void recompute(double rescaled_time);
};

extern std::vector<star>* galaxy;

double isin(double angle_deg);
double icos(double angle_deg);

void generate();

#endif
