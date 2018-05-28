#ifndef SERIAL_H_
#define SERIAL_H_

#include"util.h"

#include<functional>
#include<string>
#include<vector>

using namespace std;

struct SerialCommunicationManager;

struct SerialDevice {
    virtual bool open(bool fail_if_not_ok) = 0;
    virtual void close() = 0;
    virtual bool send(std::vector<uchar> &data) = 0;
    virtual int receive(std::vector<uchar> *data) = 0;
    virtual int fd() = 0;
    virtual SerialCommunicationManager *manager() = 0;
};

struct SerialCommunicationManager {
    virtual SerialDevice *createSerialDeviceTTY(string dev, int baud_rate) = 0;
    virtual void listenTo(SerialDevice *sd, function<void()> cb) = 0;
    virtual void stop() = 0;
    virtual void waitForStop() = 0;
    virtual bool isRunning() = 0;
};

SerialCommunicationManager *createSerialCommunicationManager(time_t exit_after_seconds);

#endif
