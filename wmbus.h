#ifndef WMBUS_H
#define WMBUS_H

#include"manufacturers.h"
#include"serial.h"
#include"util.h"

#include<inttypes.h>

#define LIST_OF_LINK_MODES X(LinkModeC1)X(LinkModeT1)X(UNKNOWN_LINKMODE)

// In link mode T1, the meter transmits a telegram every few seconds.
// Suitable for drive-by/walk-by collection of meter values.

// Link mode C1 is like T1 but uses less energy when transmitting due to
// a different radio encoding. Suitable for battery powered meters.

enum LinkMode {
#define X(name) name,
LIST_OF_LINK_MODES
#undef X
};

#define CC_B_BIDIRECTIONAL_BIT 0x80
#define CC_RD_RESPONSE_DELAY_BIT 0x40
#define CC_S_SYNCH_FRAME_BIT 0x20
#define CC_R_RELAYED_BIT 0x10
#define CC_P_HIGH_PRIO_BIT 0x08

// Bits 31-29 in SN, ie 0xc0 of the final byte in the stream,
// since the bytes arrive with the least significant first
// aka little endian.
#define SN_ENC_BITS 0xc0

using namespace std;

struct Telegram {
    int len {}; // The length of the telegram, 1 byte.
    int c_field {}; // 1 byte (0x44=telegram, no response expected!)
    int m_field {}; // Manufacturer 2 bytes
    vector<uchar> a_field; // A field 6 bytes
    // The 6 a field bytes are composed of:
    vector<uchar> a_field_address; // Address in BCD = 8 decimal 00000000...99999999 digits.
    int a_field_version {}; // 1 byte
    int a_field_device_type {}; // 1 byte

    int ci_field {}; // 1 byte

    // When ci_field==0x7a then there are 4 extra header bytes, short data header
    int acc {}; // 1 byte
    int status {}; // 1 byte
    int configuration {}; // 2 bytes

    // When ci_field==0x8d then there are 8 extra header bytes (ELL header)
    int cc_field {}; // 1 byte
    // acc; // 1 byte
    uchar sn[4] {}; // 4 bytes
    // That is 6 bytes (not 8), the next two bytes, the payload crc
    // part of this ELL header, even though they are inside the encrypted payload.

    vector<uchar> parsed; // Parsed fields
    vector<uchar> payload; // To be parsed.
    vector<uchar> content; // Decrypted content.

    bool handled {}; // Set to true, when a meter has accepted the telegram.

    // The id as written on the physical meter device.
    string id() { return bin2hex(a_field_address); }
    void parse(vector<uchar> &payload);
    void print();
    void verboseFields();

    // A vector of indentations and explanations, to be printed
    // below the raw data bytes to explain the telegram content.
    vector<pair<int,string>> explanations;
    void addExplanation(vector<uchar>::iterator &bytes, int len, const char* fmt, ...);
    void addMoreExplanation(int pos, const char* fmt, ...);
    void explainParse(string intro, int from);
};

struct WMBus {
    virtual bool ping() = 0;
    virtual uint32_t getDeviceId() = 0;
    virtual LinkMode getLinkMode() = 0;
    virtual void setLinkMode(LinkMode lm) = 0;
    virtual void onTelegram(function<void(Telegram*)> cb) = 0;
    virtual void sendTelegram(Telegram *t) = 0;
    virtual SerialDevice *serial() = 0;
    virtual void simulate() = 0;
};

#define LIST_OF_MBUS_DEVICES X(DEVICE_IM871A)X(DEVICE_AMB8465)X(DEVICE_SIMULATOR)X(DEVICE_UNKNOWN)

enum MBusDeviceType {
#define X(name) name,
LIST_OF_MBUS_DEVICES
#undef X
};

// The detect function can be supplied the device "auto" and will try default locations for the device.
// Returned is the type and the found device string.
pair<MBusDeviceType,string> detectMBusDevice(string device, SerialCommunicationManager *manager);

WMBus *openIM871A(string device, SerialCommunicationManager *manager);
WMBus *openAMB8465(string device, SerialCommunicationManager *manager);
struct WMBusSimulator;
WMBus *openSimulator(string file, SerialCommunicationManager *manager);

string manufacturer(int m_field);
string manufacturerFlag(int m_field);
string deviceType(int m_field, int a_field_device_type);
string mediaType(int m_field, int a_field_device_type);
string ciType(int ci_field);
string cType(int c_field);
string ccType(int cc_field);
string difType(int dif);
double vifScale(int vif);
string vifKey(int vif); // E.g. temperature energy power mass_flow volume_flow
string vifUnit(int vif); // E.g. m3 c kwh kw MJ MJh
string vifType(int vif); // Long description
string vifeType(int vif, int vife); // Long description
string formatData(int dif, int vif, int vife, string data);

double extract8bitAsDouble(int dif, int vif, int vife, string data);
double extract16bitAsDouble(int dif, int vif, int vife, string data);
double extract32bitAsDouble(int dif, int vif, int vife, string data);

int difLenBytes(int dif);

#endif
