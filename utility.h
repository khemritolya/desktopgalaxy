#ifndef DESKTOPGALAXY_UTILITY_H
#define DESKTOPGALAXY_UTILITY_H

#include <string>

#define PI 3.14159265

void clear();

void print(const std::string& string);
void println(const std::string& string);

double internal_sin(double angle_deg);
double internal_cos(double angle_deg);

long time();
void sleep(long millis);

#endif
