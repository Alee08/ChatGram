#include"dvparser.h"
#include"util.h"

// The parser should not crash on invalid data, but yeah, when I
// need to debug it because it crashes on invalid data, then
// I enable the following define...
//#define DEBUG_PARSER(...) fprintf(stderr, __VA_ARGS__)
#define DEBUG_PARSER(...)

using namespace std;

bool parseDV(Telegram *t,
             vector<uchar>::iterator data,
             size_t data_len,
             map<string,pair<int,string>> *values,
             vector<uchar>::iterator *format,
             size_t format_len,
             uint16_t *format_hash,
             function<int(int,int,int)> overrideDifLen)
{
    map<string,int> dv_count;
    vector<uchar> format_bytes;
    vector<uchar> data_bytes;
    string dv, key;
    size_t start_parse_here = t->parsed.size();
    vector<uchar>::iterator data_start = data;
    vector<uchar>::iterator data_end = data+data_len;
    vector<uchar>::iterator format_end;
    bool full_header = false;

    if (format == NULL) {
        format = &data;
        format_end = data_end;
        full_header = true;
    } else {
        format_end = *format+format_len;
        string s = bin2hex(*format, format_len);
        debug("(dvparser) using format \"%s\"\n", s.c_str());
    }

    // Data format is:
    // DIF byte (defines how the binary data bits should be decoded and how man data bytes there are)
    // (sometimes a DIFE byte, not yet implemented)
    // VIF byte (defines what the decoded value means, water,energy,power,etc.)
    // (sometimes a VIFE byte when VIF=0xff)
    // Data bytes
    // DIF again...

    // A DifVif(Vife) tuple (triplet) can be for example be 02FF20 for Multical21 vendor specific status bits.
    // The parser stores the data bytes under the key "02FF20" for the first occurence of the 02FF20 data.
    // The second identical tuple(triplet) stores its data under the key "02FF20_2"

    format_bytes.clear();
    for (;;) {
        DEBUG_PARSER("Remaining format data %ju\n", std::distance(*format,format_end));
        if (*format == format_end) break;
        uchar dif = **format;

        DEBUG_PARSER("dif=%02x\n", dif);
        if (dif == 0x2f) {
            t->addExplanation(*format, 1, "%02X skip", dif);
            continue;
        }
        int len = difLenBytes(dif);
        DEBUG_PARSER("len=%d\n", len);
        if (len == -1) {
            warning("(dvparser) cannot handle dif %2X\n", dif);
            return false;
        }
        if (full_header) {
            format_bytes.push_back(dif);
            t->addExplanation(*format, 1, "%02X dif (%s)", dif, difType(dif).c_str());
        } else {
            (*format)++;
        }

        if (*format == format_end) { warning("(dvparser) warning: unexpected end of data (vif expected)"); break; }

        uchar vif = **format;
        DEBUG_PARSER("vif=%02x\n", vif);
        if (full_header) {
            format_bytes.push_back(vif);
            t->addExplanation(*format, 1, "%02X vif (%s)", vif, vifType(vif).c_str());
        } else {
            (*format)++;
        }

        if (overrideDifLen) {
            int new_len = overrideDifLen(dif, vif, len);
            if (new_len != len) {
                DEBUG_PARSER("changing len %d to %d for dif=%02x vif=%02x\n", len, new_len, dif, vif);
                len = new_len;
            }
        }

        strprintf(dv, "%02X%02X", dif, vif);
        if ((vif&0x80) == 0x80) {
            // vif extension
            if (*format == format_end) { warning("(dvparser) warning: unexpected end of data (vife expected)"); break; }
            uchar vife = **format;
            DEBUG_PARSER("vife=%02x\n", vife);
            if (full_header) {
                format_bytes.push_back(vife);
                t->addExplanation(*format, 1, "%02X vife (%s)", vife, vifeType(vif, vife).c_str());
            } else {
                (*format)++;
            }
            strprintf(dv, "%02X%02X%02X", dif, vif, vife);
        }

        int count = ++dv_count[dv];
        DEBUG_PARSER("DifVif key is %s and its count is %d\n", dv.c_str(), count);
        if (count > 1) {
            strprintf(key, "%s_%d", dv.c_str(), count);
        } else {
            strprintf(key, "%s", dv.c_str());
        }

        int remaining = std::distance(data,data_end);
        DEBUG_PARSER("remaining data %d len=%d\n", remaining, len);
        if (remaining < len) {
            warning("(dvparser) warning: unexpected end of data\n");
            len = remaining;
        }

        string value = bin2hex(data, len);
        (*values)[key] = { start_parse_here+data-data_start, value };
        t->addExplanation(data, len, "%s", value.c_str());
    }

    string format_string = bin2hex(format_bytes);
    uint16_t hash = crc16_EN13757(&format_bytes[0], format_bytes.size());

    if (full_header) {
        debug("(dvparser) found format \"%s\" with hash %x\n", format_string.c_str(), hash);
    }

    return true;
}

void extractDV(string &s, uchar *dif, uchar *vif, uchar *vife)
{
    vector<uchar> bytes;
    hex2bin(s, &bytes);

    *dif = bytes[0];
    *vif = bytes[1];
    if (bytes.size() > 2) {
        *vife = bytes[2];
    } else {
        *vife = 0;
    }
}

bool extractDVuint16(map<string,pair<int,string>> *values,
                     string key,
                     int *offset,
                     uint16_t *value)
{
    if ((*values).count(key) == 0) {
        warning("(dvparser) warning: cannot extract uint16 from non-existant key \"%s\"\n", key.c_str());
        *offset = -1;
        *value = 0;
        return false;
    }
    uchar dif, vif, vife;
    extractDV(key, &dif, &vif, &vife);

    pair<int,string>&  p = (*values)[key];
    *offset = p.first;
    vector<uchar> v;
    hex2bin(p.second, &v);

    *value = v[1]<<8 | v[0];
    return true;
}

bool extractDVdouble(map<string,pair<int,string>> *values,
                    string key,
                    int *offset,
                    double *value)
{
    if ((*values).count(key) == 0) {
        warning("(dvparser) warning: cannot extract double from non-existant key \"%s\"\n", key.c_str());
        *offset = 0;
        *value = 0;
        return false;
    }
    uchar dif, vif, vife;
    extractDV(key, &dif, &vif, &vife);

    pair<int,string>&  p = (*values)[key];
    *offset = p.first;
    vector<uchar> v;
    hex2bin(p.second, &v);

    int raw = v[3]*256*256*256 + v[2]*256*256 + v[1]*256 + v[0];
    double scale = vifScale(vif);
    *value = ((double)raw) / scale;

    return true;
}

bool extractDVdoubleCombined(std::map<std::string,std::pair<int,std::string>> *values,
                            std::string key_high_bits, // Which key to extract high bits from.
                            std::string key,
                            int *offset,
                            double *value)
{
    if ((*values).count(key) == 0 || (*values).count(key_high_bits) == 0) {
        warning("(dvparser) warning: cannot extract combined double since at least one key is missing \"%s\" \"%s\"\n", key.c_str(),
                key_high_bits.c_str());
        *offset = 0;
        *value = 0;
        return false;
    }
    uchar dif, vif, vife;
    extractDV(key, &dif, &vif, &vife);

    pair<int,string>&  p = (*values)[key];
    *offset = p.first;
    vector<uchar> v;
    hex2bin(p.second, &v);

    pair<int,string>&  pp = (*values)[key_high_bits];
    vector<uchar> v_high;
    hex2bin(pp.second, &v_high);

    int raw = v_high[3]*256*256*256 + v_high[2]*256*256 + v[1]*256 + v[0];
    double scale = vifScale(vif);
    *value = ((double)raw) / scale;

    return true;
}
