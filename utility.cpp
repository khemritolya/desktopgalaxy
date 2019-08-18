#include <chrono>
#include <iostream>
#include <fstream>
#include <math.h>
#include <zconf.h>

#include "utility.h"
#include "galaxy.h"

std::string logpath;

// Delete the log file, if there is one
void clear_logs() {
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);

    if (len != -1) {
        buf[len] = '\0';
        logpath = std::string(buf);
        logpath = logpath.substr(0, logpath.find_last_of('/')) + "/desktopgalaxy.log";
    } else {
        logpath = "/";
        println("could not find path of self");
    }

    if (remove(logpath.c_str()) != 0) {
        println("error removing log file (could be fine)");
    }
}

// Print to standard output & write to a file
void print(const std::string& string) {
    std::cout << string << std::flush;

    static bool failed;
    std::ofstream logfile(logpath, std::ios::app);
    if (!logfile && !failed) {
        failed = true;
        std::cout << "could not edit log file!" << std::endl;
    }

    logfile << string;
}

// Java-style println
void println(const std::string& string) {
    print(string + "\n");
}

// It was necessary
void println() {
    println("");
}

// Sin, but for degrees
double internal_sin(double angle_deg) {
    return sin(angle_deg * PI / 180);
}

// Cos, but for degrees
double internal_cos(double angle_deg) {
   return cos(angle_deg * PI / 180);
}

// Save the galaxy to a file, one star at a time
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

// Current time in milliseconds
long time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
}

// Sleep for some amount of milliseconds
void sleep(long millis) {
    if (millis < 0) return;

    struct timespec time{};
    time.tv_sec = millis / 1000;
    time.tv_nsec = (millis % 1000) * 1000000L;

    if (nanosleep(&time, new struct timespec) < 0) {
        println("sleep failure: duration " + std::to_string(millis));
    }
}