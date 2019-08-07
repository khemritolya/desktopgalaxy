#include <csignal>

#include <netdb.h>
#include <cstring>

#include "window.h"
#include "utility.h"

void on_sig(int sigaction) {
    println("got interrupt signal " + std::to_string(sigaction));
    quit();
}

int main(int argc, char** argv) {
    clear();

    print("running desktopgalaxy as ");
    println(argv[0]);

    println("beginning to initialize desktopgalaxy");

    struct sigaction action;
    action.sa_handler = on_sig;
    sigaction(SIGTERM, &action, nullptr);
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGKILL, &action, nullptr);
    sigaction(SIGALRM, &action, nullptr);

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

    if (bind(sockfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        println("desktopgalaxy already running (or another process is using port 64335)");
        return 1;
    }

    if (!init()) {
        println("could not initialize window");
        return 1;
    }

    println("finished initialization");

    return main_loop();
}