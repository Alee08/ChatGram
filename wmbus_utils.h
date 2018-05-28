#ifndef WMBUS_UTILS_H
#define WMBUS_UTILS_H

void decryptKamstrupC1(Telegram *t, vector<uchar> &aeskey);
string frameTypeKamstrupC1(int ft);

#endif
