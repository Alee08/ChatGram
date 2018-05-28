#ifndef CMDLINE_H
#define CMDLINE_H

#include"meters.h"
#include<string.h>
#include<vector>

using namespace std;

struct MeterInfo {
    char *name;
    char *type;
    char *id;
    char *key;
    Meter *meter;

    MeterInfo(char *n, char *t, char *i, char *k) {
        name = n;
        type = t;
        id = i;
        key = k;
    }
};

struct CommandLine {
    bool need_help {};
    bool silence {};
    bool verbose {};
    bool debug {};
    bool logtelegrams {};
    bool meterfiles {};
    const char *meterfiles_dir {};
    bool json {};
    bool fields {};
    char separator { ';' };
    bool oneshot {};
    int  exitafter {}; // Seconds to exit.
    char *usb_device {};
    vector<MeterInfo> meters;
};

CommandLine *parseCommandLine(int argc, char **argv);

#endif
