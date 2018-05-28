#include"printer.h"

using namespace std;

Printer::Printer(bool json, bool fields, char separator, bool meterfiles, const char *meterfiles_dir)
{
    json_ = json;
    fields_ = fields;
    separator_ = separator;
    meterfiles_ = meterfiles;
    meterfiles_dir_ = meterfiles_dir;
}

void Printer::print(Meter *meter)
{
    FILE *output = stdout;

    if (meterfiles_) {
	char filename[128];
	memset(filename, 0, sizeof(filename));
	snprintf(filename, 127, "%s/%s", meterfiles_dir_, meter->name().c_str());
	output = fopen(filename, "w");
    }

    if (json_) {
        meter->printMeterJSON(output);
    }
    else if (fields_) {
        meter->printMeterFields(output, separator_);
    }
    else {
        meter->printMeterHumanReadable(output);
    }

    if (output != stdout) {
	fclose(output);
    }
}
