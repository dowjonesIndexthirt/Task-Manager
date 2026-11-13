# Productivity City
A simple task manager that reads the Linux directory /proc and displays necessary resource usage.

Compiling and running instructions:
```bash
cmake -S . -B build
cmake --build build
./build/CS370
```

The application requires Qt 5 or Qt 6 Widgets. On Ubuntu/Debian, install the
development package first if CMake cannot find Qt:

```bash
sudo apt install qt6-base-dev
```

If the application is run on a desktop and Qt reports that the `xcb` plugin
cannot load, install the missing X11 cursor library:

```bash
sudo apt install libxcb-cursor0
```

The application uses Qt's generic theme to avoid a crash caused by mismatched
GNOME settings schemas on some Ubuntu/VS Code installations.

When no graphical session is available, the program automatically uses Qt's
offscreen platform so it can still start for automated checks. It will display
the normal window when launched from a desktop session.


Put all 3 major branches into one, modified CMakeLists.txt to incorporate all additional files

A simple task manager that reads the Linux directory /proc and displays necessary resource usage.

We will be using c++
