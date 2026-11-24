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
    QApplication app(argc, arv);

    app.setStyleSheet(R"(
        QWidget {
            background-color: #000000;
            color: #ffffff
        }
        
        QPushButton{
            background-color: #000000
            color: #000000:
            border: 1px solid #ffffff:
            padding: 6px;
        }
        
        QLineEdit, QTextEdit, QPlainTextEdit {
            background-color: #111111;
            color: #ffffff;
            border: 1px solid #ffffff;
        }
        
        QMenu {
            background-color: #000000;
            color: #ffffff;
            border: 1px solid #ffffff;
        }

        QMenu::item:selected {
            background-color: #ffffff;
            color: #000000;
        }

        QComboBox, QSpinBox {
            background-color: #111111;
            color: #ffffff;
            border: 1px solid #ffffff;
        }
    )");

    configureQtPlatform();

    MainWindow window;
    window.show();

    return app.exec();
}
