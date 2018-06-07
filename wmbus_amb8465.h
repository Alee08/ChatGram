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
// Defines documented in the Manual for the AMBER wM-Bus Modules Version 2.7

#define AMBER_SERIAL_SOF 0xFF

#define CMD_DATA_REQ             0x00
#define CMD_DATARETRY_REQ        0x02
#define CMD_DATA_IND             0x03
#define CMD_SET_MODE_REQ         0x04
#define CMD_RESET_REQ            0x05
#define CMD_SET_CHANNEL_REQ      0x06
#define CMD_SET_REQ              0x09
#define CMD_GET_REQ              0x0A
#define CMD_SERIALNO_REQ         0x0B
#define CMD_FWV_REQ              0x0C
#define CMD_RSSI_REQ             0x0D
#define CMD_SETUARTSPEED_REQ     0x10
#define CMD_FACTORYRESET_REQ     0x11
#define CMD_DATA_PRELOAD_REQ     0x30
#define CMD_DATA_CLR_PRELOAD_REQ 0x31
#define CMD_SET_AES_KEY_REQ      0x50
#define CMD_CLR_AES_KEY_REQ      0x51
#define CMD_GET_AES_DEV_REQ      0x52
