#include <vector>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <random>
#include <algorithm>

#include "galaxy.h"
#include "window.h"
#include "utility.h"

std::vector<star>* galaxy;

star::star(double angle, double radius) {
    this->angle = angle;
    this->radius = radius;
    this->x = 0;
    this->y = 0;
}

void star::recompute(double rescaled_time) {
    this->x = internal_cos(this->angle - rescaled_time) * this->radius;
    this->y = internal_sin(this->angle - rescaled_time) * this->radius;
}

bool compare(const star& a, const star& b) {
    return a.radius > b.radius;
}

void generate() {
    srand(1234);
    std::default_random_engine generator(1234);
    std::normal_distribution<double> distribution(0.0, 0.5);

    double longest_axis = std::min(display_height, display_width) / 2.0;

    galaxy = new std::vector<star>();

    for (double i = 60; i < 250; i += 0.5) {
        galaxy->push_back(star(i, std::pow(i, 1.15) * longest_axis / 500 + longest_axis / 10 * distribution(generator)));
        galaxy->push_back(star(i+90, std::pow(i, 1.15) * longest_axis / 500 + longest_axis / 10 * distribution(generator)));
        galaxy->push_back(star(i+180, std::pow(i, 1.15) * longest_axis / 500 + longest_axis / 10 * distribution(generator)));
        galaxy->push_back(star(i+270, std::pow(i, 1.15) * longest_axis / 500 + longest_axis / 10 * distribution(generator)));
    }

    for (double i = 0; i < 360; i += 0.3) {
        galaxy->push_back(star(i, (rand() % 100 + 10) / 50.0 * longest_axis));
    }

    for (double i = 0; i < 360; i += 0.7) {
        galaxy->push_back(star(i, (rand() % 100) / 370.0 * longest_axis));
    }

    std::sort(galaxy->begin(), galaxy->end(), compare);
}