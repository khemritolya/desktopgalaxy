#include "galaxy.h"
#include "window.h"
#include "utility.h"

// Used in window.cpp
std::vector<Galaxy::star>* Galaxy::galaxy;

// Stars are really defined by their angle & radius
Galaxy::star::star(double angle, double radius) {
    this->angle = angle;
    this->radius = radius;
    this->x = 0;
    this->y = 0;
}

// Recompute x and y given time
void Galaxy::star::recompute(double rescaled_time) {
    this->x = internal_cos(this->angle - rescaled_time) * this->radius;
    this->y = internal_sin(this->angle - rescaled_time) * this->radius;
}

// Allow the sorting of stars
bool compare(const Galaxy::star& a, const Galaxy::star& b) {
    return a.radius > b.radius;
}

// If this is a valid star, add it, if not, don't
void add_star(Galaxy::star* s, int axis) {
    if (s->radius <= axis) {
        Galaxy::galaxy->push_back(*s);
    } else {
        delete s;
    }
}

// fill up galaxy with stars
void Galaxy::generate(int arm_count, int arm_length, double expansion_coefficient, int seed) {
    println("generating galaxy using seed " + std::to_string(seed));

    // initialize random number generators
    srand(seed);
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 0.5);

    // in future versions, this might have caused issues
    int shortest_axis = std::min(DesktopWindow::display_height, DesktopWindow::display_width) / 2;
    int longest_axis = std::max(DesktopWindow::display_height, DesktopWindow::display_width) / 2;
    int axis = std::round(std::sqrt(shortest_axis*shortest_axis + longest_axis * longest_axis));

    Galaxy::galaxy = new std::vector<Galaxy::star>();

    // TODO a bunch more configuration options here

    // The arms of the galaxy
    for (double i = 30; i < 60 + arm_length; i += 0.05) {
        for (int j = 0; j < arm_count; j++) {
            double len = std::pow(i, expansion_coefficient) * shortest_axis / 500 + shortest_axis / 10.0 *
                                                                                    distribution(generator);
            if (len > 100) {
                add_star(new Galaxy::star(i + 360.0 / arm_count * j, len), longest_axis);
            }
        }
    }

    // General background stars
    for (double i = 0; i < 360; i += 0.01) {
        add_star(new Galaxy::star(i, (rand() % 10000 + 1000) / 2000.0 * shortest_axis), axis);
    }

    // Outer layer of core stars
    for (double i = 0; i < 360; i += 0.07) {
        add_star(new Galaxy::star(i, (distribution(generator)+1) * shortest_axis), axis);
    }

    // Inner layer of core stars
    for (double i = 0; i < 360; i += 0.1) {
        add_star(new Galaxy::star(i, (rand() % 10000) / 47000.0 * shortest_axis), axis);
    }

    // sort the galaxy from least distance to 0,0 to greatest
    std::sort(Galaxy::galaxy->begin(), Galaxy::galaxy->end(), compare);
}