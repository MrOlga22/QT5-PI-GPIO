#include <QCoreApplication>
#include <signal.h>
#include <QThread>
#include "gpiointerface.h"

static void close(int sig)
{
    qDebug("close called with signal: %i", sig);
    qApp->quit();
}

void blink(GPIOInterface &gpio)
{
    gpio.writeGpio(high);
    QThread::msleep(5000);
    gpio.writeGpio(low);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);    
    signal(SIGINT, close); // ctl+c
    signal(SIGTERM, close); // stop button

    GPIOInterface gpio23(23, output);

    blink(gpio23);

    return a.exec();
}
