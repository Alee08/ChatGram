#ifndef DVPARSER_H
#define DVPARSER_H

#include"util.h"
#include"wmbus.h"

#include<map>
#include<stdint.h>
#include<functional>
#include<vector>

// DV stands for DIF VIF

bool parseDV(Telegram *t,
             std::vector<uchar>::iterator data,
             size_t data_len,
             std::map<std::string,std::pair<int,std::string>> *values,
             std::vector<uchar>::iterator *format = NULL,
             size_t format_len = 0,
             uint16_t *format_hash = NULL,
             std::function<int(int,int,int)> overrideDifLen = NULL
             );

bool extractDVuint16(std::map<std::string,std::pair<int,std::string>> *values,
                     std::string key,
                     int *offset,
                     uint16_t *value);

bool extractDVdouble(std::map<std::string,std::pair<int,std::string>> *values,
                    std::string key,
                    int *offset,
                    double *value);

// Extract low bits from primary entry and hight bits from secondary entry.
bool extractDVdoubleCombined(std::map<std::string,std::pair<int,std::string>> *values,
                            std::string key_high_bits, // Which key to extract high bits from.
                            std::string key,
                            int *offset,
                            double *value);

void extractDV(string &s, uchar *dif, uchar *vif, uchar *vife);

#endif
