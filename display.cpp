#include "display.h"

#include <iostream>
#include <QtDebug>
#include <QDateTime>
#include <QTimer>

#include <iostream>
#include <memory>
#include <string>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>

#include <qconsolelistener.h>

Display::Display(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setClientId("DISPLAY_CLIENT");
    m_client->setHostname("localhost");
    m_client->setPort(1883);

    connect(m_client, &QMqttClient::connected, this, &Display::onConnect);

    connect(m_client, &QMqttClient::messageReceived, this, [](const QByteArray &message, const QMqttTopicName &topic) {
        qDebug() << qUtf8Printable(message);
        //std::cout << message.toStdString();
        //Command visualizer;
        //visualizer.show(message);
    });
}

Display::~Display()
{
    m_client->disconnectFromHost();
}

void Display::init()
{
    qDebug() << "Connecting...";
    m_client->connectToHost();
}

void Display::waitingChoice()
{
    qDebug() << "Type:";
    qDebug() << "T to start displaying CPU temperature";
    qDebug() << "U to start displaying CPU usage";
    qDebug() << "F to start displaying free space of the root partition";
    qDebug() << "S to stop displaying";
    qDebug() << "a number from 500 to 5000 to set the probing interval\n";
}

void Display::visualize(QString userChoice)
{
    if (isNumber(userChoice.toStdString()))
    {
        int newSampling = userChoice.toInt();
        if (500 <= newSampling && newSampling <= 5000)
        {
            qDebug() << "\nSetting the probing interval at: " << userChoice;
            if (m_client->publish(TOPIC_COMMAND, userChoice.toStdString().c_str()) == -1)
                qDebug() << "Error while publish";
        }
        else
        {
            qDebug() << "New sampling";
        }
    }
    else if (userChoice.compare("T") == 0)
    {
        if (TOPIC_CPU_TEMP.compare(activeSub) != 0) {
            unsubscribeAll();
            qDebug() << "Start displaying CPU Temperature";

            if (!m_client->subscribe(TOPIC_CPU_TEMP, 0))
                qDebug() << "Error in subscribe";
            else
                activeSub = TOPIC_CPU_TEMP;
        }
    }
    else if (userChoice.compare("U") == 0)
    {
        if (TOPIC_CPU_LOAD.compare(activeSub) != 0) {
            unsubscribeAll();
            qDebug() << "Start displaying CPU Usage";

            if (!m_client->subscribe(TOPIC_CPU_LOAD, 0))
                qDebug() << "Error in subscribe";
            else
                activeSub = TOPIC_CPU_LOAD;
        }
    }
    else if (userChoice.compare("F") == 0)
    {
        if (TOPIC_CPU_TEMP.compare(activeSub) != 0) {
            unsubscribeAll();
            qDebug() << "Start displaying free disk space";

            if (!m_client->subscribe(TOPIC_DISK_FREE, 0))
                qDebug() << "Error in subscribe";
            else
                activeSub = TOPIC_DISK_FREE;
        }
    }
    else if (userChoice.compare("S") == 0) {
        qDebug() << "Stop displaying";
        unsubscribeAll();
    }
    else
    {
        qDebug() << "COMMAND NOT VALID: " << userChoice;
    }
}

void Display::onConnect()
{
    qDebug() << "Connected.";
    waitingChoice();
}

void Display::unsubscribeAll()
{
    m_client->unsubscribe(TOPIC_CPU_TEMP);
    m_client->unsubscribe(TOPIC_CPU_LOAD);
    m_client->unsubscribe(TOPIC_DISK_FREE);
}


bool Display::isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
