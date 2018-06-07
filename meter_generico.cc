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
#include"dvparser.h"
#include"meters.h"
#include"meters_common_implementation.h"
#include"wmbus.h"
#include"wmbus_utils.h"
#include"util.h"

#include<assert.h>
#include<map>
#include<memory.h>
#include<stdio.h>
#include<string>
#include<time.h>
#include<vector>

using namespace std;

struct MeterGenerico : public virtual GenericMeter, public virtual MeterCommonImplementation {
    MeterGenerico(WMBus *bus, const char *name, const char *id, const char *key);

    string message();

    void printMeterHumanReadable(FILE *output);
    void printMeterFields(FILE *output, char separator);
    void printMeterJSON(FILE *output);

private:
    void handleTelegram(Telegram *t);
    void processContent(Telegram *t);

    string message_; // The most recently received message.
};

MeterGenerico::MeterGenerico(WMBus *bus, const char *name, const char *id, const char *key) :
    MeterCommonImplementation(bus, name, id, key, GENERICO_METER, MANFCODE('I','M','S'), 0)
{
    MeterCommonImplementation::bus()->onTelegram(calll(this,handleTelegram,Telegram*));
}

GenericMeter *createGenerico(WMBus *bus, const char *name, const char *id, const char *key)
{
    return new MeterGenerico(bus,name,id,key);
}

string MeterGenerico::message() {
    return message_;
}

void MeterGenerico::handleTelegram(Telegram *t)
{
    if (!isTelegramForMe(t)) {
        // This telegram is not intended for this meter.
        return;
    }

    verbose("(generico) telegram for %s %02x%02x%02x%02x\n",
            name().c_str(),
            t->a_field_address[0], t->a_field_address[1], t->a_field_address[2],
            t->a_field_address[3]);

    if (useAes()) {
        vector<uchar> aeskey = key();
        decryptKamstrupC1(t, aeskey);
    } else {
        t->content = t->payload;
    }
    char log_prefix[256];
    snprintf(log_prefix, 255, "(generico) log");
    logTelegram(log_prefix, t->parsed, t->content);
    int content_start = t->parsed.size();
    processContent(t);
    if (isDebugEnabled()) {
        snprintf(log_prefix, 255, "(generico)");
        t->explainParse(log_prefix, content_start);
    }
    triggerUpdate(t);
}

void MeterGenerico::processContent(Telegram *t)
{
    vector<uchar>::iterator bytes = t->content.begin();

    string m;
    for (size_t i=0; i<t->content.size(); ++i) {
        if (t->content[i] != 0) {
            m.push_back(t->content[i]);
        }
    }
    message_ = m;
}

void MeterGenerico::printMeterHumanReadable(FILE *output)
{
    fprintf(output, "%s\t%s\t%s\t%s\n",
	    name().c_str(),
	    id().c_str(),
	    message().c_str(),
	    datetimeOfUpdateHumanReadable().c_str());
}

void MeterGenerico::printMeterFields(FILE *output, char separator)
{
    fprintf(output, "%s%c%s%c%s%c%s\n",
	    name().c_str(), separator,
	    id().c_str(), separator,
	    message().c_str(), separator,
            datetimeOfUpdateRobot().c_str());
}

#define Q(x,y) "\""#x"\":"#y","
#define QS(x,y) "\""#x"\":\""#y"\","
#define QSE(x,y) "\""#x"\":\""#y"\""

void MeterGenerico::printMeterJSON(FILE *output)
{
    fprintf(output, "{media:\"%s\",meter:\"generico\","
	    QS(name,%s)
	    QS(id,%s)
	    QS(message,%s)
	    QSE(timestamp,%s)
	    "}\n",
            mediaType(manufacturer(), media()).c_str(),
	    name().c_str(),
	    id().c_str(),
	    message().c_str(),
	    datetimeOfUpdateRobot().c_str());
}
