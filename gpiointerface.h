#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include <QObject>

typedef enum
{
    low = 0,
    high = 1
} gpiovalue;

typedef enum
{
    input = 0,
    output = 1
} gpiodirection;

class GPIOInterface : public QObject
{
    Q_OBJECT
public:
    explicit GPIOInterface(int pin, gpiodirection dir, QObject *parent = nullptr);
    ~GPIOInterface();
    void readGpio();
    void writeGpio(gpiovalue val);

signals:

private:
    void exportGpio();
    void setDirection(gpiodirection dir);
    void unexportGpio();

    char const *gpioPath = "/sys/class/gpio/";
    QString pinPath = gpioPath;
    int gpioPin;
    bool exported = false;
    gpiovalue value;
    gpiodirection direction;

};

#endif // GPIOINTERFACE_H
