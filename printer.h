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
