#include <iostream>
#include "qconsolelistener.h"

QConsoleListener::QConsoleListener()
{
    m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
    QObject::connect(m_notifier, &QSocketNotifier::activated, this, &QConsoleListener::readCommand);
}

void QConsoleListener::readCommand(int socket)
{
    Q_UNUSED(socket);

    std::string line;
    std::getline(std::cin, line);
    QString strLine = QString::fromStdString(line);
    Q_EMIT this->newLine(strLine);
}

QConsoleListener::~QConsoleListener()
{
    delete m_notifier;
    m_notifier = nullptr;
}
