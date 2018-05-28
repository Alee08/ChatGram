#ifndef METERS_COMMON_IMPLEMENTATION_H_
#define METERS_COMMON_IMPLEMENTATION_H_

#include"meters.h"

#include<map>

struct MeterCommonImplementation : public virtual Meter {
    string id();
    string name();
    MeterType type();
    int manufacturer();
    int media();
    WMBus *bus();

    string datetimeOfUpdateHumanReadable();
    string datetimeOfUpdateRobot();

    void onUpdate(function<void(Meter*)> cb);
    int numUpdates();

    bool isTelegramForMe(Telegram *t);
    bool useAes();
    vector<uchar> key();

    std::vector<std::string> getRecords();
    double getRecordAsDouble(std::string record);
    uint16_t getRecordAsUInt16(std::string record);

    MeterCommonImplementation(WMBus *bus, const char *name, const char *id, const char *key,
                              MeterType type, int manufacturer, int media);

protected:

    void triggerUpdate(Telegram *t);

private:

    MeterType type_ {};
    int manufacturer_ {};
    int media_ {};
    string name_;
    vector<uchar> id_;
    vector<uchar> key_;
    WMBus *bus_ {};
    vector<function<void(Meter*)>> on_update_;
    int num_updates_ {};
    bool use_aes_ {};
    time_t datetime_of_update_ {};

protected:
    std::map<std::string,std::pair<int,std::string>> values_;
};

#endif
