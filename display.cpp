#include <iostream>
#include <QDateTime>
#include <QTimer>

#include "display.h"
#include "qconsolelistener.h"

/**
 * @brief Display::Display
 * @param parent
 *
 * Constructor
 * instantiate QMqttClient
 */
Display::Display(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setClientId("DISPLAY_CLIENT");
    m_client->setHostname("localhost");
    m_client->setPort(1883);

    connect(m_client, &QMqttClient::connected, this, &Display::onConnect);

    connect(m_client, &QMqttClient::disconnected, this, &Display::onDisconnect);

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message) {
        logger.print(message.toStdString());
    });
}

/**
 * @brief Display::~Display
 *
 * Destructor
 * Disconnect QMqttClient
 */
Display::~Display()
{
    m_client->disconnectFromHost();
    delete m_client;
    m_client = nullptr;
}


/**
 * @brief Display::init
 * Connect to broker
 */
void Display::init()
{
    waitingChoice();

    logger.print("Connecting...");
    m_client->connectToHost();
}

/**
 * @brief Display::waitingChoice
 * display startup screen
 */
void Display::waitingChoice()
{
    logger.print("Type:");
    logger.print("T to start displaying CPU temperature");
    logger.print("U to start displaying CPU usage");
    logger.print("F to start displaying free space of the root partition");
    logger.print("S to stop displaying");
    logger.print("a number from 500 to 5000 to set the probing interval\n");
}

/**
 * @brief Display::visualize
 * @param userChoice
 * Visualize the chosen property
 * subscribing to the corresponding topic
 */
void Display::visualize(QString userChoice)
{
    if (m_client->state() != QMqttClient::Connected)
    {
        logger.print("No connection available.");
        m_client->connectToHost();
    }
    else {
        // Set new probing interval
        if (isNumber(userChoice.toStdString()))
        {
            int newSampling = userChoice.toInt();
            if (500 <= newSampling && newSampling <= 5000)
            {
                QString msg = QString("Setting the probing interval at: ").append(userChoice).append("ms");
                logger.print(msg.toStdString());
                if (m_client->publish(TOPIC_COMMAND, userChoice.toStdString().c_str()) == -1)
                    logger.print("Error while publish");
            }
            else
            {
                QString msg = QString("COMMAND NOT VALID: ").append(userChoice);
                logger.print(msg.toStdString());
            }
        }
        // Show CPU Temperature
        else if (userChoice.compare("T") == 0 || userChoice.compare("t") == 0)
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
        else if (userChoice.compare("U") == 0 || userChoice.compare("u") == 0)
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
        else if (userChoice.compare("F") == 0 || userChoice.compare("f") == 0)
        {
            if (TOPIC_DISK_FREE.compare(activeSub) != 0) {
                unsubscribeAll();
                logger.print("Start displaying free disk space");

                if (!m_client->subscribe(TOPIC_DISK_FREE, 0))
                    logger.print("Error in subscribe");
                else
                    activeSub = TOPIC_DISK_FREE;
            }
        }
        // Stop displaying
        else if (userChoice.compare("S") == 0 || userChoice.compare("s") == 0) {
            logger.print("Stop displaying");
            unsubscribeAll();
            activeSub = "";
        }
        else
        {
            QString msg = QString("COMMAND NOT VALID: ").append(userChoice);
            logger.print(msg.toStdString());
        }
    }
}

/**
 * @brief Display::onConnect
 * Once connected to the broker, display startup screen
 */
void Display::onConnect()
{
    logger.print("Connected.");
}

/**
 * @brief Display::onDisconnect
 *
 */
void Display::onDisconnect()
{
    logger.print("Disconnected.");
}

/**
 * @brief Display::unsubscribeAll
 */
void Display::unsubscribeAll()
{
    m_client->unsubscribe(TOPIC_CPU_TEMP);
    m_client->unsubscribe(TOPIC_CPU_LOAD);
    m_client->unsubscribe(TOPIC_DISK_FREE);
}

/**
 * @brief Display::isNumber
 * @param s
 * @return true if string s represent a number
 */
bool Display::isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
