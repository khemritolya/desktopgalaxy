# desktopgalaxy

### Version 0.2a  "Tunguska Calling"

**Author: Khemri Tolya**

desktopgalaxy is a little project, made for personal use. I don't really care what you do with it.

It displays a galaxy that ~~should replace~~ replaces your screen background, which slowly rotates over time. I'm planning on adding some other stuff it'll do, but we'll see. Tested by me on an XPS 15 running Fedora 30, and by other people on some other machines.

![example](https://i.imgur.com/G52qm7q.png)

`(NOTE: This picture is of an old version, though desktopgalaxy's visuals haven't changed that much since then)`

### Command Line Options

 -h, --help       prints a help message

 -f, --fpstarget  set the fps that desktopgalaxy will try to run at

    DEFAULT: 20
    NOTE: Will not accept numbers < 5

 -e, --export     export the galaxy generated to a file (does not require argument)

    DEFAULT FILE (no filename given): galaxy.txt
    NOTE: this will ignore file names starting with '-'

 -s, --seed       set the seed to generate galaxy from

    DEFAULT: 1234
    NOTE: value will be hashed, but the 'default' 1234 isn't
    
 -a, --arms       number of arms the galaxy is to generate

    DEFAULT: 4, which is a reasonable number
    NOTE: Will not accept numbers < 1

 -l, --length     length of the arms the galaxy is to generate

    DEFAULT: 200 (degrees)
    NOTE: Arms start a bit away from the center and offset ~60 degrees

 -c, --coef       expansion coefficient of the arms the galaxy is to generate

    DEFAULT: 1.15
    NOTE: A bit unstable, may result in arms dissapearing

 -b, --bfalloff   the amount that star brightness should go down from 1.0

    DEFAULT: 0.7 (on a scale 0.0 to 1.00)
    NOTE: Will not accept numbers < 0 or > 1

 -S, --size       set the rendered size of stars
  
    DEFAULT: 2.5 (px)
    NOTE: Will not accept numbers < 1

### System Requirements

- A Computer running an X Server

### Build Requirements

- Xlib, OpenGL, and GLU libraries
- CMake

### Future Plans

- even more galaxy options
- make rendering even more efficient (perhaps)

### Release Notes

##### Version 0.2a "Tunguska Calling"

- Installer which adds desktopgalaxy to auto-start on login
    - Tested on Fedora 30 with GNOME, unknown how this will behave with KDE/Unity
- Added code comments for posterity
- Better Logging
- More galaxy & visual options
- Reorganized README.md

##### Version 0.1c "A Better Starchild"

- Added launcher
- Modified configuration order
- Patched Readme

##### Version 0.1b "Starchild Improved"

- Added initial options

##### Version 0.1a  "Starchild"

- Initial commit
- Bugfixes
- Thinking about future features
