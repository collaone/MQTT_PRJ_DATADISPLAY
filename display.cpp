#include <iostream>
#include <QtDebug>
#include <QDateTime>
#include <QTimer>

#include "display.h"
#include "qconsolelistener.h"


Display::Display(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setClientId("DISPLAY_CLIENT");
    m_client->setHostname("localhost");
    m_client->setPort(1883);

    connect(m_client, &QMqttClient::connected, this, &Display::onConnect);

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message) {
        logger.print(message.toStdString());
    });
}

Display::~Display()
{
    m_client->disconnectFromHost();
}

void Display::init()
{
    logger.print("Connecting...");
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
    // Set new probing interval
    if (isNumber(userChoice.toStdString()))
    {
        int newSampling = userChoice.toInt();
        if (500 <= newSampling && newSampling <= 5000)
        {
            logger.print("Setting the probing interval at: ", userChoice.toStdString());
            if (m_client->publish(TOPIC_COMMAND, userChoice.toStdString().c_str()) == -1)
                logger.print("Error while publish");
        }
        else
        {
            qDebug() << "New sampling";
        }
    }
    // Show CPU Temperature
    else if (userChoice.compare("T") == 0)
    {
        if (TOPIC_CPU_TEMP.compare(activeSub) != 0) {
            unsubscribeAll();
            logger.print("Start displaying CPU Temperature");

            if (!m_client->subscribe(TOPIC_CPU_TEMP, 0))
                logger.print("Error in subscribe");
            else
                activeSub = TOPIC_CPU_TEMP;
        }
    }
    // Show CPU Usage
    else if (userChoice.compare("U") == 0)
    {
        if (TOPIC_CPU_LOAD.compare(activeSub) != 0) {
            unsubscribeAll();
            logger.print("Start displaying CPU Usage");

            if (!m_client->subscribe(TOPIC_CPU_LOAD, 0))
                logger.print("Error in subscribe");
            else
                activeSub = TOPIC_CPU_LOAD;
        }
    }
    // Show free disk space
    else if (userChoice.compare("F") == 0)
    {
        if (TOPIC_CPU_TEMP.compare(activeSub) != 0) {
            unsubscribeAll();
            logger.print("Start displaying free disk space");

            if (!m_client->subscribe(TOPIC_DISK_FREE, 0))
                logger.print("Error in subscribe");
            else
                activeSub = TOPIC_DISK_FREE;
        }
    }
    // Stop displaying
    else if (userChoice.compare("S") == 0) {
        logger.print("Stop displaying");
        unsubscribeAll();
    }
    else
    {
        logger.print("COMMAND NOT VALID: ", userChoice.toStdString());
    }
}

void Display::onConnect()
{
    logger.print("Connected.");
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
