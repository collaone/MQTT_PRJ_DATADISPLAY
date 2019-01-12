#include <QCoreApplication>
#include "display.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Display display;
    display.init();

    return app.exec();
}
