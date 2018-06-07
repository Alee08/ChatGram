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
