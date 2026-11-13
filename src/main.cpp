#include "mainwindow.h"

#include <QApplication>
#include <QByteArray>

namespace {
void configureQtPlatform()
{
    // Qt's GTK theme plugin can abort when the installed GNOME settings
    // schema is newer/older than the desktop integration package. The
    // generic Qt theme keeps the application independent of that schema.
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORMTHEME")) {
        qputenv("QT_QPA_PLATFORMTHEME", QByteArrayLiteral("generic"));
    }

    // A machine without a desktop session cannot load Qt's xcb/Wayland plugin.
    // Keep the app usable for CI and terminal-only environments while leaving
    // the normal desktop platform selection untouched.
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")
        && qEnvironmentVariableIsEmpty("DISPLAY")
        && qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen"));
    }
}
}

int main(int argc, char *argv[])
{
    configureQtPlatform();
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
