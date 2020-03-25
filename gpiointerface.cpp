#include "gpiointerface.h"
#include <QTextStream>
#include <QFile>
#include <QThread>

GPIOInterface::GPIOInterface(int pin, gpiodirection dir,QObject *parent) : QObject(parent)
{
    this->gpioPin = pin;

    this->pinPath += "gpio" + QString::number(this->gpioPin) + "/";
    qDebug("pinPath: %s", qPrintable(this->pinPath));

    this->exportGpio();
    QThread::msleep(100);
    this->setDirection(dir);
}

GPIOInterface::~GPIOInterface()
{
    this->unexportGpio();
}

void GPIOInterface::exportGpio()
{
    if (this->exported)
    {
        qDebug("gpio%i already exported", this->gpioPin);
        return;
    }

    QFile exportFile(QString("%1export").arg(this->gpioPath));
    exportFile.open(QIODevice::WriteOnly);
    if (!exportFile.isOpen())
    {
        qDebug("unable to export gpios");
        return;
    }
    QTextStream stream(&exportFile);
    stream << this->gpioPin;
    exportFile.close();

    this->exported = true;
    qDebug("exported gpio%i successfull", this->gpioPin);
}

void GPIOInterface::setDirection(gpiodirection dir)
{
    if (dir == this->direction)
    {
        qDebug("gpio%i direction is already %c", this->gpioPin, dir);
        return;
    }

    QFile directionFile(QString("%1direction").arg(this->pinPath));
    qDebug("%s", qPrintable(QString("%1direction").arg(this->pinPath)));
    directionFile.open(QIODevice::WriteOnly);
    if (!directionFile.isOpen())
    {
        qDebug("unable to set direction");
        return;
    }

    QTextStream stream(&directionFile);
    if (dir == output) stream << "out";
    else stream << "in";
    directionFile.close();

    this->direction = dir;
    qDebug("gpio%i direction set to %d", this->gpioPin, this->direction);
}

void GPIOInterface::readGpio()
{
    if (this->direction != input) this->setDirection(input);
    QByteArray content;
    QFile gpioFile(QString("%1value").arg(this->pinPath));
    gpioFile.open(QIODevice::ReadOnly);
    if (!gpioFile.isOpen())
    {
        qDebug("unable to open gpio at %s", qPrintable(this->pinPath));
        return;
    }
    content = gpioFile.readLine();
    gpioFile.close();

    if (content == "1\n") this->value = high;
    else this->value = low;
}

void GPIOInterface::writeGpio(gpiovalue val)
{
    if (this->direction != output) this->setDirection(output);
    QFile gpioFile(QString("%1value").arg(this->pinPath));
    gpioFile.open(QIODevice::WriteOnly);
    if (!gpioFile.isOpen())
    {
        qDebug("unable to open gpio at %s", qPrintable(this->pinPath));
        return;
    }
    if (val == high) gpioFile.write("1\n");
    else gpioFile.write("0\n");
    gpioFile.close();
}

void GPIOInterface::unexportGpio()
{
    if (!this->exported)
    {
        qDebug("gpio%i not exported", this->gpioPin);
        return;
    }

    QFile unexportFile(QString("%1unexport").arg(this->gpioPath));
    unexportFile.open(QIODevice::WriteOnly);
    if (!unexportFile.isOpen())
    {
        qDebug("unable to unexport gpios");
        return;
    }
    QTextStream stream(&unexportFile);
    stream << this->gpioPin;
    unexportFile.close();

    this->exported = false;
    qDebug("unexported gpio%i successfull", this->gpioPin);
}

