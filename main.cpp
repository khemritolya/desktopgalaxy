#include <csignal>
#include <netdb.h>
#include <cstring>

#include "window.h"
#include "utility.h"
#include "galaxy.h"

#define VERSION "desktopgalaxy v0.2a \"Tunguska Calling\""

// What to do when stuff does wrong
void on_sig(int sigaction) {
    println();
    println("got interrupt signal " + std::to_string(sigaction));
    DesktopWindow::quit();
}

int main(int argc, char** argv) {
    // If any argument is help, print help and immediately exit
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            println("Usage: desktopgalaxy [OPTION...]");

            println();

            println("  -h, --help       print this message and exit");

            println();

            println("  -f, --fpstarget  set the fps that desktopgalaxy will try to run at");
            println("  -b, --bfalloff   the amount that star brightness should go down from 1 (0.0-1.0)");
            println("  -S, --size       set the rendered size of stars");

            println();

            println("  -e, --export     export the galaxy generated to a file (does not require argument)");

            println();

            println("  -s, --seed       set the seed to generate galaxy from");
            println("  -a, --arms       number of arms the galaxy is to generate");
            println("  -l, --length     length of the arms the galaxy is to generate");
            println("  -c, --coef       expansion coefficient of the arms the galaxy is to generate");

            println();

            println("Examples:");
            println("  desktopgalaxy");
            println("  desktopgalaxy --fpstarget 30 --export hello.txt");
            println("  desktopgalaxy -s 1234");
            println("  desktopgalaxy -e");
            println("  desktopgalaxy -b 0 -S 2");

            return 0;
        }
    }

    clear_logs();

    print("running ");
    print(VERSION);
    println(" from ");
    println(argv[0]);

    println();

    println("beginning to initialize desktopgalaxy");

    // TODO maybe more?
    struct sigaction action;
    action.sa_handler = on_sig; // do this if things go wrong
    sigaction(SIGTERM, &action, nullptr);
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGKILL, &action, nullptr);
    sigaction(SIGALRM, &action, nullptr);
    sigaction(SIGSEGV, &action, nullptr);
    sigaction(SIGILL, &action, nullptr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        println("can\'t open socket?");
        return 1;
    }

    struct sockaddr_in address;

    memset((char *) &address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(64335);

    // Bind to the socket
    if (bind(sockfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        println("desktopgalaxy already running (or another process is using port 64335)");
        return 1;
    }

    // Default values
    int fps_target = 20, seed = 1234, arm_count = 4, arm_length = 200;
    double expansion_coefficient = 1.15, star_render_size = 2.5, brightness_falloff = 0.7;
    bool export_galaxy = false;
    std::string file_name;

    // Read in any coniguration settings the user may have requested
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--fpstarget") == 0 || strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                i++;
                fps_target = atoi(argv[i]);
                if (fps_target < 5) {
                    print("could not initialize fps target: ");
                    println(argv[i]);
                    fps_target = 30;
                } else {
                    println("initialized fps target: " + std::to_string(fps_target));
                }
            }
        } else if (strcmp(argv[i], "--bfalloff") == 0 || strcmp(argv[i], "-b") == 0) {
            if (i + 1 < argc) {
                i++;
                brightness_falloff = atof(argv[i]);
                if (brightness_falloff < 0 || brightness_falloff > 1) {
                    print("could not initialize brightness falloff: ");
                    println(argv[i]);
                    brightness_falloff = 0.7;
                } else {
                    println("initialized brightness falloff: " + std::to_string(brightness_falloff));
                }
            }
        } else if (strcmp(argv[i], "--size") == 0 || strcmp(argv[i], "-S") == 0) {
            if (i + 1 < argc) {
                i++;
                star_render_size = atof(argv[i]);
                if (star_render_size < 1) {
                    print("could not initialize star render size: ");
                    println(argv[i]);
                    star_render_size = 2.5;
                } else {
                    println("initialized star render size: " + std::to_string(star_render_size));
                }
            }
        } else if (strcmp(argv[i], "--export") == 0 || strcmp(argv[i], "-e") == 0) {
            export_galaxy = true;
            file_name = "galaxy.txt";
            if (i + 1 < argc && argv[i+1][0] != '-') {
                i++;
                file_name = argv[i];
            }
            println("Exporting galaxy to \"" + file_name + "\" after generation");
        } else if (strcmp(argv[i], "--seed") == 0 || strcmp(argv[i], "-s") == 0) {
            if (i + 1 < argc) {
                i++;
                seed = std::hash<std::string>()(argv[i]);
                print("selected seed \"");
                print(argv[i]);
                print("\" --> ");
                println(std::to_string(seed));
            }
        } else if (strcmp(argv[i], "--arms") == 0 || strcmp(argv[i], "-a") == 0) {
            if (i + 1 < argc) {
                i++;
                arm_count = atoi(argv[i]);
                if (arm_count < 1) {
                    print("could not initialize arm count: ");
                    println(argv[i]);
                    arm_count = 4;
                } else {
                    println("initialized arm count: " + std::to_string(arm_count));
                }
            }
        } else if (strcmp(argv[i], "--length") == 0 || strcmp(argv[i], "-l") == 0) {
            if (i + 1 < argc) {
                i++;
                arm_length = atoi(argv[i]);
                if (arm_length < 10) {
                    print("could not initialize arm length: ");
                    println(argv[i]);
                    arm_length = 200;
                } else {
                    println("initialized arm length: " + std::to_string(arm_length));
                }
            }
        } else if (strcmp(argv[i], "--coef") == 0 || strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {
                i++;
                expansion_coefficient = atof(argv[i]);
                if (expansion_coefficient < 0) {
                    print("could not initialize expansion coefficient: ");
                    println(argv[i]);
                    expansion_coefficient = 1.15;
                } else {
                    println("initialized expansion coefficient: " + std::to_string(expansion_coefficient));
                }
            }
        } else {
            print("unknown setting: ");
            println(argv[i]);
        }
    }

    // Any window initialization
    if (!DesktopWindow::init()) {
        println("could not initialize window");
        return 1;
    }

    // A new galaxy
    Galaxy::generate(arm_count, arm_length, expansion_coefficient, seed);

    println("created a galaxy of " + std::to_string(Galaxy::galaxy->size()) + " stars");

    // export, if asked to
    if (export_galaxy) {
        write_galaxy(file_name);
    }

    println("finished initialization");

    // main loop
    return DesktopWindow::main_loop(fps_target, brightness_falloff, star_render_size);
}
