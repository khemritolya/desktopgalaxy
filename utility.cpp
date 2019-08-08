#include <chrono>
#include <iostream>
#include <fstream>
#include <math.h>

#include "utility.h"
#include "galaxy.h"

void clear_logs() {
    if (remove("desktopgalaxy.log") != 0) {
        println("error removing log file (could be fine)");
    }
}

void print(const std::string& string) {
    std::cout << string << std::flush;

    static bool failed;
    std::ofstream logfile("desktopgalaxy.log", std::ios::app);
    if (!logfile && !failed) {
        failed = true;
        std::cout << "could not edit log file!" << std::endl;
    }

    logfile << string;
}

void println(const std::string& string) {
    print(string + "\n");
}

void println() {
    println("");
}

double internal_sin(double angle_deg) {
    return sin(angle_deg * PI / 180);
}

double internal_cos(double angle_deg) {
   return cos(angle_deg * PI / 180);
}

void write_galaxy(const std::string& file_name) {
    if (remove(file_name.c_str()) != 0) {
        println("error removing file " + file_name);
    }

    std::ofstream galaxy_file(file_name, std::ios::app);
    if (!galaxy_file) {
        std::cout << "could not open galaxy export file " << file_name << "!" << std::endl;
        return;
    }

    for (star& s : *galaxy) {
        galaxy_file << s.radius << ":" << s.angle << std::endl;
    }

    println("exported galaxy!");
}

long time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
}

void sleep(long millis) {
    if (millis < 0) return;

    struct timespec time{};
    time.tv_sec = millis / 1000;
    time.tv_nsec = (millis % 1000) * 1000000L;

    if (nanosleep(&time, new struct timespec) < 0) {
        println("sleep failure: duration " + std::to_string(millis));
    }
}