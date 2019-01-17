#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMqttClient>
#include <QTimer>

#include <logger.h>

class Display : public QObject
{
public:
    Display(QObject *parent=nullptr);
    ~Display();

    void init();

public slots:
    void onConnect();
    void visualize(QString userChoice);

private:
    Logger logger;
    QMqttClient *m_client;
    QTimer timer;

    void waitingChoice();
    bool isNumber(const std::string& s);
    void unsubscribeAll();

    const QString TOPIC_COMMAND = "/probe/command";
    const QString TOPIC_CPU_TEMP = "/probe/cpu/temp";
    const QString TOPIC_CPU_LOAD = "/probe/cpu/load";
    const QString TOPIC_DISK_FREE ="/probe/disk/free";
    QString activeSub;
};

#endif // DISPLAY_H
