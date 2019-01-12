#include "display.h"

#include <iostream>
#include <QtDebug>
#include <QDateTime>
#include <QTimer>

Display::Display(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setClientId("DISPLAY_CLIENT");
    m_client->setHostname("localhost");
    m_client->setPort(1883);

    connect(m_client, &QMqttClient::connected, this, &Display::onConnect);

    connect(m_client, &QMqttClient::messageReceived, this, [](const QByteArray &message, const QMqttTopicName &topic) {
        qDebug() << qUtf8Printable(message);
    });

    //connect(sub, &QMqttSubscription::messageReceived, this, &Display::onMsg);
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

void Display::WaitingChoice()
{
        qDebug() << "Type:";
        qDebug() << "T to start displaying CPU temperature";
        qDebug() << "U to start displaying CPU usage";
        qDebug() << "F to start displaying free space of the root partition";
        qDebug() << "S to stop displaying";
        qDebug() << "a number from 500 to 5000 to set the probing interval\n";

        std::string userChoice;
        std::getline(std::cin, userChoice);
        //QString qString(userChoice.c_str());
        //qDebug() << qString;


        if (isNumber(userChoice))
        {
            std::cout << "\nSetting the probing interval at: " << userChoice;
            if (m_client->publish(TOPIC_COMMAND, userChoice.c_str()) == -1)
                qDebug() << "Error while publish";
        }
        else if (userChoice.compare("T") == 0) {
            qDebug() << "Start displaying CPU Temperature";
            sub = m_client->subscribe(TOPIC_CPU_TEMP, 0);
            if (!sub) qDebug() << "Error in subscribe";
        }
        else if (userChoice.compare("U") == 0) {
            qDebug() << "Start displaying CPU Usage";
            sub = m_client->subscribe(TOPIC_CPU_LOAD, 0);
            if (!sub) qDebug() << "Error in subscribe";
        }
        else if (userChoice.compare("F") == 0) {
            qDebug() << "Start displaying free disk space";
            sub = m_client->subscribe(TOPIC_DISK_FREE, 0);
            if (!sub) qDebug() << "Error in subscribe";
        }
        else if (userChoice.compare("S") == 0) {
            qDebug() << "Stop displaying";
            sub = m_client->subscribe(TOPIC_DISK_FREE, 0);
            if (!sub) qDebug() << "Error in subscribe";
        }
}

void Display::onConnect()
{
    qDebug() << "Connected.";
    WaitingChoice();
}

bool Display::isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
