#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMqttClient>

class Display : public QObject
{
public:
    Display(QObject *parent=nullptr);
    ~Display();

    void init();
    void WaitingChoice();

public slots:
    void onConnect();
    void onMsg(const QMqttMessage &msg);

private:
    QMqttClient *m_client;
    QMqttSubscription *sub;

    bool isNumber(const std::string& s);

    const QString TOPIC_COMMAND = "/probe/command";
    const QString TOPIC_CPU_TEMP = "/probe/cpu/temp";
    const QString TOPIC_CPU_LOAD = "/probe/cpu/load";
    const QString TOPIC_DISK_FREE ="/probe/disk/free";
};

#endif // DISPLAY_H
