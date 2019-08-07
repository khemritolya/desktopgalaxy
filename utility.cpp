#include <chrono>
#include <iostream>

#include "utility.h"

void print(const std::string& string) {
    std::cout << string << std::flush;
}

void println(const std::string& string) {
    std::cout << string << std::endl;
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
        println("sleep failure duration " + std::to_string(millis));
    }
}