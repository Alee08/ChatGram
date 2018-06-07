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
#!/bin/bash

PROG="$1"

cat simulation.txt | grep '^{' > test_expected.txt
$PROG --robot=json simulation.txt \
      MyTapWater multical21 76348799 "" \
      MyHeater multical302 12345678 "" \
      MyElectricity omnipower 15947107 "" \
      > test_output.txt
if [ "$?" == "0" ]
then
    cat test_output.txt | sed 's/"timestamp":"....-..-..T..:..:..Z"/"timestamp":"1111-11-11T11:11:11Z"/' > test_responses.txt
    diff test_expected.txt test_responses.txt
    if [ "$?" == "0" ]
    then
        echo OK
    fi
else
    Failure.
fi
