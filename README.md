# desktopgalaxy

### Version 0.1b  "Starchild Improved"

desktopgalaxy is a little project, made for personal use. I don't really care what you do with it.

It displays a galaxy that ~~should replace~~ replaces your screen background, which slowly rotates over time. I'm planning on adding some other stuff it'll do, but we'll see. Tested by me on an XPS 15 running Fedora 30, and by other people on some other machines.

### Command Line Options

> -h, --help       prints a help message
> -f, --fpstarget  set the fps that desktopgalaxy will try to run at
> -s, --seed       set the seed to generate galaxy from
> -e, --export     export the galaxy generated to a file (does not require argument)
>                  NOTE: this will ignore file names starting with '-'

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

##### Version 0.1b "Starchild Improved"

- Added initial options

##### Version 0.1  "Starchild"

- Initial commit
- Bugfixes
- Thinking about future features