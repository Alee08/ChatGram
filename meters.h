/*
Copyright (C) 2017 Fredrik Öhrström

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*
Additional code in this file:
Copyright (c) 2018 Alessandro Trapasso
Also licensed to you under GPLv3 as above.
*/
#ifndef METER_H_
#define METER_H_

#include"util.h"
#include"wmbus.h"

#include<string>
#include<vector>

#define LIST_OF_METERS X(MULTICAL21_METER)X(FLOWIQ3100_METER)X(MULTICAL302_METER)X(OMNIPOWER_METER)X(GENERICO_METER)X(UNKNOWN_METER)

enum MeterType {
#define X(name) name,
LIST_OF_METERS
#undef X
};

using namespace std;

typedef unsigned char uchar;

struct Meter {
    virtual string id() = 0;
    virtual string name() = 0;
    virtual MeterType type() = 0;
    virtual int manufacturer() = 0;
    virtual int media() = 0;
    virtual WMBus *bus() = 0;

    virtual string datetimeOfUpdateHumanReadable() = 0;
    virtual string datetimeOfUpdateRobot() = 0;

    virtual void onUpdate(function<void(Meter*)> cb) = 0;
    virtual int numUpdates() = 0;

    virtual void printMeterHumanReadable(FILE *output) = 0;
    virtual void printMeterFields(FILE *output, char separator) = 0;
    virtual void printMeterJSON(FILE *output) = 0;

    virtual bool isTelegramForMe(Telegram *t) = 0;
    virtual bool useAes() = 0;
    virtual vector<uchar> key() = 0;

    // Dynamically access all data received for the meter.
    virtual std::vector<std::string> getRecords() = 0;
    virtual double getRecordAsDouble(std::string record) = 0;
    virtual uint16_t getRecordAsUInt16(std::string record) = 0;
};

struct WaterMeter : public virtual Meter {
    virtual double totalWaterConsumption() = 0; // m3
    virtual bool  hasTotalWaterConsumption() = 0;
    virtual double targetWaterConsumption() = 0; // m3
    virtual bool  hasTargetWaterConsumption() = 0;
    virtual double maxFlow() = 0;
    virtual bool  hasMaxFlow() = 0;

    virtual string statusHumanReadable() = 0;
    virtual string status() = 0;
    virtual string timeDry() = 0;
    virtual string timeReversed() = 0;
    virtual string timeLeaking() = 0;
    virtual string timeBursting() = 0;
};

struct HeatMeter : public virtual Meter {
    virtual double totalEnergyConsumption() = 0; // kwh
    virtual double currentPowerConsumption() = 0; // kw
    virtual double totalVolume() = 0; // m3
};

struct ElectricityMeter : public virtual Meter {
    virtual double totalEnergyConsumption() = 0; // kwh
    virtual double currentPowerConsumption() = 0; // kw
};

struct GenericMeter : public virtual Meter {
    virtual string message() = 0;
};

MeterType toMeterType(const char *type);
WaterMeter *createMultical21(WMBus *bus, const char *name, const char *id, const char *key, MeterType mt);
HeatMeter *createMultical302(WMBus *bus, const char *name, const char *id, const char *key);
ElectricityMeter *createOmnipower(WMBus *bus, const char *name, const char *id, const char *key);
GenericMeter *createGenerico(WMBus *bus, const char *name, const char *id, const char *key);

#endif
