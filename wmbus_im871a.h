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
// Definitions and source code imported from WMBus_HCI_Spec_V1_6.pdf
// Found here: https://wireless-solutions.de/products/gateways/wirelessadapter.html

#define IM871A_SERIAL_SOF 0xA5

#define DEVMGMT_ID 0x01
#define RADIOLINK_ID 0x02
#define RADIOLINKTEST_ID 0x03
#define HWTEST_ID 0x04

#define DEVMGMT_MSG_PING_REQ 0x01
#define DEVMGMT_MSG_PING_RSP 0x02
#define DEVMGMT_MSG_SET_CONFIG_REQ 0x03
#define DEVMGMT_MSG_SET_CONFIG_RSP 0x04
#define DEVMGMT_MSG_GET_CONFIG_REQ 0x05
#define DEVMGMT_MSG_GET_CONFIG_RSP 0x06
#define DEVMGMT_MSG_RESET_REQ 0x07
#define DEVMGMT_MSG_RESET_RSP 0x08
#define DEVMGMT_MSG_FACTORY_RESET_REQ 0x09
#define DEVMGMT_MSG_FACTORY_RESET_RSP 0x0A
#define DEVMGMT_MSG_GET_OPMODE_REQ 0x0B
#define DEVMGMT_MSG_GET_OPMODE_RSP 0x0C
#define DEVMGMT_MSG_SET_OPMODE_REQ 0x0D
#define DEVMGMT_MSG_SET_OPMODE_RSP 0x0E
#define DEVMGMT_MSG_GET_DEVICEINFO_REQ 0x0F
#define DEVMGMT_MSG_GET_DEVICEINFO_RSP 0x10
#define DEVMGMT_MSG_GET_SYSSTATUS_REQ 0x11
#define DEVMGMT_MSG_GET_SYSSTATUS_RSP 0x12
#define DEVMGMT_MSG_GET_FWINFO_REQ 0x13
#define DEVMGMT_MSG_GET_FWINFO_RSP 0x14
#define DEVMGMT_MSG_GET_RTC_REQ 0x19
#define DEVMGMT_MSG_GET_RTC_RSP 0x1A
#define DEVMGMT_MSG_SET_RTC_REQ 0x1B
#define DEVMGMT_MSG_SET_RTC_RSP 0x1C
#define DEVMGMT_MSG_ENTER_LPM_REQ 0x1D
#define DEVMGMT_MSG_ENTER_LPM_RSP 0x1E
#define DEVMGMT_MSG_SET_AES_ENCKEY_REQ 0x21
#define DEVMGMT_MSG_SET_AES_ENCKEY_RSP 0x22
#define DEVMGMT_MSG_ENABLE_AES_ENCKEY_REQ 0x23
#define DEVMGMT_MSG_ENABLE_AES_ENCKEY_RSP 0x24
#define DEVMGMT_MSG_SET_AES_DECKEY_REQ 0x25
#define DEVMGMT_MSG_SET_AES_DECKEY_RSP 0x26
#define DEVMGMT_MSG_AES_DEC_ERROR_IND 0x27

#define RADIOLINK_MSG_WMBUSMSG_REQ 0x01
#define RADIOLINK_MSG_WMBUSMSG_RSP 0x02
#define RADIOLINK_MSG_WMBUSMSG_IND 0x03
#define RADIOLINK_MSG_DATA_REQ 0x04
#define RADIOLINK_MSG_DATA_RSP 0x05

#define RADIOLINKTEST_MSG_START_REQ 0x01
#define RADIOLINKTEST_MSG_START_RSP 0x02
#define RADIOLINKTEST_MSG_STOP_REQ 0x03
#define RADIOLINKTEST_MSG_STOP_RSP 0x04
#define RADIOLINKTEST_MSG_STATUS_IND 0x07

#define HWTEST_MSG_RADIOTEST_REQ 0x01
#define HWTEST_MSG_RADIOTEST_RSP 0x02

#define LIST_OF_IM871A_LINK_MODES X(S1)X(S1m)X(S2)X(T1)X(T2)X(R2)X(C1a)X(C1b)X(C2a)X(C2b) \
    X(N1A)X(N2A)X(N1B)X(N2B)X(N1C)X(N2C)X(N1D)X(N2D)X(N1E)X(N2E)X(N1F)X(N2F)X(UNKNOWN_LINKMODE)

enum LinkModeIM871A {
#define X(name) im871a_##name,
LIST_OF_IM871A_LINK_MODES
#undef X
};
