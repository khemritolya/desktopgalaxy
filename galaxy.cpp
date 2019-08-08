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

void generate(int arm_count, int arm_length, double expansion_coefficient, int seed) {
    println("generating galaxy using seed " + std::to_string(seed));

    srand(seed);
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 0.5);

    double longest_axis = std::min(display_height, display_width) / 2.0;

    galaxy = new std::vector<star>();

    for (double i = 60; i < 60 + arm_length; i += 0.5) {
        for (int j = 0; j < arm_count; j++) {
            galaxy->push_back(star(i + 360.0 / arm_count * j, std::pow(i, expansion_coefficient) * longest_axis / 500 +
            longest_axis / 10 * distribution(generator)));
        }
    }

    for (double i = 0; i < 360; i += 0.3) {
        galaxy->push_back(star(i, (rand() % 100 + 10) / 50.0 * longest_axis));
    }

    for (double i = 0; i < 360; i += 0.7) {
        galaxy->push_back(star(i, (distribution(generator)+1) / 3.0 * longest_axis));
    }

    for (double i = 0; i < 360; i += 1) {
        galaxy->push_back(star(i, (rand() % 100) / 470.0 * longest_axis));
    }

    std::sort(galaxy->begin(), galaxy->end(), compare);
}