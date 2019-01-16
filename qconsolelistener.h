#pragma once

#include <QObject>
#include <iostream>
#include <QSocketNotifier>

class QConsoleListener : public QObject
{
    Q_OBJECT

public:
    QConsoleListener();
    ~QConsoleListener();
Q_SIGNALS:
    void newLine(const QString &strNewLine);

private:
    QSocketNotifier *m_notifier;

private Q_SLOTS:
    void readCommand(int socket);
};
