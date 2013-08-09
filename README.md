
Orbital is a shell for Wayland's Weston.
It is composed of a Weston shell plugin and a shell client, made in Qt5,
with the interface in QtQuick 2.
Its goal is to produce a light and self-contained shell running on Wayland,
without many dependencies aside Weston and Qt.

1. DEPENDENCIES
Orbital depends on two things: Weston 1.2 and Qt 5.1.
Since it uses QtQuick 2 to draw the interface it will make use of OpenGL,
so it is advisable to use a decent graphics driver, otherwise the performance
will not be good.

2. COMPILING ORBITAL
To compile Orbital run this commands from the repository root directory:
    mkdir build
    cd build
    cmake ..
    make

3. RUNNING ORBITAL
To run Orbital you need to tell Weston it should load it, so you must modify
weston.ini, which should be in your $XDG_CONFIG_HOME, or in your $HOME/.config.
Add these lines:
    [core]
    modules=/'path to orbital sources'/build/orbital-shell.so

    [orbital]
    shell_client=/'path to orbital sources'/build/orbital-client
replacing 'path to orbital sources' with the correct path.

Now you can just run 'weston' or 'weston-launch' and it will load Orbital.

4. CONFIGURING ORBITAL
The first time you start Orbital it will load a default configuration. If you
save the configuration (by closing the config dialog or by going from edit mode
to normal mode) it will save the configuration file "orbital.conf", in
$XDG_CONFIG_HOME or, if not set, in $HOME/.config. You can manually modify
the configuration file, but Orbital has (or will have) graphical tools
for configuring the environment.
