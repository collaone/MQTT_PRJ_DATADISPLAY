#include <QCoreApplication>

#include "display.h"
#include "qconsolelistener.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Display display;
    display.init();

    // listen to console input
    QConsoleListener console;
    QObject::connect(&console, &QConsoleListener::newLine, [&display](const QString &strNewLine) {
        display.visualize(strNewLine);
    });

    return app.exec();
}

