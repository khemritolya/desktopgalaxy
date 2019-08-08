# desktopgalaxy

### Version 0.1c  "A Better Starchild"

desktopgalaxy is a little project, made for personal use. I don't really care what you do with it.

It displays a galaxy that ~~should replace~~ replaces your screen background, which slowly rotates over time. I'm planning on adding some other stuff it'll do, but we'll see. Tested by me on an XPS 15 running Fedora 30, and by other people on some other machines.

### Command Line Options

 -h, --help       prints a help message

 -f, --fpstarget  set the fps that desktopgalaxy will try to run at

    DEFAULT: 20
    NOTE: Will not accept numbers < 5

 -s, --seed       set the seed to generate galaxy from

    DEFAULT: 1234
    NOTE: value will be hashed, but the 'default' 1234 isn't

 -e, --export     export the galaxy generated to a file (does not require argument)

    DEFAULT: desktopgalaxy.txt
    NOTE: this will ignore file names starting with '-'

 -a, --arms       number of arms the galaxy is to generate

    DEFAULT: 4, which is a reasonable number
    NOTE: Will not accept numbers < 1

 -l, --length     length of the arms the galaxy is to generate

    DEFAULT: 200 (degrees)
    NOTE: Arms start a bit away from the center and offset ~60 degrees

 -c, --coef       expansion coefficient of the arms the galaxy is to generate

    DEFAULT: 1.15
    NOTE: A bit unstable, may result in arms dissapearing

### Requirements

- A Computer running an X Server
- Xlib, OpenGL, and GLU libraries
- CMake

### Future Plans

- galaxy options (spiral, more arms, etc...)
- autostart after x server is running
- installer & configuration files to do this?
- make rendering more efficient

### Release Notes

##### Version 0.1c "A Better Starchild"

- Added launcher
- Modified configuration order
- Patched Readme

##### Version 0.1b "Starchild Improved"

- Added initial options

##### Version 0.1  "Starchild"

- Initial commit
- Bugfixes
- Thinking about future features