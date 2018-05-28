#include"cmdline.h"
#include"meters.h"

using namespace std;

struct Printer {
    Printer(bool json, bool fields, char separator, bool meterfiles, const char *meterfiles_dir);

    void print(Meter *meter);

    private:

    bool json_, fields_, meterfiles_;
    const char *meterfiles_dir_;
    char separator_;
};
